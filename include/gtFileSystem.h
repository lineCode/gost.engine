/*!	GOST
	\file gtFileSystem.h
	\brief work with files, dirs etc.
*/

#pragma once
#ifndef __GT_FILE_SYSTEM_H__
#define __GT_FILE_SYSTEM_H__ ///< include guard

namespace gost{

		///	чтобы не мучится в выделении памяти в разных модулях (.dll)\n
		///	проще использовать массивы с заранее выделенной паматью. Либо делать .reserve( GT_MAX_PATH );\n
		///	Размер массива который будет хранить путь к файлу
	constexpr u32 GT_MAX_PATH = 256u;

		//	Если файл открыт в текстовом режиме (подразумевается что открываемый файл - текстовый) то происходит определение формата текстового файла
	struct gtTextFileInfo{

			///	формат текста
		enum format{

			///	utf-8
			utf_8,

			///	стандартный для gost
			utf_16,

			///	на будущее
			utf_32

			// ansi нет из за сложности определения кодовых страниц
			// если файл - ansi, то открыается как utf-8
		}m_format;

			// Порядок байтов для utf-16 и utf-32
		enum endian{

			//	0xFFFE0000
			little,

			//	0x0000FEFF
			big // not implemented !

		}m_endian;

			///	Есть ли маркер последовательности байтов
		bool m_hasBOM;
	};

		///	Интерфейс для работы с файлом
	class gtFile : public gtRefObject{
	public:

		

			//	Получить актуальную информацию о текстовом файле
			// \return Вернёт иформацию о текстовом файле
		virtual gtTextFileInfo	getTextFileInfo( void ) = 0;

			//	Установить информацию о текстовом файле.
			//	Нужно делать перед записью,
			//	обязательно вызвать flush чтобы некоторые изменения вступили в силу
		virtual void	setTextFileInfo( gtTextFileInfo info ) = 0;
			
			///	Записать двоичные данные
			/// \param data: данные которые нужно записать
			/// \param size: размер данных в байтах
		virtual u32		write( u8 * data, u32 size ) = 0;

			///	Записать текст. Работает если файл открыт в текстовом режиме.
			/// \param string: text
		virtual void	write( const gtStringA& string ) = 0;

			///	Записать текст. Работает если файл открыт в текстовом режиме.
			/// \param string: text
		virtual void	write( const gtString& string ) = 0;

			///	Записать текст. Работает если файл открыт в текстовом режиме.
			/// \param string: text
		virtual void	write( const gtString32& string ) = 0;

			///	всё что в буффере запишется в файл
		virtual void	flush( void ) = 0;

			///	Прочитать двоичные данные
			/// \param data: указатель на массив куда читать
			/// \param size: сколько прочесть байтов
		virtual u64		read( u8 * data, u64 size ) = 0;

			///	Получить размер в байтах
			/// \return Вернёт размер в байтах
		virtual u64		size( void ) = 0;

			///	Получить позицию указателя
			///	\return Вернёт позицию указателя
		virtual u64		tell( void ) = 0;

			///	Для установки позиции курсора, начинать отсчёт дистанции с
		enum SeekPos{

			///	начала
			ESP_BEGIN,

			///	текущей позиции
			ESP_CURRENT,

			///	конца(хз зачем)
			ESP_END
		};

			///	установить позицию указателя
			/// \param distance: как далеко от \b pos надо поставить указатель (в байтах)
			/// \param pos: откуда начинать отсчёт \b distance
		virtual void	seek( u64 distance, SeekPos pos ) = 0;
	};

	///	для краткости
#define gtFile_t gtPtr<gtFile>
	
		///	класс для работы с файлами и папками
	class gtFileSystem : public gtRefObject{
	public:

			///	Режим доступа к файлу
		enum FileAccessMode{

				///	только чтение
			EFAM_READ,

				///	только запись
			EFAM_WRITE,

				///	чтение и запись
			EFAM_BOTH,
			
				/// добавление в конец файла, для текстового режима
			EFAM_APPEND
		};

