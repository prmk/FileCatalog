/////////////////////////////////////////////////////////////////////////////
// FileSystem.cpp - Support file and directory operations                  //
// ver 2.5                                                                 //
// ----------------------------------------------------------------------- //
// copyright � Jim Fawcett, 2012                                           //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2010                             //
// Platform:    Dell XPS 8300, Core i7, Windows 7 SP1                      //
// Application: Summer Projects, 2012                                      //
// Author:      Pramukh Shenoy,Syracuse University                         //
//              (315) 746-0822, pbantwal@syr.edu                           //
// Original:    Jim Fawcett, CST 4-187, Syracuse University                //
//              (315) 443-3948, jfawcett@twcny.rr.com                      //
/////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <sstream>
#include <iomanip>
#include <utility>
#include <cctype>
#include <locale>
#include "FileSystem.h"

using namespace FileCatalog;

/////////////////////////////////////////////////////////
// helper FileSystemSearch

class FileSystemSearch
{
public:
	FileSystemSearch();
	~FileSystemSearch();
	std::string firstFile(const std::string& path = ".", const std::string& pattern = "*.*");
	std::string nextFile();
	std::string firstDirectory(const std::string& path = ".", const std::string& pattern = "*.*");
	std::string nextDirectory();
	void close();
private:
	HANDLE hFindFile;
	WIN32_FIND_DATAA FindFileData;
	WIN32_FIND_DATAA* pFindFileData;
};

FileSystemSearch::FileSystemSearch() : pFindFileData(&FindFileData) {}
FileSystemSearch::~FileSystemSearch() { ::FindClose(hFindFile); }
void FileSystemSearch::close() { ::FindClose(hFindFile); }

//----< block constructor taking array iterators >-------------------------

Block::Block(Byte* beg, Byte* end) : bytes_(beg, end) {}

//----< push back block byte >---------------------------------------------

void Block::push_back(Byte b)
{
	bytes_.push_back(b);
}
//----< non-const indexer >------------------------------------------------

Byte& Block::operator[](size_t i)
{
	if (i<0 || bytes_.size() <= i)
		throw std::runtime_error("index out of range in Block");
	return bytes_[i];
}
//----< const indexer >----------------------------------------------------

Byte Block::operator[](size_t i) const
{
	if (i<0 || bytes_.size() <= i)
		throw std::runtime_error("index out of range in Block");
	return bytes_[i];
}
//----< equality comparison >----------------------------------------------

bool Block::operator==(const Block& block) const
{
	return bytes_ == block.bytes_;
}
//----< inequality comparison >--------------------------------------------

bool Block::operator!=(const Block& block) const
{
	return bytes_ != block.bytes_;
}
//----< return number of bytes in block >----------------------------------

size_t Block::size() const
{
	return bytes_.size();
}

//----< File constructor opens file stream >-------------------------------

File::File(const std::string& filespec)
	: name_(filespec), pIStream(nullptr), pOStream(nullptr), dirn_(in), typ_(text), good_(true)
{
}
//----< File destructor closes file stream >-------------------------------

File::~File()
{
	if (pIStream)
	{
		pIStream->close();
		delete pIStream;
		pIStream = nullptr;
		good_ = false;
	}
	if (pOStream)
	{
		pOStream->close();
		delete pOStream;
		pOStream = nullptr;
		good_ = false;
	}
}
//----< open for reading or writing >--------------------------------------

bool File::open(direction dirn, type typ)
{
	dirn_ = dirn;
	typ_ = typ;
	good_ = true;
	if (dirn == in)
	{
		pIStream = new std::ifstream;
		if (typ == binary)
			pIStream->open(name_.c_str(), std::ios::in | std::ios::binary);
		else
			pIStream->open(name_.c_str(), std::ios::in);
		if (!(*pIStream).good())
		{
			good_ = false;
			pIStream = nullptr;
			//throw std::runtime_error("\n  open for input failed in File constructor");
		}
	}
	else
	{
		pOStream = new std::ofstream;
		if (typ == binary)
			pOStream->open(name_.c_str(), std::ios::out | std::ios::binary);
		else
			pOStream->open(name_.c_str(), std::ios::out);
		if (!(*pOStream).good())
		{
			good_ = false;
			pOStream = nullptr;
			//throw std::runtime_error("\n  open for output failed in File constructor");
		}
	}
	return good_;
}
//----< reads one line of a text file >------------------------------------

