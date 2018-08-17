# gost
2D/3D Game Engine based on plugins for C++11

gost - это самописный игровой движок, который создаётся для личных нужд (не для конкуренции с топами). Главная особенность этого проекта - независимость. Конечно, быть полностью независимым не получится, но, в некоторых местах она достигается. А этих мест очень много. 

Например:

- Движок состоит из модулей - подключаемых динамических библиотек (в gost называемых плагинами). Основной модуль движка (gost.dll) независим от каких либо библиотек, и может собираться практически для любой системы.

- Поддерживаемые технологии хранятся в плагинах, они не вшиты в движок как во многих других системах. Если какую-то технологию вы не желаете использовать (например из за её лицензирования), вы можете просто не использовать этот плагин.

- Использование STL, как и любых подобных, делает проект очень зависимым. STL создан для удовлетворения потребностей различных по специализации программистов и не программистов(напр. химиков, биологов, метеорологов и т.д.), существенно снижая производительность программ, например таких как игры. gost имеет базовые аналоги, простые, быстрые и эффективные.

- Так как движок состоит из плагинов, можно без особых усилий создать свой плагин. Все базовые плагины созданы с простым стилем кода (как в туториалах), без разных ухищрений, по этому их легко можно изучить.

<a href="http://www.youtube.com/watch?feature=player_embedded&v=VYMdwpfL27g
" target="_blank"><img src="http://img.youtube.com/vi/VYMdwpfL27g/0.jpg" 
alt="gost demo - basic physics" width="240" height="180" border="10" /></a><a href="http://www.youtube.com/watch?feature=player_embedded&v=R-4EsYBYTS0
" target="_blank"><img src="http://img.youtube.com/vi/R-4EsYBYTS0/0.jpg" 
alt="gost demo - ray test" width="240" height="180" border="10" /></a><a href="http://www.youtube.com/watch?feature=player_embedded&v=NULUuxLR1y0
" target="_blank"><img src="http://img.youtube.com/vi/NULUuxLR1y0/0.jpg" 
alt="gost demo - picking" width="240" height="180" border="10" /></a>
<a href="http://www.youtube.com/watch?feature=player_embedded&v=zM-_3Nj6e7Q
" target="_blank"><img src="http://img.youtube.com/vi/zM-_3Nj6e7Q/0.jpg" 
alt="gost sprite test" width="240" height="180" border="10" /></a>

## Build

gost можно построить имея компилятор с поддержкой C++11.

Проект настроен для построения с использованием MinGW. Для удобного запуска построения, была создана утилита gost.build.exe (она находится в папке tools). Эта утилита похожа на программу make, только намного проще в использовании.
gost.build.exe принимает в качестве аргумента текстовый файл(имя не имеет значения). В этом файле описываются параметры проекта и указывается список файлов. В параметрах проекта необходимо указать папку расположения компилятора, имена компиляторов, указать тип компилятора (пока реализован GCC), указать программу для линковки, указать опции для компилятора и линковщика и т.д.
gost.build.exe создан для построения gost, нет задачи создать универсальный инструмент, по этому существуют некоторые особенности.
Для компиляции необходимо указать папку с проектом. Поиск include файлов, библиотек, создание файлов и т.д. выполняется в указанной папке с проектом.
Программа не создаёт директории, по этому нужно позаботится об этом заранее.
Программа помещает все объектные файлы в одну папку, по этому нельзя допускать использования файлов исходных кодов с одинаковыми именами.

Для автоматизации удаления старых файлов и создания необходимых папок, созданы соответствующие .bat файлы.

Построение на Linux пока не реализовано, ибо не реализованы все ОС зависимые функции в самом движке.

## Features
* D3D11 render plugin
* .obj model import
* .png .bmp image import
* 2D sprite with animation
* audio loading/audio streaming (.wav .ogg)
* XML read/write
* Bullet Physics plugin

## Documentation

Wiki for samples https://github.com/532235/GoST/wiki

## Contributing

If you want to help the project, write to the email address below.
Main conditions:
- Avoid using standard (std::) things, try not to use STL, if used, then only in special places and in small quantities (for example, in small plug-ins).
- use a similar style that is used in the code.

## Contacts

admin@imsoftworks.info


# Special thanks

* Nikita Kogut ([MrOnlineCoder](https://github.com/MrOnlineCoder))