			///	Режим открытия файла. По сути TEXT это расширение над BINARY, то есть в текстовом режиме можно записать двоичные данные
		enum FileMode{

				///	для того чтобы открыть текстовый файл и получить информацию о тексте
			EFM_TEXT,

				///	для того чтобы открыть файл для чтения/записи двоичных данных
			EFM_BINARY
		};

			///	Может ли открытый файл использоваться другими процессами
		enum FileShareMode{

				///	файл доступен только этому процессу
			EFSM_NONE,

				///	файл доступен другим процессам для удаления
			EFSM_DELETE,

				///	файл доступен другим процессам для чтения
			EFSM_READ,

				///	файл доступен другим процессам для записи
			EFSM_WRITE
		};

			/// Действие при вызове метода createFile
		enum FileAction{

				///	создаёт/открывает новый файл, сохраняя содержимое если оно есть
			EFA_OPEN,

				///	всегда создаёт/открывает новый файл, удаляя содержимое если оно есть
			EFA_OPEN_NEW,

		};

			///	Аттрибуты для файла
		enum FileAttribute{

				///	обычный файл
			EFA_NORMAL,

				///	скрытый файл
			EFA_HIDDEN,

				///	файл только для чтенияы
			EFA_READONLY
		};

			///	Создать или открыть файл (если доступен)
			/// \param fileName: имя файла
			/// \param mode: режим
			/// \param access: доступ
			/// \param action: действие
			/// \param shareMode: владение
			/// \param attributeFlags: аттрибуты
			/// \return \attention Вернётся открытый файл. В целях тестирования возвращается gtPtrNew. Если это окажется удобным, то все gtRefObjecs будут возвращаться подобным образом
		GT_API static gtPtrNew<gtFile> createFile( 
			const gtString& fileName,
			FileMode mode,
			FileAccessMode access,
			FileAction action,
			FileShareMode shareMode = FileShareMode::EFSM_NONE,
			u32 attributeFlags = FileAttribute::EFA_NORMAL
		);

			///	Удалить файл
			/// \param file: путь к файлу
			/// \return \b true если успех
			/// \remark Успех будет в том случае если файл существует, и программа имеет доступ для его удаления
		GT_API static bool deleteFile( const gtString& file );

			///	Удалить папку
			/// \param file: путь к папке
			/// \return \b true если успех
			/// \remark Успех будет в том случае если папка существует, и программа имеет доступ для её удаления
		GT_API static bool deleteDir( const gtString& dir );

			/// Проверить доступность файла
			/// \param file: путь к файлу
			/// \return \b true если доступен, \b false если не доступен
		GT_API static bool existFile( const gtString& file );

			/// Проверить доступность папки
			/// \param file: путь к папке
			/// \return \b true если доступна, \b false если не доступна
		GT_API static bool existDir( const gtString& dir );

			//	Создать папку
			// \param dir: путь к новой папке
			// \return \b true если успех
		GT_API static bool createDir( const gtString& dir );

			//	тип объекта. нужно для сканирования папки
		enum class DirObjectType{
			
				// для точек . ..
			info,

				//	папка
			folder, 

				//	файл
			file 
		};

			///	объект дирректории
		struct DirObject{
			
				/// конструктор
			DirObject( void ){
				memset( path, 0u, GT_MAX_PATH * sizeof(wchar_t) );
			}

				///	полный путь
			wchar_t path[GT_MAX_PATH];

				///	тип
			DirObjectType type;

				///	размер
			u32 size;
		};

			///	Начать сканирование папки. Перед сканированием папки нужно вызвать это
			/// \param dir: путь к папке для сканирования
		GT_API static void scanDirBegin( gtString dir );

			///	Завершает сканирование. После завершения сканирования нужно вызвать это
		GT_API static void scanDirEnd( void );

			///	Заполнить структуру DirObject если есть файл/папка.
			/// \param inout: указатель на DirObject
			/// \return \b true если успех, \b false если файла/папки нет, или сканирование папки завершено
		GT_API static bool getDirObject( DirObject* inout );