std::string File::getLine(bool keepNewLines)
{
	if (pIStream == nullptr || !pIStream->good())
		throw std::runtime_error("input stream not open");
	if (typ_ == binary)
		throw std::runtime_error("getting text line from binary file");
	if (dirn_ == out)
		throw std::runtime_error("reading output file");

	std::string store;
	while (true)
	{
		char ch = pIStream->get();
		if (!isGood())
			return store;
		if (ch == '\n')
		{
			if (keepNewLines)
				store += ch;
			return store;
		}
		store += ch;
	}
}
//----< read all lines of text file into one string >----------------------

std::string File::readAll(bool keepNewLines)
{
	std::string store;
	while (true)
	{
		if (!isGood())
			return store;
		store += getLine(keepNewLines);
		std::locale loc;
		if (store.size() > 0 && !std::isspace(store[store.size() - 1], loc))
			store += ' ';
	}
	return store;
}

//----< searches searchtext in a file >---------------------------
std::set<int> File::findString(std::string searchtext, bool keepNewLines)
{
	std::set<int> found;
	size_t searchPosition = 0;
	int pos = -1;
	std::string fileContents = readAll(keepNewLines);
	while (searchPosition < fileContents.length())
	{
		pos = fileContents.find(searchtext, searchPosition);
		if (pos >= 0)
		{
			searchPosition = pos + searchtext.length();
			found.insert(pos);
		}
		else
			searchPosition++;

	}
	return found;
}

//----< writes one line of a text to a file >------------------------------

void File::putLine(const std::string& s, bool wantReturn)
{
	if (pOStream == nullptr || !pOStream->good())
		throw std::runtime_error("output stream not open");
	if (typ_ == binary)
		throw std::runtime_error("writing text line to binary file");
	if (dirn_ == in)
		throw std::runtime_error("writing input file");
	for (size_t i = 0; i<s.size(); ++i)
		pOStream->put(s[i]);
	if (wantReturn)
		pOStream->put('\n');
	pOStream->flush();
}
//----< reads a block of bytes from binary file >--------------------------

Block File::getBlock(size_t size)
{
	if (pIStream == nullptr || !pIStream->good())
		throw std::runtime_error("input stream not open");
	if (typ_ != binary)
		throw std::runtime_error("reading binary from text file");
	if (dirn_ == out)
		throw std::runtime_error("reading output file");
	Block blk;
	if (pIStream)
	{
		for (size_t i = 0; i<size; ++i)
		{
			Byte b;
			pIStream->get(b);
			if (pIStream->good())
				blk.push_back(b);
			else
				break;
		}
	}
	return blk;
}
//----< writes a block of bytes to binary file >---------------------------

void File::putBlock(const Block& blk)
{
	if (pOStream == nullptr || !pOStream->good())
		throw std::runtime_error("output stream not open");
	if (typ_ != binary)
		throw std::runtime_error("writing binary to text file");
	if (dirn_ == in)
		throw std::runtime_error("writing input file");
	if (!pOStream->good())
		return;
	for (size_t i = 0; i<blk.size(); ++i)
	{
		pOStream->put(blk[i]);
	}
}
//----< tests for error free stream state >--------------------------------

bool File::isGood()
{
	if (!good_)
		return false;
	if (pIStream != nullptr)
		return (good_ = pIStream->good());
	if (pOStream != nullptr)
		return (good_ = pOStream->good());
	return (good_ = false);
}
//----< flushes output stream to its file >--------------------------------

void File::flush()
{
	if (pOStream != nullptr && pOStream->good())
		pOStream->flush();
}
//----< clears error state enabling operations again >---------------------

void File::clear()
{
	if (pIStream != nullptr)
		pIStream->clear();
	if (pOStream != nullptr)
		pOStream->clear();
}
//----< close file handle >------------------------------------------------

void File::close()
{
	File::flush();
	if (pIStream != nullptr)
	{
		pIStream->close();
		pIStream = nullptr;
		good_ = false;
	}
	if (pOStream)
	{
		pOStream->close();
		pOStream = nullptr;
		good_ = false;
	}
}
//----< file exists >--------------------------------------------------

bool File::exists(const std::string& file)
{
	return ::GetFileAttributesA(file.c_str()) != INVALID_FILE_ATTRIBUTES;
}
//----< copy file >----------------------------------------------------

bool File::copy(const std::string& src, const std::string& dst, bool failIfExists)
{
	return ::CopyFileA(src.c_str(), dst.c_str(), failIfExists) != 0;
}
//----< remove file >--------------------------------------------------

