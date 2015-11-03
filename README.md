DnFsMapCopy
==========
 
Copyright 2015 Takashi Inoue e01b0420@kyoto-kcg.ac.jp


DnFsMapCopy is free file(s)/dir(s) copy/move utility for windows. 

Drag and drop is enabled to append files and directories.Regular expression is enabled to append directories to destination.
Copying or moving are enabled only when destination count and source count are equal.
Their are mapped one-on-one.

##**ex.**  
F:/test[0-9]+/hoge/hage/foo/bar

analyze >>>

[Destinations]  
F:/test001/hoge/hage/foo/bar  
F:/test002/hoge/hage/foo/bar

[Sources]  
D:/001.txt  
F:/test003

[Copy/Move]  
D:/001.txt >>> F:/test001/hoge/hage/foo/bar  
F:/test003 >>> F:/test002/hoge/hage/foo/bar  

###**tools**
Qt Creator 3.5.1 + Qt 5.5.1 + MSVC2013 EE


###**License** 
DnFsMapCopy is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

DnFsMapCopy is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with DnFsMapCopy. If not, see http://www.gnu.org/licenses/.
