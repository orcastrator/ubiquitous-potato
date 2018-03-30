// specifically made for windows
#using <System.dll>
#include <fstream>
#include "directoryWatcher.h"

using namespace System;
using namespace System::IO;
using namespace System::Security::Permissions;
using namespace System::Threading;
using namespace System::Collections::Generic;

bool createFile(String^ SRC, String^ DST) {
	try {
		FileInfo^ srcFile = gcnew FileInfo(SRC);

		FileInfo^ dstFile = gcnew FileInfo(DST + "\\" + srcFile->Name);
		// if file already exists, move old file to a new name and create the backup
		if (dstFile->Exists)
			dstFile->MoveTo(DST + "\\" + dstFile->Name + ".replaced");

		srcFile->CopyTo(DST + "\\" + srcFile->Name);
	}
	catch (Exception^ e) {
		Console::WriteLine(e->ToString());
		return 1;
	}
	return 0;
}

bool copyFile(String^ SRC, String^ DST) {
	try {
		FileInfo^ srcFile = gcnew FileInfo(SRC);

		srcFile->CopyTo(DST + "\\" + srcFile->Name, true);
	}
	catch (Exception^ e) {
		Console::WriteLine(e->ToString());
		return 1;
	}
	return 0;
}

bool moveFile(String^ SRC, String^ DST) {
	try {
		FileInfo^ srcFile = gcnew FileInfo(SRC);

		srcFile->MoveTo(DST + "\\" + srcFile->Name);
	}
	catch (Exception^ e) {
		return 1;
	}
	return 0;
}

bool deleteFile(String^ SRC) {
	try {
		FileInfo^ srcFile = gcnew FileInfo(SRC);

		FileInfo^ backupFile = gcnew FileInfo(Globals::BackupDirectory + "\\" + srcFile->Name);

		backupFile->Delete();
	}
	catch (Exception^ e) {
		return 1;
	}
	return 0;
}

void Watcher::OnChanged(Object^ source, FileSystemEventArgs^ e)
{
	// Specify what is done when a file is changed, created, or deleted.
	Console::WriteLine("File: {0} {1}", e->FullPath, e->ChangeType);

	switch (e->ChangeType) {
	case System::IO::WatcherChangeTypes::Created:
		createFile(e->FullPath, Globals::BackupDirectory);
		break;
	case System::IO::WatcherChangeTypes::Changed:
		copyFile(e->FullPath, Globals::BackupDirectory);
		break;
	case System::IO::WatcherChangeTypes::Deleted:
		Console::WriteLine("Will only delete backup 30 days after it has been untouched");
		break;
	case System::IO::WatcherChangeTypes::Renamed:
		Console::WriteLine("How did that get here?");
		break;
	case System::IO::WatcherChangeTypes::All:
		break;
	}

	Console::WriteLine("File: {0} {1}", e->FullPath, e->ChangeType);
}

void Watcher::OnRenamed(Object^ source, RenamedEventArgs^ e)
{
	// Specify what is done when a file is renamed.
	Console::WriteLine("File: {0} renamed to {1}", e->OldFullPath, e->FullPath);
	copyFile(e->FullPath, Globals::BackupDirectory);
	deleteFile(e->OldFullPath);
}

[PermissionSet(SecurityAction::Demand, Name = "FullTrust")]
int Watcher::run()
{
	array<String^>^args = System::Environment::GetCommandLineArgs();

	// If a directory is not specified, exit program.
	if (args->Length != 3)
	{
		// Display the proper way to call the program.
		Console::WriteLine("Usage: Watcher.exe (watch directory) (backup directory)");
		return 1;
	}

	// Create a new FileSystemWatcher and set its properties.
	FileSystemWatcher^ watcher = gcnew FileSystemWatcher;
	watcher->Path = args[1];
	Console::WriteLine("Watching directory: {0}", args[1]);
	Globals::BackupDirectory = args[2];
	Console::WriteLine("Backup directory: {0}", args[2]);

	/* Watch for changes in LastAccess and LastWrite times, and
	the renaming of files or directories. */
	watcher->NotifyFilter = static_cast<NotifyFilters>(NotifyFilters::LastAccess |
		NotifyFilters::LastWrite | NotifyFilters::FileName | NotifyFilters::DirectoryName);

	// Watch all files and directories
	watcher->Filter = "*";

	// Watch all subdirectories as well
	watcher->IncludeSubdirectories = true;

	// Add event handlers.
	watcher->Changed += gcnew FileSystemEventHandler(Watcher::OnChanged);
	watcher->Created += gcnew FileSystemEventHandler(Watcher::OnChanged);
	watcher->Deleted += gcnew FileSystemEventHandler(Watcher::OnChanged);
	watcher->Renamed += gcnew RenamedEventHandler(Watcher::OnRenamed);

	// Begin watching.
	watcher->EnableRaisingEvents = true;

	// Wait for the user to quit the program.
	Console::WriteLine("Press \'q\' to quit the sample.");
	while (Console::Read() != 'q');

	return 0;
}