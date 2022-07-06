
## MariaDB: cmake
/usr/bin/cmake -DCMAKE_BUILD_TYPE=Debug /usr/local/mysql " -DMYSQL_DATADIR=/mydata/data" " -DWITH_INNOBASE_STORAGE_ENGINE=1" " -DWITH_ARCHIVE_STORAGE_ENGINE=1" " -DWITH_BLACKHOLE_STORAGE_ENGINE=1" " -DWITH_READLINE=1" " -DWITH_SSL=system" " -DWITH_ZLIB=system" " -DWITH_LIBWRAP=0" " -DMYSQL_UNIX_ADDR=/var/lib/mysql/mysql.sock" " -DDEFAULT_CHARSET=utf8mb4" " -DDEFAULT_COLLATION=utf8mb4_general_ci" -G "CodeBlocks - Unix Makefiles" -S /tmp/mariadb-10.9.1
Code status:
------------

* [![Appveyor CI status](https://ci.appveyor.com/api/projects/status/4u6pexmtpuf8jq66?svg=true)](https://ci.appveyor.com/project/rasmushoj/server) ci.appveyor.com

## MariaDB: The open source relational database 

MariaDB was designed as a drop-in replacement of MySQL(R) with more
features, new storage engines, fewer bugs, and better performance.

MariaDB is brought to you by the MariaDB Foundation and the MariaDB Corporation.
Please read the CREDITS file for details about the MariaDB Foundation,
and who is developing MariaDB.

MariaDB is developed by many of the original developers of MySQL who
now work for the MariaDB Corporation, the MariaDB Foundation and by
many people in the community.

MySQL, which is the base of MariaDB, is a product and trademark of Oracle
Corporation, Inc. For a list of developers and other contributors,
see the Credits appendix.  You can also run 'SHOW authors' to get a
list of active contributors.

A description of the MariaDB project and a manual can be found at:

https://mariadb.org

https://mariadb.com/kb/en/

https://mariadb.com/kb/en/mariadb-vs-mysql-features/

https://mariadb.com/kb/en/mariadb-versus-mysql-compatibility/

https://mariadb.com/kb/en/new-and-old-releases/

Help
-----

More help is available from the Maria Discuss mailing list
https://launchpad.net/~maria-discuss, MariaDB's Zulip
instance, https://mariadb.zulipchat.com/ 

Live QA for beginner contributors
----
MariaDB has a dedicated time each week when we answer new contributor questions live on Zulip.
From 8:00 to 10:00 UTC on Mondays, and 10:00 to 12:00 UTC on Thursdays,
anyone can ask any questions they’d like, and a live developer will be available to assist.

New contributors can ask questions any time, but we will provide immediate feedback during that interval.

Licensing
---------

***************************************************************************

NOTE: 

MariaDB is specifically available only under version 2 of the GNU
General Public License (GPLv2). (I.e. Without the "any later version"
clause.) This is inherited from MySQL. Please see the README file in
the MySQL distribution for more information.

License information can be found in the COPYING file. Third party
license information can be found in the THIRDPARTY file.

***************************************************************************

Bug Reports
------------

Bug and/or error reports regarding MariaDB should be submitted at:
https://jira.mariadb.org

For reporting security vulnerabilities see:
https://mariadb.org/about/security-policy/

The code for MariaDB, including all revision history, can be found at:
https://github.com/MariaDB/server