bool File::remove(const std::string& file)
{
	return ::DeleteFileA(file.c_str()) != 0;
}
//----< constructor >--------------------------------------------------

FileInfo::FileInfo(const std::string& fileSpec)
{
	HANDLE hFile = ::FindFirstFileA(fileSpec.c_str(), &data);
	if (hFile == INVALID_HANDLE_VALUE)
		good_ = false;
	else
		good_ = true;
}
//----< is passed filespec valid? >------------------------------------

bool FileInfo::good()
{
	return good_;
}
//----< return file name >---------------------------------------------

std::string FileInfo::name() const
{
	return Path::getName(data.cFileName);
}
//----< conversion helper >--------------------------------------------

std::string FileInfo::intToString(long i)
{
	std::ostringstream out;
	out.fill('0');
	out << std::setw(2) << i;
	return out.str();
}
//----< return file date >---------------------------------------------

std::string FileInfo::date(dateFormat df) const
{
	std::string dateStr, timeStr;
	FILETIME ft;
	SYSTEMTIME st;
	::FileTimeToLocalFileTime(&data.ftLastWriteTime, &ft);
	::FileTimeToSystemTime(&ft, &st);
	dateStr = intToString(st.wMonth) + '/' + intToString(st.wDay) + '/' + intToString(st.wYear);
	timeStr = intToString(st.wHour) + ':' + intToString(st.wMinute) + ':' + intToString(st.wSecond);
	if (df == dateformat)
		return dateStr;
	if (df == timeformat)
		return timeStr;
	return dateStr + " " + timeStr;
}
//----< return file size >---------------------------------------------

size_t FileInfo::size() const
{
	return (size_t)(data.nFileSizeLow + (data.nFileSizeHigh << 8));
}
//----< is type archive? >---------------------------------------------

bool FileInfo::isArchive() const
{
	return (data.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) != 0;
}
//----< is type compressed? >------------------------------------------

bool FileInfo::isCompressed() const
{
	return (data.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED) != 0;
}
//----< is type directory? >-------------------------------------------