			///	Копировать существующий файл.
			/// \param existingFileName: путь к файлу
			/// \param newFileName: имя для нового файла
			/// \param overwrite: \b true если нужно перезаписать
			/// \return \b true если успех
		GT_API static bool copyFile( const gtString& existingFileName, const gtString& newFileName, bool overwrite );

			///	Получить путь к папке в которой лежит исполняемый файл программы, например "C:/Games/SuperGame/"
			/// \return Вернёт путь к папке
		GT_API static gtString getProgramPath( void );

			///	Получить путь к системной папке
			/// \return Вернёт путь к системной папке
		GT_API static gtString getSystemPath( void );

			//	Переведёт путь вида например '../media/file.ext' в полный путь
		GT_API static gtString getRealPath( const gtString& in );

	};

	namespace util{
	
		/**
			Укороченные версии функций.
			Нужно будет дополнить для двоичных файлов.
		*/

		GT_FORCE_INLINE gtPtrNew<gtFile> openFileForReadText( const gtString& fileName ){
			return gtFileSystem::createFile( fileName, gtFileSystem::FileMode::EFM_TEXT, gtFileSystem::FileAccessMode::EFAM_READ, 
				gtFileSystem::FileAction::EFA_OPEN );
		}
		
		GT_FORCE_INLINE gtPtrNew<gtFile> createFileForReadText( const gtString& fileName ){
			return gtFileSystem::createFile( fileName, gtFileSystem::FileMode::EFM_TEXT, gtFileSystem::FileAccessMode::EFAM_READ, 
				gtFileSystem::FileAction::EFA_OPEN_NEW );
		}

		GT_FORCE_INLINE gtPtrNew<gtFile> openFileForWriteText( const gtString& fileName ){
			return gtFileSystem::createFile( fileName, gtFileSystem::FileMode::EFM_TEXT, gtFileSystem::FileAccessMode::EFAM_APPEND, 
				gtFileSystem::FileAction::EFA_OPEN );
		}
		
		GT_FORCE_INLINE	gtPtrNew<gtFile> createFileForWriteText( const gtString& fileName ){
			return gtFileSystem::createFile( fileName, gtFileSystem::FileMode::EFM_TEXT, gtFileSystem::FileAccessMode::EFAM_WRITE,
				gtFileSystem::FileAction::EFA_OPEN_NEW );
		}

		GT_FORCE_INLINE gtPtrNew<gtFile> openFileForReadBin( const gtString& fileName ){
			return gtFileSystem::createFile( fileName, gtFileSystem::FileMode::EFM_BINARY, gtFileSystem::FileAccessMode::EFAM_READ, 
				gtFileSystem::FileAction::EFA_OPEN );
		}

		GT_FORCE_INLINE gtPtrNew<gtFile> openFileForReadBinShared( const gtString& fileName ){
			return gtFileSystem::createFile( fileName, gtFileSystem::FileMode::EFM_BINARY, gtFileSystem::FileAccessMode::EFAM_READ, 
				gtFileSystem::FileAction::EFA_OPEN, gtFileSystem::EFSM_READ );
		}

		GT_FORCE_INLINE gtPtrNew<gtFile> openFileForWriteBin( const gtString& fileName ){
			return gtFileSystem::createFile( fileName, gtFileSystem::FileMode::EFM_BINARY, gtFileSystem::FileAccessMode::EFAM_WRITE, 
				gtFileSystem::FileAction::EFA_OPEN );
		}

		GT_FORCE_INLINE gtPtrNew<gtFile> createFileForWriteBin( const gtString& fileName ){
			return gtFileSystem::createFile( fileName, gtFileSystem::FileMode::EFM_BINARY, gtFileSystem::FileAccessMode::EFAM_WRITE, 
				gtFileSystem::FileAction::EFA_OPEN_NEW );
		}
	}


}

#endif

/*
Copyright (c) 2017, 2018 532235

Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
and associated documentation files (the "Software"), to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT 
LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/