bool FileInfo::isDirectory() const
{
	return (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
}
//----< is type encrypted? >---------------------------------------------

bool FileInfo::isEncrypted() const
{
	return (data.dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED) != 0;
}
//----< is type hiddent? >---------------------------------------------

bool FileInfo::isHidden() const
{
	return (data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0;
}
//----< is type normal? >---------------------------------------------

bool FileInfo::isNormal() const
{
	return (data.dwFileAttributes & FILE_ATTRIBUTE_NORMAL) != 0;
}
//----< is type offline? >---------------------------------------------

bool FileInfo::isOffLine() const
{
	return (data.dwFileAttributes & FILE_ATTRIBUTE_OFFLINE) != 0;
}
//----< is type readonly? >--------------------------------------------

bool FileInfo::isReadOnly() const
{
	return (data.dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0;
}
//----< is type system? >----------------------------------------------

bool FileInfo::isSystem() const
{
	return (data.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) != 0;
}
//----< is type temporary? >-------------------------------------------

bool FileInfo::isTemporary() const
{
	return (data.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY) != 0;
}
//----< compare names alphabetically >---------------------------------

bool FileInfo::operator<(const FileInfo& fi) const
{
	return strcmp(data.cFileName, fi.data.cFileName) == -1;
}
//----< compare names alphabetically >---------------------------------

bool FileInfo::operator==(const FileInfo& fi) const
{
	return strcmp(data.cFileName, fi.data.cFileName) == 0;
}
//----< compare names alphabetically >---------------------------------

bool FileInfo::operator>(const FileInfo& fi) const
{
	return strcmp(data.cFileName, fi.data.cFileName) == 1;
}
//----< compare file times >-------------------------------------------

bool FileInfo::earlier(const FileInfo& fi) const
{
	FILETIME ft1 = data.ftLastWriteTime;
	FILETIME ft2 = fi.data.ftLastWriteTime;
	return ::CompareFileTime(&ft1, &ft2) == -1;
}
//----< compare file times >-------------------------------------------

bool FileInfo::later(const FileInfo& fi) const
{
	FILETIME ft1 = data.ftLastWriteTime;
	FILETIME ft2 = fi.data.ftLastWriteTime;
	return ::CompareFileTime(&ft1, &ft2) == 1;
}
//----< smaller >------------------------------------------------------

bool FileInfo::smaller(const FileInfo &fi) const
{
	return size() < fi.size();
}
//----< larger >-------------------------------------------------------

bool FileInfo::larger(const FileInfo &fi) const
{
	return size() > fi.size();
}
//----< convert string to lower case chars >---------------------------

std::string Path::toLower(const std::string& src)
{
	std::string temp;
	for (size_t i = 0; i<src.length(); ++i)
		temp += tolower(src[i]);
	return temp;
}
//----< convert string to upper case chars >---------------------------


std::string Path::toUpper(const std::string& src)
{
	std::string temp;
	for (size_t i = 0; i<src.length(); ++i)
		temp += toupper(src[i]);
	return temp;
}
//----< get path from fileSpec >---------------------------------------

std::string Path::getName(const std::string &fileSpec, bool withExt)
{
	size_t pos = fileSpec.find_last_of("/");
	if (pos >= fileSpec.length())
	{
		pos = fileSpec.find_last_of("\\");
		if (pos >= fileSpec.length())
		{
			// no path prepended
			if (withExt)
				return fileSpec;
			else
			{
				// remove ext
				size_t pos = fileSpec.find(".");
				if (pos > fileSpec.size())
					return fileSpec;
				return fileSpec.substr(0, pos - 1);
			}
		}
	}
	if (withExt)
		return fileSpec.substr(pos + 1, fileSpec.length() - pos);
	else
	{
		// remove ext
		size_t pos2 = fileSpec.find(".", pos);
		if (pos2 > fileSpec.size())
			// no ext
			return fileSpec.substr(pos + 1);
		return fileSpec.substr(pos + 1, pos2 - pos - 1);
	}
}
//----< get extension from fileSpec >----------------------------------

std::string Path::getExt(const std::string& fileSpec)
{
	size_t pos1 = fileSpec.find_last_of('/');
	size_t pos2 = fileSpec.find_last_of('\\');
	size_t pos = fileSpec.find_last_of('.');
	// handle ../ or ..\\ with no extension
	if (pos1 < fileSpec.length() || pos2 < fileSpec.length())
	{
		if (pos < min(pos1, pos2))
			return std::string("");
	}
	// only . is extension delimiter
	if (0 <= pos && pos < fileSpec.length())
		return toLower(fileSpec.substr(pos + 1, fileSpec.length() - pos));
	return std::string("");
}
//----< get path from fileSpec >---------------------------------------

std::string Path::getPath(const std::string &fileSpec)
{
	size_t pos = fileSpec.find_last_of("/");
	if (pos >= fileSpec.length())
		pos = fileSpec.find_last_of("\\");
	if (pos >= fileSpec.length())
		return ".";
	if (fileSpec.find(".", pos + 1))
		return fileSpec.substr(0, pos + 1);
	return fileSpec;
}
//----< get absoluth path from fileSpec >------------------------------

std::string Path::getFullFileSpec(const std::string &fileSpec)
{
	const size_t BufSize = 256;
	char buffer[BufSize];
	char filebuffer[BufSize];  // don't use but GetFullPathName will
	char* name = filebuffer;
	::GetFullPathNameA(fileSpec.c_str(), BufSize, buffer, &name);
	return std::string(buffer);
}
//----< create file spec from path and name >--------------------------

std::string Path::fileSpec(const std::string &path, const std::string &name)
{
	std::string fs;
	size_t len = path.size();
	if (path[len - 1] == '/' || path[len - 1] == '\\')
		fs = path + name;
	else
	{
		if (path.find("/") < path.size())
			fs = path + "/" + name;
		else if (path.find("\\") < path.size())
			fs = path + "\\" + name;
		else
			fs = path + "/" + name;
	}
	return fs;
}
//----< return name of the current directory >-----------------------------

std::string Directory::getCurrentDirectory()
{
	char buffer[MAX_PATH];
	::GetCurrentDirectoryA(MAX_PATH, buffer);
	return std::string(buffer);
}
//----< change the current directory to path >-----------------------------

bool Directory::setCurrentDirectory(const std::string& path)
{
	return ::SetCurrentDirectoryA(path.c_str()) != 0;
}
//----< get names of all the files matching pattern (path:name) >----------

std::vector<std::string> Directory::getFiles(const std::string& path, const std::string& pattern)
{
	std::vector<std::string> files;
	FileSystemSearch fss;
	std::string file = fss.firstFile(path, pattern);
	if (file.size() == 0)
		return files;
	files.push_back(file);
	while (true)
	{
		file = fss.nextFile();
		if (file.size() == 0)
			return files;
		files.push_back(file);
	}
	return files;
}
//----< get names of all directories matching pattern (path:name) >--------

std::vector<std::string> Directory::getDirectories(const std::string& path, const std::string& pattern)
{
	std::vector<std::string> dirs;
	FileSystemSearch fss;
	std::string dir = fss.firstDirectory(path, pattern);
	if (dir.size() == 0)
		return dirs;
	dirs.push_back(dir);
	while (true)
	{
		dir = fss.nextDirectory();
		if (dir.size() == 0)
			return dirs;
		dirs.push_back(dir);
	}
	return dirs;
}
//----< create directory >-------------------------------------------------

bool Directory::create(const std::string& path)
{
	return ::CreateDirectoryA(path.c_str(), NULL) == 0;
}
//----< does directory exist? >--------------------------------------------

bool Directory::exists(const std::string& path)
{
	DWORD dwAttrib = GetFileAttributesA(path.c_str());

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}
//----< remove directory >-------------------------------------------------

bool Directory::remove(const std::string& path)
{
	return ::RemoveDirectoryA(path.c_str()) == 0;
}
//----< find first file >--------------------------------------------------

std::string FileSystemSearch::firstFile(const std::string& path, const std::string& pattern)
{
	hFindFile = ::FindFirstFileA(Path::fileSpec(path, pattern).c_str(), pFindFileData);
	if (hFindFile != INVALID_HANDLE_VALUE)
	{
		if (!(pFindFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			return pFindFileData->cFileName;
		else
			while (::FindNextFileA(hFindFile, pFindFileData))
				if (!(pFindFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
					return pFindFileData->cFileName;
	}
	return "";
}
//----< find next file >---------------------------------------------------

std::string FileSystemSearch::nextFile()
{
	while (::FindNextFileA(hFindFile, pFindFileData))
		if (!(pFindFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			return pFindFileData->cFileName;
	return "";
}
//----< find first file >--------------------------------------------------

std::string FileSystemSearch::firstDirectory(const std::string& path, const std::string& pattern)
{
	hFindFile = ::FindFirstFileA(Path::fileSpec(path, pattern).c_str(), pFindFileData);
	if (hFindFile != INVALID_HANDLE_VALUE)
	{
		if (pFindFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			return pFindFileData->cFileName;
		else
			while (::FindNextFileA(hFindFile, pFindFileData))
				if (pFindFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					return pFindFileData->cFileName;
	}
	return "";
}
//----< find next file >---------------------------------------------------

std::string FileSystemSearch::nextDirectory()
{
	while (::FindNextFileA(hFindFile, pFindFileData))
		if (pFindFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			return pFindFileData->cFileName;
	return "";
}
//----< test stub >--------------------------------------------------------

#ifdef TEST_FILESYSTEM

void title(const std::string& title, char ch = '=')
{
	std::cout << "\n  " << title;
	std::cout << "\n " << std::string(title.size() + 2, ch);
}
int main(int argc, char* argv[])
{
	title("Demonstrate Directory class");

	std::cout << "\n  current directory is:\n    " << Directory::getCurrentDirectory();
	std::cout << "\n  It contains files:";
	std::vector<std::string> currfiles = Directory::getFiles();
	for (size_t i = 0; i<currfiles.size(); ++i)
		std::cout << "\n    " << currfiles[i].c_str();
	std::cout << "\n  and contains directories:";
	std::vector<std::string> currdirs = Directory::getDirectories();
	for (size_t i = 0; i<currdirs.size(); ++i)
		std::cout << "\n    " << currdirs[i].c_str();
	std::cout << "\n";

	std::string currDir = Directory::getCurrentDirectory();
	Directory::setCurrentDirectory("c:/temp/");
	for (size_t i = 0; i<currfiles.size(); ++i)
		std::cout << "\n    " << Path::getFullFileSpec(currfiles[i]).c_str();
	Directory::setCurrentDirectory(currDir);

	for (size_t i = 0; i<currfiles.size(); ++i)
		std::cout << "\n    " << Path::fileSpec("c:\\temp\\", currfiles[i]).c_str();

	std::cout << "\n";
	std::cout << "\n  directories residing in C:/temp are:";
	currdirs = Directory::getDirectories("c:/temp/");
	for (size_t i = 0; i<currdirs.size(); ++i)
		std::cout << "\n    " << currdirs[i].c_str();
	std::cout << "\n";
}
#endif
