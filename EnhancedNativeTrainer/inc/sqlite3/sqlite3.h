/*
** 2001 年 9 月 15 日
**
** 作者放弃对此源代码的版权，作为法律声明的替代。
** 这里是一段祝福：
**
**    愿你行善而不作恶。
**    愿你找到对自己的宽恕并宽恕他人。
**    愿你自由分享，永远索取不超过你给予的。
**
*************************************************************************
** 此头文件定义了 SQLite 库向客户端程序提供的接口。
** 如果某个 C 函数、结构体、数据类型或常量定义未出现在此文件中，
** 则它不是 SQLite 的公开 API，可能会在没有通知的情况下更改，
** 并且不应被使用 SQLite 的程序引用。
**
** 此文件中的某些定义被标记为“实验性”。
** 实验性接口通常是最近添加到 SQLite 的新功能。
** 我们不预期对实验性接口进行更改，但保留进行微小更改的权利，
** 如果实际使用经验表明此类更改是明智的。
**
** SQLite 的官方 C 语言 API 文档源自此文件中的注释。
** 此文件是关于 SQLite 接口应如何操作的权威来源。
**
** 此文件在配置管理中的名称为 "sqlite.h.in"。
** 在构建过程中，makefile 会对此文件进行一些微小更改
** （例如插入版本号），并将其名称更改为 "sqlite3.h"。
*/
#ifndef _SQLITE3_H_
#define _SQLITE3_H_
#include <stdarg.h>     /* 需要用于定义 va_list */

/*
** 确保我们可以从 C++ 中调用这些内容。
*/
#ifdef __cplusplus
extern "C" {
#endif


/*
** 提供覆盖接口链接特性的能力。
*/
#ifndef SQLITE_EXTERN
# define SQLITE_EXTERN extern
#endif
#ifndef SQLITE_API
# define SQLITE_API
#endif
#ifndef SQLITE_CDECL
# define SQLITE_CDECL
#endif
#ifndef SQLITE_STDCALL
# define SQLITE_STDCALL
#endif

/*
** 这些空操作宏用于在接口前标记这些接口为已弃用或实验性。
** 新应用程序不应使用已弃用的接口——它们仅用于向后兼容。
** 应用程序开发者应注意，实验性接口可能会在点版本中更改。
**
** 这些宏过去会解析为各种编译器魔法，在使用时会生成警告消息。
** 但该编译器魔法最终引发了大量错误报告，因此我们已将其全部移除，
** 并重新使用简单的空操作宏。
*/
#define SQLITE_DEPRECATED
#define SQLITE_EXPERIMENTAL

/*
** 确保这些符号没有被之前的头文件定义。
*/
#ifdef SQLITE_VERSION
# undef SQLITE_VERSION
#endif
#ifdef SQLITE_VERSION_NUMBER
# undef SQLITE_VERSION_NUMBER
#endif

/*
** CAPI3REF: 编译时库版本号
**
** ^([sqlite3.h 头文件中的 [SQLITE_VERSION] C 预处理器宏
** 求值为一个字符串字面量，表示 SQLite 的版本号，格式为 "X.Y.Z"，
** 其中 X 是主版本号（对于 SQLite3 始终为 3），
** Y 是次版本号，Z 是发布号。)^
** ^([SQLITE_VERSION_NUMBER] C 预处理器宏解析为一个整数，
** 其值为 (X*1000000 + Y*1000 + Z)，其中 X、Y 和 Z 是
** [SQLITE_VERSION] 中使用的相同数字。)^
** 任何给定版本的 SQLITE_VERSION_NUMBER 也会大于其衍生版本。
** 要么 Y 保持不变且 Z 递增，要么 Y 递增且 Z 重置为零。
**
** 自版本 3.6.18 起，SQLite 源代码已存储在
** <a href="http://www.fossil-scm.org/">Fossil 配置管理系统</a> 中。
** ^SQLITE_SOURCE_ID 宏求值为一个字符串，用于标识
** 配置管理系统中 SQLite 的特定检入。^SQLITE_SOURCE_ID
** 字符串包含检入的日期和时间（UTC）以及整个源代码树的 SHA1 哈希值。
**
** 另请参阅：[sqlite3_libversion()]、
** [sqlite3_libversion_number()]、[sqlite3_sourceid()]、
** [sqlite_version()] 和 [sqlite_source_id()]。
*/
#define SQLITE_VERSION        "3.8.11"
#define SQLITE_VERSION_NUMBER 3008011
#define SQLITE_SOURCE_ID      "2015-05-25 09:33:48 8c3929bd42ff9c041df341aced5b7dbc0f563506"

/*
** CAPI3REF: 运行时库版本号
** 关键字: sqlite3_version, sqlite3_sourceid
**
** 这些接口提供与 [SQLITE_VERSION]、[SQLITE_VERSION_NUMBER] 和 [SQLITE_SOURCE_ID] C 预处理器宏相同的信息，
** 但它们与库相关联，而不是头文件。^(谨慎的程序员可能会在其应用程序中包含 assert() 语句，
** 以验证这些接口返回的值与头文件中的宏匹配，从而确保应用程序使用匹配的库和头文件进行编译。
**
** <blockquote><pre>
** assert( sqlite3_libversion_number()==SQLITE_VERSION_NUMBER );
** assert( strcmp(sqlite3_sourceid(),SQLITE_SOURCE_ID)==0 );
** assert( strcmp(sqlite3_libversion(),SQLITE_VERSION)==0 );
** </pre></blockquote>)^
**
** ^sqlite3_version[] 字符串常量包含 [SQLITE_VERSION] 宏的文本。
** ^sqlite3_libversion() 函数返回指向 sqlite3_version[] 字符串常量的指针。
** sqlite3_libversion() 函数是为 DLL 提供的，因为 DLL 用户通常无法直接访问 DLL 中的字符串常量。
** ^sqlite3_libversion_number() 函数返回一个等于 [SQLITE_VERSION_NUMBER] 的整数。
** ^sqlite3_sourceid() 函数返回一个指向字符串常量的指针，其值与 [SQLITE_SOURCE_ID] C 预处理器宏相同。
**
** 另请参阅: [sqlite_version()] 和 [sqlite_source_id()]。
*/
SQLITE_API SQLITE_EXTERN const char sqlite3_version[];
SQLITE_API const char *SQLITE_STDCALL sqlite3_libversion(void);
SQLITE_API const char *SQLITE_STDCALL sqlite3_sourceid(void);
SQLITE_API int SQLITE_STDCALL sqlite3_libversion_number(void);

/*
** CAPI3REF: 运行时库编译选项诊断
**
** ^sqlite3_compileoption_used() 函数返回 0 或 1，表示指定选项是否在编译时定义。^传递给 sqlite3_compileoption_used() 的选项名称可以省略 SQLITE_ 前缀。
**
** ^sqlite3_compileoption_get() 函数允许通过返回第 N 个编译时选项字符串来遍历在编译时定义的选项列表。^如果 N 超出范围，sqlite3_compileoption_get() 返回一个空指针。^由 sqlite3_compileoption_get() 返回的任何字符串都会省略 SQLITE_ 前缀。
**
** ^如果在编译时指定了 [SQLITE_OMIT_COMPILEOPTION_DIAGS] 选项，则可以省略对诊断函数 sqlite3_compileoption_used() 和 sqlite3_compileoption_get() 的支持。
**
** 另见：SQL 函数 [sqlite_compileoption_used()] 和 [sqlite_compileoption_get()] 以及 [compile_options pragma]。
*/
#ifndef SQLITE_OMIT_COMPILEOPTION_DIAGS
SQLITE_API int SQLITE_STDCALL sqlite3_compileoption_used(const char *zOptName);
SQLITE_API const char *SQLITE_STDCALL sqlite3_compileoption_get(int N);
#endif

/*
** CAPI3REF: 测试库是否线程安全
**
** ^sqlite3_threadsafe() 函数仅当 SQLite 由于 [SQLITE_THREADSAFE] 编译时选项设置为 0
** 而省略了互斥代码时返回零。
**
** SQLite 可以带有或不带互斥锁进行编译。当 [SQLITE_THREADSAFE] C 预处理器宏为 1 或 2 时，
** 启用互斥锁，SQLite 是线程安全的。当 [SQLITE_THREADSAFE] 宏为 0 时，
** 互斥锁被省略。没有互斥锁的情况下，从多个线程并发使用 SQLite 是不安全的。
**
** 启用互斥锁会导致可衡量的性能损失。
** 因此，如果速度至关重要，禁用互斥锁是有意义的。但为了最大安全性，应启用互斥锁。
** ^默认行为是启用互斥锁。
**
** 此接口可用于应用程序确保其链接的 SQLite 版本使用 [SQLITE_THREADSAFE] 宏的所需设置进行编译。
**
** 此接口仅报告 [SQLITE_THREADSAFE] 标志的编译时互斥设置。
** 如果 SQLite 编译时设置了 SQLITE_THREADSAFE=1 或 =2，则默认启用互斥锁，
** 但可以使用带有 [SQLITE_CONFIG_SINGLETHREAD]、[SQLITE_CONFIG_MULTITHREAD] 或
** [SQLITE_CONFIG_SERIALIZED] 参数的 [sqlite3_config()] 调用完全或部分禁用互斥锁。
** ^(sqlite3_threadsafe() 函数的返回值仅显示线程安全的编译时设置，而不是由
** sqlite3_config() 在运行时对该设置所做的任何更改。换句话说，sqlite3_threadsafe()
** 的返回值不会因对 sqlite3_config() 的调用而改变。) ^
**
** 有关更多信息，请参阅 [线程模式] 文档。
*/
SQLITE_API int SQLITE_STDCALL sqlite3_threadsafe(void);

/*
** CAPI3REF: 数据库连接句柄
** 关键词: {数据库连接} {数据库连接}
**
** 每个打开的SQLite数据库都由指向名为"sqlite3"的不透明结构体的实例的指针表示。
** 将sqlite3指针视为对象是很有用的。[sqlite3_open()], [sqlite3_open16()], 和
** [sqlite3_open_v2()]接口是其构造函数，而[sqlite3_close()]和[sqlite3_close_v2()]
** 是其析构函数。还有许多其他接口（如
** [sqlite3_prepare_v2()], [sqlite3_create_function()], 和
** [sqlite3_busy_timeout()]仅举三个）是sqlite3对象的方法。
*/
typedef struct sqlite3 sqlite3;

/*
** CAPI3REF: 64位整数类型
** 关键词: sqlite_int64 sqlite_uint64
**
** 由于没有跨平台的方法来指定64位整数类型，SQLite包含了64位有符号和无符号整数的类型定义。
**
** sqlite3_int64 和 sqlite3_uint64 是推荐使用的类型定义。sqlite_int64 和 sqlite_uint64 类型仅为了向后兼容而支持。
**
** ^sqlite3_int64 和 sqlite_int64 类型可以存储介于 -9223372036854775808 和 +9223372036854775807 之间的整数值（包括边界值）。^sqlite3_uint64 和 sqlite_uint64 类型可以存储介于 0 和 +18446744073709551615 之间的整数值（包括边界值）。
*/
#ifdef SQLITE_INT64_TYPE
  typedef SQLITE_INT64_TYPE sqlite_int64;
  typedef unsigned SQLITE_INT64_TYPE sqlite_uint64;
#elif defined(_MSC_VER) || defined(__BORLANDC__)
  typedef __int64 sqlite_int64;
  typedef unsigned __int64 sqlite_uint64;
#else
  typedef long long int sqlite_int64;
  typedef unsigned long long int sqlite_uint64;
#endif
typedef sqlite_int64 sqlite3_int64;
typedef sqlite_uint64 sqlite3_uint64;

/*
** 如果编译的目标处理器缺乏浮点数支持，
** 则用整数替代浮点数。
*/
#ifdef SQLITE_OMIT_FLOATING_POINT
# define double sqlite3_int64
#endif

/**
** CAPI3REF: 关闭数据库连接
** DESTRUCTOR: sqlite3
**
** ^sqlite3_close() 和 sqlite3_close_v2() 函数是 [sqlite3] 对象的析构函数。
** ^如果成功销毁 [sqlite3] 对象并释放所有相关资源，则调用 sqlite3_close() 和 sqlite3_close_v2() 返回 [SQLITE_OK]。
**
** ^如果数据库连接与未完成的准备语句或未完成的 sqlite3_backup 对象相关联，则 sqlite3_close() 将保持数据库连接打开并返回 [SQLITE_BUSY]。
** ^如果 sqlite3_close_v2() 被调用时存在未完成的准备语句和/或未完成的 sqlite3_backups，则数据库连接将变成一个不可用的“僵尸”，当最后一个准备语句被最终化或最后一个 sqlite3_backup 完成，它将自动被释放。sqlite3_close_v2() 接口旨在用于带有垃圾回收的主语言，其中析构函数的调用顺序是任意的。
**
** 应用程序应在尝试关闭对象之前，[sqlite3_finalize | 最终化] 所有 [准备语句]，[sqlite3_blob_close | 关闭] 所有 [BLOB 句柄]，并 
** [sqlite3_backup_finish | 完成] 所有与 [sqlite3] 对象关联的 [sqlite3_backup] 对象。  ^如果在仍有未完成的 [准备语句]，[BLOB 句柄] 和/或
** [sqlite3_backup] 对象的情况下调用 sqlite3_close_v2()，则它返回 [SQLITE_OK]，并且资源的释放将推迟到所有 [准备语句]，[BLOB 句柄] 和
** [sqlite3_backup] 对象也被销毁为止。
**
** 如果在事务打开时销毁 [sqlite3] 对象，则事务将自动回滚。
**
** [sqlite3_close(C)] 和 [sqlite3_close_v2(C)] 的 C 参数必须是一个 NULL
** 指针或从 [sqlite3_open()]，[sqlite3_open16()] 或
** [sqlite3_open_v2()] 获得的 [sqlite3] 对象指针，并且之前未关闭。
** ^使用 NULL 指针对 sqlite3_close() 或 sqlite3_close_v2() 进行调用是一个无害的不操作。
*/
SQLITE_API int SQLITE_STDCALL sqlite3_close(sqlite3*);
SQLITE_API int SQLITE_STDCALL sqlite3_close_v2(sqlite3*);

/*
** 回调函数的类型。
** 这是遗留的，已被弃用。 它被包含以保持历史
** 兼容性，并未被文档化。
*/
typedef int (*sqlite3_callback)(void*,int,char**, char**);

/**
 * CAPI3REF: 一步查询执行接口
 * 方法: sqlite3
 *
 * sqlite3_exec() 接口是 [sqlite3_prepare_v2()], [sqlite3_step()], 和 [sqlite3_finalize()] 的便利包装器，
 * 允许应用程序在不需要使用大量C代码的情况下运行多个SQL语句。
 *
 * ^sqlite3_exec() 接口在作为其第一个参数传入的 [数据库连接] 的上下文中运行零个或多个UTF-8编码的、
 * 分号分隔的SQL语句，这些语句作为其第二个参数传入。^如果传递给 sqlite3_exec() 的第三个参数的回调函数不为 NULL，
 * 则每次从评估的SQL语句中出来的结果行都会调用它。^传递给 sqlite3_exec() 的第四个参数会传递给每次回调调用的第一个参数。
 * ^如果传递给 sqlite3_exec() 的回调指针为 NULL，则从不调用回调，并忽略结果行。
 *
 * ^如果在评估传递给 sqlite3_exec() 的SQL语句时发生错误，则当前语句的执行停止，并跳过后续语句。^如果传递给 sqlite3_exec() 的
 * 第五个参数不为 NULL，则任何错误消息都会写入从 [sqlite3_malloc()] 获得的内存中，并通过第五个参数传递回来。为了避免内存泄漏，
 * 应用程序应在不再需要通过 sqlite3_exec() 的第五个参数返回的错误消息字符串后调用 [sqlite3_free()]。^如果传递给 sqlite3_exec() 的
 * 第五个参数不为 NULL 且没有发生错误，则在返回之前，sqlite3_exec() 将其第五个参数中的指针设置为 NULL。
 *
 * ^如果 sqlite3_exec() 的回调返回非零值，sqlite3_exec() 例程将返回 SQLITE_ABORT，不再调用回调，并且不运行任何后续的 SQL 语句。
 *
 * ^传递给 sqlite3_exec() 回调函数的第二个参数是结果中的列数。^传递给 sqlite3_exec() 回调的第三个参数是一个指针数组，指向从
 * [sqlite3_column_text()] 获得的字符串，每个列一个。^如果结果行的一个元素为 NULL，则 sqlite3_exec() 回调对应的字符串指针为 NULL 指针。
 * ^传递给 sqlite3_exec() 回调的第四个参数是一个指针数组，每个条目代表从 [sqlite3_column_name()] 获得的相应结果列的名称。
 *
 * ^如果传递给 sqlite3_exec() 的第二个参数是空指针、指向空字符串的指针，或者只包含空白和/或 SQL 注释的指针，则不评估任何 SQL 语句，
 * 并且数据库不发生变化。
 *
 * 限制：
 *
 * <ul>
 * <li> 应用程序必须确保传递给 sqlite3_exec() 的第一个参数是一个有效且打开的 [数据库连接]。
 * <li> 应用程序在 sqlite3_exec() 运行时不得关闭由第一个参数指定的 [数据库连接]。
 * <li> 应用程序在 sqlite3_exec() 运行时不得修改传递给 sqlite3_exec() 的第二个参数的 SQL 语句文本。
 * </ul>
 */
SQLITE_API int SQLITE_STDCALL sqlite3_exec(
  sqlite3*,                                  /* 一个开放的数据库 */
  const char *sql,                           /* 要评估的 SQL */
  int (*callback)(void*,int,char**,char**),  /* 回调函数 */
  void *,                                    /* 第一个参数传递给回调函数 */
  char **errmsg                              /* 错误信息写在这里 */
);

/*
** CAPI3REF: 结果代码
** 关键词: {结果代码定义}
**
** 许多SQLite函数返回一组如下的整型结果代码，以指示成功或失败。
**
** 在SQLite的未来版本中可能会添加新的错误代码。
**
** 另见: [扩展结果代码定义]
*/
#define SQLITE_OK           0   /* 成功结果 */
/* 错误代码开始 */
#define SQLITE_ERROR        1   /* SQL 错误或数据库缺失 */
#define SQLITE_INTERNAL     2   /* SQLite 内部逻辑错误 */
#define SQLITE_PERM         3   /* 访问权限被拒绝 */
#define SQLITE_ABORT        4   /* 回调函数请求中止 */
#define SQLITE_BUSY         5   /* 数据库文件被锁定 */
#define SQLITE_LOCKED       6   /* 数据库中的表被锁定 */
#define SQLITE_NOMEM        7   /* malloc() 失败 */
#define SQLITE_READONLY     8   /* 尝试写入只读数据库 */
#define SQLITE_INTERRUPT    9   /* 操作被 sqlite3_interrupt() 终止 */
#define SQLITE_IOERR       10   /* 发生了某种磁盘 I/O 错误 */
#define SQLITE_CORRUPT     11   /* 数据库磁盘映像损坏 */
#define SQLITE_NOTFOUND    12   /* sqlite3_file_control() 中的未知操作码 */
#define SQLITE_FULL        13   /* 插入失败，因为数据库已满 */
#define SQLITE_CANTOPEN    14   /* 无法打开数据库文件 */
#define SQLITE_PROTOCOL    15   /* 数据库锁定协议错误 */
#define SQLITE_EMPTY       16   /* 数据库为空 */
#define SQLITE_SCHEMA      17   /* 数据库模式已更改 */
#define SQLITE_TOOBIG      18   /* 字符串或 BLOB 超出大小限制 */
#define SQLITE_CONSTRAINT  19   /* 由于约束冲突而中止 */
#define SQLITE_MISMATCH    20   /* 数据类型不匹配 */
#define SQLITE_MISUSE      21   /* 库使用不正确 */
#define SQLITE_NOLFS       22   /* 使用了主机不支持的 OS 功能 */
#define SQLITE_AUTH        23   /* 授权被拒绝 */
#define SQLITE_FORMAT      24   /* 辅助数据库格式错误 */
#define SQLITE_RANGE       25   /* sqlite3_bind 的第二个参数超出范围 */
#define SQLITE_NOTADB      26   /* 打开的文件不是数据库文件 */
#define SQLITE_NOTICE      27   /* 来自 sqlite3_log() 的通知 */
#define SQLITE_WARNING     28   /* 来自 sqlite3_log() 的警告 */
#define SQLITE_ROW         100  /* sqlite3_step() 已准备好另一行 */
#define SQLITE_DONE        101  /* sqlite3_step() 已完成执行 */
/* 错误代码结束 */

/*
** CAPI3REF: 扩展结果代码
** 关键词: {扩展结果代码定义}
**
** 在其默认配置中，SQLite API例程返回30个整数之一
** [结果代码]。然而，经验表明，许多
** 这些结果代码过于粗略。它们没有提供程序员可能想要的
** 关于问题的信息。为了解决这个问题，较新版本的SQLite（版本3.3.8及以后版本）包括
** 支持提供更详细错误信息的额外结果代码。这些[扩展结果代码]
** 可以使用[sqlite3_extended_result_codes()] API在
** 每个数据库连接的基础上启用或禁用。或者，可以使用
** [sqlite3_extended_errcode()]获取最近错误的扩展代码。
*/
#define SQLITE_IOERR_READ              (SQLITE_IOERR | (1<<8))
#define SQLITE_IOERR_SHORT_READ        (SQLITE_IOERR | (2<<8))
#define SQLITE_IOERR_WRITE             (SQLITE_IOERR | (3<<8))
#define SQLITE_IOERR_FSYNC             (SQLITE_IOERR | (4<<8))
#define SQLITE_IOERR_DIR_FSYNC         (SQLITE_IOERR | (5<<8))
#define SQLITE_IOERR_TRUNCATE          (SQLITE_IOERR | (6<<8))
#define SQLITE_IOERR_FSTAT             (SQLITE_IOERR | (7<<8))
#define SQLITE_IOERR_UNLOCK            (SQLITE_IOERR | (8<<8))
#define SQLITE_IOERR_RDLOCK            (SQLITE_IOERR | (9<<8))
#define SQLITE_IOERR_DELETE            (SQLITE_IOERR | (10<<8))
#define SQLITE_IOERR_BLOCKED           (SQLITE_IOERR | (11<<8))
#define SQLITE_IOERR_NOMEM             (SQLITE_IOERR | (12<<8))
#define SQLITE_IOERR_ACCESS            (SQLITE_IOERR | (13<<8))
#define SQLITE_IOERR_CHECKRESERVEDLOCK (SQLITE_IOERR | (14<<8))
#define SQLITE_IOERR_LOCK              (SQLITE_IOERR | (15<<8))
#define SQLITE_IOERR_CLOSE             (SQLITE_IOERR | (16<<8))
#define SQLITE_IOERR_DIR_CLOSE         (SQLITE_IOERR | (17<<8))
#define SQLITE_IOERR_SHMOPEN           (SQLITE_IOERR | (18<<8))
#define SQLITE_IOERR_SHMSIZE           (SQLITE_IOERR | (19<<8))
#define SQLITE_IOERR_SHMLOCK           (SQLITE_IOERR | (20<<8))
#define SQLITE_IOERR_SHMMAP            (SQLITE_IOERR | (21<<8))
#define SQLITE_IOERR_SEEK              (SQLITE_IOERR | (22<<8))
#define SQLITE_IOERR_DELETE_NOENT      (SQLITE_IOERR | (23<<8))
#define SQLITE_IOERR_MMAP              (SQLITE_IOERR | (24<<8))
#define SQLITE_IOERR_GETTEMPPATH       (SQLITE_IOERR | (25<<8))
#define SQLITE_IOERR_CONVPATH          (SQLITE_IOERR | (26<<8))
#define SQLITE_LOCKED_SHAREDCACHE      (SQLITE_LOCKED |  (1<<8))
#define SQLITE_BUSY_RECOVERY           (SQLITE_BUSY   |  (1<<8))
#define SQLITE_BUSY_SNAPSHOT           (SQLITE_BUSY   |  (2<<8))
#define SQLITE_CANTOPEN_NOTEMPDIR      (SQLITE_CANTOPEN | (1<<8))
#define SQLITE_CANTOPEN_ISDIR          (SQLITE_CANTOPEN | (2<<8))
#define SQLITE_CANTOPEN_FULLPATH       (SQLITE_CANTOPEN | (3<<8))
#define SQLITE_CANTOPEN_CONVPATH       (SQLITE_CANTOPEN | (4<<8))
#define SQLITE_CORRUPT_VTAB            (SQLITE_CORRUPT | (1<<8))
#define SQLITE_READONLY_RECOVERY       (SQLITE_READONLY | (1<<8))
#define SQLITE_READONLY_CANTLOCK       (SQLITE_READONLY | (2<<8))
#define SQLITE_READONLY_ROLLBACK       (SQLITE_READONLY | (3<<8))
#define SQLITE_READONLY_DBMOVED        (SQLITE_READONLY | (4<<8))
#define SQLITE_ABORT_ROLLBACK          (SQLITE_ABORT | (2<<8))
#define SQLITE_CONSTRAINT_CHECK        (SQLITE_CONSTRAINT | (1<<8))
#define SQLITE_CONSTRAINT_COMMITHOOK   (SQLITE_CONSTRAINT | (2<<8))
#define SQLITE_CONSTRAINT_FOREIGNKEY   (SQLITE_CONSTRAINT | (3<<8))
#define SQLITE_CONSTRAINT_FUNCTION     (SQLITE_CONSTRAINT | (4<<8))
#define SQLITE_CONSTRAINT_NOTNULL      (SQLITE_CONSTRAINT | (5<<8))
#define SQLITE_CONSTRAINT_PRIMARYKEY   (SQLITE_CONSTRAINT | (6<<8))
#define SQLITE_CONSTRAINT_TRIGGER      (SQLITE_CONSTRAINT | (7<<8))
#define SQLITE_CONSTRAINT_UNIQUE       (SQLITE_CONSTRAINT | (8<<8))
#define SQLITE_CONSTRAINT_VTAB         (SQLITE_CONSTRAINT | (9<<8))
#define SQLITE_CONSTRAINT_ROWID        (SQLITE_CONSTRAINT |(10<<8))
#define SQLITE_NOTICE_RECOVER_WAL      (SQLITE_NOTICE | (1<<8))
#define SQLITE_NOTICE_RECOVER_ROLLBACK (SQLITE_NOTICE | (2<<8))
#define SQLITE_WARNING_AUTOINDEX       (SQLITE_WARNING | (1<<8))
#define SQLITE_AUTH_USER               (SQLITE_AUTH | (1<<8))

/*
** CAPI3REF: 文件打开操作的标志
**
** 这些位值用于
** [sqlite3_open_v2()] 接口的第3个参数和
** [sqlite3_vfs.xOpen] 方法的第4个参数。
*/
#define SQLITE_OPEN_READONLY         0x00000001  /* 适用于 sqlite3_open_v2() */
#define SQLITE_OPEN_READWRITE        0x00000002  /* 适用于 sqlite3_open_v2() */
#define SQLITE_OPEN_CREATE           0x00000004  /* 适用于 sqlite3_open_v2() */
#define SQLITE_OPEN_DELETEONCLOSE    0x00000008  /* 仅适用于 VFS */
#define SQLITE_OPEN_EXCLUSIVE        0x00000010  /* 仅适用于 VFS */
#define SQLITE_OPEN_AUTOPROXY        0x00000020  /* 仅适用于 VFS */
#define SQLITE_OPEN_URI              0x00000040  /* 适用于 sqlite3_open_v2() */
#define SQLITE_OPEN_MEMORY           0x00000080  /* 适用于 sqlite3_open_v2() */
#define SQLITE_OPEN_MAIN_DB          0x00000100  /* 仅适用于 VFS */
#define SQLITE_OPEN_TEMP_DB          0x00000200  /* 仅适用于 VFS */
#define SQLITE_OPEN_TRANSIENT_DB     0x00000400  /* 仅适用于 VFS */
#define SQLITE_OPEN_MAIN_JOURNAL     0x00000800  /* 仅适用于 VFS */
#define SQLITE_OPEN_TEMP_JOURNAL     0x00001000  /* 仅适用于 VFS */
#define SQLITE_OPEN_SUBJOURNAL       0x00002000  /* 仅适用于 VFS */
#define SQLITE_OPEN_MASTER_JOURNAL   0x00004000  /* 仅适用于 VFS */
#define SQLITE_OPEN_NOMUTEX          0x00008000  /* 适用于 sqlite3_open_v2() */
#define SQLITE_OPEN_FULLMUTEX        0x00010000  /* 适用于 sqlite3_open_v2() */
#define SQLITE_OPEN_SHAREDCACHE      0x00020000  /* 适用于 sqlite3_open_v2() */
#define SQLITE_OPEN_PRIVATECACHE     0x00040000  /* 适用于 sqlite3_open_v2() */
#define SQLITE_OPEN_WAL              0x00080000  /* 仅适用于 VFS */

/* 保留:                         0x00F00000 */

/*
** CAPI3REF: 设备特性
**
** [sqlite3_io_methods] 对象的 xDeviceCharacteristics 方法返回一个整数，
** 该整数是这些位值的向量，表示 [sqlite3_io_methods] 所引用的文件所在的
** 大容量存储设备的 I/O 特性。
**
** SQLITE_IOCAP_ATOMIC 属性表示任何大小的写操作都是原子的。SQLITE_IOCAP_ATOMICnnn 值
** 表示大小为 nnn 字节的块，且地址是对 nnn 的整数倍的写操作是原子的。
** SQLITE_IOCAP_SAFE_APPEND 值表示向文件追加数据时，首先追加数据，然后扩展文件大小，
** 而不是反过来。SQLITE_IOCAP_SEQUENTIAL 属性表示信息按照 xWrite() 调用的顺序
** 写入磁盘。SQLITE_IOCAP_POWERSAFE_OVERWRITE 属性表示在崩溃或断电后重新启动，
** 文件中只有在应用程序级别写入的字节可能已更改，相邻字节，即使是同一扇区内的字节
** 都保证未更改。SQLITE_IOCAP_UNDELETABLE_WHEN_OPEN 标志表示打开的文件不能被删除。
** SQLITE_IOCAP_IMMUTABLE 标志表示文件位于只读媒体上，即使具有提升权限的进程也无法更改。
*/
#define SQLITE_IOCAP_ATOMIC                 0x00000001
#define SQLITE_IOCAP_ATOMIC512              0x00000002
#define SQLITE_IOCAP_ATOMIC1K               0x00000004
#define SQLITE_IOCAP_ATOMIC2K               0x00000008
#define SQLITE_IOCAP_ATOMIC4K               0x00000010
#define SQLITE_IOCAP_ATOMIC8K               0x00000020
#define SQLITE_IOCAP_ATOMIC16K              0x00000040
#define SQLITE_IOCAP_ATOMIC32K              0x00000080
#define SQLITE_IOCAP_ATOMIC64K              0x00000100
#define SQLITE_IOCAP_SAFE_APPEND            0x00000200
#define SQLITE_IOCAP_SEQUENTIAL             0x00000400
#define SQLITE_IOCAP_UNDELETABLE_WHEN_OPEN  0x00000800
#define SQLITE_IOCAP_POWERSAFE_OVERWRITE    0x00001000
#define SQLITE_IOCAP_IMMUTABLE              0x00002000

/*
** CAPI3REF: 文件锁定级别
**
** SQLite 使用这些整数值之一作为对 [sqlite3_io_methods] 对象的 xLock() 和 xUnlock() 方法的调用中的第二个参数。
*/
#define SQLITE_LOCK_NONE          0
#define SQLITE_LOCK_SHARED        1
#define SQLITE_LOCK_RESERVED      2
#define SQLITE_LOCK_PENDING       3
#define SQLITE_LOCK_EXCLUSIVE     4

/*
** CAPI3REF: 同步类型标志
**
** 当SQLite调用[sqlite3_io_methods]对象的xSync()方法时，
** 它使用这些整数值的组合作为第二个参数。
**
** 当使用SQLITE_SYNC_DATAONLY标志时，意味着同步操作只需要将数据刷新到存储设备。
** 无需刷新inode信息。如果标志的最低四位等于SQLITE_SYNC_NORMAL，
** 则意味着使用正常的fsync()语义。如果最低四位等于SQLITE_SYNC_FULL，
** 则意味着使用Mac OS X风格的完全同步而不是fsync()。
**
** 不要混淆SQLITE_SYNC_NORMAL和SQLITE_SYNC_FULL标志与
** [PRAGMA synchronous]=NORMAL和[PRAGMA synchronous]=FULL设置。
** [同步pragma]确定何时调用xSync VFS方法，并统一应用于所有平台。
** SQLITE_SYNC_NORMAL和SQLITE_SYNC_FULL标志确定同步操作的强度或严格程度，
** 并且仅在Mac OSX上的默认SQLite代码中有所不同。
** （第三方VFS实现也可能区分SQLITE_SYNC_NORMAL和SQLITE_SYNC_FULL，
** 但在SQLite原生支持的操作系统之间，只有Mac OSX关心这种差异。）
*/
#define SQLITE_SYNC_NORMAL        0x00002
#define SQLITE_SYNC_FULL          0x00003
#define SQLITE_SYNC_DATAONLY      0x00010

/*
** CAPI3REF: 操作系统接口打开文件句柄
**
** 一个 [sqlite3_file] 对象表示在
** [sqlite3_vfs | 操作系统接口层] 中打开的文件。各个操作系统接口
** 实现将希望通过添加额外的字段来扩展这个对象以供自己使用。pMethods
** 条目是指向一个 [sqlite3_io_methods] 对象的指针，该对象定义了
** 对打开文件执行 I/O 操作的方法。
*/
typedef struct sqlite3_file sqlite3_file;
struct sqlite3_file {
  const struct sqlite3_io_methods *pMethods;  /* 方法用于打开文件 */
};

/*
* 
** CAPI3REF: 操作系统接口文件虚拟方法对象
**
** 每个通过 [sqlite3_vfs.xOpen] 方法打开的文件都会填充一个
** [sqlite3_file] 对象（或更常见的 [sqlite3_file] 对象的子类），
** 并指向此对象的实例。此对象定义了用于对 [sqlite3_file] 对象表示的
** 打开文件执行各种操作的方法。
**
** 如果 [sqlite3_vfs.xOpen] 方法将 sqlite3_file.pMethods 元素设置为非 NULL 指针，
** 则即使 [sqlite3_vfs.xOpen] 报告失败，也可能会调用 sqlite3_io_methods.xClose 方法。
** 防止在 [sqlite3_vfs.xOpen] 失败后调用 xClose 的唯一方法是
** 让 [sqlite3_vfs.xOpen] 将 sqlite3_file.pMethods 元素设置为 NULL。
**
** xSync 的标志参数可以是 [SQLITE_SYNC_NORMAL] 或 [SQLITE_SYNC_FULL] 之一。
** 第一种选择是正常的 fsync()。第二种选择是 Mac OS X 风格的 fullsync。
** [SQLITE_SYNC_DATAONLY] 标志可以按位或操作，以指示仅需要同步文件的数据，
** 而不需要同步其 inode。
**
** xLock() 和 xUnlock() 的整数值可以是以下之一：
** <ul>
** <li> [SQLITE_LOCK_NONE],
** <li> [SQLITE_LOCK_SHARED],
** <li> [SQLITE_LOCK_RESERVED],
** <li> [SQLITE_LOCK_PENDING], 或
** <li> [SQLITE_LOCK_EXCLUSIVE]。
** </ul>
** xLock() 增加锁级别。xUnlock() 减少锁级别。
** xCheckReservedLock() 方法检查是否有任何数据库连接（无论是当前进程还是其他进程）
** 持有文件的 RESERVED、PENDING 或 EXCLUSIVE 锁。如果存在此类锁，则返回 true，
** 否则返回 false。
**
** xFileControl() 方法是一个通用接口，允许自定义 VFS 实现使用
** [sqlite3_file_control()] 接口直接控制打开的文件。第二个 "op" 参数是一个整数操作码。
** 第三个参数是一个通用指针，旨在指向可能包含参数或用于写入返回值的结构。
** xFileControl() 的潜在用途可能是启用带超时的阻塞锁、更改锁定策略
** （例如使用点文件锁）、查询锁的状态或打破陈旧的锁。
** SQLite 核心保留所有小于 100 的操作码供自己使用。
** 提供了一个[文件控制操作码 | 操作码列表]，其中包含小于 100 的操作码。
** 定义自定义 xFileControl 方法的应用程序应使用大于 100 的操作码以避免冲突。
** VFS 实现应为其不识别文件控制操作码返回 [SQLITE_NOTFOUND]。
**
** xSectorSize() 方法返回文件底层设备的扇区大小。扇区大小是可以在不干扰文件中
** 其他字节的情况下执行的最小写入单位。xDeviceCharacteristics() 方法返回一个位向量，
** 描述底层设备的行为：
**
** <ul>
** <li> [SQLITE_IOCAP_ATOMIC]
** <li> [SQLITE_IOCAP_ATOMIC512]
** <li> [SQLITE_IOCAP_ATOMIC1K]
** <li> [SQLITE_IOCAP_ATOMIC2K]
** <li> [SQLITE_IOCAP_ATOMIC4K]
** <li> [SQLITE_IOCAP_ATOMIC8K]
** <li> [SQLITE_IOCAP_ATOMIC16K]
** <li> [SQLITE_IOCAP_ATOMIC32K]
** <li> [SQLITE_IOCAP_ATOMIC64K]
** <li> [SQLITE_IOCAP_SAFE_APPEND]
** <li> [SQLITE_IOCAP_SEQUENTIAL]
** </ul>
**
** SQLITE_IOCAP_ATOMIC 属性表示任何大小的写入都是原子的。
** SQLITE_IOCAP_ATOMICnnn 值表示大小为 nnn 字节且对齐到 nnn 的整数倍地址的块写入是原子的。
** SQLITE_IOCAP_SAFE_APPEND 值表示在将数据附加到文件时，数据首先被附加，
** 然后文件大小被扩展，而不是相反。SQLITE_IOCAP_SEQUENTIAL 属性表示信息以与 xWrite() 调用
** 相同的顺序写入磁盘。
**
** 如果 xRead() 返回 SQLITE_IOERR_SHORT_READ，则还必须用零填充缓冲区的未读部分。
** 未能对短读取进行零填充的 VFS 可能看起来可以工作。然而，未能对短读取进行零填充最终会导致
** 数据库损坏。
*/
typedef struct sqlite3_io_methods sqlite3_io_methods;
struct sqlite3_io_methods {
  int iVersion;
  int (*xClose)(sqlite3_file*);
  int (*xRead)(sqlite3_file*, void*, int iAmt, sqlite3_int64 iOfst);
  int (*xWrite)(sqlite3_file*, const void*, int iAmt, sqlite3_int64 iOfst);
  int (*xTruncate)(sqlite3_file*, sqlite3_int64 size);
  int (*xSync)(sqlite3_file*, int flags);
  int (*xFileSize)(sqlite3_file*, sqlite3_int64 *pSize);
  int (*xLock)(sqlite3_file*, int);
  int (*xUnlock)(sqlite3_file*, int);
  int (*xCheckReservedLock)(sqlite3_file*, int *pResOut);
  int (*xFileControl)(sqlite3_file*, int op, void *pArg);
  int (*xSectorSize)(sqlite3_file*);
  int (*xDeviceCharacteristics)(sqlite3_file*);
  /* 方法以上适用于版本1 */
  int (*xShmMap)(sqlite3_file*, int iPg, int pgsz, int, void volatile**);
  int (*xShmLock)(sqlite3_file*, int offset, int n, int flags);
  void (*xShmBarrier)(sqlite3_file*);
  int (*xShmUnmap)(sqlite3_file*, int deleteFlag);
  /* 方法以上适用于版本2 */
  int (*xFetch)(sqlite3_file*, sqlite3_int64 iOfst, int iAmt, void **pp);
  int (*xUnfetch)(sqlite3_file*, sqlite3_int64 iOfst, void *p);
  /* 上述方法适用于版本3 */
  /* 以后可能会添加其他方法 */
};

/*
** CAPI3REF: 标准文件控制操作码
** 关键字: {文件控制操作码} {文件控制操作码}
**
** 这些整数常量是 [sqlite3_io_methods] 对象的 xFileControl 方法
** 以及 [sqlite3_file_control()] 接口的操作码。
**
** <ul>
** <li>[[SQLITE_FCNTL_LOCKSTATE]]
** [SQLITE_FCNTL_LOCKSTATE] 操作码用于调试。此操作码使 xFileControl 方法
** 将当前锁的状态（[SQLITE_LOCK_NONE]、[SQLITE_LOCK_SHARED]、
** [SQLITE_LOCK_RESERVED]、[SQLITE_LOCK_PENDING] 或 [SQLITE_LOCK_EXCLUSIVE] 之一）
** 写入 pArg 参数指向的整数中。此功能在测试期间使用，并且仅在启用 SQLITE_TEST
** 编译选项时可用。
**
** <li>[[SQLITE_FCNTL_SIZE_HINT]]
** [SQLITE_FCNTL_SIZE_HINT] 操作码由 SQLite 使用，用于向 VFS 层提示
** 当前事务期间数据库文件将增长到的大小。此提示不保证准确，但通常接近。
** 底层 VFS 可能会根据此提示预分配数据库文件空间，以帮助加快数据库文件的写入速度。
**
** <li>[[SQLITE_FCNTL_CHUNK_SIZE]]
** [SQLITE_FCNTL_CHUNK_SIZE] 操作码用于请求 VFS 以用户指定的大小块
** 扩展和截断数据库文件。[sqlite3_file_control()] 的第四个参数应指向
** 一个包含新块大小的整数（类型为 int）。以较大的块（例如每次 1MB）分配数据库文件空间
** 可能会减少文件系统碎片，并在某些系统上提高性能。
**
** <li>[[SQLITE_FCNTL_FILE_POINTER]]
** [SQLITE_FCNTL_FILE_POINTER] 操作码用于获取与特定数据库连接关联的
** [sqlite3_file] 对象的指针。有关更多信息，请参阅 [sqlite3_file_control()] 文档。
**
** <li>[[SQLITE_FCNTL_SYNC_OMITTED]]
** 不再使用。
**
** <li>[[SQLITE_FCNTL_SYNC]]
** [SQLITE_FCNTL_SYNC] 操作码由 SQLite 内部生成，并在对数据库文件描述符调用
** xSync 方法之前立即发送到 VFS。或者，如果由于用户配置了 [PRAGMA synchronous | PRAGMA synchronous=OFF]
** 而未调用 xSync 方法，则会在 xSync 方法的位置调用它。在大多数情况下，与此文件控制一起传递的指针参数为 NULL。
** 但是，如果数据库文件作为多数据库提交的一部分同步，则参数指向一个以 nul 结尾的字符串，
** 其中包含事务的主日志文件名。不需要此信号的 VFS 应静默忽略此操作码。应用程序不应使用此操作码调用
** [sqlite3_file_control()]，因为这样做可能会干扰需要此操作码的专用 VFS 的操作。
**
** <li>[[SQLITE_FCNTL_COMMIT_PHASETWO]]
** [SQLITE_FCNTL_COMMIT_PHASETWO] 操作码由 SQLite 内部生成，并在事务提交后立即
** 但在数据库解锁之前发送到 VFS。不需要此信号的 VFS 应静默忽略此操作码。应用程序不应使用此操作码调用
** [sqlite3_file_control()]，因为这样做可能会干扰需要此操作码的专用 VFS 的操作。
**
** <li>[[SQLITE_FCNTL_WIN32_AV_RETRY]]
** ^[SQLITE_FCNTL_WIN32_AV_RETRY] 操作码用于配置 Windows [VFS] 的某些磁盘 I/O 操作的
** 自动重试次数和间隔，以在存在防病毒程序的情况下提供鲁棒性。默认情况下，Windows VFS 将重试文件读取、
** 文件写入和文件删除操作最多 10 次，第一次重试前延迟 25 毫秒，每次后续重试延迟增加 25 毫秒。
** 此操作码允许调整这两个值（10 次重试和 25 毫秒延迟）。这些值在同一进程内的所有数据库连接中更改。
** 参数是指向两个整数的数组的指针，其中第一个整数是新的重试次数，第二个整数是延迟。
** 如果任一整数为负，则不更改设置，而是将先前的设置值写入数组条目，从而允许查询当前的重试设置。
** zDbName 参数被忽略。
**
** <li>[[SQLITE_FCNTL_PERSIST_WAL]]
** ^[SQLITE_FCNTL_PERSIST_WAL] 操作码用于设置或查询持久 [WAL | 预写日志] 设置。
** 默认情况下，用于事务控制的辅助预写日志和共享内存文件在关闭与数据库的最新连接时自动删除。
** 设置持久 WAL 模式会使这些文件在关闭后保留。保留这些文件在以下情况下很有用：当其他进程没有对包含数据库文件的目录的写权限
** 但希望读取数据库文件时，因为 WAL 和共享内存文件必须存在才能使数据库可读。
** 此操作码的 [sqlite3_file_control()] 的第四个参数应指向一个整数。该整数为 0 表示禁用持久 WAL 模式，
** 为 1 表示启用持久 WAL 模式。如果整数为 -1，则将其覆盖为当前的 WAL 持久性设置。
**
** <li>[[SQLITE_FCNTL_POWERSAFE_OVERWRITE]]
** ^[SQLITE_FCNTL_POWERSAFE_OVERWRITE] 操作码用于设置或查询持久的“powersafe-overwrite”或“PSOW”设置。
** PSOW 设置决定了 xDeviceCharacteristics 方法的 [SQLITE_IOCAP_POWERSAFE_OVERWRITE] 位。
** 此操作码的 [sqlite3_file_control()] 的第四个参数应指向一个整数。该整数为 0 表示禁用零损坏模式，
** 为 1 表示启用零损坏模式。如果整数为 -1，则将其覆盖为当前的零损坏模式设置。
**
** <li>[[SQLITE_FCNTL_OVERWRITE]]
** ^[SQLITE_FCNTL_OVERWRITE] 操作码由 SQLite 在打开写事务后调用，以指示除非由于某种原因回滚，
** 否则整个数据库文件将被当前事务覆盖。此操作码用于 VACUUM 操作。
**
** <li>[[SQLITE_FCNTL_VFSNAME]]
** ^[SQLITE_FCNTL_VFSNAME] 操作码可用于获取 VFS 堆栈中所有 [VFSes] 的名称。
** 所有 VFS 垫片和最终底层 VFS 的名称被写入从 [sqlite3_malloc()] 获得的内存中，
** 结果存储在 [sqlite3_file_control()] 的第四个参数指向的 char* 变量中。
** 调用者在使用完毕后负责释放内存。与所有文件控制操作一样，不能保证此操作码实际执行任何操作。
** 调用者应将 char* 变量初始化为 NULL 指针，以防此文件控制未实现。此文件控制仅用于诊断目的。
**
** <li>[[SQLITE_FCNTL_PRAGMA]]
** ^每当解析 [PRAGMA] 语句时，都会向与 PRAGMA 语句引用的数据库文件对应的打开的 [sqlite3_file] 对象
** 发送 [SQLITE_FCNTL_PRAGMA] 文件控制。^[SQLITE_FCNTL_PRAGMA] 文件控制的参数是指向字符串（char**）的指针数组，
** 其中数组的第二个元素是 PRAGMA 的名称，第三个元素是 PRAGMA 的参数，如果 PRAGMA 没有参数，则为 NULL。
** ^[SQLITE_FCNTL_PRAGMA] 文件控制的处理程序可以选择使 char** 参数的第一个元素指向从 [sqlite3_mprintf()]
** 或等效函数获得的字符串，该字符串将成为 PRAGMA 的结果或 PRAGMA 失败时的错误消息。
** ^如果 [SQLITE_FCNTL_PRAGMA] 文件控制返回 [SQLITE_NOTFOUND]，则继续正常的 [PRAGMA] 处理。
** ^如果 [SQLITE_FCNTL_PRAGMA] 文件控制返回 [SQLITE_OK]，则解析器假定 VFS 已自行处理 PRAGMA，
** 如果结果字符串为 NULL，则解析器生成一个无操作准备语句，如果字符串为非 NULL，则返回结果字符串的副本。
** ^如果 [SQLITE_FCNTL_PRAGMA] 文件控制返回 [SQLITE_OK] 或 [SQLITE_NOTFOUND] 以外的任何结果代码，
** 这意味着 VFS 在处理 [PRAGMA] 时遇到错误，并且 PRAGMA 的编译失败并返回错误。
** ^[SQLITE_FCNTL_PRAGMA] 文件控制在 PRAGMA 语句分析的开始时发生，因此它能够覆盖内置的 [PRAGMA] 语句。
**
** <li>[[SQLITE_FCNTL_BUSYHANDLER]]
** ^[SQLITE_FCNTL_BUSYHANDLER] 文件控制可能在数据库文件句柄打开后不久由 SQLite 调用，
** 以便为自定义 VFS 提供对连接忙处理程序回调的访问。参数的类型为 (void **) —— 一个包含两个 (void *) 值的数组。
** 第一个 (void *) 实际上指向一个类型为 (int (*)(void *)) 的函数。为了调用连接的忙处理程序，
** 应使用数组中的第二个 (void *) 作为唯一参数调用此函数。如果它返回非零，则应重试操作。
** 如果它返回零，则自定义 VFS 应放弃当前操作。
**
** <li>[[SQLITE_FCNTL_TEMPFILENAME]]
** ^应用程序可以调用 [SQLITE_FCNTL_TEMPFILENAME] 文件控制，以使 SQLite 使用与为 TEMP 表
** 和其他内部用途生成临时文件名相同的算法生成临时文件名。参数应为 char**，它将填充从 [sqlite3_malloc()]
** 获得的内存中写入的文件名。调用者应对结果调用 [sqlite3_free()] 以避免内存泄漏。
**
** <li>[[SQLITE_FCNTL_MMAP_SIZE]]
** [SQLITE_FCNTL_MMAP_SIZE] 文件控制用于查询或设置将用于内存映射 I/O 的最大字节数。
** 参数是指向 sqlite3_int64 类型值的指针，该值是文件中建议的最大内存映射字节数。
** 指针被覆盖为旧值。如果最初指向的值为负，则不会更改限制，因此可以通过传递指向负数的指针来查询当前限制。
** 此文件控制在内部用于实现 [PRAGMA mmap_size]。
**
** <li>[[SQLITE_FCNTL_TRACE]]
** [SQLITE_FCNTL_TRACE] 文件控制向 VFS 提供有关 SQLite 堆栈高层正在执行的操作的咨询信息。
** 此文件控制由一些 VFS 活动跟踪 [垫片] 使用。参数是一个以零结尾的字符串。
** 如果启用了 [SQLITE_USE_FCNTL_TRACE] 编译时选项，SQLite 堆栈的高层可能会生成此文件控制的实例。
**
** <li>[[SQLITE_FCNTL_HAS_MOVED]]
** [SQLITE_FCNTL_HAS_MOVED] 文件控制将其参数解释为指向整数的指针，并根据文件自首次打开以来是否被重命名、
** 移动或删除，将布尔值写入该整数。
**
** <li>[[SQLITE_FCNTL_WIN32_SET_HANDLE]]
** [SQLITE_FCNTL_WIN32_SET_HANDLE] 操作码用于调试。此操作码使 xFileControl 方法
** 将文件句柄与 pArg 参数指向的文件句柄交换。此功能在测试期间使用，并且仅在定义 SQLITE_TEST 时需要支持。
**
** <li>[[SQLITE_FCNTL_WAL_BLOCK]]
** [SQLITE_FCNTL_WAL_BLOCK] 是向 VFS 层发出的信号，表示如果 WAL 锁不可立即获得，
** 则在下一次 WAL 锁上阻塞可能是有利的。WAL 子系统在罕见情况下发出此信号以解决优先级反转问题。
** 应用程序<em>不应</em>使用此文件控制。
**
** <li>[[SQLITE_FCNTL_ZIPVFS]]
** [SQLITE_FCNTL_ZIPVFS] 操作码仅由 zipvfs 实现。所有其他 VFS 应为此操作码返回 SQLITE_NOTFOUND。
**
** <li>[[SQLITE_FCNTL_OTA]]
** [SQLITE_FCNTL_OTA] 操作码仅由 OTA 扩展使用的特殊 VFS 实现。所有其他 VFS 应为此操作码返回 SQLITE_NOTFOUND。
** </ul>
*/
#define SQLITE_FCNTL_LOCKSTATE               1
#define SQLITE_FCNTL_GET_LOCKPROXYFILE       2
#define SQLITE_FCNTL_SET_LOCKPROXYFILE       3
#define SQLITE_FCNTL_LAST_ERRNO              4
#define SQLITE_FCNTL_SIZE_HINT               5
#define SQLITE_FCNTL_CHUNK_SIZE              6
#define SQLITE_FCNTL_FILE_POINTER            7
#define SQLITE_FCNTL_SYNC_OMITTED            8
#define SQLITE_FCNTL_WIN32_AV_RETRY          9
#define SQLITE_FCNTL_PERSIST_WAL            10
#define SQLITE_FCNTL_OVERWRITE              11
#define SQLITE_FCNTL_VFSNAME                12
#define SQLITE_FCNTL_POWERSAFE_OVERWRITE    13
#define SQLITE_FCNTL_PRAGMA                 14
#define SQLITE_FCNTL_BUSYHANDLER            15
#define SQLITE_FCNTL_TEMPFILENAME           16
#define SQLITE_FCNTL_MMAP_SIZE              18
#define SQLITE_FCNTL_TRACE                  19
#define SQLITE_FCNTL_HAS_MOVED              20
#define SQLITE_FCNTL_SYNC                   21
#define SQLITE_FCNTL_COMMIT_PHASETWO        22
#define SQLITE_FCNTL_WIN32_SET_HANDLE       23
#define SQLITE_FCNTL_WAL_BLOCK              24
#define SQLITE_FCNTL_ZIPVFS                 25
#define SQLITE_FCNTL_OTA                    26

/* 已弃用的名称 */
#define SQLITE_GET_LOCKPROXYFILE      SQLITE_FCNTL_GET_LOCKPROXYFILE
#define SQLITE_SET_LOCKPROXYFILE      SQLITE_FCNTL_SET_LOCKPROXYFILE
#define SQLITE_LAST_ERRNO             SQLITE_FCNTL_LAST_ERRNO


/*
** CAPI3REF: 互斥锁句柄
**
** SQLite中的互斥模块将[sqlite3_mutex]定义为互斥对象的抽象类型。
** SQLite核心代码从不查看[sqlite3_mutex]的内部结构实现，
** 所有操作都通过指向[sqlite3_mutex]对象的指针来完成。
**
** 互斥锁通过[sqlite3_mutex_alloc()]函数创建。
*/
typedef struct sqlite3_mutex sqlite3_mutex;

/*
** CAPI3REF: 操作系统接口对象
**
** sqlite3_vfs 对象的实例定义了 SQLite 核心与底层操作系统之间的接口。
** 对象名称中的“vfs”代表“虚拟文件系统”。有关更多信息，请参阅 [VFS | VFS 文档]。
**
** iVersion 字段的初始值为 1，但在 SQLite 的未来版本中可能会更大。
** 当 iVersion 值增加时，可能会向此对象追加其他字段。请注意，sqlite3_vfs 对象的结构
** 在 SQLite 版本 3.5.9 和 3.6.0 之间的过渡中发生了变化，但 iVersion 字段并未修改。
**
** szOsFile 字段是此 VFS 使用的子类化 [sqlite3_file] 结构的大小。
** mxPathname 是此 VFS 中路径名的最大长度。
**
** 注册的 sqlite3_vfs 对象通过 pNext 指针形成一个链表。[sqlite3_vfs_register()]
** 和 [sqlite3_vfs_unregister()] 接口以线程安全的方式管理此列表。
** [sqlite3_vfs_find()] 接口搜索此列表。应用程序代码和 VFS 实现都不应使用 pNext 指针。
**
** pNext 字段是 sqlite3_vfs 结构中 SQLite 会修改的唯一字段。SQLite 仅在持有特定静态互斥锁时
** 访问或修改此字段。一旦对象被注册，应用程序不应修改 sqlite3_vfs 对象中的任何内容。
**
** zName 字段保存 VFS 模块的名称。该名称在所有 VFS 模块中必须是唯一的。
**
** [[sqlite3_vfs.xOpen]]
** ^SQLite 保证传递给 xOpen 的 zFilename 参数要么是 NULL 指针，要么是从 xFullPathname()
** 获得的字符串，并带有可选的附加后缀。^如果向 zFilename 参数添加了后缀，它将由单个“-”字符
** 后跟不超过 11 个字母数字和/或“-”字符组成。^SQLite 进一步保证该字符串在调用 xClose() 之前
** 是有效且不变的。由于上一句话，[sqlite3_file] 可以安全地存储指向文件名的指针，
** 如果它需要出于某种原因记住文件名。如果传递给 xOpen 的 zFilename 参数是 NULL 指针，
** 则 xOpen 必须为文件发明自己的临时名称。^每当 xFilename 参数为 NULL 时，flags 参数
** 也将包括 [SQLITE_OPEN_DELETEONCLOSE]。
**
** 传递给 xOpen() 的 flags 参数包括传递给 [sqlite3_open_v2()] 的 flags 参数中的所有位。
** 或者，如果使用 [sqlite3_open()] 或 [sqlite3_open16()]，则 flags 至少包括
** [SQLITE_OPEN_READWRITE] | [SQLITE_OPEN_CREATE]。如果 xOpen() 以只读方式打开文件，
** 则它将 *pOutFlags 设置为包括 [SQLITE_OPEN_READONLY]。*pOutFlags 中的其他位可能会被设置。
**
** ^(SQLite 还会根据打开的对象向 xOpen() 调用添加以下标志之一：
**
** <ul>
** <li>  [SQLITE_OPEN_MAIN_DB]
** <li>  [SQLITE_OPEN_MAIN_JOURNAL]
** <li>  [SQLITE_OPEN_TEMP_DB]
** <li>  [SQLITE_OPEN_TEMP_JOURNAL]
** <li>  [SQLITE_OPEN_TRANSIENT_DB]
** <li>  [SQLITE_OPEN_SUBJOURNAL]
** <li>  [SQLITE_OPEN_MASTER_JOURNAL]
** <li>  [SQLITE_OPEN_WAL]
** </ul>)^
**
** 文件 I/O 实现可以使用对象类型标志来更改其处理文件的方式。例如，不关心崩溃恢复或回滚的应用程序
** 可能会使日志文件的打开成为无操作。对此日志的写入也将是无操作，任何读取日志的尝试都将返回 SQLITE_IOERR。
** 或者，实现可能会识别到数据库文件将以随机顺序执行页面对齐的扇区读取和写入，并相应地设置其 I/O 子系统。
**
** SQLite 还可能向 xOpen 方法添加以下标志之一：
**
** <ul>
** <li> [SQLITE_OPEN_DELETEONCLOSE]
** <li> [SQLITE_OPEN_EXCLUSIVE]
** </ul>
**
** [SQLITE_OPEN_DELETEONCLOSE] 标志表示文件在关闭时应被删除。^[SQLITE_OPEN_DELETEONCLOSE]
** 将为 TEMP 数据库及其日志、临时数据库和子日志设置。
**
** ^[SQLITE_OPEN_EXCLUSIVE] 标志始终与 [SQLITE_OPEN_CREATE] 标志一起使用，这两个标志直接
** 类似于 POSIX open() API 的 O_EXCL 和 O_CREAT 标志。SQLITE_OPEN_EXCLUSIVE 标志与
** SQLITE_OPEN_CREATE 配对时，用于指示应始终创建文件，并且如果文件已存在则是错误。
** 它<i>不</i>用于指示文件应以独占访问方式打开。
**
** ^SQLite 至少分配 szOsFile 字节的内存来保存作为第三个参数传递给 xOpen 的 [sqlite3_file] 结构。
** xOpen 方法不必分配该结构；它只需填充它。请注意，xOpen 方法必须将 sqlite3_file.pMethods 设置为
** 有效的 [sqlite3_io_methods] 对象或 NULL。即使打开失败，xOpen 也必须这样做。SQLite 期望
** 在 xOpen 返回后，无论 xOpen 调用成功与否，sqlite3_file.pMethods 元素都是有效的。
**
** [[sqlite3_vfs.xAccess]]
** ^传递给 xAccess() 的 flags 参数可以是 [SQLITE_ACCESS_EXISTS] 以测试文件是否存在，
** 或 [SQLITE_ACCESS_READWRITE] 以测试文件是否可读和可写，或 [SQLITE_ACCESS_READ]
** 以测试文件是否至少可读。文件可以是目录。
**
** ^SQLite 将始终为 xFullPathname 的输出缓冲区分配至少 mxPathname+1 字节。输出缓冲区的确切大小
** 也作为参数传递给这两种方法。如果输出缓冲区不够大，则应返回 [SQLITE_CANTOPEN]。
** 由于 SQLite 将此视为致命错误，VFS 实现应通过将 mxPathname 设置为足够大的值来防止这种情况。
**
** xRandomness()、xSleep()、xCurrentTime() 和 xCurrentTimeInt64() 接口严格来说不是文件系统的一部分，
** 但为了完整性，它们被包含在 VFS 结构中。xRandomness() 函数尝试将 nBytes 字节的高质量随机性
** 返回到 zOut 中。返回值是实际获得的随机性字节数。xSleep() 方法使调用线程至少睡眠指定的微秒数。
** ^xCurrentTime() 方法返回当前日期和时间的儒略日数作为浮点值。
** ^xCurrentTimeInt64() 方法以整数形式返回儒略日数乘以 86400000（24 小时中的毫秒数）。
** ^如果 xCurrentTimeInt64() 方法可用（如果 iVersion 为 2 或更高且函数指针不为 NULL），
** SQLite 将使用 xCurrentTimeInt64() 方法获取当前日期和时间，如果 xCurrentTimeInt64() 不可用，
** 则回退到 xCurrentTime()。
**
** ^xSetSystemCall()、xGetSystemCall() 和 xNestSystemCall() 接口不被 SQLite 核心使用。
** 这些可选接口由一些 VFS 提供，以便于测试 VFS 代码。通过用其控制的函数覆盖系统调用，
** 测试程序可以模拟否则难以或不可能引发的故障和错误条件。可以覆盖的系统调用集因 VFS 而异，
** 并且同一 VFS 的不同版本之间也可能不同。使用这些接口的应用程序必须准备好这些接口中的任何一个
** 或全部为 NULL，或者它们的行为从一个版本到下一个版本发生变化。如果 VFS 的 iVersion 小于 3，
** 应用程序不得尝试访问这些方法中的任何一个。
*/
typedef struct sqlite3_vfs sqlite3_vfs;
typedef void (*sqlite3_syscall_ptr)(void);
struct sqlite3_vfs {
	int iVersion;            /* 结构版本号（当前为3） */
	int szOsFile;            /* 子类化的 sqlite3_file 的大小 */
	int mxPathname;          /* 文件路径名的最大长度 */
	sqlite3_vfs *pNext;      /* 下一个注册的 VFS */
	const char *zName;       /* 该虚拟文件系统的名称 */
	void *pAppData;          /* 指向应用程序特定数据的指针 */
  int (*xOpen)(sqlite3_vfs*, const char *zName, sqlite3_file*,
			   int flags, int *pOutFlags);
  int (*xDelete)(sqlite3_vfs*, const char *zName, int syncDir);
  int (*xAccess)(sqlite3_vfs*, const char *zName, int flags, int *pResOut);
  int (*xFullPathname)(sqlite3_vfs*, const char *zName, int nOut, char *zOut);
  void *(*xDlOpen)(sqlite3_vfs*, const char *zFilename);
  void (*xDlError)(sqlite3_vfs*, int nByte, char *zErrMsg);
  void (*(*xDlSym)(sqlite3_vfs*,void*, const char *zSymbol))(void);
  void (*xDlClose)(sqlite3_vfs*, void*);
  int (*xRandomness)(sqlite3_vfs*, int nByte, char *zOut);
  int (*xSleep)(sqlite3_vfs*, int microseconds);
  int (*xCurrentTime)(sqlite3_vfs*, double*);
  int (*xGetLastError)(sqlite3_vfs*, int, char *);
  /*
  ** 上述方法是 sqlite_vfs 对象定义的第1版中的方法。
  ** 接下来的方法是在第2版或更高版本中添加的。
  */
  int (*xCurrentTimeInt64)(sqlite3_vfs*, sqlite3_int64*);
  /*
  ** 上面的方法属于sqlite_vfs对象的第1版和第2版。
  ** 下面的是第3版及更高版本的方法。
  */
  int (*xSetSystemCall)(sqlite3_vfs*, const char *zName, sqlite3_syscall_ptr);
  sqlite3_syscall_ptr (*xGetSystemCall)(sqlite3_vfs*, const char *zName);
  const char *(*xNextSystemCall)(sqlite3_vfs*, const char *zName);
  /*
  ** 上述方法是 sqlite_vfs 对象的第1版到第3版中的方法。
  ** 可以在后续版本中追加新字段。iVersion 值将在每次发生这种情况时递增。
  */
};

/*
** CAPI3REF: xAccess VFS 方法的标志位
**
** 这些整型常量可作为 [sqlite3_vfs] 对象的 xAccess 方法的第三个参数使用。
** 它们决定了 xAccess 方法要检查的权限类型。
** 使用 SQLITE_ACCESS_EXISTS 时，xAccess 方法
** 仅检查文件是否存在。
** 使用 SQLITE_ACCESS_READWRITE 时，xAccess 方法
** 检查指定目录是否可读且可写
** （换句话说，是否可以在该目录中添加、删除和重命名文件）。
** 目前，SQLITE_ACCESS_READWRITE 常量仅被
** [temp_store_directory pragma] 使用，不过在未来的 SQLite 版本中
** 可能会有所变化。
** 使用 SQLITE_ACCESS_READ 时，xAccess 方法
** 检查文件是否可读。目前 SQLITE_ACCESS_READ 常量
** 并未被使用，但在未来的 SQLite 版本中
** 可能会被使用。
*/
#define SQLITE_ACCESS_EXISTS    0
#define SQLITE_ACCESS_READWRITE 1   /* 由PRAGMA temp_store_directory使用 */
#define SQLITE_ACCESS_READ      2   /* 未使用的 */

/*
** CAPI3REF: xShmLock VFS 方法的标志
**
** 这些整数常量定义了 [sqlite3_io_methods] 的 xShmLock 方法允许的各种锁定操作。
** 以下是 xShmLock 方法的唯一合法标志组合：
**
** <ul>
** <li>  SQLITE_SHM_LOCK | SQLITE_SHM_SHARED
** <li>  SQLITE_SHM_LOCK | SQLITE_SHM_EXCLUSIVE
** <li>  SQLITE_SHM_UNLOCK | SQLITE_SHM_SHARED
** <li>  SQLITE_SHM_UNLOCK | SQLITE_SHM_EXCLUSIVE
** </ul>
**
** 解锁时，必须提供与相应锁相同的 SHARED 或 EXCLUSIVE 标志。
**
** xShmLock 方法可以在未锁定和 SHARED 之间或未锁定和 EXCLUSIVE 之间转换。
** 它不能在 SHARED 和 EXCLUSIVE 之间转换。
*/
#define SQLITE_SHM_UNLOCK       1
#define SQLITE_SHM_LOCK         2
#define SQLITE_SHM_SHARED       4
#define SQLITE_SHM_EXCLUSIVE    8

/*
** CAPI3REF: 最大 xShmLock 索引
**
** [sqlite3_io_methods] 上的 xShmLock 方法可以使用从 0 到这个上限的值作为其 "offset" 参数。
** SQLite 核心永远不会尝试获取或释放这个范围之外的锁。
*/
#define SQLITE_SHM_NLOCK        8

/*
** CAPI3REF: 初始化 SQLite 库
**
** ^sqlite3_initialize() 例程初始化 SQLite 库。^sqlite3_shutdown() 例程
** 释放由 sqlite3_initialize() 分配的任何资源。这些例程旨在帮助嵌入式系统中的
** 进程初始化和关闭。使用 SQLite 的工作站应用程序通常不需要调用这些例程。
**
** 如果 sqlite3_initialize() 是进程生命周期内第一次调用，或者是在调用 sqlite3_shutdown() 后
** 第一次调用 sqlite3_initialize()，则该调用是“有效的”。^(只有有效的 sqlite3_initialize() 调用
** 才会执行初始化。所有其他调用都是无害的无操作。)^
**
** 如果 sqlite3_shutdown() 是自上次 sqlite3_initialize() 以来第一次调用 sqlite3_shutdown()，
** 则该调用是“有效的”。^(只有有效的 sqlite3_shutdown() 调用才会执行反初始化。
** 所有其他有效的 sqlite3_shutdown() 调用都是无害的无操作。)^
**
** sqlite3_initialize() 接口是线程安全的，但 sqlite3_shutdown() 不是。
** sqlite3_shutdown() 接口必须仅从单个线程调用。在调用 sqlite3_shutdown() 之前，
** 必须关闭所有打开的 [数据库连接]，并且必须释放所有其他 SQLite 资源。
**
** 除其他外，^sqlite3_initialize() 将调用 sqlite3_os_init()。类似地，^sqlite3_shutdown()
** 将调用 sqlite3_os_end()。
**
** ^sqlite3_initialize() 例程在成功时返回 [SQLITE_OK]。^如果由于某种原因，sqlite3_initialize()
** 无法初始化库（例如无法分配所需的资源，如互斥锁），则返回 [SQLITE_OK] 以外的 [错误代码]。
**
** ^sqlite3_initialize() 例程由许多其他 SQLite 接口内部调用，因此应用程序通常不需要
** 直接调用 sqlite3_initialize()。例如，[sqlite3_open()] 调用 sqlite3_initialize()，
** 因此如果尚未初始化，则在调用 [sqlite3_open()] 时将自动初始化 SQLite 库。
** ^但是，如果 SQLite 使用 [SQLITE_OMIT_AUTOINIT] 编译时选项编译，则省略对 sqlite3_initialize()
** 的自动调用，应用程序必须在使用任何其他 SQLite 接口之前直接调用 sqlite3_initialize()。
** 为了获得最大的可移植性，建议应用程序始终在使用任何其他 SQLite 接口之前直接调用 sqlite3_initialize()。
** 未来的 SQLite 版本可能需要这样做。换句话说，使用 [SQLITE_OMIT_AUTOINIT] 编译 SQLite 时
** 表现出的行为可能会成为未来某些 SQLite 版本的默认行为。
**
** sqlite3_os_init() 例程执行 SQLite 库的特定于操作系统的初始化。sqlite3_os_end() 例程
** 撤销 sqlite3_os_init() 的效果。这些例程执行的典型任务包括分配或释放静态资源、
** 初始化全局变量、设置默认的 [sqlite3_vfs] 模块，或使用 [sqlite3_config()] 设置默认配置。
**
** 应用程序永远不应直接调用 sqlite3_os_init() 或 sqlite3_os_end()。应用程序应仅调用
** sqlite3_initialize() 和 sqlite3_shutdown()。sqlite3_os_init() 接口由 sqlite3_initialize()
** 自动调用，sqlite3_os_end() 由 sqlite3_shutdown() 调用。当 SQLite 为 Unix、Windows 或 OS/2 编译时，
** 内置了 sqlite3_os_init() 和 sqlite3_os_end() 的适当实现。当 [自定义构建 | 为其他平台构建]
** （使用 [SQLITE_OS_OTHER=1] 编译时选项）时，应用程序必须提供 sqlite3_os_init() 和 sqlite3_os_end()
** 的合适实现。应用程序提供的 sqlite3_os_init() 或 sqlite3_os_end() 实现必须在成功时返回 [SQLITE_OK]，
** 并在失败时返回其他 [错误代码]。
*/
SQLITE_API int SQLITE_STDCALL sqlite3_initialize(void);
SQLITE_API int SQLITE_STDCALL sqlite3_shutdown(void);
SQLITE_API int SQLITE_STDCALL sqlite3_os_init(void);
SQLITE_API int SQLITE_STDCALL sqlite3_os_end(void);

/*
** CAPI3REF: 配置SQLite库
**
** sqlite3_config()接口用于对SQLite进行全局配置更改，
** 以调整SQLite以满足应用程序的特定需求。默认配置
** 对于大多数应用程序是推荐的，因此通常不需要此
** 例程。它提供是为了支持具有特殊需求的罕见应用程序。
**
** sqlite3_config()接口不是线程安全的。应用程序必须
** 确保在sqlite3_config()运行时没有其他线程调用其他
** SQLite接口。此外，sqlite3_config()只能在库初始化
** 使用[sqlite3_initialize()]之前或关闭后使用
** [sqlite3_shutdown()]。^如果在[sqlite3_initialize()]之后
** 和[sqlite3_shutdown()]之前调用sqlite3_config()，则
** 它将返回SQLITE_MISUSE。请注意，然而，sqlite3_config()
** 可以作为应用程序定义的[sqlite3_os_init()]实现的一部分调用。
**
** sqlite3_config()的第一个参数是一个整数[配置选项]，
** 决定了要配置的SQLite的哪个属性。后续参数根据第一个
** 参数中的[配置选项]而变化。
**
** ^当设置配置选项时，sqlite3_config()返回[SQLITE_OK]。
** ^如果选项未知或SQLite无法设置该选项，则此例程返回
** 非零[错误代码]。
*/
SQLITE_API int SQLITE_CDECL sqlite3_config(int, ...);

/*
** CAPI3REF: 配置数据库连接
** METHOD: sqlite3
**
** sqlite3_db_config() 接口用于对 [数据库连接] 进行配置更改。该接口类似于
** [sqlite3_config()]，但更改仅适用于单个 [数据库连接]（在第一个参数中指定）。
**
** sqlite3_db_config(D, V, ...) 的第二个参数是
** [SQLITE_DBCONFIG_LOOKASIDE | 配置动词] - 一个整型代码，指示正在配置的
** [数据库连接] 的哪个方面。后续参数根据配置动词的不同而变化。
**
** ^只有当调用被认为是成功时，对 sqlite3_db_config() 的调用才会返回 SQLITE_OK。
*/
SQLITE_API int SQLITE_CDECL sqlite3_db_config(sqlite3*, int op, ...);

/*
** CAPI3REF: 内存分配例程
**
** 此对象的实例定义了SQLite与低级内存分配例程之间的接口。
**
** 此对象仅在SQLite接口中的一个地方使用。
** 当配置选项为[SQLITE_CONFIG_MALLOC]或[SQLITE_CONFIG_GETMALLOC]时，
** 指向此对象实例的指针是[sqlite3_config()]的参数。
** 通过创建此对象的实例并将其传递给配置过程中的
** [sqlite3_config]([SQLITE_CONFIG_MALLOC])，
** 应用程序可以指定SQLite用于所有动态内存需求的替代内存分配子系统。
**
** 请注意，SQLite带有几个[内置内存分配器]，它们对于绝大多数应用程序来说已经足够，
** 而此对象仅对极少数具有专门内存分配需求的应用程序有用。
** 此对象还用于在测试SQLite时指定一个替代内存分配器，
** 以模拟内存耗尽条件，以验证SQLite是否能从这种条件下优雅地恢复。
**
** xMalloc、xRealloc和xFree方法必须像标准C库中的malloc()、realloc()和free()函数一样工作。
** ^SQLite保证xRealloc的第二个参数总是之前调用xRoundup返回的值。
**
** xSize应返回从xMalloc或xRealloc获得的内存分配的已分配大小。
** 已分配的大小至少与请求的大小一样大，但可能更大。
**
** xRoundup方法返回给定请求大小的内存分配的分配大小。
** 大多数内存分配器至少将内存分配向上舍入到下一个8的倍数。
** 有些分配器向上舍入到更大的倍数或2的幂。
** 通过[sqlite3_malloc()]或[sqlite3_realloc()]传入的每个内存分配请求首先调用xRoundup。
** 如果xRoundup返回0，则导致相应的内存分配失败。
**
** xInit方法初始化内存分配器。例如，它可能会分配任何所需的互斥锁或初始化内部数据结构。
** xShutdown方法由[sqlite3_shutdown()]间接调用，应释放xInit获取的任何资源。
** pAppData指针用作xInit和xShutdown的唯一参数。
**
** SQLite在调用xInit方法时持有[SQLITE_MUTEX_STATIC_MASTER]互斥锁，
** 因此xInit方法不需要是线程安全的。xShutdown方法仅从[sqlite3_shutdown()]调用，
** 因此也不需要是线程安全的。对于所有其他方法，只要[SQLITE_CONFIG_MEMSTATUS]配置选项打开（默认是打开的），
** SQLite就会持有[SQLITE_MUTEX_STATIC_MEM]互斥锁，因此这些方法会自动序列化。
** 然而，如果禁用了[SQLITE_CONFIG_MEMSTATUS]，则其他方法必须是线程安全的，
** 或者必须为自己安排序列化。
**
** SQLite永远不会在调用xShutdown()之前调用xInit()超过一次。
*/
typedef struct sqlite3_mem_methods sqlite3_mem_methods;
struct sqlite3_mem_methods {
	void *(*xMalloc)(int);         /* 内存分配函数 */
	void (*xFree)(void*);          /* 释放之前分配的内存 */
	void *(*xRealloc)(void*,int);  /* 调整已分配内存的大小 */
	int (*xSize)(void*);           /* 返回已分配内存的大小 */
	int (*xRoundup)(int);          /* 将请求的大小向上舍入为分配大小 */
	int (*xInit)(void*);           /* 初始化内存分配器 */
	void (*xShutdown)(void*);      /* 反初始化内存分配器 */
	void *pAppData;                /* 传递给 xInit() 和 xShutdown() 的参数 */
};

/*
** CAPI3REF: 配置选项
** 关键字: {配置选项}
**
** 这些常量是可以通过 [sqlite3_config()] 接口的第一个参数传递的可用整数配置选项。
**
** 在未来的 SQLite 版本中可能会添加新的配置选项。
** 现有的配置选项可能会被弃用。应用程序应检查 [sqlite3_config()] 的返回代码以确保调用成功。
** 如果调用已弃用或不支持的配置选项，[sqlite3_config()] 接口将返回非零的 [错误代码]。
**
** <dl>
** [[SQLITE_CONFIG_SINGLETHREAD]] <dt>SQLITE_CONFIG_SINGLETHREAD</dt>
** <dd>此选项没有参数。^此选项将 [线程模式] 设置为单线程。换句话说，它禁用所有互斥锁，并将 SQLite 置于只能由单个线程使用的模式。
** ^如果 SQLite 使用 [SQLITE_THREADSAFE | SQLITE_THREADSAFE=0] 编译时选项编译，则无法将 [线程模式] 从其默认的单线程值更改，
** 因此如果使用 SQLITE_CONFIG_SINGLETHREAD 配置选项调用 [sqlite3_config()]，将返回 [SQLITE_ERROR]。</dd>
**
** [[SQLITE_CONFIG_MULTITHREAD]] <dt>SQLITE_CONFIG_MULTITHREAD</dt>
** <dd>此选项没有参数。^此选项将 [线程模式] 设置为多线程。换句话说，它禁用 [数据库连接] 和 [预编译语句] 对象上的互斥锁。
** 应用程序负责对 [数据库连接] 和 [预编译语句] 的访问进行序列化。但其他互斥锁已启用，因此只要没有两个线程同时尝试使用同一个 [数据库连接]，
** SQLite 就可以在多线程环境中安全使用。^如果 SQLite 使用 [SQLITE_THREADSAFE | SQLITE_THREADSAFE=0] 编译时选项编译，
** 则无法设置多线程 [线程模式]，如果使用 SQLITE_CONFIG_MULTITHREAD 配置选项调用 [sqlite3_config()]，将返回 [SQLITE_ERROR]。</dd>
**
** [[SQLITE_CONFIG_SERIALIZED]] <dt>SQLITE_CONFIG_SERIALIZED</dt>
** <dd>此选项没有参数。^此选项将 [线程模式] 设置为序列化。换句话说，此选项启用所有互斥锁，包括 [数据库连接] 和 [预编译语句] 对象上的递归互斥锁。
** 在此模式下（当 SQLite 使用 [SQLITE_THREADSAFE=1] 编译时的默认模式），SQLite 库本身将对 [数据库连接] 和 [预编译语句] 的访问进行序列化，
** 因此应用程序可以自由地在不同线程中同时使用相同的 [数据库连接] 或相同的 [预编译语句]。
** ^如果 SQLite 使用 [SQLITE_THREADSAFE | SQLITE_THREADSAFE=0] 编译时选项编译，则无法设置序列化 [线程模式]，
** 如果使用 SQLITE_CONFIG_SERIALIZED 配置选项调用 [sqlite3_config()]，将返回 [SQLITE_ERROR]。</dd>
**
** [[SQLITE_CONFIG_MALLOC]] <dt>SQLITE_CONFIG_MALLOC</dt>
** <dd> ^(SQLITE_CONFIG_MALLOC 选项接受一个参数，该参数是指向 [sqlite3_mem_methods] 结构体实例的指针。
** 该参数指定用于替代 SQLite 内置内存分配例程的低级内存分配例程。)^ ^SQLite 在 [sqlite3_config()] 调用返回之前，
** 会创建 [sqlite3_mem_methods] 结构体内容的私有副本。</dd>
**
** [[SQLITE_CONFIG_GETMALLOC]] <dt>SQLITE_CONFIG_GETMALLOC</dt>
** <dd> ^(SQLITE_CONFIG_GETMALLOC 选项接受一个参数，该参数是指向 [sqlite3_mem_methods] 结构体实例的指针。
** [sqlite3_mem_methods] 结构体将填充当前定义的内存分配例程。)^ 此选项可用于重载默认的内存分配例程，
** 例如使用包装器模拟内存分配失败或跟踪内存使用情况。</dd>
**
** [[SQLITE_CONFIG_MEMSTATUS]] <dt>SQLITE_CONFIG_MEMSTATUS</dt>
** <dd> ^SQLITE_CONFIG_MEMSTATUS 选项接受一个 int 类型的参数，解释为布尔值，用于启用或禁用内存分配统计信息的收集。
** ^(当内存分配统计信息被禁用时，以下 SQLite 接口将变得不可操作：
**   <ul>
**   <li> [sqlite3_memory_used()]
**   <li> [sqlite3_memory_highwater()]
**   <li> [sqlite3_soft_heap_limit64()]
**   <li> [sqlite3_status64()]
**   </ul>)^
** ^默认情况下，内存分配统计信息是启用的，除非 SQLite 使用 [SQLITE_DEFAULT_MEMSTATUS]=0 编译，在这种情况下，内存分配统计信息默认是禁用的。
** </dd>
**
** [[SQLITE_CONFIG_SCRATCH]] <dt>SQLITE_CONFIG_SCRATCH</dt>
** <dd> ^SQLITE_CONFIG_SCRATCH 选项指定 SQLite 可用于临时内存的静态内存缓冲区。
** ^(SQLITE_CONFIG_SCRATCH 有三个参数：一个指向 8 字节对齐内存缓冲区的指针，用于从中分配临时内存，
** 每个临时分配的大小 (sz)，以及最大临时分配数量 (N)。)^ 第一个参数必须是指向至少 sz*N 字节内存的 8 字节对齐缓冲区的指针。
** ^SQLite 每个线程不会使用超过一个临时缓冲区。
** ^SQLite 永远不会请求超过数据库页面大小 6 倍的临时缓冲区。
** ^如果 SQLite 需要超出此配置选项提供的额外临时内存，则将使用 [sqlite3_malloc()] 获取所需的内存。<p>
** ^当应用程序使用 SQLITE_CONFIG_SCRATCH 提供任何数量的临时内存时，SQLite 会避免不必要的大 [sqlite3_malloc|堆分配]。
** 这可以帮助 [Robson 证明|防止内存分配失败]，在低内存嵌入式系统中由于堆碎片化而导致的内存分配失败。
** </dd>
**
** [[SQLITE_CONFIG_PAGECACHE]] <dt>SQLITE_CONFIG_PAGECACHE</dt>
** <dd> ^SQLITE_CONFIG_PAGECACHE 选项指定 SQLite 可用于默认页面缓存实现的静态内存缓冲区。
** 如果使用 [SQLITE_CONFIG_PCACHE2] 配置选项加载了应用程序定义的页面缓存实现，则不应使用此配置。
** ^SQLITE_CONFIG_PAGECACHE 有三个参数：一个指向 8 字节对齐内存的指针，每个页面缓冲区的大小 (sz)，以及页面数量 (N)。
** sz 参数应该是最大数据库页面大小（512 到 65536 之间的 2 的幂）加上每个页面头部的额外字节数。
** ^可以使用 [sqlite3_config()] 的 [SQLITE_CONFIG_PCACHE_HDRSZ] 选项确定页面头部所需的额外字节数。
** ^除了浪费内存外，sz 参数大于必要值是无害的。第一个参数应指向至少 sz*N 字节内存的 8 字节对齐内存块，否则后续行为未定义。
** ^SQLite 将使用第一个参数提供的内存来满足其添加到缓存的前 N 页的内存需求。
** ^如果超出此选项提供的页面缓存内存需求，SQLite 将使用 [sqlite3_malloc()] 获取额外的存储空间。</dd>
**
** [[SQLITE_CONFIG_HEAP]] <dt>SQLITE_CONFIG_HEAP</dt>
** <dd> ^SQLITE_CONFIG_HEAP 选项指定 SQLite 将用于其所有动态内存分配需求的静态内存缓冲区，
** 超出 [SQLITE_CONFIG_SCRATCH] 和 [SQLITE_CONFIG_PAGECACHE] 提供的需求。
** ^SQLITE_CONFIG_HEAP 选项仅在 SQLite 使用 [SQLITE_ENABLE_MEMSYS3] 或 [SQLITE_ENABLE_MEMSYS5] 编译时可用，
** 否则调用时将返回 [SQLITE_ERROR]。
** ^SQLITE_CONFIG_HEAP 有三个参数：一个 8 字节对齐的内存指针，内存缓冲区中的字节数，以及最小分配大小。
** ^如果第一个指针（内存指针）为 NULL，则 SQLite 恢复使用其默认内存分配器（系统 malloc() 实现），
** 撤销任何先前对 [SQLITE_CONFIG_MALLOC] 的调用。^如果内存指针不为 NULL，则替代内存分配器将处理 SQLite 的所有内存分配需求。
** 第一个指针（内存指针）必须对齐到 8 字节边界，否则 SQLite 的后续行为未定义。
** 最小分配大小上限为 2**12。最小分配大小的合理值为 2**5 到 2**8。</dd>
**
** [[SQLITE_CONFIG_MUTEX]] <dt>SQLITE_CONFIG_MUTEX</dt>
** <dd> ^(SQLITE_CONFIG_MUTEX 选项接受一个参数，该参数是指向 [sqlite3_mutex_methods] 结构体实例的指针。
** 该参数指定用于替代 SQLite 内置互斥例程的低级互斥例程。)^ ^SQLite 在 [sqlite3_config()] 调用返回之前，
** 会创建 [sqlite3_mutex_methods] 结构体内容的副本。^如果 SQLite 使用 [SQLITE_THREADSAFE | SQLITE_THREADSAFE=0] 编译时选项编译，
** 则互斥子系统将从构建中省略，因此使用 SQLITE_CONFIG_MUTEX 配置选项调用 [sqlite3_config()] 将返回 [SQLITE_ERROR]。</dd>
**
** [[SQLITE_CONFIG_GETMUTEX]] <dt>SQLITE_CONFIG_GETMUTEX</dt>
** <dd> ^(SQLITE_CONFIG_GETMUTEX 选项接受一个参数，该参数是指向 [sqlite3_mutex_methods] 结构体实例的指针。
** [sqlite3_mutex_methods] 结构体将填充当前定义的互斥例程。)^ 此选项可用于重载默认的互斥分配例程，
** 例如使用包装器跟踪互斥使用情况以进行性能分析或测试。^如果 SQLite 使用 [SQLITE_THREADSAFE | SQLITE_THREADSAFE=0] 编译时选项编译，
** 则互斥子系统将从构建中省略，因此使用 SQLITE_CONFIG_GETMUTEX 配置选项调用 [sqlite3_config()] 将返回 [SQLITE_ERROR]。</dd>
**
** [[SQLITE_CONFIG_LOOKASIDE]] <dt>SQLITE_CONFIG_LOOKASIDE</dt>
** <dd> ^(SQLITE_CONFIG_LOOKASIDE 选项接受两个参数，用于确定每个 [数据库连接] 的默认后备内存大小。
** 第一个参数是每个后备缓冲区槽的大小，第二个参数是分配给每个数据库连接的槽数。)^ ^(SQLITE_CONFIG_LOOKASIDE
** 设置 <i>默认</i> 后备大小。可以使用 [sqlite3_db_config()] 的 [SQLITE_DBCONFIG_LOOKASIDE] 选项更改单个连接的后备配置。)^ </dd>
**
** [[SQLITE_CONFIG_PCACHE2]] <dt>SQLITE_CONFIG_PCACHE2</dt>
** <dd> ^(SQLITE_CONFIG_PCACHE2 选项接受一个参数，该参数是指向 [sqlite3_pcache_methods2] 对象的指针。
** 此对象指定自定义页面缓存实现的接口。)^ ^SQLite 会创建 [sqlite3_pcache_methods2] 对象的副本。</dd>
**
** [[SQLITE_CONFIG_GETPCACHE2]] <dt>SQLITE_CONFIG_GETPCACHE2</dt>
** <dd> ^(SQLITE_CONFIG_GETPCACHE2 选项接受一个参数，该参数是指向 [sqlite3_pcache_methods2] 对象的指针。
** SQLite 将当前页面缓存实现的副本复制到该对象中。)^ </dd>
**
** [[SQLITE_CONFIG_LOG]] <dt>SQLITE_CONFIG_LOG</dt>
** <dd> SQLITE_CONFIG_LOG 选项用于配置 SQLite 全局 [错误日志]。
** (^SQLITE_CONFIG_LOG 选项接受两个参数：一个指向具有 void(*)(void*,int,const char*) 调用签名的函数的指针，
** 以及一个指向 void 的指针。^如果函数指针不为 NULL，则 [sqlite3_log()] 调用该函数处理每个日志事件。
** ^如果函数指针为 NULL，则 [sqlite3_log()] 接口变为无操作。^作为 SQLITE_CONFIG_LOG 第二个参数的 void 指针，
** 在每次调用应用程序定义的日志函数时作为第一个参数传递。^日志函数的第二个参数是对应 [sqlite3_log()] 调用的第一个参数的副本，
** 应为 [结果代码] 或 [扩展结果代码]。^传递给日志函数的第三个参数是经过 [sqlite3_snprintf()] 格式化后的日志消息。
** SQLite 日志接口不可重入；应用程序提供的日志函数不得调用任何 SQLite 接口。
** 在多线程应用程序中，应用程序定义的日志函数必须是线程安全的。 </dd>
**
** [[SQLITE_CONFIG_URI]] <dt>SQLITE_CONFIG_URI
** <dd>^(SQLITE_CONFIG_URI 选项接受一个 int 类型的参数。如果非零，则全局启用 URI 处理。如果参数为零，则全局禁用 URI 处理。)^
** ^如果全局启用 URI 处理，则传递给 [sqlite3_open()]、[sqlite3_open_v2()]、[sqlite3_open16()] 或作为 [ATTACH] 命令一部分的所有文件名都将被解释为 URI，
** 无论打开数据库连接时是否设置了 [SQLITE_OPEN_URI] 标志。^如果全局禁用 URI 处理，则只有在打开数据库连接时设置了 SQLITE_OPEN_URI 标志时，文件名才会被解释为 URI。
** ^(默认情况下，URI 处理是全局禁用的。可以通过使用 [SQLITE_USE_URI] 符号定义编译来更改默认值。)^
**
** [[SQLITE_CONFIG_COVERING_INDEX_SCAN]] <dt>SQLITE_CONFIG_COVERING_INDEX_SCAN
** <dd>^SQLITE_CONFIG_COVERING_INDEX_SCAN 选项接受一个整数参数，解释为布尔值，用于启用或禁用查询优化器中使用覆盖索引进行全表扫描。
** ^默认设置由 [SQLITE_ALLOW_COVERING_INDEX_SCAN] 编译时选项确定，如果省略该编译时选项，则默认为“启用”。
** 禁用覆盖索引用于全表扫描的能力是因为某些错误编码的遗留应用程序在启用优化时可能会发生故障。
** 提供禁用优化的能力允许旧的、有缺陷的应用程序代码无需更改即可在新版本的 SQLite 中工作。
**
** [[SQLITE_CONFIG_PCACHE]] [[SQLITE_CONFIG_GETPCACHE]]
** <dt>SQLITE_CONFIG_PCACHE 和 SQLITE_CONFIG_GETPCACHE
** <dd> 这些选项已过时，新代码不应使用。它们保留用于向后兼容，但现在是无操作的。
** </dd>
**
** [[SQLITE_CONFIG_SQLLOG]]
** <dt>SQLITE_CONFIG_SQLLOG
** <dd>此选项仅在 SQLite 使用 [SQLITE_ENABLE_SQLLOG] 预处理器宏定义编译时可用。第一个参数应是指向类型为 void(*)(void*,sqlite3*,const char*, int) 的函数的指针。
** 第二个参数应为 (void*) 类型。库在三种不同的情况下调用回调，由作为第四个参数传递的值标识。如果第四个参数为 0，则作为第二个参数传递的数据库连接刚刚打开。
** 第三个参数指向包含主数据库文件名称的缓冲区。如果第四个参数为 1，则第三个参数指向的 SQL 语句刚刚执行。或者，如果第四个参数为 2，
** 则作为第二个参数传递的连接正在关闭。在这种情况下，第三个参数传递 NULL。可以在规范 SQLite 源代码树中的 "test_sqllog.c" 源文件中看到使用此配置选项的示例。</dd>
**
** [[SQLITE_CONFIG_MMAP_SIZE]]
** <dt>SQLITE_CONFIG_MMAP_SIZE
** <dd>^SQLITE_CONFIG_MMAP_SIZE 接受两个 64 位整数 (sqlite3_int64) 值，分别是默认的 mmap 大小限制（[PRAGMA mmap_size] 的默认设置）和允许的最大 mmap 大小限制。
** ^默认设置可以通过每个数据库连接使用 [PRAGMA mmap_size] 命令或 [SQLITE_FCNTL_MMAP_SIZE] 文件控制覆盖。
** ^(允许的最大 mmap 大小将根据需要静默截断，以确保不超过由 [SQLITE_MAX_MMAP_SIZE] 编译时选项设置的编译时最大 mmap 大小。)^
** ^如果此选项的任一参数为负，则该参数将更改为其编译时默认值。
**
** [[SQLITE_CONFIG_WIN32_HEAPSIZE]]
** <dt>SQLITE_CONFIG_WIN32_HEAPSIZE
** <dd>^SQLITE_CONFIG_WIN32_HEAPSIZE 选项仅在 SQLite 使用 [SQLITE_WIN32_MALLOC] 预处理器宏定义为 Windows 编译时可用。
** ^SQLITE_CONFIG_WIN32_HEAPSIZE 接受一个 32 位无符号整数值，指定创建的堆的最大大小。
**
** [[SQLITE_CONFIG_PCACHE_HDRSZ]]
** <dt>SQLITE_CONFIG_PCACHE_HDRSZ
** <dd>^SQLITE_CONFIG_PCACHE_HDRSZ 选项接受一个参数，该参数是指向整数的指针，并将 [SQLITE_CONFIG_PAGECACHE] 中每页所需的额外字节数写入该整数。
** 所需的额外空间量可能会根据编译器、目标平台和 SQLite 版本而变化。
**
** [[SQLITE_CONFIG_PMASZ]]
** <dt>SQLITE_CONFIG_PMASZ
** <dd>^SQLITE_CONFIG_PMASZ 选项接受一个参数，该参数是一个无符号整数，并将多线程排序器的“最小 PMA 大小”设置为该整数。
** 默认的最小 PMA 大小由 [SQLITE_SORTER_PMASZ] 编译时选项设置。当启用多线程排序（使用 [PRAGMA threads] 命令）且要排序的内容量超过页面大小乘以 [PRAGMA cache_size] 设置和此值的最小值时，
** 将启动新线程以帮助进行排序操作。
** </dl>
*/
#define SQLITE_CONFIG_SINGLETHREAD  1  /* 空 */
#define SQLITE_CONFIG_MULTITHREAD   2  /* 空 */
#define SQLITE_CONFIG_SERIALIZED    3  /* 空 */
#define SQLITE_CONFIG_MALLOC        4  /* sqlite3_mem_methods* */
#define SQLITE_CONFIG_GETMALLOC     5  /* sqlite3_mem_methods* */
#define SQLITE_CONFIG_SCRATCH       6  /* void*, int sz, int N */
#define SQLITE_CONFIG_PAGECACHE     7  /* void*, int sz, int N */
#define SQLITE_CONFIG_HEAP          8  /* void*, int nByte, int min */
#define SQLITE_CONFIG_MEMSTATUS     9  /* 布尔值 */
#define SQLITE_CONFIG_MUTEX        10  /* sqlite3_mutex_methods* */
#define SQLITE_CONFIG_GETMUTEX     11  /* sqlite3_mutex_methods* */
/* 之前的 SQLITE_CONFIG_CHUNKALLOC 12 现已未使用。 */
#define SQLITE_CONFIG_LOOKASIDE    13  /* int int */
#define SQLITE_CONFIG_PCACHE       14  /* 无操作 */
#define SQLITE_CONFIG_GETPCACHE    15  /* 无操作 */
#define SQLITE_CONFIG_LOG          16  /* xFunc, void* */
#define SQLITE_CONFIG_URI          17  /* int */
#define SQLITE_CONFIG_PCACHE2      18  /* sqlite3_pcache_methods2* */
#define SQLITE_CONFIG_GETPCACHE2   19  /* sqlite3_pcache_methods2* */
#define SQLITE_CONFIG_COVERING_INDEX_SCAN 20  /* int */
#define SQLITE_CONFIG_SQLLOG       21  /* xSqllog, void* */
#define SQLITE_CONFIG_MMAP_SIZE    22  /* sqlite3_int64, sqlite3_int64 */
#define SQLITE_CONFIG_WIN32_HEAPSIZE      23  /* int nByte */
#define SQLITE_CONFIG_PCACHE_HDRSZ        24  /* int *psz */
#define SQLITE_CONFIG_PMASZ               25  /* unsigned int szPma */

/*
** CAPI3REF: 数据库连接配置选项
**
** 这些常量是可以作为第二个参数传递给 [sqlite3_db_config()] 接口的可用整数配置选项。
**
** 将来版本的SQLite可能会添加新的配置选项。
** 现有的配置选项可能会被弃用。应用程序应该检查 [sqlite3_db_config()] 的返回码以确保调用成功。
** ^如果调用了一个已弃用或不支持的配置选项，[sqlite3_db_config()] 接口将返回一个非零的 [错误码]。
**
** <dl>
** <dt>SQLITE_DBCONFIG_LOOKASIDE</dt>
** <dd> ^此选项需要三个额外的参数，这些参数决定了 [数据库连接] 的 [lookaside内存分配器] 配置。
** ^第一个参数（[sqlite3_db_config()] 的第三个参数）是一个指向用于lookaside内存的内存缓冲区的指针。
** ^SQLITE_DBCONFIG_LOOKASIDE 动词后的第一个参数可以是NULL，在这种情况下，SQLite将使用 [sqlite3_malloc()] 分配lookaside缓冲区。
** ^第二个参数是每个lookaside缓冲区插槽的大小。^第三个参数是插槽的数量。第一个参数中的缓冲区大小必须大于或等于第二个和第三个参数的乘积。
** 缓冲区必须按8字节对齐。^如果传递给 SQLITE_DBCONFIG_LOOKASIDE 的第二个参数不是8的倍数，它将在内部向下舍入到下一个较小的8的倍数。
** ^(数据库连接的lookaside内存配置只能在当前未使用lookaside内存时更改，换句话说，当 [sqlite3_db_status](D,[SQLITE_CONFIG_LOOKASIDE],...) 返回的 "当前值" 为零时。
** 任何在lookaside内存正在使用时尝试更改lookaside内存配置的尝试都将使配置保持不变并返回 [SQLITE_BUSY]。)^</dd>
**
** <dt>SQLITE_DBCONFIG_ENABLE_FKEY</dt>
** <dd> ^此选项用于启用或禁用 [外键约束] 的强制执行。应该有两个额外的参数。
** 第一个参数是一个整数，0表示禁用FK强制，正数表示启用FK强制，负数表示保持FK强制不变。
** 第二个参数是一个指向整数的指针，该整数将被写入0或1以指示此调用后FK强制是否关闭或开启。
** 第二个参数可以是NULL指针，在这种情况下，FK强制设置不会被报告回来。 </dd>
**
** <dt>SQLITE_DBCONFIG_ENABLE_TRIGGER</dt>
** <dd> ^此选项用于启用或禁用 [CREATE TRIGGER | 触发器]。应该有两个额外的参数。
** 第一个参数是一个整数，0表示禁用触发器，正数表示启用触发器，负数表示保持设置不变。
** 第二个参数是一个指向整数的指针，该整数将被写入0或1以指示此调用后触发器是否禁用或启用。
** 第二个参数可以是NULL指针，在这种情况下，触发器设置不会被报告回来。 </dd>
**
** </dl>
*/
#define SQLITE_DBCONFIG_LOOKASIDE       1001  /* void* int int */
#define SQLITE_DBCONFIG_ENABLE_FKEY     1002  /* int int* */
#define SQLITE_DBCONFIG_ENABLE_TRIGGER  1003  /* int int* */


/**
 * CAPI3REF: 启用或禁用扩展结果代码
 * 方法: sqlite3
 *
 * ^sqlite3_extended_result_codes() 函数用于启用或禁用 SQLite 的
 * [扩展结果代码] 功能。^默认情况下，扩展结果代码是禁用的，以保持与历史版本的兼容性。
 */
SQLITE_API int SQLITE_STDCALL sqlite3_extended_result_codes(sqlite3*, int onoff);

/**
 * CAPI3REF: 最后插入的行ID
 * 方法: sqlite3
 *
 * 在大多数SQLite表（除[WITHOUT ROWID]表外）中的每一项都有一个唯一的64位有符号整数键，称为[ROWID | "rowid"]。
 * 只要这些名称没有也被显式声明的列使用，rowid总是可以作为未声明列ROWID、OID或_ROWID_来使用。
 * 如果表有一个类型为[INTEGER PRIMARY KEY]的列，那么该列是rowid的另一个别名。
 *
 * sqlite3_last_insert_rowid(D)接口返回数据库连接D中最近成功插入到rowid表或[虚拟表]的[行id]。
 * 对[WITHOUT ROWID]表的插入不会被记录。
 * 如果数据库连接D从未发生过成功的对rowid表的[INSERT]，则sqlite3_last_insert_rowid(D)返回零。
 *
 * 如果在触发器或[虚拟表]方法内发生[INSERT]，则此例程将在触发器或虚拟表方法运行期间返回插入行的[行id]。
 * 但一旦触发器或虚拟表方法结束，此例程返回的值将恢复到触发器或虚拟表方法开始前的值。
 *
 * 由于约束违反而失败的[INSERT]不是成功的[INSERT]，不会改变此例程返回的值。
 * 因此，当它们的插入失败时，INSERT OR FAIL、INSERT OR IGNORE、INSERT OR ROLLBACK和INSERT OR ABORT不会改变此例程的返回值。
 * 当INSERT OR REPLACE遇到约束违反时，它不会失败。插入将继续完成，删除导致约束问题的行，因此INSERT OR REPLACE总是会改变此接口的返回值。
 *
 * 为了此例程的目的，即使随后被回滚，[INSERT]也被认为是成功的。
 *
 * 此函数可通过[last_insert_rowid() SQL函数]在SQL语句中访问。
 *
 * 如果一个单独的线程在[sqlite3_last_insert_rowid()]函数运行时对同一数据库连接执行新的[INSERT]，从而改变了最后插入的[行id]，
 * 则[sqlite3_last_insert_rowid()]返回的值是不可预测的，可能既不等于旧的也不等于新的最后插入的[行id]。
 */
SQLITE_API sqlite3_int64 SQLITE_STDCALL sqlite3_last_insert_rowid(sqlite3*);

/**
 * 计算修改的行数
 * 方法：sqlite3
 *
 * 这个函数返回最近完成的INSERT、UPDATE或DELETE语句在指定数据库连接上修改、插入或删除的行数。
 * 执行任何其他类型的SQL语句不会修改此函数返回的值。
 *
 * 仅考虑INSERT、UPDATE或DELETE语句直接做出的更改 - 由[CREATE TRIGGER | 触发器]、
 * [外键操作]或[REPLACE]约束解决引起的辅助更改不计入。
 * 
 * 由[INSTEAD OF触发器 | INSTEAD OF触发器]拦截的对视图的更改不计入。
 * 在视图上运行的INSERT、UPDATE或DELETE语句后立即调用sqlite3_changes()的值总是零。
 * 只计算对真实表的更改。
 *
 * 如果在触发器程序运行时执行sqlite3_changes()函数，情况会更复杂。这可能发生在
 * 程序使用[changes() SQL函数]的情况下，或者如果某些其他回调函数直接调用sqlite3_changes()。
 * 基本上：
 * 
 * <ul>
 * <li> 在进入触发器程序之前，sqlite3_changes()函数返回的值被保存。
 * 触发器程序结束后，原始值将被恢复。
 * 
 * <li> 在触发器程序内部，每个INSERT、UPDATE和DELETE语句在完成时都会像正常一样设置
 * sqlite3_changes()返回的值。当然，这个值不会包括子触发器执行的任何更改，
 * 因为在每个子触发器运行后，sqlite3_changes()值将被保存和恢复。
 * </ul>
 * 
 * 这意味着如果在触发器中的第一个INSERT、UPDATE或DELETE语句使用了changes() SQL函数（或类似函数），
 * 它返回的值是在调用语句开始执行时设置的值。
 * 如果它在触发器程序的第二个或后续此类语句中使用，返回的值反映了在同一触发器内
 * 前一个INSERT、UPDATE或DELETE语句修改的行数。
 *
 * 另请参阅[sqlite3_total_changes()]接口、[count_changes pragma]和[changes() SQL函数]。
 *
 * 如果在运行[sqlite3_changes()]时另一个线程对同一数据库连接进行更改，
 * 则返回的值是不可预测的且没有意义。
 */
SQLITE_API int SQLITE_STDCALL sqlite3_changes(sqlite3*);

/*
** CAPI3REF: 总修改的行数
** 方法: sqlite3
**
** ^此函数返回自数据库连接打开以来，所有完成的 [INSERT]、[UPDATE] 或
** [DELETE] 语句插入、修改或删除的总行数，包括作为触发器程序部分执行的语句。^执行任何其他类型的 SQL 语句
** 不会影响 sqlite3_total_changes() 返回的值。
** 
** ^作为 [外键操作] 一部分所做的更改包含在计数中，但作为 REPLACE 约束解决
** 部分所做的更改不包含在内。^由 INSTEAD OF 触发器拦截的对视图的更改
** 也不计入。
** 
** 另请参阅 [sqlite3_changes()] 接口、
** [count_changes pragma] 和 [total_changes() SQL 函数]。
**
** 如果在 [sqlite3_total_changes()] 运行时另一个线程对同一数据库连接
** 进行更改，则返回的值是不可预测的且没有意义。
*/
SQLITE_API int SQLITE_STDCALL sqlite3_total_changes(sqlite3*);

/**
 * CAPI3REF: 中断长时间运行的查询
 * 方法: sqlite3
 *
 * 这个函数会导致任何待处理的数据库操作在最早的可能时刻中止并返回。
 * 这个例程通常是在用户操作如按下“取消”或Ctrl-C时调用的，用户希望
 * 长时间运行的查询操作立即停止。
 *
 * 从与当前运行数据库操作的线程不同的线程调用此例程是安全的。
 * 但是，使用已关闭或可能在sqlite3_interrupt()返回之前关闭的
 * [数据库连接]调用此例程是不安全的。
 *
 * 如果在调用sqlite3_interrupt()时SQL操作几乎已经完成，那么它可能
 * 没有机会被中断并可能继续完成。
 *
 * 被中断的SQL操作将返回[SQLITE_INTERRUPT]。
 * 如果被中断的SQL操作是在显式事务内的INSERT、UPDATE或DELETE，
 * 那么整个事务将自动回滚。
 *
 * sqlite3_interrupt(D)调用在所有当前正在运行的SQL语句在
 * [数据库连接]D上完成之前一直有效。
 * 在sqlite3_interrupt()调用之后和运行语句达到零之前启动的任何新的
 * SQL语句将被中断，就像它们在sqlite3_interrupt()调用之前就已经在运行一样。
 * 在运行语句计数达到零之后启动的新SQL语句不受sqlite3_interrupt()的影响。
 * 当没有正在运行的SQL语句时调用sqlite3_interrupt()是一个无操作，
 * 对在sqlite3_interrupt()调用返回后启动的SQL语句没有影响。
 *
 * 如果在[sqlite3_interrupt()]运行时数据库连接关闭，那么可能会发生
 * 不好的事情。
 */
SQLITE_API void SQLITE_STDCALL sqlite3_interrupt(sqlite3*);

/*
** CAPI3REF: 判断SQL语句是否完整
**
** 这些例程在命令行输入时很有用，可以确定当前输入的文本是否形成一个完整的SQL语句，
** 或者是否需要在发送文本到SQLite进行解析之前需要更多的输入。^这些例程如果输入字符串
** 似乎是一个完整的SQL语句，则返回1。^如果语句以分号标记结束，并且不是良好形成的
** CREATE TRIGGER语句的前缀，则认为该语句是完整的。^嵌入在字符串字面量、引用标识符名称
** 或注释中的分号不是独立的标记（它们是嵌入其中的标记的一部分），因此不作为语句终止符。
** ^在最终分号之后的空白和注释将被忽略。
**
** ^如果语句不完整，这些例程返回0。^如果内存分配失败，则返回SQLITE_NOMEM。
**
** ^这些例程不解析SQL语句，因此不会检测语法错误的SQL。
**
** ^如果在调用sqlite3_complete16()之前没有使用[sqlite3_initialize()]初始化SQLite，
** 则sqlite3_complete16()会自动调用sqlite3_initialize()。如果该初始化失败，
** 则无论输入的SQL是否完整，sqlite3_complete16()的返回值都将是非零值。
**
** [sqlite3_complete()]的输入必须是一个以零终止的UTF-8字符串。
**
** [sqlite3_complete16()]的输入必须是一个以零终止的本地字节顺序的UTF-16字符串。
*/
SQLITE_API int SQLITE_STDCALL sqlite3_complete(const char *sql);
SQLITE_API int SQLITE_STDCALL sqlite3_complete16(const void *sql);

/**
 * 注册一个回调函数来处理SQLITE_BUSY错误
 * 关键词：{busy-handler callback} {busy handler}
 * 方法：sqlite3
 *
 * sqlite3_busy_handler(D, X, P)函数设置一个回调函数X，
 * 当尝试访问与[数据库连接]D关联的数据库表时，
 * 如果另一个线程或进程已经锁定该表，则可能会用参数P调用该回调函数。
 * sqlite3_busy_handler()接口用于实现[sqlite3_busy_timeout()]和[PRAGMA busy_timeout]。
 *
 * 如果忙碌回调为NULL，则在遇到锁定时立即返回[SQLITE_BUSY]。
 * 如果忙碌回调不为NULL，则可能会用两个参数调用该回调函数。
 *
 * 忙碌处理程序的第一个参数是void*指针的副本，
 * 该指针是sqlite3_busy_handler()的第三个参数。
 * 忙碌处理程序回调的第二个参数是忙碌处理程序之前为同一锁定事件被调用的次数。
 * 如果忙碌回调返回0，则不再尝试访问数据库，并返回[SQLITE_BUSY]给应用程序。
 * 如果回调返回非零值，则再次尝试访问数据库并重复循环。
 *
 * 有忙碌处理程序的存在并不能保证它在锁定争用时会被调用。
 * 如果SQLite确定调用忙碌处理程序可能导致死锁，
 * 它将直接返回[SQLITE_BUSY]给应用程序，而不是调用忙碌处理程序。
 * 考虑一个场景，一个进程持有读锁并试图将其提升为保留锁，
 * 而第二个进程持有保留锁并试图将其提升为排他锁。
 * 第一个进程无法继续，因为它被第二个进程阻塞，
 * 第二个进程也无法继续，因为它被第一个进程阻塞。
 * 如果两个进程都调用忙碌处理程序，它们都不会取得任何进展。
 * 因此，SQLite对第一个进程返回[SQLITE_BUSY]，希望这会促使第一个进程释放其读锁并允许第二个进程继续。
 *
 * 默认的忙碌回调为NULL。
 *
 * 每个数据库连接只能定义一个忙碌处理程序。
 * 设置新的忙碌处理程序会清除任何之前设置的处理程序。
 * 注意，调用[sqlite3_busy_timeout()]或评估[PRAGMA busy_timeout=N]会更改忙碌处理程序，
 * 从而清除任何之前设置的忙碌处理程序。
 *
 * 忙碌回调不应采取任何修改调用忙碌处理程序的数据库连接的操作。
 * 换句话说，忙碌处理程序不是可重入的。
 * 任何此类操作都会导致未定义的行为。
 *
 * 忙碌处理程序不得关闭调用忙碌处理程序的数据库连接或[准备语句]。
 */
SQLITE_API int SQLITE_STDCALL sqlite3_busy_handler(sqlite3*, int(*)(void*,int), void*);

/**
 ** 设置一个忙等待超时
 ** 方法：sqlite3
 **
 ** 这个例程设置了一个[sqlite3_busy_handler | 忙等待处理程序]，当表被锁定时，它会睡眠指定的时间。
 ** 处理程序会多次睡眠，直到累积至少 "ms" 毫秒的睡眠时间。至少 "ms" 毫秒的睡眠时间后，
 ** 处理程序返回 0，这导致 [sqlite3_step()] 返回 [SQLITE_BUSY]。
 **
 ** 调用这个例程并传入小于或等于零的参数会关闭所有忙等待处理程序。
 **
 **^(在任何给定时刻，特定[数据库连接]只能有一个忙等待处理程序。如果在调用此例程之前
 ** 定义了另一个忙等待处理程序（使用 [sqlite3_busy_handler()]），则该其他忙等待处理程序将被清除。) ^
 **
 ** 另见：[PRAGMA busy_timeout]
 */
SQLITE_API int SQLITE_STDCALL sqlite3_busy_timeout(sqlite3*, int ms);

/*
** CAPI3REF: 方便运行查询的例程
** 方法: sqlite3
**
** 这是一个为了向后兼容而保留的旧接口。不推荐使用此接口。
**
** 定义: <b>结果表</b>是由[sqlite3_get_table()]接口创建的内存数据结构。结果表记录了一个或多个查询的完整查询结果。
**
** 表在概念上有一定数量的行和列。但这些数字不是结果表本身的一部分。这些数字是单独获取的。设N为行数，M为列数。
**
** 结果表是一个指向以零终止的UTF-8字符串的指针数组。数组中有(N+1)*M个元素。前M个指针指向包含列名的以零终止的字符串。
** 剩余的条目都指向查询结果。NULL值导致NULL指针。所有其他值都是以零终止的UTF-8字符串表示，由[sqlite3_column_text()]返回。
**
** 结果表可能由一个或多个内存分配组成。直接将结果表传递给[sqlite3_free()]是不安全的。应该使用[sqlite3_free_table()]来释放结果表。
**
** ^例如，假设查询结果如下：
**
** <blockquote><pre>
**        姓名        | 年龄
**        -----------------------
**        Alice       | 43
**        Bob         | 28
**        Cindy       | 21
** </pre></blockquote>
**
** 有两列（M==2）和三行（N==3）。因此，结果表有8个条目。假设结果表存储在一个名为azResult的数组中。那么azResult包含以下内容：
**
** <blockquote><pre>
**        azResult[0] = "姓名";
**        azResult[1] = "年龄";
**        azResult[2] = "Alice";
**        azResult[3] = "43";
**        azResult[4] = "Bob";
**        azResult[5] = "28";
**        azResult[6] = "Cindy";
**        azResult[7] = "21";
** </pre></blockquote>)^
**
** ^sqlite3_get_table()函数评估其第二个参数中的零终止UTF-8字符串中的一个或多个用分号分隔的SQL语句，并将结果表返回给其第三个参数指向的指针。
**
** 应用程序完成对sqlite3_get_table()的结果后，必须将结果表指针传递给sqlite3_free_table()以释放已分配的内存。由于sqlite3_get_table()内部发生的[sqlite3_malloc()]的方式，调用函数不能尝试直接调用[sqlite3_free()]。只有[sqlite3_free_table()]能够正确且安全地释放内存。
**
** sqlite3_get_table()接口是围绕[sqlite3_exec()]实现的包装器。sqlite3_get_table()例程无法访问SQLite的任何内部数据结构。它只使用这里定义的公共接口。因此，在内部[sqlite3_exec()]调用之外的包装层中发生的错误不会反映在后续对[sqlite3_errcode()]或[sqlite3_errmsg()]的调用中。
*/
SQLITE_API int SQLITE_STDCALL sqlite3_get_table(
  sqlite3 *db,          /* 一个开放的数据库 */
  const char *zSql,     /*  要评估的 SQL */ 
  char ***pazResult,    /* 查询结果 */
  int *pnRow,           // 这里写入结果行数
  int *pnColumn,        // 这里写入结果列数
  char **pzErrmsg       /* 错误信息写在这里 */
);
SQLITE_API void SQLITE_STDCALL sqlite3_free_table(char **result);

/*
** CAPI3REF: 格式化字符串打印函数
**
** 这些例程是标准C库中"printf()"函数家族的工作 alike。
** 这些例程理解大多数常见的K&R格式选项，
** 加上一些额外的非标准格式，详情如下。
** 请注意，最近C库标准中的一些不太常见的格式选项
** 在此实现中被省略。
**
** ^sqlite3_mprintf()和sqlite3_vmprintf()例程将它们的结果写入
** 来自[sqlite3_malloc()]的内存。
** 这两个例程返回的字符串应由[sqlite3_free()]释放。
** 如果[sqlite3_malloc()]无法分配足够的内存来保存结果字符串，
** 这两个例程都返回一个空指针。
**
** ^(sqlite3_snprintf()例程类似于标准C库中的"snprintf()"。
** 结果被写入第二个参数提供的缓冲区，其大小由第一个参数给出。
** 注意，前两个参数的顺序与snprintf()相反。)^ 这是一个历史事故，
** 不修复将破坏向后兼容性。 ^(另外，sqlite3_snprintf()返回
** 一个指向其缓冲区的指针，而不是实际写入缓冲区的字符数。)^
** 我们承认，写入的字符数是一个更有用的返回值，
** 但我们现在不能改变sqlite3_snprintf()的实现而不破坏兼容性。
**
** ^只要缓冲区大小大于零，sqlite3_snprintf()保证缓冲区总是以零终止。
** ^第一个参数"n"是缓冲区的总大小，包括零终止符的空间。
** 因此，可以完全写入的最长字符串将是n-1个字符。
**
** ^sqlite3_vsnprintf()例程是sqlite3_snprintf()的可变参数版本。
**
** 这些例程都实现了一些额外的格式化选项，
** 对于构建SQL语句很有用。所有通常的printf()格式选项都适用。
** 此外，还有"%q", "%Q", "%w"和"%z"选项。
**
** ^( %q选项的工作方式类似于%s，它从参数列表中替换一个以nul终止的字符串。
** 但%q还双倍每个'\''字符。( %q设计用于字符串字面量内部。)^ 通过双倍每个'\''字符，
** 它转义该字符并允许它插入到字符串中。
**
** 例如，假设字符串变量zText包含以下文本：
**
** <blockquote><pre>
**  char *zText = "It's a happy day!";
** </pre></blockquote>
**
** 可以在SQL语句中使用此文本，如下所示：
**
** <blockquote><pre>
**  char *zSQL = sqlite3_mprintf("INSERT INTO table VALUES('%q')", zText);
**  sqlite3_exec(db, zSQL, 0, 0, 0);
**  sqlite3_free(zSQL);
** </pre></blockquote>
**
** 因为使用了%q格式字符串，zText中的'\''字符被转义，
** 生成的SQL如下：
**
** <blockquote><pre>
**  INSERT INTO table1 VALUES('It''s a happy day!')
** </pre></blockquote>
**
** 这是正确的。如果我们使用了%s而不是%q，生成的SQL将如下所示：
**
** <blockquote><pre>
**  INSERT INTO table1 VALUES('It's a happy day!');
** </pre></blockquote>
**
** 第二个例子是SQL语法错误。作为一个一般规则，你应该总是在将文本插入到字符串字面量时使用%q而不是%s。
**
** ^( %Q选项的工作方式类似于%q，只是它还在整个字符串的外部添加了单引号。
** 另外，如果参数列表中的参数是空指针，%Q将替换文本"NULL"（没有单引号）。)^ 例如，可以说：
**
** <blockquote><pre>
**  char *zSQL = sqlite3_mprintf("INSERT INTO table VALUES(%Q)", zText);
**  sqlite3_exec(db, zSQL, 0, 0, 0);
**  sqlite3_free(zSQL);
** </pre></blockquote>
**
** 上面的代码将在zSQL变量中生成一个正确的SQL语句，即使zText变量是空指针。
**
** ^( "%w"格式选项类似于"%q"，只是它期望被包含在双引号而不是单引号中，
** 并且它转义双引号字符而不是单引号字符。)^ "%w"格式选项旨在安全地将表和列名插入到构造的SQL语句中。
**
** ^( "%z"格式选项的工作方式类似于"%s"，但增加了一个功能，
** 即在字符串被读取并复制到结果后，对输入字符串调用[sqlite3_free()]。)^
*/
SQLITE_API char *SQLITE_CDECL sqlite3_mprintf(const char*,...);
SQLITE_API char *SQLITE_STDCALL sqlite3_vmprintf(const char*, va_list);
SQLITE_API char *SQLITE_CDECL sqlite3_snprintf(int,char*,const char*, ...);
SQLITE_API char *SQLITE_STDCALL sqlite3_vsnprintf(int,char*,const char*, va_list);

/*
** CAPI3REF: 内存分配子系统
**
** SQLite核心使用这三个例程来满足其所有的内部内存分配需求。
** “核心”在上一句话中不包括特定于操作系统的VFS实现。Windows VFS
** 对某些操作使用本地的malloc()和free()。
**
** ^sqlite3_malloc()例程返回一个至少N字节的内存块的指针，其中N是参数。
** ^如果sqlite3_malloc()无法获取足够的空闲内存，它返回一个NULL指针。
** ^如果sqlite3_malloc()的参数N为零或负数，则sqlite3_malloc()返回一个NULL指针。
**
** ^sqlite3_malloc64(N)例程的工作方式与sqlite3_malloc(N)相同，只是N是一个无符号的64位整数，而不是有符号的32位整数。
**
** ^使用sqlite3_free()释放先前由sqlite3_malloc()或sqlite3_realloc()返回的指针，以便可以重用该内存。
** ^如果sqlite3_free()用一个NULL指针调用，则它不执行任何操作。传递NULL指针给sqlite3_free()是无害的。
** 在释放后，不应读取或写入内存。即使读取先前释放的内存也可能导致段错误或其他严重错误。
** 如果sqlite3_free()用一个非NULL指针调用，该指针不是从sqlite3_malloc()或sqlite3_realloc()获得的，则可能导致内存损坏、段错误或其他严重错误。
**
** ^sqlite3_realloc(X,N)接口尝试将先前的内存分配X调整为至少N字节。
** ^如果sqlite3_realloc(X,N)的X参数是NULL指针，则其行为与调用sqlite3_malloc(N)完全相同。
** ^如果sqlite3_realloc(X,N)的N参数为零或负数，则其行为与调用sqlite3_free(X)完全相同。
** ^sqlite3_realloc(X,N)返回一个至少N字节大小的内存分配指针，如果内存不足则返回NULL。
** ^如果M是先前分配的大小，则将先前分配的min(N,M)字节复制到sqlite3_realloc(X,N)返回的缓冲区的开头，并释放先前的分配。
** ^如果sqlite3_realloc(X,N)返回NULL且N为正数，则不释放先前的分配。
**
** ^sqlite3_realloc64(X,N)接口的工作方式与sqlite3_realloc(X,N)相同，只是N是一个64位无符号整数，而不是32位有符号整数。
**
** ^如果X是从sqlite3_malloc()、sqlite3_malloc64()、sqlite3_realloc()或sqlite3_realloc64()先前获得的内存分配，则sqlite3_msize(X)返回该内存分配的大小（以字节为单位）。
** ^sqlite3_msize(X)返回的值可能大于分配X时请求的字节数。^如果X是NULL指针，则sqlite3_msize(X)返回零。
** 如果X指向的不是内存分配的开始，或者指向的是现在已释放的曾经有效的内存分配，则sqlite3_msize(X)的行为是未定义的，可能是有害的。
**
** ^sqlite3_malloc()、sqlite3_realloc()、sqlite3_malloc64()和sqlite3_realloc64()返回的内存总是至少对齐到8字节边界，或者如果使用了[SQLITE_4_BYTE_ALIGNED_MALLOC]编译时选项，则对齐到4字节边界。
**
** 在SQLite版本3.5.0和3.5.1中，可以定义SQLITE_OMIT_MEMORY_ALLOCATION，这将导致省略这些例程的内置实现。该功能现已不再提供。只能使用内置的内存分配器。
**
** 在SQLite版本3.7.10之前，Windows操作系统接口层在将文件名从SQLite使用的UTF-8编码转换为特定Windows安装使用的文件名编码时直接调用系统malloc()和free()。
** 检测到内存分配错误，但它们被报告为[SQLITE_CANTOPEN]或[SQLITE_IOERR]，而不是[SQLITE_NOMEM]。
**
** 传递给[sqlite3_free()]和[sqlite3_realloc()]的指针参数必须是NULL，或者是从先前的[sqlite3_malloc()]或[sqlite3_realloc()]调用获得的指针，且尚未释放。
**
** 应用程序必须在使用[sqlite3_free()]或[sqlite3_realloc()]释放内存块后，不读取或写入该内存块的任何部分。
*/
SQLITE_API void *SQLITE_STDCALL sqlite3_malloc(int);
SQLITE_API void *SQLITE_STDCALL sqlite3_malloc64(sqlite3_uint64);
SQLITE_API void *SQLITE_STDCALL sqlite3_realloc(void*, int);
SQLITE_API void *SQLITE_STDCALL sqlite3_realloc64(void*, sqlite3_uint64);
SQLITE_API void SQLITE_STDCALL sqlite3_free(void*);
SQLITE_API sqlite3_uint64 SQLITE_STDCALL sqlite3_msize(void*);

/*
** CAPI3REF: 内存分配器统计
**
** SQLite 提供了这两个接口来报告 [sqlite3_malloc()], [sqlite3_free()] 和 [sqlite3_realloc()]
** 例程的状态，这些例程构成了内置的内存分配子系统。
**
** ^[sqlite3_memory_used()] 例程返回当前未清偿（已分配但未释放）的内存字节数。
** ^[sqlite3_memory_highwater()] 例程返回自上次重置高水位标记以来
** [sqlite3_memory_used()] 的最大值。^[sqlite3_memory_used()] 和
** [sqlite3_memory_highwater()] 返回的值包括 SQLite 在实现 [sqlite3_malloc()] 时
** 添加的任何开销，但不包括 [sqlite3_malloc()] 可能调用的任何底层系统库例程添加的开销。
**
** ^内存高水位标记仅在 [sqlite3_memory_highwater()] 的参数为 true 时才会重置为
** [sqlite3_memory_used()] 的当前值。^[sqlite3_memory_highwater(1)] 返回的值是
** 重置之前的高水位标记。
*/
SQLITE_API sqlite3_int64 SQLITE_STDCALL sqlite3_memory_used(void);
SQLITE_API sqlite3_int64 SQLITE_STDCALL sqlite3_memory_highwater(int resetFlag);

/**
 * @brief 伪随机数据生成器
 *
 * SQLite 包含一个高质量的伪随机数生成器（PRNG），用于在向表中插入新记录时选择随机的 [ROWID | ROWIDs]，
 * 当表已经使用了最大的可能的 [ROWID] 时。PRNG 也用于内置的 random() 和 randomblob() SQL 函数。
 * 此接口允许应用程序为其他目的访问相同的 PRNG。
 *
 * 调用此例程会将 N 字节的随机性存储到缓冲区 P 中。
 * P 参数可以是 NULL 指针。
 *
 * 如果此例程之前未被调用，或者之前的调用中 N 小于一或 P 为 NULL 指针，
 * 则 PRNG 会使用从默认 [sqlite3_vfs] 对象的 xRandomness 方法获得的随机性进行种子初始化。
 * 如果之前的调用到此例程具有 N 大于一或更多的非 NULL P，则伪随机性是在内部生成
 * 而无需借助 [sqlite3_vfs] xRandomness 方法。
 */
SQLITE_API void SQLITE_STDCALL sqlite3_randomness(int N, void *P);

/*
** CAPI3REF: 编译时授权回调
** 方法: sqlite3
**
** ^此例程为特定的[数据库连接]注册一个授权回调，该连接作为第一个参数提供。
** ^在[sqlite3_prepare()]或其变体[sqlite3_prepare_v2()]、[sqlite3_prepare16()]和[sqlite3_prepare16_v2()]编译SQL语句时，会调用授权回调。
** ^在编译过程中的不同点，当创建逻辑以执行各种操作时，会调用授权回调以查看这些操作是否被允许。
** ^授权回调应返回[SQLITE_OK]以允许操作，[SQLITE_IGNORE]以不允许特定操作但允许SQL语句继续编译，或[SQLITE_DENY]以导致整个SQL语句因错误而被拒绝。
** ^如果授权回调返回除[SQLITE_IGNORE]、[SQLITE_OK]或[SQLITE_DENY]之外的任何值，则触发授权的[sqlite3_prepare_v2()]或等效调用将以错误消息失败。
**
** 当回调返回[SQLITE_OK]时，意味着请求的操作是允许的。
** ^当回调返回[SQLITE_DENY]时，触发授权的[sqlite3_prepare_v2()]或等效调用将以解释访问被拒绝的错误消息失败。
**
** 授权回调的第一个参数是sqlite3_set_authorizer()接口的第三个参数的副本。
** ^回调的第二个参数是一个整数[SQLITE_COPY | 动作代码]，指定要授权的特定动作。
** ^回调的第三到第六个参数是零终止的字符串，包含有关要授权的动作的额外详细信息。
**
** ^如果动作代码是[SQLITE_READ]且回调返回[SQLITE_IGNORE]，则[准备语句]构造为用NULL值替换如果返回[SQLITE_OK]则会被读取的表列。
** ^如果动作代码是[SQLITE_DELETE]且回调返回[SQLITE_IGNORE]，则[DELETE]操作继续，但[截断优化]被禁用，所有行将被单独删除。
**
** 当从不受信任的源[准备]SQL语句时，使用授权器以确保SQL语句不会尝试访问不允许查看的数据，或不会尝试执行损坏数据库的恶意语句。
** 例如，应用程序可能允许用户输入任意的SQL查询以供数据库评估。
** 但应用程序不希望用户能够对数据库进行任意更改。
** 在用户输入的SQL正在[准备]时，可以放置一个只允许[SELECT]语句的授权器。
**
** 需要处理来自不受信任源的SQL的应用程序还可能考虑使用[sqlite3_limit()]降低资源限制，并使用[max_page_count] [PRAGMA]限制数据库大小，除了使用授权器。
**
** ^一次只能在数据库连接上放置一个授权器。每次调用sqlite3_set_authorizer都会覆盖之前的调用。^通过安装NULL回调来禁用授权器。默认情况下，授权器是禁用的。
**
** 授权回调不得执行任何会修改调用授权回调的数据库连接的操作。
** 请注意，[sqlite3_prepare_v2()]和[sqlite3_step()]都会修改其数据库连接，这里的“修改”是指本段中的含义。
**
** ^当使用[sqlite3_prepare_v2()]准备语句时，由于模式更改，可能在[sqlite3_step()]期间重新准备语句。
** 因此，应用程序应确保在[sqlite3_step()]期间保持正确的授权回调。
**
** ^请注意，授权回调只在[sqlite3_prepare()]或其变体期间调用。
** 除非如前一段所述，sqlite3_step()在模式更改后调用sqlite3_prepare_v2()重新准备语句，否则在[sqlite3_step()]期间不会执行授权。
*/
SQLITE_API int SQLITE_STDCALL sqlite3_set_authorizer(
  sqlite3*,
  int (*xAuth)(void*,int,const char*,const char*,const char*,const char*),
  void *pUserData
);

/*
** CAPI3REF: 授权者返回代码
**
** [sqlite3_set_authorizer | 授权者回调函数] 必须返回 [SQLITE_OK] 或者以下两个常量之一，
** 以便通知SQLite该操作是否被允许。有关更多信息，请参阅 [sqlite3_set_authorizer | 授权者文档]。
**
** 注意，SQLITE_IGNORE 也被用作从 [sqlite3_vtab_on_conflict()] 接口返回的
** [冲突解决模式]。
*/
#define SQLITE_DENY   1   /* 中止 SQL 语句并报错 */
#define SQLITE_IGNORE 2   /* 不允许访问，但不生成错误 */

/*
*
** CAPI3REF: 授权器操作代码
**
** [sqlite3_set_authorizer()] 接口注册了一个回调函数，用于授权某些 SQL 语句操作。
** 回调函数的第二个参数是一个整数代码，用于指定正在授权的操作。以下是授权器回调可能传递的整数操作代码。
**
** 这些操作代码值表示要授权的操作类型。授权回调函数的第三个和第四个参数将是参数或 NULL，具体取决于这些代码中的哪一个被用作第二个参数。
** ^(授权器回调的第五个参数是数据库的名称（"main"、"temp" 等），如果适用的话。)^
** ^授权器回调的第六个参数是负责访问尝试的最内层触发器或视图的名称，如果此访问尝试直接来自顶级 SQL 代码，则为 NULL。
*/
/******************************************* 3rd ************ 4th ***********/
#define SQLITE_CREATE_INDEX          1   /* 索引名称        表名称          */
#define SQLITE_CREATE_TABLE          2   /* 表名称          NULL            */
#define SQLITE_CREATE_TEMP_INDEX     3   /* 索引名称        表名称          */
#define SQLITE_CREATE_TEMP_TABLE     4   /* 表名称          NULL            */
#define SQLITE_CREATE_TEMP_TRIGGER   5   /* 触发器名称      表名称          */
#define SQLITE_CREATE_TEMP_VIEW      6   /* 视图名称        NULL            */
#define SQLITE_CREATE_TRIGGER        7   /* 触发器名称      表名称          */
#define SQLITE_CREATE_VIEW           8   /* 视图名称        NULL            */
#define SQLITE_DELETE                9   /* 表名称          NULL            */
#define SQLITE_DROP_INDEX           10   /* 索引名称        表名称          */
#define SQLITE_DROP_TABLE           11   /* 表名称          NULL            */
#define SQLITE_DROP_TEMP_INDEX      12   /* 索引名称        表名称          */
#define SQLITE_DROP_TEMP_TABLE      13   /* 表名称          NULL            */
#define SQLITE_DROP_TEMP_TRIGGER    14   /* 触发器名称      表名称          */
#define SQLITE_DROP_TEMP_VIEW       15   /* 视图名称        NULL            */
#define SQLITE_DROP_TRIGGER         16   /* 触发器名称      表名称          */
#define SQLITE_DROP_VIEW            17   /* 视图名称        NULL            */
#define SQLITE_INSERT               18   /* 表名称          NULL            */
#define SQLITE_PRAGMA               19   /* Pragma 名称     第一个参数或 NULL */
#define SQLITE_READ                 20   /* 表名称          列名称          */
#define SQLITE_SELECT               21   /* NULL            NULL            */
#define SQLITE_TRANSACTION          22   /* 操作            NULL            */
#define SQLITE_UPDATE               23   /* 表名称          列名称          */
#define SQLITE_ATTACH               24   /* 文件名          NULL            */
#define SQLITE_DETACH               25   /* 数据库名称      NULL            */
#define SQLITE_ALTER_TABLE          26   /* 数据库名称      表名称          */
#define SQLITE_REINDEX              27   /* 索引名称        NULL            */
#define SQLITE_ANALYZE              28   /* 表名称          NULL            */
#define SQLITE_CREATE_VTABLE        29   /* 表名称          模块名称        */
#define SQLITE_DROP_VTABLE          30   /* 表名称          模块名称        */
#define SQLITE_FUNCTION             31   /* NULL            函数名称        */
#define SQLITE_SAVEPOINT            32   /* 操作            保存点名称      */
#define SQLITE_COPY                  0   /* 不再使用                        */
#define SQLITE_RECURSIVE            33   /* NULL            NULL            */

/*
** CAPI3REF: 跟踪与分析函数
** METHOD: sqlite3
**
** 这些例程用于注册回调函数，可以用来跟踪和分析SQL语句的执行。
**
** ^由sqlite3_trace()注册的回调函数在SQL语句由[sqlite3_step()]执行时
** 在不同时间点被调用。^当语句开始执行时，sqlite3_trace()回调函数
** 会带着SQL语句文本的UTF-8编码版本被调用。^(在每个触发子程序进入时，
** 可能会发生额外的sqlite3_trace()回调。触发器的回调包含一个UTF-8 SQL
** 注释，用于标识触发器。) ^
**
** 可以使用[SQLITE_TRACE_SIZE_LIMIT]编译时选项来限制sqlite3_trace()输出中
** [绑定参数]扩展的长度。
**
** ^由sqlite3_profile()注册的回调函数在每个SQL语句完成时被调用。^
** 配置文件回调包含原始的语句文本和该语句运行所花费的挂钟时间的估计。
** ^配置文件回调时间以纳秒为单位，但当前实现只能达到毫秒分辨率，因此
** 时间中的六位最低有效数字是无意义的。未来版本的SQLite可能会在配置文件
** 回调上提供更高的分辨率。sqlite3_profile()函数被认为是实验性的，并
** 可能会在未来版本的SQLite中发生变化。
*/
SQLITE_API void *SQLITE_STDCALL sqlite3_trace(sqlite3*, void(*xTrace)(void*,const char*), void*);
SQLITE_API SQLITE_EXPERIMENTAL void *SQLITE_STDCALL sqlite3_profile(sqlite3*,
   void(*xProfile)(void*,const char*,sqlite3_uint64), void*);

/**
** 查询进度回调
** 方法: sqlite3
**
** sqlite3_progress_handler(D,N,X,P) 接口会在对数据库连接 D 进行长时间运行的调用 [sqlite3_exec()]、[sqlite3_step()] 和 [sqlite3_get_table()] 期间定期调用回调函数 X。例如，这个接口可以用来在大型查询期间更新 GUI。
**
** 参数 P 作为唯一参数传递给回调函数 X。参数 N 是在连续调用回调 X 之间评估的近似 [虚拟机指令] 数量。如果 N 小于一，则禁用进度处理程序。
**
** 每个数据库连接一次只能定义一个进度处理程序；设置新的进度处理程序会取消旧的进度处理程序。将参数 X 设置为 NULL 可以禁用进度处理程序。将 N 设置为小于 1 的值也会禁用进度处理程序。
**
** 如果进度回调返回非零值，操作将被中断。这个特性可以用来在 GUI 进度对话框中实现一个“取消”按钮。
**
** 进度处理程序回调不能执行任何会修改调用进度处理程序的数据库连接的操作。注意，[sqlite3_prepare_v2()] 和 [sqlite3_step()] 都会修改它们的数据库连接，这是本段中“修改”的含义。
**
*/
SQLITE_API void SQLITE_STDCALL sqlite3_progress_handler(sqlite3*, int, int(*)(void*), void*);

/*
** CAPI3REF: 打开一个新的数据库连接
** 构造函数: sqlite3
**
** 这些例程打开由文件名参数指定的SQLite数据库文件。sqlite3_open()和sqlite3_open_v2()将文件名参数解释为UTF-8，sqlite3_open16()将其解释为本地字节顺序的UTF-16。即使发生错误，通常也会在*ppDb中返回一个[数据库连接]句柄。唯一的例外是，如果SQLite无法分配内存来保存[sqlite3]对象，那么将向*ppDb写入NULL而不是指向[sqlite3]对象的指针。如果数据库成功打开（和/或创建），则返回[SQLITE_OK]。否则返回一个[错误代码]。在sqlite3_open()例程失败后，可以使用[sqlite3_errmsg()]或[sqlite3_errmsg16()]例程获取错误的英文描述。
**
** 使用sqlite3_open()或sqlite3_open_v2()创建的数据库的默认编码为UTF-8。使用sqlite3_open16()创建的数据库的默认编码为本地字节顺序的UTF-16。
**
** 无论打开时是否发生错误，都应通过将其传递给[sqlite3_close()]来释放与[数据库连接]句柄关联的资源。
**
** sqlite3_open_v2()接口的工作方式类似于sqlite3_open()，但它接受两个额外的参数以对新的数据库连接进行更多控制。sqlite3_open_v2()的flags参数可以采用以下三个值之一，可以选择性地与其他[SQLITE_OPEN_NOMUTEX]、[SQLITE_OPEN_FULLMUTEX]、[SQLITE_OPEN_SHAREDCACHE]、[SQLITE_OPEN_PRIVATECACHE]和/或[SQLITE_OPEN_URI]标志结合使用：
**
** <dl>
** <dt>[SQLITE_OPEN_READONLY]</dt>
** <dd>以只读模式打开数据库。如果数据库不存在，将返回错误。</dd>
**
** <dt>[SQLITE_OPEN_READWRITE]</dt>
** <dd>尽可能以读写模式打开数据库，如果文件被操作系统写保护，则以只读模式打开。在任何情况下，数据库必须已经存在，否则将返回错误。</dd>
**
** <dt>[SQLITE_OPEN_READWRITE] | [SQLITE_OPEN_CREATE]</dt>
** <dd>以读写模式打开数据库，如果不存在则创建。这是sqlite3_open()和sqlite3_open16()始终使用的行为。</dd>
** </dl>
**
** 如果sqlite3_open_v2()的第三个参数不是上述组合之一，可以选择性地与其他[SQLITE_OPEN_READONLY | SQLITE_OPEN_* bits]结合，则行为未定义。
**
** 如果设置了[SQLITE_OPEN_NOMUTEX]标志，则数据库连接以多线程[线程模式]打开，只要在编译时或启动时未设置单线程模式。如果设置了[SQLITE_OPEN_FULLMUTEX]标志，则数据库连接以序列化[线程模式]打开，除非在编译时或启动时之前已选择单线程。[SQLITE_OPEN_SHAREDCACHE]标志使数据库连接有资格使用[共享缓存模式]，无论是否使用[sqlite3_enable_shared_cache()]启用了共享缓存。[SQLITE_OPEN_PRIVATECACHE]标志使数据库连接不参与[共享缓存模式]，即使已启用。
**
** sqlite3_open_v2()的第四个参数是定义新数据库连接应使用的操作系统接口的[sqlite3_vfs]对象的名称。如果第四个参数是NULL指针，则使用默认的[sqlite3_vfs]对象。
**
** 如果文件名是":memory:"，则为连接创建一个私有的、临时的内存数据库。当数据库连接关闭时，这个内存数据库将消失。未来版本的SQLite可能会使用以":"字符开头的其他特殊文件名。建议当数据库文件名实际上以":"字符开头时，应在文件名前加上路径，如"./"，以避免歧义。
**
** 如果文件名是空字符串，则创建一个私有的、临时的磁盘数据库。当数据库连接关闭时，这个私有数据库将自动删除。
**
** [[URI filenames in sqlite3_open()]] <h3>URI文件名</h3>
**
** 如果启用了[URI文件名]解释，并且文件名参数以"file:"开头，则将文件名解释为URI。如果设置了[SQLITE_OPEN_URI]标志，或者使用[SQLITE_CONFIG_URI]选项与[sqlite3_config()]方法全局启用了URI文件名解释，则启用URI文件名解释。从SQLite版本3.7.7开始，默认情况下关闭URI文件名解释，但未来版本的SQLite可能会默认启用URI文件名解释。有关更多信息，请参见"[URI文件名]"。
**
** URI文件名根据RFC 3986进行解析。如果URI包含权威部分，则它必须是空字符串或字符串"localhost"。如果权威部分不是空字符串或"localhost"，则向调用者返回错误。如果存在，则忽略URI的片段组件。
**
** SQLite使用URI的路径组件作为包含数据库的磁盘文件的名称。如果路径以'/'字符开头，则解释为绝对路径。如果路径不以'/'开头（意味着URI中省略了权威部分），则解释为相对路径。在Windows上，绝对路径的第一个组件是驱动器规范（例如"C:"）。
**
** [[core URI query parameters]]
** URI的查询组件可能包含由SQLite本身或[VFS | 自定义VFS实现]解释的参数。SQLite及其内置的[VFSes]解释以下查询参数：
**
** <ul>
**   <li> <b>vfs</b>: "vfs"参数可用于指定提供应用于访问磁盘上数据库文件的操作系统接口的VFS对象的名称。如果此选项设置为空字符串，则使用默认的VFS对象。指定未知的VFS是错误。如果使用sqlite3_open_v2()并且vfs选项存在，则选项指定的VFS优先于作为sqlite3_open_v2()的第四个参数传递的值。
**
**   <li> <b>mode</b>: mode参数可以设置为"ro"、"rw"、"rwc"或"memory"。尝试将其设置为任何其他值都是错误。如果指定"ro"，则数据库以只读访问打开，就像在sqlite3_open_v2()的第三个参数中设置了[SQLITE_OPEN_READONLY]标志一样。如果mode选项设置为"rw"，则数据库以读写（但不创建）访问打开，就像设置了SQLITE_OPEN_READWRITE（但未设置SQLITE_OPEN_CREATE）一样。"rwc"值等同于同时设置SQLITE_OPEN_READWRITE和SQLITE_OPEN_CREATE。如果mode选项设置为"memory"，则使用一个纯粹的[内存数据库]，它从未从磁盘读取或写入。指定一个比在sqlite3_open_v2()的第三个参数中传递的标志更宽松的mode参数值是错误。
**
**   <li> <b>cache</b>: cache参数可以设置为"shared"或"private"。将其设置为"shared"等同于在传递给sqlite3_open_v2()的标志参数中设置SQLITE_OPEN_SHAREDCACHE位。将cache参数设置为"private"等同于设置SQLITE_OPEN_PRIVATECACHE位。如果使用sqlite3_open_v2()并且URI文件名中存在"cache"参数，则其值覆盖通过设置SQLITE_OPEN_PRIVATECACHE或SQLITE_OPEN_SHAREDCACHE标志请求的任何行为。
**
**  <li> <b>psow</b>: psow参数指示存储数据库文件的存储介质是否应用[电源安全覆盖]属性。
**
**  <li> <b>nolock</b>: nolock参数是一个布尔查询参数，如果设置，则在回滚日志模式下禁用文件锁定。这对于访问不支持锁定的文件系统的数据库很有用。警告：如果两个或更多进程写入同一数据库，并且其中任何一个进程使用nolock=1，则可能导致数据库损坏。
**
**  <li> <b>immutable</b>: immutable参数是一个布尔查询参数，指示数据库文件存储在只读介质上。当immutable设置时，SQLite假设数据库文件不能被更改，即使是由具有更高特权的进程更改，因此数据库以只读方式打开，并禁用所有锁定和更改检测。警告：如果实际上会更改的数据库文件上设置了immutable属性，则可能导致不正确的查询结果和/或[SQLITE_CORRUPT]错误。另见：[SQLITE_IOCAP_IMMUTABLE]。
**       
** </ul>
**
** 在URI的查询组件中指定未知的参数不是错误。未来版本的SQLite可能会理解额外的查询参数。有关更多信息，请参见"[对SQLite有特殊意义的查询参数]"。
**
** [[URI filename examples]] <h3>URI文件名示例</h3>
**
** <table border="1" align=center cellpadding=5>
** <tr><th> URI文件名 <th> 结果
** <tr><td> file:data.db <td> 在当前目录中打开文件"data.db"。
** <tr><td> file:/home/fred/data.db<br>
**          file:///home/fred/data.db <br> 
**          file://localhost/home/fred/data.db <br> <td> 打开数据库文件"/home/fred/data.db"。
** <tr><td> file://darkstar/home/fred/data.db <td> 错误。"darkstar"不是识别的权威。
** <tr><td style="white-space:nowrap"> 
**          file:///C:/Documents%20and%20Settings/fred/Desktop/data.db
**     <td> 仅限Windows：在驱动器C:上fred的桌面上打开文件"data.db"。请注意，此示例中的%20转义不是严格必要的 - 可以在URI文件名中字面上使用空格字符。
** <tr><td> file:data.db?mode=ro&cache=private <td> 在当前目录中以只读访问打开文件"data.db"。无论默认是否启用了共享缓存模式，都使用私有缓存。
** <tr><td> file:/home/fred/data.db?vfs=unix-dotfile <td>
**          打开文件"/home/fred/data.db"。使用特殊的VFS "unix-dotfile"，它使用点文件代替posix建议锁定。
** <tr><td> file:data.db?mode=readonly <td> 错误。"readonly"不是"mode"参数的有效选项。
** </table>
**
** ^URI十六进制转义序列（%HH）在URI的路径和查询组件中支持。十六进制转义序列由百分号 - "%" - 后跟正好两个十六进制数字指定一个八位字节值。在解释URI文件名的路径或查询组件之前，它们使用UTF-8编码，并且所有十六进制转义序列被替换为包含相应八位字节的单个字节。如果此过程生成无效的UTF-8编码，则结果未定义。
**
** <b>Windows用户注意：</b> sqlite3_open()和sqlite3_open_v2()的文件名参数的编码必须是UTF-8，而不是当前定义的任何代码页。包含国际字符的文件名必须在传递给sqlite3_open()或sqlite3_open_v2()之前转换为UTF-8。
**
** <b>Windows Runtime用户注意：</b> 在调用sqlite3_open()或sqlite3_open_v2()之前必须设置临时目录。否则，需要使用临时文件的各种功能可能会失败。
**
** 另见：[sqlite3_temp_directory]
*/
SQLITE_API int SQLITE_STDCALL sqlite3_open(
  const char *filename,   /* 数据库名称（UTF-8编码） */
  sqlite3 **ppDb          /* OUT: SQLite 数据库句柄 */
);
SQLITE_API int SQLITE_STDCALL sqlite3_open16(
  const void *filename,  // 数据库文件名（UTF-16）
  sqlite3 **ppDb          /* OUT: SQLite 数据库句柄 */
);
SQLITE_API int SQLITE_STDCALL sqlite3_open_v2(
  const char *filename,   /* 数据库名称（UTF-8） */
  sqlite3 **ppDb,         /* OUT: SQLite 数据库句柄 */
  int flags,              /* 标志 */
  const char *zVfs        /* 使用虚拟文件系统（VFS）模块的名称 */
);

/*
** CAPI3REF: 获取URI参数的值
**
** 这些是实用程序例程，对VFS实现很有用，它们会检查数据库文件是否包含特定的查询参数的URI，并获取该查询参数的值。
**
** 如果F是指向数据库文件名的指针，当xOpen()方法的标志参数设置了[SQLITE_OPEN_URI]或[SQLITE_OPEN_MAIN_DB]位时传递给VFS实现的xOpen()方法，并且P是查询参数的名称，那么
** sqlite3_uri_parameter(F,P)返回P参数的值（如果存在）或P不出现在F的查询参数中的空指针。如果P是F的查询参数但没有显式值，那么sqlite3_uri_parameter(F,P)返回
** 指向空字符串的指针。
**
** sqlite3_uri_boolean(F,P,B)例程假设P是布尔参数，并根据P的值返回true (1)或false (0)。sqlite3_uri_boolean(F,P,B)例程在查询参数P的值为"yes"、"true"或"on"（不区分大小写）
** 或以非零数字开头时返回true (1)。sqlite3_uri_boolean(F,P,B)例程在查询参数P的值为"no"、"false"或"off"（不区分大小写）或以数字零开头时返回false (0)。如果P不是F的查询
** 参数，或者P的值不匹配上述任何一种，那么sqlite3_uri_boolean(F,P,B)返回(B!=0)。
**
** sqlite3_uri_int64(F,P,D)例程将P的值转换为64位有符号整数并返回该整数，或者如果P不存在则返回D。如果P的值不是整数，则返回零。
** 
** 如果F是空指针，那么sqlite3_uri_parameter(F,P)返回NULL，sqlite3_uri_boolean(F,P,B)返回B。如果F不是空指针并且不是SQLite传递给xOpen VFS方法的数据库文件路径名指针，
** 那么此例程的行为是未定义的，可能是不可取的。
*/
SQLITE_API const char *SQLITE_STDCALL sqlite3_uri_parameter(const char *zFilename, const char *zParam);
SQLITE_API int SQLITE_STDCALL sqlite3_uri_boolean(const char *zFile, const char *zParam, int bDefault);
SQLITE_API sqlite3_int64 SQLITE_STDCALL sqlite3_uri_int64(const char*, const char*, sqlite3_int64);


/*
** CAPI3REF: 错误代码和消息
** 方法: sqlite3
**
** ^如果与[数据库连接] D 相关的最新 sqlite3_* API 调用失败，
** 则 sqlite3_errcode(D) 接口返回该 API 调用的数字[结果代码]或[扩展结果代码]。
** 如果最近一次 API 调用成功，
** 则 sqlite3_errcode() 的返回值是未定义的。
** ^sqlite3_extended_errcode() 接口相同，
** 只是它总是返回[扩展结果代码]，即使扩展结果代码被禁用。
**
** ^sqlite3_errmsg() 和 sqlite3_errmsg16() 返回描述错误的英文文本，
** 分别为 UTF-8 或 UTF-16。
** ^(错误消息字符串的内存由内部管理。
** 应用程序不需要担心释放结果。
** 但是，错误字符串可能会被后续调用其他 SQLite 接口函数覆盖或释放。) ^
**
** ^sqlite3_errstr() 接口返回描述[结果代码]的英文文本，为 UTF-8。
** ^(错误消息字符串的内存由内部管理，
** 应用程序不得释放它。) ^
**
** 当使用序列化[线程模式]时，可能会在第一次错误发生和调用这些接口之间，
** 在另一个线程上发生第二次错误。
** 在这种情况下，将报告第二次错误，因为这些接口总是报告最近的结果。
** 为了避免这种情况，每个线程可以通过在开始使用 D 之前调用 [sqlite3_mutex_enter]([sqlite3_db_mutex](D))
** 来获得对[数据库连接] D 的独占使用，并在完成所有调用这些接口的调用后调用 [sqlite3_mutex_leave]([sqlite3_db_mutex](D))。
**
** 如果接口因 SQLITE_MISUSE 而失败，这意味着应用程序错误地调用了该接口。
** 在这种情况下，错误代码和消息可能已设置，也可能未设置。
*/
SQLITE_API int SQLITE_STDCALL sqlite3_errcode(sqlite3 *db);
SQLITE_API int SQLITE_STDCALL sqlite3_extended_errcode(sqlite3 *db);
SQLITE_API const char *SQLITE_STDCALL sqlite3_errmsg(sqlite3*);
SQLITE_API const void *SQLITE_STDCALL sqlite3_errmsg16(sqlite3*);
SQLITE_API const char *SQLITE_STDCALL sqlite3_errstr(int);

/*
** CAPI3REF: 预编译语句对象
** 关键词: {prepared statement} {prepared statements}
**
** 这个对象的实例代表一个已经被编译成二进制形式并准备好执行的单独的SQL语句。
**
** 把每个SQL语句想象成一个单独的计算机程序。原始的SQL文本是源代码。预编译语句对象
** 是编译后的目标代码。所有的SQL都必须转换成预编译语句才能执行。
**
** 预编译语句对象的生命周期通常如下：
**
** <ol>
** <li> 使用 [sqlite3_prepare_v2()] 创建预编译语句对象。
** <li> 使用 sqlite3_bind_*() 接口为 [parameters] 绑定值。
** <li> 通过调用 [sqlite3_step()] 一次或多次来执行SQL。
** <li> 使用 [sqlite3_reset()] 重置预编译语句，然后返回步骤2。重复此过程零次或多次。
** <li> 使用 [sqlite3_finalize()] 销毁对象。
** </ol>
*/
typedef struct sqlite3_stmt sqlite3_stmt;

/*
** CAPI3REF: 运行时限制
** 方法: sqlite3
**
** ^(这个接口允许在连接的基础上限制各种构造的大小。第一个参数是要设置或查询限制的
** [database connection]。第二个参数是定义要限制大小的构造类的 [limit categories] 之一。
** 第三个参数是该构造的新限制。)^
**
** ^如果新限制是一个负数，则限制保持不变。^(对于每个限制类别 SQLITE_LIMIT_<i>NAME</i>，
** 都有一个由 C 预处理器宏 [limits | SQLITE_MAX_<i>NAME</i>] 在编译时设置的
** [limits | 硬上限]。
** （名称中的 "_LIMIT_" 被改为 "_MAX_"）。)^
** ^尝试将限制增加到其硬上限之上会被静默地截断到硬上限。
**
** ^无论限制是否被更改，[sqlite3_limit()] 接口都会返回限制的先前值。
** ^因此，要查找当前的限制值而不更改它，只需将第三个参数设置为 -1 调用此接口即可。
**
** 运行时限制旨在用于管理内部数据库和由不受信任的外部来源控制的数据库的应用程序中。
** 例如，一个应用程序可能是一个有自己的数据库用于存储历史记录和由互联网下载的JavaScript应用程序
** 控制的单独数据库的网页浏览器。内部数据库可以给予大、默认的限制。由外部来源管理的数据库可以给予
** 更小的限制，旨在防止拒绝服务攻击。开发者还可能希望使用 [sqlite3_set_authorizer()] 接口来进一步控制不受信任的SQL。
** 可以使用 [max_page_count] [PRAGMA] 来限制由不受信任脚本创建的数据库的大小。
**
** 可能会在未来版本中添加新的运行时限制类别。
*/
SQLITE_API int SQLITE_STDCALL sqlite3_limit(sqlite3*, int id, int newVal);

/*
** CAPI3REF: 运行时限制类别
** 关键词: {limit category} {*limit categories}
**
** 这些常量定义了可以使用 [sqlite3_limit()] 在运行时降低的各种性能限制。
** 下面显示了各种限制含义的摘要。
** 有关更多信息，请参阅 [limits | SQLite中的限制]。
**
** <dl>
** [[SQLITE_LIMIT_LENGTH]] ^(<dt>SQLITE_LIMIT_LENGTH</dt>
** <dd>任何字符串或BLOB或表行的最大大小，以字节为单位。<dd>)^
**
** [[SQLITE_LIMIT_SQL_LENGTH]] ^(<dt>SQLITE_LIMIT_SQL_LENGTH</dt>
** <dd>任何SQL语句的最大长度，以字节为单位。</dd>)^
**
** [[SQLITE_LIMIT_COLUMN]] ^(<dt>SQLITE_LIMIT_COLUMN</dt>
** <dd>表定义中的最大列数，或 [SELECT] 的结果集中的最大列数，
** 或索引中的最大列数，或在 ORDER BY 或 GROUP BY 子句中的最大列数。</dd>)^
**
** [[SQLITE_LIMIT_EXPR_DEPTH]] ^(<dt>SQLITE_LIMIT_EXPR_DEPTH</dt>
** <dd>任何表达式解析树的最大深度。</dd>)^
**
** [[SQLITE_LIMIT_COMPOUND_SELECT]] ^(<dt>SQLITE_LIMIT_COMPOUND_SELECT</dt>
** <dd>复合SELECT语句中的最大项数。</dd>)^
**
** [[SQLITE_LIMIT_VDBE_OP]] ^(<dt>SQLITE_LIMIT_VDBE_OP</dt>
** <dd>用于实现SQL语句的虚拟机程序中的最大指令数。
** 此限制目前尚未强制执行，但可能会在SQLite的某个未来版本中添加。</dd>)^
**
** [[SQLITE_LIMIT_FUNCTION_ARG]] ^(<dt>SQLITE_LIMIT_FUNCTION_ARG</dt>
** <dd>函数上的最大参数数。</dd>)^
**
** [[SQLITE_LIMIT_ATTACHED]] ^(<dt>SQLITE_LIMIT_ATTACHED</dt>
** <dd>[ATTACH | 附加数据库] 的最大数量。</dd>)^
**
** [[SQLITE_LIMIT_LIKE_PATTERN_LENGTH]]
** ^(<dt>SQLITE_LIMIT_LIKE_PATTERN_LENGTH</dt>
** <dd>[LIKE] 或 [GLOB] 操作符的模式参数的最大长度。</dd>)^
**
** [[SQLITE_LIMIT_VARIABLE_NUMBER]]
** ^(<dt>SQLITE_LIMIT_VARIABLE_NUMBER</dt>
** <dd>SQL语句中任何 [parameter] 的最大索引号。</dd>)^
**
** [[SQLITE_LIMIT_TRIGGER_DEPTH]] ^(<dt>SQLITE_LIMIT_TRIGGER_DEPTH</dt>
** <dd>触发器的最大递归深度。</dd>)^
**
** [[SQLITE_LIMIT_WORKER_THREADS]] ^(<dt>SQLITE_LIMIT_WORKER_THREADS</dt>
** <dd>单个 [prepared statement] 可能启动的辅助工作线程的最大数量。</dd>)^
** </dl>
*/
#define SQLITE_LIMIT_LENGTH                    0
#define SQLITE_LIMIT_SQL_LENGTH                1
#define SQLITE_LIMIT_COLUMN                    2
#define SQLITE_LIMIT_EXPR_DEPTH                3
#define SQLITE_LIMIT_COMPOUND_SELECT           4
#define SQLITE_LIMIT_VDBE_OP                   5
#define SQLITE_LIMIT_FUNCTION_ARG              6
#define SQLITE_LIMIT_ATTACHED                  7
#define SQLITE_LIMIT_LIKE_PATTERN_LENGTH       8
#define SQLITE_LIMIT_VARIABLE_NUMBER           9
#define SQLITE_LIMIT_TRIGGER_DEPTH            10
#define SQLITE_LIMIT_WORKER_THREADS           11

/*
** CAPI3REF: 编译 SQL 语句
** 关键词: {SQL 语句编译器}
** 方法: sqlite3
** 构造函数: sqlite3_stmt
**
** 要执行 SQL 查询，必须首先使用这些例程之一将其编译成字节码程序。
**
** 第一个参数 "db" 是一个 [数据库连接]，它来自于之前成功调用的 [sqlite3_open()]、[sqlite3_open_v2()] 或 [sqlite3_open16()]。数据库连接不能已关闭。
**
** 第二个参数 "zSql" 是要编译的语句，编码为 UTF-8 或 UTF-16。sqlite3_prepare() 和 sqlite3_prepare_v2() 接口使用 UTF-8，而 sqlite3_prepare16() 和 sqlite3_prepare16_v2() 使用 UTF-16。
**
** 如果 nByte 参数为负，则 zSql 读取到第一个零终止符。如果 nByte 为正，则它是从 zSql 读取的字节数。如果 nByte 为零，则不生成准备好的语句。
** 如果调用者知道提供的字符串是以 null 结尾的，那么传递一个 nByte 参数，其值为输入字符串的字节数（包括 null 终止符）会有一个小性能优势。
**
** 如果 pzTail 不为 NULL，则 *pzTail 指向 zSql 中第一个 SQL 语句末尾的第一个字节。这些例程只编译 zSql 中的第一个语句，因此 *pzTail 留指向未编译的部分。
**
** *ppStmt 指向一个已编译的 [准备好的语句]，可以使用 [sqlite3_step()] 执行。如果出现错误，*ppStmt 设置为 NULL。如果输入文本不包含 SQL（如果输入是空字符串或注释），则 *ppStmt 设置为 NULL。
** 调用过程负责使用 [sqlite3_finalize()] 删除已编译的 SQL 语句。ppStmt 不能为 NULL。
**
** 成功时，sqlite3_prepare() 系列例程返回 [SQLITE_OK]；否则返回一个 [错误代码]。
**
** 建议所有新程序使用 sqlite3_prepare_v2() 和 sqlite3_prepare16_v2() 接口。保留两个旧接口是为了向后兼容，但建议不要使用它们。
** ^在 "v2" 接口中，返回的准备好的语句（[sqlite3_stmt] 对象）包含原始 SQL 文本的副本。这导致 [sqlite3_step()] 接口在三个方面表现不同：
**
** <ol>
** <li>
** ^如果数据库模式更改，[sqlite3_step()] 不会像以前那样返回 [SQLITE_SCHEMA]，而是自动重新编译 SQL 语句并尝试再次运行。在 sqlite3_step() 放弃并返回错误之前，最多会进行 [SQLITE_MAX_SCHEMA_RETRY] 次重试。
** </li>
**
** <li>
** ^当出现错误时，[sqlite3_step()] 将返回一个详细的 [错误代码] 或 [扩展错误代码]。^以前的行为是 [sqlite3_step()] 只会返回通用的 [SQLITE_ERROR] 结果代码，并且应用程序必须进行第二次调用 [sqlite3_reset()] 以找到问题的根本原因。使用 "v2" 准备接口时，错误的原因会立即返回。
** </li>
**
** <li>
** ^如果绑定到 WHERE 子句中的 [参数 | 主机参数] 的特定值可能会影响语句的查询计划的选择，那么在第一次 [sqlite3_step()] 调用之后，如果该 [参数] 的 [sqlite3_bind_text | 绑定] 发生更改，则语句将自动重新编译，就像发生了模式更改一样。
** ^WHERE 子句 [参数] 的特定值可能会影响查询计划的选择，如果参数是 [LIKE] 或 [GLOB] 运算符的左侧，或者如果参数与索引列进行比较并且启用了 [SQLITE_ENABLE_STAT3] 编译时选项。
** </li>
** </ol>
*/
SQLITE_API int SQLITE_STDCALL sqlite3_prepare(
  sqlite3 *db,            /* 数据库句柄 */
  const char *zSql,       /* SQL 语句，UTF-8 编码 */
  int nByte,              /* zSql 的最大长度（字节数） */
  sqlite3_stmt **ppStmt,  /* 输出：语句句柄 */
  const char **pzTail     /* 输出：指向 zSql 未使用部分的指针 */
);
SQLITE_API int SQLITE_STDCALL sqlite3_prepare_v2(
  sqlite3 *db,            /* 数据库句柄 */
  const char *zSql,       /* SQL 语句，UTF-8 编码 */
  int nByte,              /* zSql 的最大长度（字节数） */
  sqlite3_stmt **ppStmt,  /* 输出：语句句柄 */
  const char **pzTail     /* 输出：指向 zSql 未使用部分的指针 */
);
SQLITE_API int SQLITE_STDCALL sqlite3_prepare16(
  sqlite3 *db,            /* 数据库句柄 */
  const void *zSql,       /* SQL 语句，UTF-16 编码 */
  int nByte,              /* zSql 的最大长度（字节数） */
  sqlite3_stmt **ppStmt,  /* 输出：语句句柄 */
  const void **pzTail     /* 输出：指向 zSql 未使用部分的指针 */
);
SQLITE_API int SQLITE_STDCALL sqlite3_prepare16_v2(
  sqlite3 *db,            /* 数据库句柄 */
  const void *zSql,       /* SQL 语句，UTF-16 编码 */
  int nByte,              /* zSql 的最大长度（字节数） */
  sqlite3_stmt **ppStmt,  /* 输出：语句句柄 */
  const void **pzTail     /* 输出：指向 zSql 未使用部分的指针 */
);

/*
** CAPI3REF: 检索语句SQL
** 方法: sqlite3_stmt
**
** ^此接口可用于检索用于创建[准备语句]的原始SQL文本的保存副本，
** 如果该语句是使用[sqlite3_prepare_v2()]或[sqlite3_prepare16_v2()]编译的。
*/
SQLITE_API const char *SQLITE_STDCALL sqlite3_sql(sqlite3_stmt *pStmt);

/**
 ** CAPI3REF: 判断SQL语句是否写入数据库
 ** METHOD: sqlite3_stmt
 **
 ** ^sqlite3_stmt_readonly(X) 接口返回 true (非零) 当且仅当 [准备好的语句] X
 ** 没有直接更改数据库文件的内容。
 **
 ** 注意，[应用程序定义的SQL函数] 或 [虚拟表] 可能会间接地通过副作用更改数据库。
 ** ^(例如，如果应用程序定义了一个函数 "eval()" 调用 [sqlite3_exec()]，那么以下SQL语句
 ** 会通过副作用更改数据库文件：
 **
 ** <blockquote><pre>
 **    SELECT eval('DELETE FROM t1') FROM t2;
 ** </pre></blockquote>
 **
 ** 但因为 [SELECT] 语句没有直接更改数据库文件，sqlite3_stmt_readonly() 仍然会返回 true。) ^
 **
 ** ^事务控制语句如 [BEGIN], [COMMIT], [ROLLBACK], [SAVEPOINT], 和 [RELEASE]
 ** 会导致 sqlite3_stmt_readonly() 返回 true，因为这些语句实际上并没有修改数据库，
 ** 而是控制其他语句修改数据库的时机。^ [ATTACH] 和 [DETACH] 语句也会导致
 ** sqlite3_stmt_readonly() 返回 true，因为这些语句改变了数据库连接的配置，
 ** 但并没有更改磁盘上的数据库文件内容。
 */
SQLITE_API int SQLITE_STDCALL sqlite3_stmt_readonly(sqlite3_stmt *pStmt);

/**
 ** CAPI3REF: 判断已准备好的语句是否已重置
 ** METHOD: sqlite3_stmt
 **
 ** ^sqlite3_stmt_busy(S) 接口在 [prepared statement] S 已至少使用一次 [sqlite3_step(S)] 进行了步骤但尚未完成运行和/或尚未使用 [sqlite3_reset(S)] 进行重置时返回 true (非零)。^如果 S 是一个空指针，sqlite3_stmt_busy(S) 接口返回 false。如果 S 不是一个空指针且不是一个指向有效 [prepared statement] 对象的指针，那么行为是未定义的，并且可能是不可取的。
 **
 ** 这个接口可以与 [sqlite3_next_stmt()] 结合使用，以定位与数据库连接相关联的所有需要重置的已准备好的语句。例如，这可以用于诊断例程中搜索正在保持事务打开的已准备好的语句。
 */
SQLITE_API int SQLITE_STDCALL sqlite3_stmt_busy(sqlite3_stmt*);

/*
** CAPI3REF: 动态类型值对象
** 关键词: {protected sqlite3_value} {unprotected sqlite3_value}
**
** SQLite使用sqlite3_value对象来表示可以存储在数据库表中的所有值。
** SQLite对它存储的值使用动态类型。^存储在sqlite3_value对象中的值可以是整数、浮点数、字符串、BLOB或NULL。
**
** 一个sqlite3_value对象可以是“受保护的”或“不受保护的”。
** 一些接口需要受保护的sqlite3_value。其他接口可以接受受保护的或不受保护的sqlite3_value。
** 每个接受sqlite3_value参数的接口都指定是否需要受保护的sqlite3_value。
** 可以使用[sqlite3_value_dup()]接口从不受保护的sqlite3_value构建一个新的受保护的sqlite3_value。
**
** “受保护”和“不受保护”术语指的是是否持有互斥锁。受保护的sqlite3_value对象持有内部互斥锁，
** 而不受保护的sqlite3_value对象不持有互斥锁。如果SQLite编译为单线程（使用[SQLITE_THREADSAFE=0]并且[sqlite3_threadsafe()]返回0），
** 或者如果SQLite在减少互斥模式[SQLITE_CONFIG_SINGLETHREAD]或[SQLITE_CONFIG_MULTITHREAD]下运行，
** 则受保护的sqlite3_value对象和不受保护的sqlite3_value对象之间没有区别，可以互换使用。
** 然而，为了最大程度地确保代码的可移植性，建议即使在严格不需要的情况下，
** 应用程序仍然要区分受保护的和不受保护的sqlite3_value对象。
**
** ^传递给[应用程序定义的SQL函数]实现的sqlite3_value对象是受保护的。
** ^由[sqlite3_column_value()]返回的sqlite3_value对象是不受保护的。
** 不受保护的sqlite3_value对象只能与[sqlite3_result_value()]和[sqlite3_bind_value()]一起使用。
** [sqlite3_value_blob | sqlite3_value_type()]系列接口需要受保护的sqlite3_value对象。
*/
typedef struct Mem sqlite3_value;

/*
** CAPI3REF: SQL函数上下文对象
**
** SQL函数执行的上下文存储在sqlite3_context对象中。^指向sqlite3_context对象的指针总是
** [应用程序定义的SQL函数]的第一个参数。应用程序定义的SQL函数实现将此指针传递给
** [sqlite3_result_int | sqlite3_result()]、[sqlite3_aggregate_context()]、[sqlite3_user_data()]、
** [sqlite3_context_db_handle()]、[sqlite3_get_auxdata()]和/或[sqlite3_set_auxdata()]的调用。
*/
typedef struct sqlite3_context sqlite3_context;

/*
** CAPI3REF: 将值绑定到准备好的语句
** 关键词: {主机参数} {主机参数} {主机参数名}
** 关键词: {SQL参数} {SQL参数} {参数绑定}
** 方法: sqlite3_stmt
**
** ^在输入到[sqlite3_prepare_v2()]及其变体的SQL语句文本中，字面量可以被以下模板匹配的
** [参数]替换：
**
** <ul>
** <li>  ?
** <li>  ?NNN
** <li>  :VVV
** <li>  @VVV
** <li>  $VVV
** </ul>
**
** 在上述模板中，NNN代表整数字面量，VVV代表字母数字标识符。)^ ^这些参数的值（也称为“主机参数名”或“SQL参数”）可以使用这里定义的sqlite3_bind_*()例程设置。
**
** ^sqlite3_bind_*()例程的第一个参数总是从[sqlite3_prepare_v2()]或其变体返回的[sqlite3_stmt]对象的指针。
**
** ^第二个参数是要设置的SQL参数的索引。^最左边的SQL参数的索引为1。^当相同的命名SQL参数使用多次时，第二次及后续出现的索引与第一次出现的索引相同。
** ^如果需要，可以使用[sqlite3_bind_parameter_index()] API查找命名参数的索引。^“?NNN”参数的索引是NNN的值。
** ^NNN值必须在1和[sqlite3_limit()]参数[SQLITE_LIMIT_VARIABLE_NUMBER]（默认值：999）之间。
**
** ^第三个参数是要绑定到参数的值。^如果sqlite3_bind_text()或sqlite3_bind_text16()或sqlite3_bind_blob()的第三个参数是NULL指针，
** 则忽略第四个参数，最终结果与sqlite3_bind_null()相同。
**
** ^(在有第四个参数的例程中，其值是参数中的字节数。为了明确：值是值中的字节数，不是字符数。) ^
** ^如果sqlite3_bind_text()或sqlite3_bind_text16()的第四个参数是负数，则字符串的长度是
** 第一个零终止符之前的字节数。如果sqlite3_bind_blob()的第四个参数是负数，则行为是未定义的。
** 如果为sqlite3_bind_text()或sqlite3_bind_text16()或sqlite3_bind_text64()提供了非负的第四个参数，
** 则该参数必须是假设字符串以NUL终止时NUL终止符会出现的字节偏移量。
** 如果在小于第四个参数值的字节偏移量处出现任何NUL字符，则结果字符串值将包含嵌入的NUL。
** 涉及嵌入NUL的字符串的表达式的结果是未定义的。
**
** ^BLOB和字符串绑定接口的第五个参数是用于在SQLite处理完之后处理BLOB或字符串的析构函数。^即使调用绑定API失败，
** 也会调用析构函数来处理BLOB或字符串。^如果第五个参数是特殊值[SQLITE_STATIC]，则SQLite假设信息位于静态、未管理的空间中，
** 不需要释放。^如果第五个参数的值为[SQLITE_TRANSIENT]，则SQLite会立即在sqlite3_bind_*()例程返回之前
** 制作数据的私有副本。
**
** ^sqlite3_bind_text64()的第六个参数必须是[SQLITE_UTF8]、[SQLITE_UTF16]、[SQLITE_UTF16BE]或[SQLITE_UTF16LE]之一，
** 以指定第三个参数中文本的编码。如果sqlite3_bind_text64()的第六个参数不是上面显示的允许值之一，
** 或者文本编码与第六个参数指定的编码不同，则行为是未定义的。
**
** ^sqlite3_bind_zeroblob()例程绑定一个长度为N的BLOB，该BLOB填充了零。^在处理过程中，zeroblob使用固定量的内存（仅一个整数来保存其大小）。
** Zeroblobs旨在作为使用[sqlite3_blob_open | 增量BLOB I/O]例程稍后写入内容的BLOB的占位符。^Zeroblob的负值将导致零长度的BLOB。
**
** ^如果任何sqlite3_bind_*()例程使用[准备好的语句]的NULL指针调用，或者使用最近一次调用[sqlite3_step()]比[sqlite3_reset()]更早的准备好的语句，
** 则调用将返回[SQLITE_MISUSE]。如果任何sqlite3_bind_()例程传递了已完成的[准备好的语句]，则结果是未定义的，并且可能有危害。
**
** ^[sqlite3_reset()]例程不会清除绑定。^未绑定的参数被解释为NULL。
**
** ^sqlite3_bind_*例程在成功时返回[SQLITE_OK]，如果出现任何问题则返回[错误代码]。
** ^如果字符串或BLOB的大小超过了[sqlite3_limit]([SQLITE_LIMIT_LENGTH])或[SQLITE_MAX_LENGTH]强加的限制，可能会返回[SQLITE_TOOBIG]。
** ^如果参数索引超出范围，则返回[SQLITE_RANGE]。^如果malloc()失败，则返回[SQLITE_NOMEM]。
**
** 另见: [sqlite3_bind_parameter_count()]、
** [sqlite3_bind_parameter_name()]和[sqlite3_bind_parameter_index()]。
*/
SQLITE_API int SQLITE_STDCALL sqlite3_bind_blob(sqlite3_stmt*, int, const void*, int n, void(*)(void*));
SQLITE_API int SQLITE_STDCALL sqlite3_bind_blob64(sqlite3_stmt*, int, const void*, sqlite3_uint64,
						void(*)(void*));
SQLITE_API int SQLITE_STDCALL sqlite3_bind_double(sqlite3_stmt*, int, double);
SQLITE_API int SQLITE_STDCALL sqlite3_bind_int(sqlite3_stmt*, int, int);
SQLITE_API int SQLITE_STDCALL sqlite3_bind_int64(sqlite3_stmt*, int, sqlite3_int64);
SQLITE_API int SQLITE_STDCALL sqlite3_bind_null(sqlite3_stmt*, int);
SQLITE_API int SQLITE_STDCALL sqlite3_bind_text(sqlite3_stmt*,int,const char*,int,void(*)(void*));
SQLITE_API int SQLITE_STDCALL sqlite3_bind_text16(sqlite3_stmt*, int, const void*, int, void(*)(void*));
SQLITE_API int SQLITE_STDCALL sqlite3_bind_text64(sqlite3_stmt*, int, const char*, sqlite3_uint64,
						 void(*)(void*), unsigned char encoding);
SQLITE_API int SQLITE_STDCALL sqlite3_bind_value(sqlite3_stmt*, int, const sqlite3_value*);
SQLITE_API int SQLITE_STDCALL sqlite3_bind_zeroblob(sqlite3_stmt*, int, int n);

/*
** CAPI3REF: SQL 参数的数目
** 方法: sqlite3_stmt
**
** 此例程可用于查找[准备语句]中的[SQL参数]数量。
** SQL参数是形如"?", "?NNN", ":AAA", "$AAA"或"@AAA"的标记，
** 它们作为稍后[sqlite3_bind_blob | 绑定]到参数的值的占位符。
**
** 实际上，此例程返回的是最大（最右侧）参数的索引。
** 对于除?NNN之外的所有形式，这将对应于唯一参数的数量。
** 如果使用了?NNN形式的参数，列表中可能会有间隙。
**
** 另见：[sqlite3_bind_blob|sqlite3_bind()]，
** [sqlite3_bind_parameter_name()]和
** [sqlite3_bind_parameter_index()]。
*/
SQLITE_API int SQLITE_STDCALL sqlite3_bind_parameter_count(sqlite3_stmt*);

/*
** CAPI3REF: 主机参数名称
** 方法: sqlite3_stmt
**
** ^sqlite3_bind_parameter_name(P,N) 接口返回
** [准备语句] P 中第 N 个 [SQL 参数] 的名称。
** ^(形如 "?NNN" 或 ":AAA" 或 "@AAA" 或 "$AAA" 的 SQL 参数
** 的名称分别是字符串 "?NNN" 或 ":AAA" 或 "@AAA" 或 "$AAA"。
** 换句话说，初始的 ":" 或 "$" 或 "@" 或 "?" 
** 作为名称的一部分包含在内。) ^
** ^形如 "?" 而不带后续整数的参数没有名称
** 并被称为“无名”或“匿名参数”。
**
** ^第一个主机参数的索引为 1，而不是 0。
**
** ^如果值 N 超出范围，或者第 N 个参数是无名的，
** 则返回 NULL。 ^返回的字符串总是 UTF-8 编码，
** 即使命名参数最初在 [sqlite3_prepare16()] 或
** [sqlite3_prepare16_v2()] 中指定为 UTF-16。
**
** 另见: [sqlite3_bind_blob|sqlite3_bind()],
** [sqlite3_bind_parameter_count()], 和
** [sqlite3_bind_parameter_index()].
*/
SQLITE_API const char *SQLITE_STDCALL sqlite3_bind_parameter_name(sqlite3_stmt*, int);

/*
 ** CAPI3REF: 指定名称的参数索引
 ** METHOD: sqlite3_stmt
 **
 ** 返回给定名称的SQL参数的索引。返回的索引值适用于作为
 ** [sqlite3_bind_blob|sqlite3_bind()]的第二个参数。
 ** 如果没有找到匹配的参数，则返回零。即使原始语句是使用
 ** [sqlite3_prepare16_v2()]从UTF-16文本准备的，参数名称也必须以UTF-8格式给出。
 **
 ** 另见: [sqlite3_bind_blob|sqlite3_bind()],
 ** [sqlite3_bind_parameter_count()], 和
 ** [sqlite3_bind_parameter_index()].
 */

SQLITE_API int SQLITE_STDCALL sqlite3_bind_parameter_index(sqlite3_stmt*, const char *zName);

/*
** CAPI3REF: 重置准备语句上的所有绑定
** METHOD: sqlite3_stmt
**
** ^与许多人的直觉相反，[sqlite3_reset()] 不会重置 [prepared statement] 上的 [sqlite3_bind_blob | 绑定]。
** ^使用此例程将所有主机参数重置为 NULL。
*/
SQLITE_API int SQLITE_STDCALL sqlite3_clear_bindings(sqlite3_stmt*);

/*
** CAPI3REF: 结果集中列的数量
** 方法: sqlite3_stmt
**
** ^返回由[准备语句]返回的结果集中的列数。^如果pStmt是一个不返回数据的SQL语句（例如[UPDATE]），则此例程返回0。
**
** 另见: [sqlite3_data_count()]
*/
SQLITE_API int SQLITE_STDCALL sqlite3_column_count(sqlite3_stmt *pStmt);

/*
** CAPI3REF: 列名在结果集中
** METHOD: sqlite3_stmt
**
** ^这些例程返回结果集中特定列的名称
** 在一个 [SELECT] 语句中。 ^sqlite3_column_name() 接口返回一个指向以零终止的 UTF-8 字符串的指针
** sqlite3_column_name16() 返回一个指向以零终止的 UTF-16 字符串的指针。 ^第一个参数是
** 实现 [SELECT] 语句的 [准备好的语句]。 ^第二个参数是
** 列号。 ^最左边的列是编号 0。
**
** ^返回的字符串指针在 [准备好的语句]
** 被通过 [sqlite3_finalize()] 销毁之前有效，或者直到语句在特定运行中通过
** 对 [sqlite3_step()] 的第一次调用自动重新准备，或者直到对同一列的
** 下一次调用 sqlite3_column_name() 或 sqlite3_column_name16()。
**
** ^如果在处理任何一个例程期间 sqlite3_malloc() 失败
** （例如在从 UTF-8 转换为 UTF-16 期间）则返回一个
** NULL 指针。
**
** ^结果列的名称是
** 该列的 "AS" 子句的值（如果有 AS 子句）。如果没有 AS 子句
** 那么列的名称是未指定的，并且可能会从一个版本的 SQLite 更改到下一个版本。
*/
SQLITE_API const char *SQLITE_STDCALL sqlite3_column_name(sqlite3_stmt*, int N);
SQLITE_API const void *SQLITE_STDCALL sqlite3_column_name16(sqlite3_stmt*, int N);

/*
** CAPI3REF: 查询结果的数据来源
** METHOD: sqlite3_stmt
**
** ^这些例程提供了一种手段来确定数据库、表和
** 表列，这些是 [SELECT] 语句中特定结果列的来源。
** ^数据库、表或列的名称可以作为
** UTF-8 或 UTF-16 字符串返回。 ^_database_ 例程返回
** 数据库名称，_table_ 例程返回表名，_origin_ 例程返回列名。
** ^返回的字符串在 [准备好的语句] 使用 [sqlite3_finalize()] 销毁
** 之前有效，或者直到语句在特定运行中通过
** 对 [sqlite3_step()] 的第一次调用自动重新准备，或者直到以不同编码请求
** 相同信息。
**
** ^返回的名称是数据库、表和列的原始非别名的名称。
**
** ^这些接口的第一个参数是 [准备好的语句]。
** ^这些函数返回关于由
** 语句返回的第 N 个结果列的信息，其中 N 是第二个函数参数。
** ^对于这些例程来说，最左边的列是列 0。
**
** ^如果语句返回的第 N 列是一个表达式或
** 子查询并且不是列值，那么所有这些函数都返回
** NULL。 ^如果内存分配错误发生，这些例程也可能返回 NULL。 ^否则，它们返回查询结果列提取的
** 附加数据库、表或列的名称。
**
** ^与所有其他 SQLite API 一样，名称以 "16" 结尾的函数返回
** UTF-16 编码的字符串，其他函数返回 UTF-8。
**
** ^只有在库使用 [SQLITE_ENABLE_COLUMN_METADATA] C 预处理器符号
** 编译时，这些 API 才可用。
**
** 如果两个或多个线程同时调用这些例程中的一个或多个
** 相同的准备好的语句和列，则结果未定义。
**
** 如果两个或多个线程同时调用这些例程中的一个或多个
** [sqlite3_column_database_name | 列元数据接口]
** 相同的准备好的语句和结果列，则结果未定义。
*/
SQLITE_API const char *SQLITE_STDCALL sqlite3_column_database_name(sqlite3_stmt*,int);
SQLITE_API const void *SQLITE_STDCALL sqlite3_column_database_name16(sqlite3_stmt*,int);
SQLITE_API const char *SQLITE_STDCALL sqlite3_column_table_name(sqlite3_stmt*,int);
SQLITE_API const void *SQLITE_STDCALL sqlite3_column_table_name16(sqlite3_stmt*,int);
SQLITE_API const char *SQLITE_STDCALL sqlite3_column_origin_name(sqlite3_stmt*,int);
SQLITE_API const void *SQLITE_STDCALL sqlite3_column_origin_name16(sqlite3_stmt*,int);

/*
** CAPI3REF: 查询结果的声明数据类型
** METHOD: sqlite3_stmt
**
** ^(第一个参数是一个[准备好的语句]。
** 如果这个语句是一个[SELECT]语句，并且返回的结果集中第N列是表列（而不是表达式或子查询），
** 则返回表列的声明类型。) ^如果结果集中的第N列是表达式或子查询，则返回一个空指针。
** ^返回的字符串总是UTF-8编码的。
**
** ^(例如，给定数据库模式：
**
** CREATE TABLE t1(c1 VARIANT);
**
** 和以下要编译的语句：
**
** SELECT c1 + 1, c1 FROM t1;
**
** 这个例程将返回字符串"VARIANT"作为第二个结果列（i==1），并返回一个空指针作为第一个结果列（i==0）。)^
**
** ^SQLite使用动态运行时类型。 ^所以仅仅因为一列被声明为包含特定类型，
** 并不意味着存储在该列中的数据是声明类型。 SQLite是强类型的，但类型是动态的，不是静态的。 ^类型
** 与单个值相关联，而不是与用于容纳这些值的容器相关联。
*/
SQLITE_API const char *SQLITE_STDCALL sqlite3_column_decltype(sqlite3_stmt*,int);
SQLITE_API const void *SQLITE_STDCALL sqlite3_column_decltype16(sqlite3_stmt*,int);

/*
** CAPI3REF: SQL语句求值
** 方法: sqlite3_stmt
**
** 在使用[sqlite3_prepare_v2()]或[sqlite3_prepare16_v2()]或其中一个旧接口[sqlite3_prepare()]或[sqlite3_prepare16()]准备[准备语句]之后，
** 必须调用此函数一次或多次来评估语句。
**
** sqlite3_step()接口的行为细节取决于语句是使用较新的"v2"接口[sqlite3_prepare_v2()]和[sqlite3_prepare16_v2()]还是旧的遗留接口[sqlite3_prepare()]和[sqlite3_prepare16()]准备的。
** 对于新应用程序，推荐使用新的"v2"接口，但将继续支持遗留接口。
**
** ^在遗留接口中，返回值将是[SQLITE_BUSY]、[SQLITE_DONE]、[SQLITE_ROW]、[SQLITE_ERROR]或[SQLITE_MISUSE]之一。
** ^在"v2"接口中，可能会返回其他任何[result codes]或[extended result codes]。
**
** ^[SQLITE_BUSY]表示数据库引擎无法获取其工作所需的数据库锁。如果语句是[COMMIT]或在显式事务之外，则可以重试该语句。
** 如果语句不是[COMMIT]并在显式事务内，则在继续之前应回滚事务。
**
** ^[SQLITE_DONE]表示语句已成功执行完毕。在对此虚拟机再次调用sqlite3_step()之前，应先调用[sqlite3_reset()]将虚拟机重置为其初始状态。
**
** ^如果执行的SQL语句返回任何数据，则每次有新数据行准备好供调用者处理时，都会返回[SQLITE_ROW]。可以使用[column access functions]访问这些值。
** 再次调用sqlite3_step()以检索下一行数据。
**
** ^[SQLITE_ERROR]表示发生了运行时错误（例如约束违规）。不应在此VM上再次调用sqlite3_step()。
** 可以通过调用[sqlite3_errmsg()]找到更多信息。^在遗留接口中，通过在[准备语句]上调用[sqlite3_reset()]，可以获得更具体的错误代码（例如，
** [SQLITE_INTERRUPT]、[SQLITE_SCHEMA]、[SQLITE_CORRUPT]等）。^在"v2"接口中，更具体的错误代码直接由sqlite3_step()返回。
**
** [SQLITE_MISUSE]表示不适当地调用了此例程。也许它是在已经[sqlite3_finalize | finalized]的[准备语句]上调用，或者是在之前返回[SQLITE_ERROR]或[SQLITE_DONE]的[准备语句]上调用。
** 或者可能是同一数据库连接正在同时被两个或更多线程使用。
**
** 对于所有版本的SQLite，包括3.6.23.1及之前的版本，在sqlite3_step()返回除[SQLITE_ROW]之外的任何内容之前，都需要调用[sqlite3_reset()]，然后才能进行任何后续的sqlite3_step()调用。
** 如果不使用[sqlite3_reset()]重置准备语句，将导致sqlite3_step()返回[SQLITE_MISUSE]。但从3.6.23.1版本之后，sqlite3_step()在这些情况下自动调用[sqlite3_reset()]，而不是返回[SQLITE_MISUSE]。
** 这不认为是兼容性中断，因为任何曾经收到SQLITE_MISUSE错误的程序按定义都是错误的。[SQLITE_OMIT_AUTORESET]编译时选项可用于恢复遗留行为。
**
** <b>Goofy Interface Alert:</b> 在遗留接口中，sqlite3_step() API在除[SQLITE_BUSY]和[SQLITE_MISUSE]之外的任何错误后总是返回一个通用的错误代码[SQLITE_ERROR]。
** 必须调用[sqlite3_reset()]或[sqlite3_finalize()]以找到一个更好地描述错误的特定[error codes]。
** 我们承认这是一个愚蠢的设计。该问题已在"v2"接口中修复。如果您使用[sqlite3_prepare_v2()]或[sqlite3_prepare16_v2()]而不是遗留的[sqlite3_prepare()]和[sqlite3_prepare16()]接口准备所有SQL语句，
** 则更具体的[error codes]将直接由sqlite3_step()返回。推荐使用"v2"接口。
*/
SQLITE_API int SQLITE_STDCALL sqlite3_step(sqlite3_stmt*);

/**
** CAPI3REF: 结果集中列的数量
** 方法: sqlite3_stmt
**
** ^sqlite3_data_count(P) 接口返回 [准备语句] P 的结果集中当前行的列数。
** ^如果准备语句 P 没有准备好返回结果
** （通过调用 [sqlite3_column_int | sqlite3_column_*()] 接口）
** 则 sqlite3_data_count(P) 返回 0。
** ^如果 P 是一个空指针，sqlite3_data_count(P) 也会返回 0。
** ^如果前一次调用 [sqlite3_step](P) 返回 [SQLITE_DONE]，
** 则 sqlite3_data_count(P) 返回 0。
** ^如果前一次调用 [sqlite3_step](P) 返回 [SQLITE_ROW]，
** 则 sqlite3_data_count(P) 将返回非零值，
** 除非在 [PRAGMA incremental_vacuum] 的情况下，
** 在这种情况下，它总是返回 0，因为该多步骤 pragma 的每一步返回 0 列数据。
**
** 另见: [sqlite3_column_count()]
*/
SQLITE_API int SQLITE_STDCALL sqlite3_data_count(sqlite3_stmt *pStmt);

/*
** CAPI3REF: 基本数据类型
** 关键词: SQLITE_TEXT
**
** ^(SQLite中的每个值都有五种基本数据类型之一：
**
** <ul>
** <li> 64位有符号整数
** <li> 64位IEEE浮点数
** <li> 字符串
** <li> BLOB
** <li> NULL
** </ul>)^
**
** 这些常量是这些类型的代码。
**
** 注意，SQLITE_TEXT常量在SQLite版本2中也被用于完全不同的含义。链接到SQLite版本2和SQLite版本3的软件应使用SQLITE3_TEXT，而不是SQLITE_TEXT。
*/
#define SQLITE_INTEGER  1
#define SQLITE_FLOAT    2
#define SQLITE_BLOB     4
#define SQLITE_NULL     5
#ifdef SQLITE_TEXT
# undef SQLITE_TEXT
#else
# define SQLITE_TEXT     3
#endif
#define SQLITE3_TEXT     3

/*
** CAPI3REF: 查询的结果值
** 关键词: {列访问函数}
** 方法: sqlite3_stmt
**
** 这些例程返回查询当前结果行的单列信息。在每种情况下，第一个参数是指向正在评估的[准备语句]的指针（即从[sqlite3_prepare_v2()]或其变体返回的[sqlite3_stmt*]），第二个参数应返回信息的列的索引。结果集的最左列索引为0。可以使用[sqlite3_column_count()]确定结果中的列数。
**
** 如果SQL语句当前不指向有效行，或者列索引超出范围，结果是未定义的。这些例程只能在最近一次调用[sqlite3_step()]返回[SQLITE_ROW]之后，且在此之后没有调用[sqlite3_reset()]或[sqlite3_finalize()]时调用。如果在调用[sqlite3_reset()]或[sqlite3_finalize()]之后，或者[sqlite3_step()]返回除[SQLITE_ROW]之外的其他值后调用这些例程，结果未定义。如果[sqlite3_step()]或[sqlite3_reset()]或[sqlite3_finalize()]从不同的线程调用，而任何这些例程仍在挂起，则结果未定义。
**
** sqlite3_column_type()例程返回结果列初始数据类型的[SQLITE_INTEGER | 数据类型代码]。返回值是[SQLITE_INTEGER]、[SQLITE_FLOAT]、[SQLITE_TEXT]、[SQLITE_BLOB]或[SQLITE_NULL]之一。如果未发生如下所述的类型转换，sqlite3_column_type()返回的值才有意义。类型转换后，sqlite3_column_type()返回的值是未定义的。未来版本的SQLite可能会在类型转换后更改sqlite3_column_type()的行为。
**
** 如果结果是BLOB或UTF-8字符串，则sqlite3_column_bytes()例程返回该BLOB或字符串中的字节数。如果结果是UTF-16字符串，则sqlite3_column_bytes()将字符串转换为UTF-8，然后返回字节数。如果结果是数值，则sqlite3_column_bytes()使用[sqlite3_snprintf()]将该值转换为UTF-8字符串并返回该字符串中的字节数。如果结果是NULL，则sqlite3_column_bytes()返回零。
**
** 如果结果是BLOB或UTF-16字符串，则sqlite3_column_bytes16()例程返回该BLOB或字符串中的字节数。如果结果是UTF-8字符串，则sqlite3_column_bytes16()将字符串转换为UTF-16，然后返回字节数。如果结果是数值，则sqlite3_column_bytes16()使用[sqlite3_snprintf()]将该值转换为UTF-16字符串并返回该字符串中的字节数。如果结果是NULL，则sqlite3_column_bytes16()返回零。
**
** [sqlite3_column_bytes()]和[sqlite3_column_bytes16()]返回的值不包括字符串末尾的零终止符。为了清晰起见：[sqlite3_column_bytes()]和[sqlite3_column_bytes16()]返回的是字符串中的字节数，而不是字符数。
**
** 由sqlite3_column_text()和sqlite3_column_text16()返回的字符串，即使是空字符串，总是以零终止。sqlite3_column_blob()对于零长度BLOB的返回值是NULL指针。
**
** <b>警告：</b> [sqlite3_column_value()]返回的对象是一个[未保护的sqlite3_value]对象。在多线程环境中，未保护的sqlite3_value对象只能与[sqlite3_bind_value()]和[sqlite3_result_value()]安全地一起使用。如果以任何其他方式使用由sqlite3_column_value()返回的未保护的sqlite3_value对象，包括对[sqlite3_value_int()]、[sqlite3_value_text()]或[sqlite3_value_bytes()]等例程的调用，行为不是线程安全的。
**
** 这些例程尝试在适当的情况下转换值。例如，如果内部表示是FLOAT，并且请求文本结果，则内部使用[sqlite3_snprintf()]自动执行转换。以下是应用的转换的详细信息：
**
** <blockquote>
** <table border="1">
** <tr><th> 内部<br>类型 <th> 请求的<br>类型 <th>  转换
**
** <tr><td>  NULL    <td> INTEGER   <td> 结果是0
** <tr><td>  NULL    <td>  FLOAT    <td> 结果是0.0
** <tr><td>  NULL    <td>   TEXT    <td> 结果是NULL指针
** <tr><td>  NULL    <td>   BLOB    <td> 结果是NULL指针
** <tr><td> INTEGER  <td>  FLOAT    <td> 从整数转换为浮点数
** <tr><td> INTEGER  <td>   TEXT    <td> 整数的ASCII表示
** <tr><td> INTEGER  <td>   BLOB    <td> 与INTEGER->TEXT相同
** <tr><td>  FLOAT   <td> INTEGER   <td> [CAST]到INTEGER
** <tr><td>  FLOAT   <td>   TEXT    <td> 浮点的ASCII表示
** <tr><td>  FLOAT   <td>   BLOB    <td> [CAST]到BLOB
** <tr><td>  TEXT    <td> INTEGER   <td> [CAST]到INTEGER
** <tr><td>  TEXT    <td>  FLOAT    <td> [CAST]到REAL
** <tr><td>  TEXT    <td>   BLOB    <td> 无变化
** <tr><td>  BLOB    <td> INTEGER   <td> [CAST]到INTEGER
** <tr><td>  BLOB    <td>  FLOAT    <td> [CAST]到REAL
** <tr><td>  BLOB    <td>   TEXT    <td> 如需添加零终止符
** </table>
** </blockquote>
**
** 注意，当发生类型转换时，先前对sqlite3_column_blob()、sqlite3_column_text()和/或sqlite3_column_text16()的调用返回的指针可能会失效。
** 类型转换和指针失效可能发生在以下情况：
**
** <ul>
** <li> 初始内容是BLOB，并调用sqlite3_column_text()或sqlite3_column_text16()。可能需要向字符串添加零终止符。</li>
** <li> 初始内容是UTF-8文本，并调用sqlite3_column_bytes16()或sqlite3_column_text16()。内容必须转换为UTF-16。</li>
** <li> 初始内容是UTF-16文本，并调用sqlite3_column_bytes()或sqlite3_column_text()。内容必须转换为UTF-8。</li>
** </ul>
**
** ^UTF-16be和UTF-16le之间的转换总是就地完成，不会使先前的指针失效，尽管先前指针引用的缓冲区内容当然会被修改。其他类型的转换在可能的情况下总是就地完成，但在这些情况下，先前指针会被失效。
**
** 最安全的策略是按以下方式之一调用这些例程：
**
** <ul>
**  <li>sqlite3_column_text()后跟sqlite3_column_bytes()</li>
**  <li>sqlite3_column_blob()后跟sqlite3_column_bytes()</li>
**  <li>sqlite3_column_text16()后跟sqlite3_column_bytes16()</li>
** </ul>
**
** 换句话说，你应该先调用sqlite3_column_text()、sqlite3_column_blob()或sqlite3_column_text16()，以强制结果进入所需格式，然后调用sqlite3_column_bytes()或sqlite3_column_bytes16()来查找结果的大小。不要混合调用sqlite3_column_text()或sqlite3_column_blob()与sqlite3_column_bytes16()，也不要混合调用sqlite3_column_text16()与sqlite3_column_bytes()。
**
** ^返回的指针在如上所述的类型转换发生之前，或者调用[sqlite3_step()]、[sqlite3_reset()]或[sqlite3_finalize()]之前都有效。用于保存字符串和BLOB的内存空间会自动释放。不要将[sqlite3_column_blob()]、[sqlite3_column_text()]等返回的指针传递给[sqlite3_free()]。
**
** ^(如果在评估这些例程期间发生内存分配错误，将返回默认值。默认值是整数0、浮点数0.0或NULL指针。后续调用[sqlite3_errcode()]将返回[SQLITE_NOMEM]。)^
*/
SQLITE_API const void *SQLITE_STDCALL sqlite3_column_blob(sqlite3_stmt*, int iCol);
SQLITE_API int SQLITE_STDCALL sqlite3_column_bytes(sqlite3_stmt*, int iCol);
SQLITE_API int SQLITE_STDCALL sqlite3_column_bytes16(sqlite3_stmt*, int iCol);
SQLITE_API double SQLITE_STDCALL sqlite3_column_double(sqlite3_stmt*, int iCol);
SQLITE_API int SQLITE_STDCALL sqlite3_column_int(sqlite3_stmt*, int iCol);
SQLITE_API sqlite3_int64 SQLITE_STDCALL sqlite3_column_int64(sqlite3_stmt*, int iCol);
SQLITE_API const unsigned char *SQLITE_STDCALL sqlite3_column_text(sqlite3_stmt*, int iCol);
SQLITE_API const void *SQLITE_STDCALL sqlite3_column_text16(sqlite3_stmt*, int iCol);
SQLITE_API int SQLITE_STDCALL sqlite3_column_type(sqlite3_stmt*, int iCol);
SQLITE_API sqlite3_value *SQLITE_STDCALL sqlite3_column_value(sqlite3_stmt*, int iCol);

/**
** CAPI3REF: 销毁一个已准备好的语句对象
** DESTRUCTOR: sqlite3_stmt
**
** ^sqlite3_finalize() 函数被调用来删除一个 [已准备好的语句]。
** ^如果最近的语句评估未遇到错误，或者该语句从未被评估过，那么 sqlite3_finalize() 返回
** SQLITE_OK。 ^如果最近的语句 S 的评估失败，那么 sqlite3_finalize(S) 返回相应的 [错误代码] 或
** [扩展错误代码]。
**
** ^可以在 [已准备好的语句] S 的生命周期中的任何时间点调用 sqlite3_finalize(S)：
** 在语句 S 被评估之前，在
** 一次或多次调用 [sqlite3_reset()] 之后，或者在调用 [sqlite3_step()] 之后，
** 无论语句是否已完成执行。
**
** ^对空指针调用 sqlite3_finalize() 是一个无害的空操作。
**
** 应用程序必须最终化每个 [已准备好的语句] 以避免资源泄漏。
** 应用程序尝试在已最终化的 [已准备好的语句] 之后使用它是严重的错误。
** 在已最终化的 [已准备好的语句] 之后使用它可能会导致未定义和
** 不希望的行为，如段错误和堆损坏。
*/
SQLITE_API int SQLITE_STDCALL sqlite3_finalize(sqlite3_stmt *pStmt);

/**
** CAPI3REF: 重置一个已准备好的语句对象
** METHOD: sqlite3_stmt
**
** sqlite3_reset() 函数被调用来将 [已准备好的语句] 对象重置回其初始状态，准备重新执行。
** ^任何使用 [sqlite3_bind_blob | sqlite3_bind_*() API] 绑定值的 SQL 语句变量保留其值。
** 使用 [sqlite3_clear_bindings()] 来重置绑定。
**
** ^[sqlite3_reset(S)] 接口将 [已准备好的语句] S 重置回其程序的开始。
**
** ^如果最近的 [sqlite3_step(S)] 调用对于 [已准备好的语句] S 返回 [SQLITE_ROW] 或 [SQLITE_DONE]，
** 或者如果 [sqlite3_step(S)] 从未在 S 上被调用过，
** 那么 [sqlite3_reset(S)] 返回 [SQLITE_OK]。
**
** ^如果最近的 [sqlite3_step(S)] 调用对于 [已准备好的语句] S 指示了一个错误，那么
** [sqlite3_reset(S)] 返回一个适当的 [错误代码]。
**
** ^[sqlite3_reset(S)] 接口不会改变 [已准备好的语句] S 上的任何 [sqlite3_bind_blob|绑定] 的值。
*/
SQLITE_API int SQLITE_STDCALL sqlite3_reset(sqlite3_stmt *pStmt);

/*
** CAPI3REF: 创建或重新定义SQL函数
** 关键词: {函数创建例程}
** 关键词: {应用程序定义的SQL函数}
** 关键词: {应用程序定义的SQL函数}
** 方法: sqlite3
**
** ^这些函数（统称为“函数创建例程”）用于添加SQL函数或聚合，或重新定义现有SQL函数或聚合的行为。这些例程之间的唯一区别是第二个参数（要创建的函数的名称）期望的文本编码，以及应用程序数据指针的析构回调的存在或缺失。
**
** ^第一个参数是要添加SQL函数的[数据库连接]。^如果应用程序使用多个数据库连接，则必须分别向每个数据库连接添加应用程序定义的SQL函数。
**
** ^第二个参数是要创建或重新定义的SQL函数的名称。^名称的长度限制为255字节，以UTF-8表示，不包括终止符。^请注意，名称长度限制是以UTF-8字节为单位，而不是字符或UTF-16字节。^任何尝试创建名称更长的函数都会导致返回[SQLITE_MISUSE]。
**
** ^第三个参数（nArg）是SQL函数或聚合接受的参数数量。^如果此参数为-1，则SQL函数或聚合可以接受0到由[sqlite3_limit]([SQLITE_LIMIT_FUNCTION_ARG])设置的限值之间的任意数量的参数。如果第三个参数小于-1或大于127，则行为是未定义的。
**
** ^第四个参数eTextRep指定SQL函数对参数的[SQLITE_UTF8 | 文本编码]偏好。应用程序应将此参数设置为[SQLITE_UTF16LE]，如果函数实现调用了[sqlite3_value_text16le()]来处理输入，或[SQLITE_UTF16BE]，如果实现调用了[sqlite3_value_text16be()]来处理输入，或[SQLITE_UTF16]，如果使用了[sqlite3_value_text16()]，否则设置为[SQLITE_UTF8]。^相同的SQL函数可以使用不同的首选文本编码多次注册，每种编码都有不同的实现。^当同一函数有多个实现可用时，SQLite将选择涉及最少数据转换的那个。
**
** ^第四个参数可以选择性地与[SQLITE_DETERMINISTIC]进行按位或操作，以指示函数在单个SQL语句中给定相同输入时总是返回相同的结果。大多数SQL函数是确定性的。内置的[random()] SQL函数是一个非确定性的函数示例。SQLite查询计划器能够对确定性函数进行额外的优化，因此建议在可能的情况下使用[SQLITE_DETERMINISTIC]标志。
**
** ^(第五个参数是一个任意指针。函数的实现可以使用[sqlite3_user_data()]访问此指针。) ^
**
** ^第六、第七和第八个参数xFunc、xStep和xFinal是指向实现SQL函数或聚合的C语言函数的指针。^标量SQL函数只需要xFunc回调的实现；必须为xStep和xFinal参数传递NULL指针。^聚合SQL函数需要xStep和xFinal的实现，必须为xFunc传递NULL指针。^要删除现有的SQL函数或聚合，必须为所有三个函数回调传递NULL指针。
**
** ^(如果sqlite3_create_function_v2()的第九个参数不为NULL，则它是应用程序数据指针的析构函数。当函数被删除，无论是被覆盖还是当数据库连接关闭时，都会调用析构函数。) ^如果调用sqlite3_create_function_v2()失败，也会调用析构函数。^当第十个参数的析构回调被调用时，它会传递一个参数，该参数是sqlite3_create_function_v2()的第五个参数应用程序数据指针的副本。
**
** ^允许使用相同名称但参数数量不同或首选文本编码不同的多个实现来注册相同的函数。^SQLite将使用与SQL函数使用方式最匹配的实现。^具有非负nArg参数的函数实现比具有负nArg的函数实现更匹配。^首选文本编码与数据库编码匹配的函数比编码不同的函数更匹配。^编码差异在UTF16le和UTF16be之间的函数比编码差异在UTF8和UTF16之间的函数更匹配。
**
** ^内置函数可以被新的应用程序定义的函数覆盖。
**
** ^允许应用程序定义的函数调用其他SQLite接口。然而，这样的调用不得关闭数据库连接，也不得完成或重置正在运行函数的预准备语句。
*/
SQLITE_API int SQLITE_STDCALL sqlite3_create_function(
  sqlite3 *db,
  const char *zFunctionName,
  int nArg,
  int eTextRep,
  void *pApp,
  void (*xFunc)(sqlite3_context*,int,sqlite3_value**),
  void (*xStep)(sqlite3_context*,int,sqlite3_value**),
  void (*xFinal)(sqlite3_context*)
);
SQLITE_API int SQLITE_STDCALL sqlite3_create_function16(
  sqlite3 *db,
  const void *zFunctionName,
  int nArg,
  int eTextRep,
  void *pApp,
  void (*xFunc)(sqlite3_context*,int,sqlite3_value**),
  void (*xStep)(sqlite3_context*,int,sqlite3_value**),
  void (*xFinal)(sqlite3_context*)
);
SQLITE_API int SQLITE_STDCALL sqlite3_create_function_v2(
  sqlite3 *db,
  const char *zFunctionName,
  int nArg,
  int eTextRep,
  void *pApp,
  void (*xFunc)(sqlite3_context*,int,sqlite3_value**),
  void (*xStep)(sqlite3_context*,int,sqlite3_value**),
  void (*xFinal)(sqlite3_context*),
  void(*xDestroy)(void*)
);

/*
** CAPI3REF: 文本编码
**
** 这些常量定义了表示SQLite支持的各种文本编码的整数代码。
*/
#define SQLITE_UTF8           1    /* IMP: R-37514-35566 */
#define SQLITE_UTF16LE        2    /* IMP: R-03371-37637 */
#define SQLITE_UTF16BE        3    /* IMP: R-51971-34154 */
#define SQLITE_UTF16          4    /* 使用本地字节顺序 */
#define SQLITE_ANY            5    /* 已弃用 */
#define SQLITE_UTF16_ALIGNED  8    /* sqlite3_create_collation only */
/*
** CAPI3REF: 函数标志
**
** 这些常量可以与 [SQLITE_UTF8 | 首选文本编码] 一起作为第四个参数
** 传递给 [sqlite3_create_function()], [sqlite3_create_function16()], 或
** [sqlite3_create_function_v2()].
*/
#define SQLITE_DETERMINISTIC    0x800

/*
** CAPI3REF: 已弃用的函数
** 已弃用
**
** 这些函数已被[弃用]。为了与旧代码保持向后兼容，
** 这些函数仍然得到支持。然而，新的应用程序应避免
** 使用这些函数。为了鼓励程序员避免使用这些函数，
** 我们不会解释它们的作用。
*/
#ifndef SQLITE_OMIT_DEPRECATED
SQLITE_API SQLITE_DEPRECATED int SQLITE_STDCALL sqlite3_aggregate_count(sqlite3_context*);
SQLITE_API SQLITE_DEPRECATED int SQLITE_STDCALL sqlite3_expired(sqlite3_stmt*);
SQLITE_API SQLITE_DEPRECATED int SQLITE_STDCALL sqlite3_transfer_bindings(sqlite3_stmt*, sqlite3_stmt*);
SQLITE_API SQLITE_DEPRECATED int SQLITE_STDCALL sqlite3_global_recover(void);
SQLITE_API SQLITE_DEPRECATED void SQLITE_STDCALL sqlite3_thread_cleanup(void);
SQLITE_API SQLITE_DEPRECATED int SQLITE_STDCALL sqlite3_memory_alarm(void(*)(void*,sqlite3_int64,int),
					  void*,sqlite3_int64);
#endif
/*
** CAPI3REF: 获取SQL值
** 方法: sqlite3_value
**
** SQL函数和聚合的C语言实现使用这组接口例程来访问函数或聚合的参数值。
**
** [sqlite3_create_function()] 和 [sqlite3_create_function16()] 的 xFunc（对于标量函数）或 xStep（对于聚合）参数定义了实现SQL函数和聚合的回调。
** 这些回调的第三个参数是一个指向 [protected sqlite3_value] 对象的指针数组。
** 对于SQL函数的每个参数，都有一个 [sqlite3_value] 对象。
** 这些例程用于从 [sqlite3_value] 对象中提取值。
**
** 这些例程仅适用于 [protected sqlite3_value] 对象。
** 任何尝试在 [unprotected sqlite3_value] 对象上使用这些例程都会导致未定义的行为。
**
** ^这些例程的工作方式与相应的 [列访问函数] 相同，
** 只是这些例程接受一个 [protected sqlite3_value] 对象指针，
** 而不是 [sqlite3_stmt*] 指针和整数列号。
**
** ^sqlite3_value_text16() 接口提取主机机器本地字节序的 UTF-16 字符串。
** ^sqlite3_value_text16be() 和 sqlite3_value_text16le() 接口分别提取大端和小端的 UTF-16 字符串。
**
** ^(sqlite3_value_numeric_type() 接口尝试对值应用数值亲和性。
** 这意味着会尝试将值转换为整数或浮点数。
** 如果这种转换可以在不失信息的情况下进行（换句话说，如果值是一个看起来像数字的字符串），
** 则执行转换。否则不进行转换。
** 返回转换后的 [SQLITE_INTEGER | datatype]。)^
**
** 请特别注意，从 [sqlite3_value_blob()]、[sqlite3_value_text()] 或
** [sqlite3_value_text16()] 返回的指针可能会被后续对
** [sqlite3_value_bytes()]、[sqlite3_value_bytes16()]、[sqlite3_value_text()] 或
** [sqlite3_value_text16()] 的调用失效。
**
** 这些例程必须从提供 [sqlite3_value*] 参数的 SQL 函数的同一线程中调用。
*/
SQLITE_API const void *SQLITE_STDCALL sqlite3_value_blob(sqlite3_value*);
SQLITE_API int SQLITE_STDCALL sqlite3_value_bytes(sqlite3_value*);
SQLITE_API int SQLITE_STDCALL sqlite3_value_bytes16(sqlite3_value*);
SQLITE_API double SQLITE_STDCALL sqlite3_value_double(sqlite3_value*);
SQLITE_API int SQLITE_STDCALL sqlite3_value_int(sqlite3_value*);
SQLITE_API sqlite3_int64 SQLITE_STDCALL sqlite3_value_int64(sqlite3_value*);
SQLITE_API const unsigned char *SQLITE_STDCALL sqlite3_value_text(sqlite3_value*);
SQLITE_API const void *SQLITE_STDCALL sqlite3_value_text16(sqlite3_value*);
SQLITE_API const void *SQLITE_STDCALL sqlite3_value_text16le(sqlite3_value*);
SQLITE_API const void *SQLITE_STDCALL sqlite3_value_text16be(sqlite3_value*);
SQLITE_API int SQLITE_STDCALL sqlite3_value_type(sqlite3_value*);
SQLITE_API int SQLITE_STDCALL sqlite3_value_numeric_type(sqlite3_value*);

/*
** CAPI3REF: 复制和释放 SQL 值
** METHOD: sqlite3_value
**
** ^sqlite3_value_dup(V) 接口复制一个 [sqlite3_value] 对象 D 并返回该副本的指针。
** ^返回的 [sqlite3_value] 是一个 [受保护的 sqlite3_value] 对象，即使输入对象不是。
** ^如果 V 为 NULL 或内存分配失败，sqlite3_value_dup(V) 接口返回 NULL。
**
** ^sqlite3_value_free(V) 接口释放一个先前通过 [sqlite3_value_dup()] 获得的 [sqlite3_value] 对象。
** ^如果 V 是一个 NULL 指针，那么 sqlite3_value_free(V) 是一个无害的空操作。
*/
SQLITE_API SQLITE_EXPERIMENTAL sqlite3_value *SQLITE_STDCALL sqlite3_value_dup(const sqlite3_value*);
SQLITE_API SQLITE_EXPERIMENTAL void SQLITE_STDCALL sqlite3_value_free(sqlite3_value*);

/*
** CAPI3REF: 获取聚合函数上下文
** METHOD: sqlite3_context
**
** 聚合 SQL 函数的实现使用此例程来分配内存以存储其状态。
**
** ^第一次为特定聚合函数调用 sqlite3_aggregate_context(C,N) 例程时，
** SQLite 会分配 N 字节的内存，将该内存清零，并返回指向该内存的指针。
** ^对于同一聚合函数实例的第二次及后续调用，
** 将返回相同的缓冲区。Sqlite3_aggregate_context() 通常
** 在每次调用 xStep 回调时调用一次，然后在调用 xFinal 回调时最后调用一次。
** ^(当聚合查询没有匹配的行时，聚合函数实现的 xStep() 回调永远不会被调用，
** 而 xFinal() 只会被调用一次。在这种情况下，sqlite3_aggregate_context() 
** 可能会在 xFinal() 中首次被调用。)^
**
** ^如果 N 小于或等于零，或者内存分配失败，sqlite3_aggregate_context(C,N) 例程
** 在首次调用时会返回 NULL 指针。
**
** ^(sqlite3_aggregate_context(C,N) 分配的内存空间大小由第一次成功调用时的 N 参数决定。
** 在同一聚合函数实例中，后续调用 sqlite3_aggregate_context() 时更改 N 的值
** 不会调整内存分配的大小。)^ 在 xFinal 回调中，通常会将 N 设置为 0，
** 以避免无意义的内存分配。
**
** ^当聚合查询结束时，SQLite 会自动释放由 sqlite3_aggregate_context() 分配的内存。
**
** 第一个参数必须是实现聚合函数的 xStep 或 xFinal 回调例程的第一个参数
** [sqlite3_context | SQL 函数上下文] 的副本。
**
** 此例程必须从运行聚合 SQL 函数的同一线程中调用。
*/
SQLITE_API void *SQLITE_STDCALL sqlite3_aggregate_context(sqlite3_context*, int nBytes);

/**
 * CAPI3REF: 用户数据函数
 * 方法: sqlite3_context
 *
 * ^sqlite3_user_data() 接口返回一个指针的副本，
 * 该指针是 pUserData 参数（第5个参数），
 * 它原本是 [sqlite3_create_function()] 和
 * [sqlite3_create_function16()] 例程的参数，
 * 这些例程最初注册了应用程序定义的函数。
 *
 * 此例程必须从与应用程序定义的函数运行的相同线程中调用。
 */
SQLITE_API void *SQLITE_STDCALL sqlite3_user_data(sqlite3_context*);

/*
** CAPI3REF: 数据库连接函数
** METHOD: sqlite3_context
**
** ^sqlite3_context_db_handle() 接口返回一个指向
** [数据库连接]（第一个参数）的副本，
** 这个连接是 [sqlite3_create_function()]
** 和 [sqlite3_create_function16()] 例程最初
** 注册的应用定义函数。
*/
SQLITE_API sqlite3 *SQLITE_STDCALL sqlite3_context_db_handle(sqlite3_context*);

/*
** CAPI3REF: 函数辅助数据
** 方法: sqlite3_context
**
** 这些函数可以由（非聚合）SQL函数使用，以与参数值关联元数据。如果在查询执行期间，相同的值传递给同一SQL函数的多次调用，在某些情况下，可能会保留关联的元数据。例如，正则表达式匹配函数中可能会用到这一点。正则表达式的编译版本可以作为与模式字符串关联的元数据存储。只要模式字符串保持不变，编译后的正则表达式可以在同一函数的多次调用中重复使用。
**
** ^sqlite3_get_auxdata() 接口返回由 sqlite3_set_auxdata() 函数与应用程序定义函数的第N个参数值关联的元数据的指针。^如果没有与函数参数关联的元数据，sqlite3_get_auxdata() 接口将返回一个空指针。
**
** ^sqlite3_set_auxdata(C,N,P,X) 接口将P保存为应用程序定义函数的第N个参数的元数据。^后续对 sqlite3_get_auxdata(C,N) 的调用将返回最近的 sqlite3_set_auxdata(C,N,P,X) 调用的P，如果元数据仍然有效，或者为NULL，如果元数据已被丢弃。
** ^每次调用 sqlite3_set_auxdata(C,N,P,X) 时，如果X不为NULL，SQLite将在元数据被丢弃时精确调用一次析构函数X，参数为P。SQLite可以在任何时候丢弃元数据，包括： <ul>
** <li> 当相应的函数参数更改时，或者
** <li> 当为SQL语句调用 [sqlite3_reset()] 或 [sqlite3_finalize()] 时，或者
** <li> 当在同一参数上再次调用 sqlite3_set_auxdata() 时，或者
** <li> 在原始 sqlite3_set_auxdata() 调用期间发生内存分配错误时。 </ul>)^
**
** 特别注意最后一个项目。sqlite3_set_auxdata(C,N,P,X) 中的析构函数X可能在立即被调用，甚至在 sqlite3_set_auxdata() 接口返回之前。因此，sqlite3_set_auxdata() 应该在函数实现的末尾调用，并且函数实现不应在调用 sqlite3_set_auxdata() 后使用P。
**
** 实际上，对于在编译时是常量的函数参数，包括字面值和 [参数] 以及由相同的表达式组成的表达式，会在函数调用之间保留元数据。
**
** 这些例程必须从运行SQL函数的同一线程中调用。
*/
SQLITE_API void *SQLITE_STDCALL sqlite3_get_auxdata(sqlite3_context*, int N);
SQLITE_API void SQLITE_STDCALL sqlite3_set_auxdata(sqlite3_context*, int N, void*, void (*)(void*));


/*
** CAPI3REF: 常量定义特殊析构行为
**
** 这些是作为最终参数传递给类似 [sqlite3_result_blob()] 的例程的特殊析构值。
** ^如果析构参数是 SQLITE_STATIC，意味着内容指针是常量且永远不会改变。
** 不需要销毁它。^析构参数为 SQLITE_TRANSIENT 时，表示内容可能在不久的将来改变，
** SQLite 应该在返回之前制作内容的私有副本。
**
** 使用 typedef 是为了解决某些 C++ 编译器中的问题。
*/
typedef void (*sqlite3_destructor_type)(void*);
#define SQLITE_STATIC      ((sqlite3_destructor_type)0)
#define SQLITE_TRANSIENT   ((sqlite3_destructor_type)-1)

/*
** CAPI3REF: 设置SQL函数的结果
** 方法: sqlite3_context
**
** 这些例程被xFunc或xFinal回调使用，这些回调实现了SQL函数和聚合。参见
** [sqlite3_create_function()] 和 [sqlite3_create_function16()]
** 获取更多信息。
**
** 这些函数的工作方式非常类似于[参数绑定]函数家族，这些函数用于将值绑定到
** 预备语句中的主机参数。参考[SQL参数]文档获取更多信息。
**
** ^sqlite3_result_blob()接口将应用程序定义的函数的结果设置为BLOB，
** 其内容由第二个参数指向，长度为N字节，其中N是第三个参数。
**
** ^sqlite3_result_zeroblob()接口将应用程序定义的函数的结果设置为一个
** 包含所有零字节的BLOB，大小为N字节，其中N是第二个参数的值。
**
** ^sqlite3_result_double()接口将应用程序定义的函数的结果设置为一个
** 浮点数，由其第二个参数指定。
**
** ^sqlite3_result_error()和sqlite3_result_error16()函数导致实现的SQL
** 函数抛出异常。^SQLite使用sqlite3_result_error()或sqlite3_result_error16()
** 的第二个参数指向的字符串作为错误消息的文本。^SQLite将sqlite3_result_error()
** 的错误消息字符串解释为UTF-8。^SQLite将sqlite3_result_error16()的字符串
** 解释为本地字节顺序的UTF-16。^如果sqlite3_result_error()或
** sqlite3_result_error16()的第三个参数是负数，则SQLite将错误消息视为
** 从第二个参数开始直到第一个零字符的所有文本。^如果sqlite3_result_error()
** 或sqlite3_result_error16()的第三个参数是非负数，则SQLite将从第二个
** 参数中取出那么多字节（不是字符）作为错误消息。^sqlite3_result_error()
** 和sqlite3_result_error16()例程在返回之前对错误消息文本进行私有复制。
** 因此，调用函数在它们返回后可以释放或修改文本而不会造成伤害。^
** sqlite3_result_error_code()函数更改函数错误导致的SQLite返回的错误代码。
** ^默认情况下，错误代码是SQLITE_ERROR。^对sqlite3_result_error()或
** sqlite3_result_error16()的后一次调用将错误代码重置为SQLITE_ERROR。
**
** ^sqlite3_result_error_toobig()接口导致SQLite抛出一个错误，指示字符串
** 或BLOB太长无法表示。
**
** ^sqlite3_result_error_nomem()接口导致SQLite抛出一个错误，指示内存分配失败。
**
** ^sqlite3_result_int()接口将应用程序定义的函数的返回值设置为第二个
** 参数中给出的32位有符号整数值。^sqlite3_result_int64()接口将应用程序定义的
** 函数的返回值设置为第二个参数中给出的64位有符号整数值。
**
** ^sqlite3_result_null()接口将应用程序定义的函数的返回值设置为NULL。
**
** ^sqlite3_result_text(), sqlite3_result_text16(), sqlite3_result_text16le(),
** 和sqlite3_result_text16be()接口将应用程序定义的函数的返回值设置为文本字符串，
** 分别以UTF-8, UTF-16本地字节顺序, UTF-16小端或UTF-16大端表示。^sqlite3_result_text64()
** 接口将应用程序定义的函数的返回值设置为以第五个（也是最后一个）参数指定的编码
** 表示的文本字符串，该参数必须是[SQLITE_UTF8], [SQLITE_UTF16], [SQLITE_UTF16BE],
** 或[SQLITE_UTF16LE]之一。^SQLite从sqlite3_result_text*接口的第二个参数中获取
** 应用程序的文本结果。^如果sqlite3_result_text*接口的第三个参数是负数，则SQLite
** 从第二个参数获取结果文本，直到第一个零字符。^如果sqlite3_result_text*接口的
** 第三个参数是非负数，则从第二个参数指向的文本中取出那么多字节（不是字符）作为
** 应用程序定义的函数的结果。如果第三个参数是非负数，则它必须是字符串中NUL终止符
** 出现的偏移量。如果在第三个参数值之内的字符串中出现任何NUL字符，则结果字符串将
** 包含嵌入的NUL，并且对包含嵌入NUL的字符串进行操作的表达式的结果是未定义的。^
** 如果sqlite3_result_text*接口或sqlite3_result_blob的第四个参数是非空指针，
** 则SQLite在完成使用该结果时调用该函数作为文本或BLOB结果的析构函数。^如果
** sqlite3_result_text*接口或sqlite3_result_blob的第四个参数是特殊常量SQLITE_STATIC，
** 则SQLite假设文本或BLOB结果位于常量空间中，并且在返回时不复制参数的内容，也不在
** 完成使用该结果时调用内容的析构函数。^如果sqlite3_result_text*接口或sqlite3_result_blob
** 的第四个参数是特殊常量SQLITE_TRANSIENT，则SQLite在返回之前从[sqlite3_malloc()]
** 获得的空间中复制结果。
**
** ^sqlite3_result_value()接口将应用程序定义的函数的结果设置为第二个参数
** 指定的[未保护sqlite3_value]对象的副本。^sqlite3_result_value()接口复制
** [sqlite3_value]，因此参数中指定的[sqlite3_value]在sqlite3_result_value()返回后
** 可以更改或释放，而不会造成伤害。^[保护sqlite3_value]对象可以始终用于需要
** [未保护sqlite3_value]对象的地方，因此这两种类型的[sqlite3_value]对象都可以
** 与此接口一起使用。
**
** 如果这些例程从与应用程序定义的函数接收[sqlite3_context]指针的不同线程中调用，
** 则结果未定义。
*/
SQLITE_API void SQLITE_STDCALL sqlite3_result_blob(sqlite3_context*, const void*, int, void(*)(void*));
SQLITE_API void SQLITE_STDCALL sqlite3_result_blob64(sqlite3_context*,const void*,
						   sqlite3_uint64,void(*)(void*));
SQLITE_API void SQLITE_STDCALL sqlite3_result_double(sqlite3_context*, double);
SQLITE_API void SQLITE_STDCALL sqlite3_result_error(sqlite3_context*, const char*, int);
SQLITE_API void SQLITE_STDCALL sqlite3_result_error16(sqlite3_context*, const void*, int);
SQLITE_API void SQLITE_STDCALL sqlite3_result_error_toobig(sqlite3_context*);
SQLITE_API void SQLITE_STDCALL sqlite3_result_error_nomem(sqlite3_context*);
SQLITE_API void SQLITE_STDCALL sqlite3_result_error_code(sqlite3_context*, int);
SQLITE_API void SQLITE_STDCALL sqlite3_result_int(sqlite3_context*, int);
SQLITE_API void SQLITE_STDCALL sqlite3_result_int64(sqlite3_context*, sqlite3_int64);
SQLITE_API void SQLITE_STDCALL sqlite3_result_null(sqlite3_context*);
SQLITE_API void SQLITE_STDCALL sqlite3_result_text(sqlite3_context*, const char*, int, void(*)(void*));
SQLITE_API void SQLITE_STDCALL sqlite3_result_text64(sqlite3_context*, const char*,sqlite3_uint64,
						   void(*)(void*), unsigned char encoding);
SQLITE_API void SQLITE_STDCALL sqlite3_result_text16(sqlite3_context*, const void*, int, void(*)(void*));
SQLITE_API void SQLITE_STDCALL sqlite3_result_text16le(sqlite3_context*, const void*, int,void(*)(void*));
SQLITE_API void SQLITE_STDCALL sqlite3_result_text16be(sqlite3_context*, const void*, int,void(*)(void*));
SQLITE_API void SQLITE_STDCALL sqlite3_result_value(sqlite3_context*, sqlite3_value*);
SQLITE_API void SQLITE_STDCALL sqlite3_result_zeroblob(sqlite3_context*, int n);

/*
** CAPI3REF: 定义新的排序序列
** 方法: sqlite3
**
** 这些函数向指定的[数据库连接]添加、移除或修改一个[排序规则]。
**
** ^sqlite3_create_collation()和sqlite3_create_collation_v2()的排序规则名称是一个UTF-8字符串，
** sqlite3_create_collation16()的排序规则名称是一个UTF-16字符串，字节顺序为本地字节顺序。
** ^根据[sqlite3_strnicmp()]比较相等的排序规则名称被认为是相同的名称。
**
** ^(第三个参数(eTextRep)必须是以下常量之一：
** <ul>
** <li> [SQLITE_UTF8],
** <li> [SQLITE_UTF16LE],
** <li> [SQLITE_UTF16BE],
** <li> [SQLITE_UTF16], 或
** <li> [SQLITE_UTF16_ALIGNED].
** </ul>)^
** ^eTextRep参数决定了传递给排序规则函数回调xCallback的字符串的编码。
** ^eTextRep参数的[SQLITE_UTF16]和[SQLITE_UTF16_ALIGNED]值强制字符串为本地字节顺序的UTF16。
** ^eTextRep参数的[SQLITE_UTF16_ALIGNED]值强制字符串从偶数字节地址开始。
**
** ^第四个参数pArg是一个应用程序数据指针，作为第一个参数传递给排序规则函数回调。
**
** ^第五个参数xCallback是指向排序规则函数的指针。
** ^可以使用相同的名称注册多个排序规则函数，但eTextRep参数不同，
** SQLite将使用需要最少数据转换的函数。
** ^如果xCallback参数为NULL，则删除排序规则函数。
** ^当所有具有相同名称的排序规则函数都被删除时，该排序规则将不再可用。
**
** ^排序规则函数回调被调用时，会传递一个pArg应用程序数据指针的副本，
** 以及按eTextRep参数指定的编码的两个字符串。
** ^排序规则函数必须返回一个整数，该整数如果是负数、零或正数，
** 则分别表示第一个字符串小于、等于或大于第二个字符串。
** 排序规则函数必须始终对相同的输入返回相同的答案。
** 如果两个或多个排序规则函数注册到相同的排序规则名称（使用不同的eTextRep值），
** 则所有函数在用等效字符串调用时必须给出等效的答案。
** 排序规则函数必须遵守对所有字符串A、B和C的以下属性：
**
** <ol>
** <li> 如果A==B，则B==A。
** <li> 如果A==B且B==C，则A==C。
** <li> 如果A<B，则B>A。
** <li> 如果A<B且B<C，则A<C。
** </ol>
**
** 如果排序规则函数违反了上述任何约束，并且该排序规则函数已注册并使用，
** 则SQLite的行为是未定义的。
**
** ^sqlite3_create_collation_v2()的工作方式类似于sqlite3_create_collation()，
** 不同之处在于当排序规则函数被删除时，会调用xDestroy回调。
** ^排序规则函数在它们被后续的排序规则创建函数覆盖或使用
** [sqlite3_close()]关闭[数据库连接]时被删除。
**
** ^如果sqlite3_create_collation_v2()函数失败，则不会调用xDestroy回调。
** 调用sqlite3_create_collation_v2()并带有非NULL的xDestroy参数的应用程序应检查返回代码，
** 并自行处理应用程序数据指针，而不是期望SQLite为他们处理。
** 这与其他所有SQLite接口不同。这种不一致是不幸的，但无法更改，
** 否则会破坏向后兼容性。
**
** 另见: [sqlite3_collation_needed()] 和 [sqlite3_collation_needed16()]。
*/
SQLITE_API int SQLITE_STDCALL sqlite3_create_collation(
  sqlite3*, 
  const char *zName, 
  int eTextRep, 
  void *pArg,
  int(*xCompare)(void*,int,const void*,int,const void*)
);
SQLITE_API int SQLITE_STDCALL sqlite3_create_collation_v2(
  sqlite3*, 
  const char *zName, 
  int eTextRep, 
  void *pArg,
  int(*xCompare)(void*,int,const void*,int,const void*),
  void(*xDestroy)(void*)
);
SQLITE_API int SQLITE_STDCALL sqlite3_create_collation16(
  sqlite3*, 
  const void *zName,
  int eTextRep, 
  void *pArg,
  int(*xCompare)(void*,int,const void*,int,const void*)
);

/*
** CAPI3REF: 需要排序的回调函数
** 方法: sqlite3
**
** 为了避免在数据库可以使用之前就必须注册所有排序序列，可以通过
** [数据库连接] 注册一个回调函数，在需要未定义的排序序列时调用。
**
** 如果使用 sqlite3_collation_needed() API 注册该函数，
** 则会将未定义的排序序列名称作为 UTF-8 编码的字符串传递给它。
** 如果使用 sqlite3_collation_needed16()，则名称会以机器本地字节顺序的 UTF-16 传递。
** 调用任一函数都会替换现有的排序需求回调。
**
** 当回调被调用时，传递的第一个参数是传递给 sqlite3_collation_needed() 或
** sqlite3_collation_needed16() 的第二个参数的副本。
** 第二个参数是数据库连接。第三个参数是 [SQLITE_UTF8]、[SQLITE_UTF16BE] 或
** [SQLITE_UTF16LE] 之一，表示所需的排序序列函数的最理想形式。
** 第四个参数是所需的排序序列的名称。
**
** 回调函数应使用 [sqlite3_create_collation()]、[sqlite3_create_collation16()] 或
** [sqlite3_create_collation_v2()] 注册所需的排序。
*/
SQLITE_API int SQLITE_STDCALL sqlite3_collation_needed(
  sqlite3*, 
  void*, 
  void(*)(void*,sqlite3*,int eTextRep,const char*)
);
SQLITE_API int SQLITE_STDCALL sqlite3_collation_needed16(
  sqlite3*, 
  void*,
  void(*)(void*,sqlite3*,int eTextRep,const void*)
);

#ifdef SQLITE_HAS_CODEC
/*
** 指定用于加密数据库的密钥。此例程应在sqlite3_open()之后立即调用。
**
** 实现此API的代码在SQLite的公开版本中不可用。
*/
SQLITE_API int SQLITE_STDCALL sqlite3_key(
  sqlite3 *db,                   /* 数据库对象 */
  const void *pKey, int nKey     /* 密钥 */
);
SQLITE_API int SQLITE_STDCALL sqlite3_key_v2(
  sqlite3 *db,                   /* 数据库需要重新设置密钥 */

  const char *zDbName,           /* 数据库名称 */

  const void *pKey, int nKey     /* 关键值 */
);

/*
** 更改打开数据库的密钥。如果当前数据库未加密，此例程将对其进行加密。如果 pNew==0 或 nNew==0，则数据库将被解密。
**
** 实现此API的代码在SQLite的公开版本中不可用。
*/
SQLITE_API int SQLITE_STDCALL sqlite3_rekey(
  sqlite3 *db,                   /* 数据库需要重新密钥化 */
  const void *pKey, int nKey     /* 新关键值 */
);
SQLITE_API int SQLITE_STDCALL sqlite3_rekey_v2(
  sqlite3 *db,                   /* 数据库需要重新密钥化 */
  const char *zDbName,           /* 数据库名称 */
  const void *pKey, int nKey     /* 新关键值 */
);

/*
** 指定SEE数据库的激活密钥。除非
** 激活后，所有SEE例程将无法工作。
*/
SQLITE_API void SQLITE_STDCALL sqlite3_activate_see(
  const char *zPassPhrase        /* Activation phrase */
);
#endif

#ifdef SQLITE_ENABLE_CEROD
/*
** 指定CEROD数据库的激活密钥。除非激活，否则CEROD的任何例程都无法工作。
*/
SQLITE_API void SQLITE_STDCALL sqlite3_activate_cerod(
  const char *zPassPhrase        /* 激活短语 */
);
#endif

/**
 * 暂停执行一段时间
 *
 * sqlite3_sleep() 函数会使当前线程暂停执行至少其参数指定数量的毫秒。
 *
 * 如果操作系统不支持具有毫秒时间分辨率的睡眠请求，则时间将四舍五入到最近的秒。
 * 实际向操作系统请求的睡眠毫秒数将被返回。
 *
 * ^SQLite 通过调用默认 [sqlite3_vfs] 对象的 xSleep() 方法来实现此接口。
 * 如果默认 VFS 的 xSleep() 方法没有正确实现，或者根本没有实现，那么 sqlite3_sleep() 的行为可能与前几段中的描述不同。
 */
SQLITE_API int SQLITE_STDCALL sqlite3_sleep(int);

/*
** CAPI3REF: 临时文件所在文件夹的名称
**
** ^(如果将此全局变量设置为指向一个字符串，该字符串是文件夹（也称为目录）的名称，
** 则在使用内置的 [sqlite3_vfs | VFS] 时，SQLite 创建的所有临时文件都将放置在该目录中。)^
** ^如果此变量是 NULL 指针，则 SQLite 会搜索适当的临时文件目录。^
**
** 强烈建议应用程序不要使用这个全局变量。
** 在 Windows Runtime (WinRT) 上需要设置临时文件夹。
** 但对于所有其他平台，强烈建议应用程序既不读取也不写入此变量。
** 这个全局变量是遗留应用程序向后兼容的遗物，应避免在新项目中使用。^
**
** 同时在多个线程中读取或修改此变量是不安全的。
** 如果在另一个线程中同时使用 [数据库连接]，则读取或修改此变量也是不安全的。
** 设计意图是在进程初始化时设置此变量一次，并在调用任何 SQLite 接口例程之前，
** 并且在此后保持此变量不变。^
**
** [temp_store_directory pragma] 可能会修改此变量，并使其指向从 [sqlite3_malloc] 获取的内存。^
** 此外，[temp_store_directory pragma] 总是假设此变量指向的任何字符串都是使用
** [sqlite3_malloc] 获取的内存，并且该 pragma 可能会尝试使用 [sqlite3_free] 释放该内存。^
** 因此，如果直接修改此变量，要么将其设置为 NULL，要么使其指向从 [sqlite3_malloc] 获取的内存，
** 否则应避免使用 [temp_store_directory pragma]。^
** 除非 [temp_store_directory pragma] 要求，否则 SQLite 不会释放 sqlite3_temp_directory 指向的内存。
** 如果应用程序希望释放该内存，必须自行操作，并注意只在所有 [数据库连接] 对象被销毁后才进行。^
**
** <b>Windows Runtime 用户注意：</b> 临时目录必须在调用 [sqlite3_open] 或 [sqlite3_open_v2] 之前设置。
** 否则，需要使用临时文件的各种功能可能会失败。以下是使用 C++ 和 Windows Runtime 设置临时目录的示例：^
**
** <blockquote><pre>
** LPCWSTR zPath = Windows::Storage::ApplicationData::Current->
** &nbsp;     TemporaryFolder->Path->Data();
** char zPathBuf[MAX_PATH + 1];
** memset(zPathBuf, 0, sizeof(zPathBuf));
** WideCharToMultiByte(CP_UTF8, 0, zPath, -1, zPathBuf, sizeof(zPathBuf),
** &nbsp;     NULL, NULL);
** sqlite3_temp_directory = sqlite3_mprintf("%s", zPathBuf);
** </pre></blockquote>
*/
SQLITE_API SQLITE_EXTERN char *sqlite3_temp_directory;

/*
** CAPI3REF: 数据库文件所在文件夹的名称
**
** ^(如果将此全局变量设置为指向一个表示文件夹（也称为目录）的字符串，
** 则当使用内置的windows [sqlite3_vfs | VFS]时，SQLite创建或访问的所有具有相对路径的数据库文件
** 都将被假定为相对于该目录。)^ ^如果此变量为NULL指针，
** 则SQLite假设所有具有相对路径的数据库文件相对于进程的当前目录。
** 只有windows VFS使用此全局变量；unix VFS会忽略它。
**
** 在数据库连接打开时更改此变量的值可能导致数据库损坏。
**
** 同时在多个线程中读取或修改此变量是不安全的。
** 如果在单独的线程中同时使用 [database connection]，则读取或修改此变量也是不安全的。
** 设计意图是在进程初始化期间设置此变量一次，
** 在调用任何SQLite接口例程之前，并且在此后保持此变量不变。
**
** ^[data_store_directory pragma] 可能会修改此变量并使其指向从 [sqlite3_malloc] 获取的内存。
** ^此外，[data_store_directory pragma] 总是假设此变量指向的任何字符串
** 都保存在从 [sqlite3_malloc] 获取的内存中，pragma 可能会尝试使用 [sqlite3_free] 释放该内存。
** 因此，如果直接修改此变量，要么将其设置为NULL，要么使其指向从 [sqlite3_malloc] 获取的内存，
** 否则应避免使用 [data_store_directory pragma]。
*/
SQLITE_API SQLITE_EXTERN char *sqlite3_data_directory;

/*
** CAPI3REF: 测试自动提交模式
** 关键词: {autocommit mode}
** 方法: sqlite3
**
** ^sqlite3_get_autocommit() 接口返回非零或零，
** 如果给定的数据库连接处于或未处于自动提交模式，则分别返回。
** ^默认情况下，自动提交模式是开启的。
** ^通过 [BEGIN] 语句可以禁用自动提交模式。
** ^通过 [COMMIT] 或 [ROLLBACK] 可以重新启用自动提交模式。
**
** 如果在多语句事务中的语句上发生某些类型的错误（错误包括 [SQLITE_FULL]、[SQLITE_IOERR]、
** [SQLITE_NOMEM]、[SQLITE_BUSY] 和 [SQLITE_INTERRUPT]），则事务可能会被自动回滚。
** 检查SQLite在错误后是否自动回滚事务的唯一方法是使用此函数。
**
** 如果在运行此例程时另一个线程更改了数据库连接的自动提交状态，
** 则返回值是未定义的。
*/
SQLITE_API int SQLITE_STDCALL sqlite3_get_autocommit(sqlite3*);

/**
 ** CAPI3REF: 获取准备语句的数据库句柄
 ** METHOD: sqlite3_stmt
 **
 ** ^sqlite3_db_handle 接口返回一个 [数据库连接] 句柄，该句柄属于一个 [准备语句]。 ^由 sqlite3_db_handle 返回的 [数据库连接] 句柄与最初用于创建该语句的 [sqlite3_prepare_v2()] 调用（或其变体）的第一个参数是相同的 [数据库连接]。
 */
SQLITE_API sqlite3 *SQLITE_STDCALL sqlite3_db_handle(sqlite3_stmt*);

/**
 ** CAPI3REF: 返回数据库连接的文件名
 ** METHOD: sqlite3
 **
 ** ^sqlite3_db_filename(D,N) 接口返回与连接 D 的数据库 N 关联的文件名的指针。 ^主数据库文件的名称为 "main"。 如果在数据库连接 D 上没有附加数据库 N，或者如果数据库 N 是临时数据库或内存数据库，则返回 NULL 指针。
 **
 ** ^此函数返回的文件名是 [VFS] 的 xFullPathname 方法的输出。 ^换句话说，即使最初用于打开数据库的文件名是 URI 或相对路径名，文件名也将是绝对路径名。
 */
SQLITE_API const char *SQLITE_STDCALL sqlite3_db_filename(sqlite3 *db, const char *zDbName);

/**
 ** CAPI3REF: 确定数据库是否为只读
 ** METHOD: sqlite3
 **
 ** ^sqlite3_db_readonly(D,N) 接口如果连接 D 的数据库 N 是只读的，则返回 1；如果是读/写，则返回 0；如果 N 不是连接 D 上的数据库的名称，则返回 -1。
 */
SQLITE_API int SQLITE_STDCALL sqlite3_db_readonly(sqlite3 *db, const char *zDbName);

/**
 ** CAPI3REF: 查找下一个准备语句
 ** METHOD: sqlite3
 **
 ** ^此接口返回与 [数据库连接] pDb 关联的 pStmt 后面的下一个 [准备语句] 的指针。 ^如果 pStmt 是 NULL，则此接口返回与数据库连接 pDb 关联的第一个准备语句的指针。 ^如果没有准备语句满足此例程的条件，则返回 NULL。
 **
 ** 在调用 [sqlite3_next_stmt(D,S)] 时，[数据库连接] 指针 D 必须引用一个打开的数据库连接，特别是不能是 NULL 指针。
 */

SQLITE_API sqlite3_stmt *SQLITE_STDCALL sqlite3_next_stmt(sqlite3 *pDb, sqlite3_stmt *pStmt);

/*
** CAPI3REF: 提交和回滚通知回调
** METHOD: sqlite3
**
** ^sqlite3_commit_hook() 接口注册一个回调函数，每当事务[COMMIT | 提交]时都会被调用。
** ^任何由之前的 sqlite3_commit_hook() 调用为同一数据库连接设置的回调都会被覆盖。
** ^sqlite3_rollback_hook() 接口注册一个回调函数，每当事务[ROLLBACK | 回滚]时都会被调用。
** ^任何由之前的 sqlite3_rollback_hook() 调用为同一数据库连接设置的回调都会被覆盖。
** ^pArg 参数会传递给回调函数。
** ^如果提交钩子函数的回调返回非零值，则提交会转换为回滚。
**
** ^sqlite3_commit_hook(D,C,P) 和 sqlite3_rollback_hook(D,C,P) 函数
** 返回上一次调用同一函数在同一 [数据库连接] D 时的 P 参数，或者对于 D 的每个函数的第一次调用返回 NULL。
**
** 提交和回滚钩子回调不是可重入的。
** 回调实现不能做任何修改调用回调的数据库连接的事情。
** 任何修改数据库连接的操作必须推迟到触发提交或回滚钩子的 [sqlite3_step()] 调用完成之后。
** 注意，运行任何其他 SQL 语句，包括 SELECT 语句，或者仅仅调用 [sqlite3_prepare_v2()] 和 [sqlite3_step()] 都会修改数据库连接，这里所说的“修改”是指本段中的含义。
**
** ^注册一个 NULL 函数可以禁用回调。
**
** ^当提交钩子回调例程返回零时，[COMMIT] 操作会正常继续。
** ^如果提交钩子返回非零，则 [COMMIT] 会转换为 [ROLLBACK]。
** ^回滚钩子会在由于提交钩子返回非零而导致的回滚时被调用，就像在任何其他回滚时一样。
**
** ^为了本 API 的目的，如果执行了显式的 "ROLLBACK" 语句，或者由于错误或约束导致隐式回滚，则认为事务已被回滚。
** ^如果事务因为数据库连接关闭而自动回滚，则不会调用回滚回调。
**
** 另请参阅 [sqlite3_update_hook()] 接口。
*/
SQLITE_API void *SQLITE_STDCALL sqlite3_commit_hook(sqlite3*, int(*)(void*), void*);
SQLITE_API void *SQLITE_STDCALL sqlite3_rollback_hook(sqlite3*, void(*)(void *), void*);

/**
 * \brief 数据变更通知回调
 * \method sqlite3
 *
 * sqlite3_update_hook() 接口通过第一个参数指定的 [数据库连接] 注册一个回调函数，
 * 当在 rowid 表中更新、插入或删除行时，将调用此回调函数。
 * 任何由之前的对此函数的调用为同一数据库连接设置的回调将被覆盖。
 *
 * 第二个参数是指向当在 rowid 表中更新、插入或删除行时将被调用的函数的指针。
 * 回调的第一个参数是 sqlite3_update_hook() 的第三个参数的副本。
 * 回调的第二个参数是 [SQLITE_INSERT]、[SQLITE_DELETE] 或 [SQLITE_UPDATE] 之一，
 * 取决于导致回调被调用的操作。
 * 回调的第三个和第四个参数包含指向包含受影响行的数据库和表名的指针。
 * 最后一个回调参数是行的 [rowid]。
 * 在更新的情况下，这是更新后的 [rowid]。
 *
 * 更新钩子不会在修改内部系统表时被调用（即 sqlite_master 和 sqlite_sequence）。
 * 更新钩子在修改 [WITHOUT ROWID] 表时也不会被调用。
 *
 * 在当前实现中，由于 [ON CONFLICT | ON CONFLICT REPLACE] 子句删除重复行时，
 * 更新钩子不会被调用。使用 [truncate optimization] 删除行时，更新钩子也不会被调用。
 * 本段中定义的例外可能会在 SQLite 的未来版本中更改。
 *
 * 更新钩子的实现不能做任何修改调用更新钩子的数据库连接的事情。
 * 任何修改数据库连接的操作必须推迟到触发更新钩子的 [sqlite3_step()] 调用完成后。
 * 注意，[sqlite3_prepare_v2()] 和 [sqlite3_step()] 都会修改它们的数据库连接，
 * 在本段中“修改”的意义上。
 *
 * sqlite3_update_hook(D,C,P) 函数返回同一 [数据库连接] D 上前一次调用的 P 参数，
 * 或者在 D 上的第一次调用时返回 NULL。
 *
 * 另见 [sqlite3_commit_hook()] 和 [sqlite3_rollback_hook()] 接口。
 */
SQLITE_API void *SQLITE_STDCALL sqlite3_update_hook(
  sqlite3*, 
  void(*)(void *,int ,char const *,char const *,sqlite3_int64),
  void*
);

/*
** CAPI3REF: 启用或禁用共享页缓存
**
** ^(如果参数为真，此例程将启用对同一数据库的[数据库连接 | 连接]之间的数据库缓存和模式数据结构的共享；
** 如果参数为假，则禁用共享。) ^
**
** ^从SQLite版本3.5.0开始，共享是针对整个进程启用或禁用的。
** 在SQLite的早期版本中，共享是单独为每个线程启用或禁用的，这是一个变化。
**
** ^(此接口设置的缓存共享模式会影响所有后续对[sqlite3_open()]、
** [sqlite3_open_v2()]和[sqlite3_open16()]的调用。
** 现有的数据库连接将继续使用在它们打开时有效的共享模式。) ^
**
** ^(如果共享缓存成功启用或禁用，此例程将返回[SQLITE_OK]。
** 否则将返回[错误代码]。)^
**
** ^默认情况下，共享缓存是禁用的，但这可能会在SQLite的未来版本中改变。
** 关心共享缓存设置的应用程序应显式设置它。
**
** 注意：此方法在MacOS X 10.7和iOS版本5.0上被禁用，并将始终返回SQLITE_MISUSE。
** 在这些系统上，应通过[sqlite3_open_v2()]与[SQLITE_OPEN_SHAREDCACHE]一起
** 为每个数据库连接启用共享缓存模式。
**
** 此接口在处理器上编写32位整数是原子的，因此是线程安全的。
**
** 另见：[SQLite共享缓存模式]
*/
SQLITE_API int SQLITE_STDCALL sqlite3_enable_shared_cache(int);

/**
** CAPI3REF: 尝试释放堆内存
**
** ^sqlite3_release_memory() 接口尝试通过释放数据库库持有的非必要内存分配来释放 N 字节的堆内存。
** 用于缓存数据库页面以提高性能的内存是非必要内存的示例。^sqlite3_release_memory() 返回实际释放的字节数，
** 该字节数可能比请求的多或少。^如果 SQLite 未编译带有 [SQLITE_ENABLE_MEMORY_MANAGEMENT] 选项，
** sqlite3_release_memory() 例程将是一个返回零的无操作。
**
** 另见：[sqlite3_db_release_memory()]
*/
SQLITE_API int SQLITE_STDCALL sqlite3_release_memory(int);

/**
** CAPI3REF: 释放数据库连接使用的内存
** METHOD: sqlite3
**
** ^sqlite3_db_release_memory(D) 接口尝试释放尽可能多的数据库连接 D 使用的堆内存。
** 与 [sqlite3_release_memory()] 接口不同，即使省略了 [SQLITE_ENABLE_MEMORY_MANAGEMENT] 编译时选项，
** 该接口也有效。
**
** 另见：[sqlite3_release_memory()]
*/
SQLITE_API int SQLITE_STDCALL sqlite3_db_release_memory(sqlite3*);

/**
 * 设置或查询SQLite可以分配的最大堆内存的软限制。
 * SQLite会通过减少页面缓存中持有的页面数量来努力保持堆内存使用量低于软限制。
 * 软限制是“软”的，因为即使SQLite努力保持在限制之下，它也会在生成[SQLITE_NOMEM]错误之前超过限制。
 * 换句话说，软堆限制仅供参考。
 *
 * sqlite3_soft_heap_limit64()的返回值是在调用之前的软堆限制的大小，或者在错误情况下为负数。
 * 如果参数N为负，则不对软堆限制进行更改。
 * 因此，可以通过使用负参数调用sqlite3_soft_heap_limit64()来确定当前软堆限制的大小。
 *
 * 如果参数N为零，则禁用软堆限制。
 *
 * 如果以下条件之一为真，则当前实现中不强制执行软堆限制：
 *
 * <ul>
 * <li> 软堆限制设置为零。
 * <li> 使用[sqlite3_config]([SQLITE_CONFIG_MEMSTATUS],...)启动选项和[SQLITE_DEFAULT_MEMSTATUS]编译时选项的组合禁用内存会计。
 * <li> 使用[sqlite3_config]([SQLITE_CONFIG_PCACHE2],...)指定替代页面缓存实现。
 * <li> 页面缓存从其自己的内存池分配，该内存池由[sqlite3_config]([SQLITE_CONFIG_PAGECACHE],...)提供，而不是从堆中分配。
 * </ul>
 *
 * 从SQLite版本3.7.3开始，无论是否调用[SQLITE_ENABLE_MEMORY_MANAGEMENT]编译时选项，都会强制执行软堆限制。
 * 使用[SQLITE_ENABLE_MEMORY_MANAGEMENT]时，每次内存分配都会强制执行软堆限制。
 * 不使用[SQLITE_ENABLE_MEMORY_MANAGEMENT]时，只有在页面缓存分配内存时才会强制执行软堆限制。
 * 测试表明，由于页面缓存是SQLite中最主要的内存使用者，大多数应用程序可以在不使用[SQLITE_ENABLE_MEMORY_MANAGEMENT]的情况下实现足够的软堆限制强制执行。
 *
 * SQLite将在未来版本中更改强制执行软堆限制的情况。
 */
SQLITE_API sqlite3_int64 SQLITE_STDCALL sqlite3_soft_heap_limit64(sqlite3_int64 N);

/*
** CAPI3REF: 已弃用的软堆限制接口
** 已弃用
**
** 这是 [sqlite3_soft_heap_limit64()] 接口的已弃用版本。
** 此例程仅提供历史兼容性。所有新应用程序应使用
** [sqlite3_soft_heap_limit64()] 接口，而不是这个。
*/
SQLITE_API SQLITE_DEPRECATED void SQLITE_STDCALL sqlite3_soft_heap_limit(int N);


/**
 ** CAPI3REF: 提取关于表列的元数据
 ** METHOD: sqlite3
 **
 ** ^(sqlite3_table_column_metadata(X,D,T,C,....) 函数返回数据库 D 中表 T 的列 C 的信息
 ** 在 [数据库连接] X 中。) ^ 如果指定的列存在，sqlite3_table_column_metadata() 接口将返回 SQLITE_OK 并用适当的值填充最后五个参数中的非空指针。
 ** 如果指定的列不存在，sqlite3_table_column_metadata() 接口将返回 SQLITE_ERROR。
 ** 如果传递给 sqlite3_table_column_metadata() 的列名参数是 NULL 指针，则此例程仅检查表是否存在，如果表存在则返回 SQLITE_OK，如果不存在则返回 SQLITE_ERROR。
 **
 ** ^列由该函数的第二个、第三个和第四个参数标识。^(第二个参数是包含指定表的数据库的名称（即 "main"、"temp" 或附加的数据库）或 NULL。) ^ 如果是 NULL，则使用数据库引擎解析未限定表引用的相同算法在所有附加的数据库中搜索表。
 **
 ** ^该函数的第三个和第四个参数分别是所需列的表和列名。
 **
 ** ^元数据通过写入作为此函数的第 5 个及后续参数传递的内存位置返回。 ^这些参数中的任何一个都可能是 NULL，在这种情况下，相应的元数据元素将被省略。
 **
 ** ^(<blockquote>
 ** <table border="1">
 ** <tr><th> 参数 <th> 输出<br>类型 <th>  描述
 **
 ** <tr><td> 第 5 个 <td> const char* <td> 数据类型
 ** <tr><td> 第 6 个 <td> const char* <td> 默认的排序序列名称
 ** <tr><td> 第 7 个 <td> int         <td> 如果列有 NOT NULL 约束则为真
 ** <tr><td> 第 8 个 <td> int         <td> 如果列是 PRIMARY KEY 的一部分则为真
 ** <tr><td> 第 9 个 <td> int         <td> 如果列是 [AUTOINCREMENT] 则为真
 ** </table>
 ** </blockquote>)^
 **
 ** ^对于声明类型和排序序列返回的字符指针所指向的内存，直到下一次调用任何 SQLite API 函数之前都有效。
 **
 ** ^如果指定的表实际上是一个视图，则返回一个 [错误代码]。
 **
 ** ^如果指定的列是 "rowid"、"oid" 或 "_rowid_"，并且表不是 [WITHOUT ROWID] 表，并且已明确声明了 [INTEGER PRIMARY KEY] 列，则输出参数设置为明确声明的列。 ^(如果没有 [INTEGER PRIMARY KEY] 列，则 [rowid] 的输出设置为如下：
 **
 ** <pre>
 **     数据类型: "INTEGER"
 **     排序序列: "BINARY"
 **     not null: 0
 **     primary key: 1
 **     auto increment: 0
 ** </pre>)^
 **
 ** ^此函数会导致所有数据库模式从磁盘读取并解析（如果尚未完成），如果在加载模式时遇到任何错误，则返回错误。
 */
SQLITE_API int SQLITE_STDCALL sqlite3_table_column_metadata(
	sqlite3 *db,                /* 连接句柄 */
	const char *zDbName,        /* 数据库名称或 NULL */
	const char *zTableName,     /* 表名称 */
	const char *zColumnName,    /* 列名称 */
	char const **pzDataType,    /* 输出：声明的数据类型 */
	char const **pzCollSeq,     /* 输出：排序规则名称 */
	int *pNotNull,              /* 输出：如果存在 NOT NULL 约束则为真 */
	int *pPrimaryKey,           /* 输出：如果列是主键的一部分则为真 */
	int *pAutoinc               /* 输出：如果列是自增列则为真 */
);

/**
 * CAPI3REF: 加载扩展
 * 方法: sqlite3
 *
 * 这个接口从指定的文件中加载一个SQLite扩展库。
 *
 * sqlite3_load_extension() 接口尝试从文件 zFile 中加载一个
 * [SQLite扩展] 库。如果文件不能直接加载，会尝试添加各种操作系统
 * 特定的扩展名进行加载。例如，如果无法加载 "samplelib"，则会尝试
 * 加载 "samplelib.so"、"samplelib.dylib" 或 "samplelib.dll" 等名称。
 *
 * 入口点是 zProc。
 * zProc 可能为 0，此时 SQLite 会尝试自行确定一个入口点名称。
 * 它首先尝试 "sqlite3_extension_init"。如果这不起作用，它会构建一个
 * 名称 "sqlite3_X_init"，其中 X 由文件名中从最后一个 "/" 到第一个
 * 后面的 "." 之间的所有 ASCII 字母字符的小写等效字符组成，并省略任何
 * 开头的 "lib"。
 *
 * sqlite3_load_extension() 接口在成功时返回 [SQLITE_OK]，如果出现错误则返回
 * [SQLITE_ERROR]。如果发生错误且 pzErrMsg 不为 0，则
 * [sqlite3_load_extension()] 接口将尝试使用从 [sqlite3_malloc()] 获得的
 * 内存中的错误消息文本填充 *pzErrMsg。调用函数应通过调用
 * [sqlite3_free()] 释放此内存。
 *
 * 在调用此 API 之前，必须使用 [sqlite3_enable_load_extension()]
 * 启用扩展加载，否则将返回错误。
 *
 * 另请参阅 [load_extension() SQL 函数]。
 */
SQLITE_API int SQLITE_STDCALL sqlite3_load_extension(
  sqlite3 *db,          /* 将扩展加载到此数据库连接中 */
  const char *zFile,    /* 共享库的名称，包含扩展名 */
  const char *zProc,    /* 入口点。如果为0，则派生自zFile */
  char **pzErrMsg       /* 将错误信息放在这里，如果不是0 */

);

/*
** CAPI3REF: 启用或禁用扩展加载
** 方法: sqlite3
**
** ^为了不在未准备好处理[扩展加载]的旧应用程序中打开安全漏洞，
** 并作为在评估用户输入的SQL时禁用[扩展加载]的手段，提供了以下API
** 来打开和关闭[sqlite3_load_extension()]机制。
**
** ^默认情况下，扩展加载是禁用的。
** ^调用sqlite3_enable_load_extension()例程并将onoff==1
** 来打开扩展加载，用onoff==0来关闭它。
*/
SQLITE_API int SQLITE_STDCALL sqlite3_enable_load_extension(sqlite3 *db, int onoff);

/*
** CAPI3REF: 自动加载静态链接扩展
**
** ^此接口导致每次创建新的[数据库连接]时都会调用xEntryPoint()函数。这里的思想是，
** xEntryPoint()是一个静态链接的[SQLite扩展]的入口点，该扩展将自动加载到所有新的数据库连接中。
**
** ^(尽管函数原型显示xEntryPoint()不接受任何参数并且返回void，但SQLite会带着三个
** 参数调用xEntryPoint()，并期望得到一个整数结果，就好像入口点的签名如下：
**
** <blockquote><pre>
** &nbsp;  int xEntryPoint(
** &nbsp;    sqlite3 *db,
** &nbsp;    const char **pzErrMsg,
** &nbsp;    const struct sqlite3_api_routines *pThunk
** &nbsp;  );
** </pre></blockquote>)^
**
** 如果xEntryPoint例程遇到错误，它应该使*pzErrMsg指向适当的错误消息（从[sqlite3_mprintf()]获得）
** 并返回适当的[错误代码]。^SQLite确保在调用xEntryPoint()之前*pzErrMsg为NULL。^SQLite将在
** xEntryPoint()返回后对*pzErrMsg调用[sqlite3_free()]。^如果任何xEntryPoint()返回错误，
** 触发xEntryPoint()的[sqlite3_open()]、[sqlite3_open16()]或[sqlite3_open_v2()]调用将失败。
**
** ^使用已经存在于自动扩展列表中的入口点X调用sqlite3_auto_extension(X)是无害的空操作。
** ^每个打开的数据库连接不会调用任何入口点超过一次。
**
** 另见：[sqlite3_reset_auto_extension()]
** 和 [sqlite3_cancel_auto_extension()]
*/
SQLITE_API int SQLITE_STDCALL sqlite3_auto_extension(void (*xEntryPoint)(void));

/*
** CAPI3REF: 取消自动扩展加载
**
** ^[sqlite3_cancel_auto_extension(X)] 接口用于注销之前通过
** [sqlite3_auto_extension(X)] 调用注册的初始化例程 X。^
** [sqlite3_cancel_auto_extension(X)] 例程在成功注销初始化例程 X 时返回 1，
** 如果 X 不在初始化例程列表中，则返回 0。
*/
SQLITE_API int SQLITE_STDCALL sqlite3_cancel_auto_extension(void (*xEntryPoint)(void));

/*
** CAPI3REF: 重置自动扩展加载
**
** ^此接口禁用所有之前使用 [sqlite3_auto_extension()] 注册的自动扩展。
*/
SQLITE_API void SQLITE_STDCALL sqlite3_reset_auto_extension(void);

/*
** 虚拟表机制的接口目前被认为是实验性的。接口可能会以不兼容的方式进行更改。
** 如果这对您构成问题，请勿在此时期使用该接口。
**
** 当虚拟表机制稳定时，我们将声明接口固定，无限期支持，并删除此注释。
*/

/*
** 虚拟表接口使用的结构
*/
typedef struct sqlite3_vtab sqlite3_vtab;
typedef struct sqlite3_index_info sqlite3_index_info;
typedef struct sqlite3_vtab_cursor sqlite3_vtab_cursor;
typedef struct sqlite3_module sqlite3_module;

/*
** CAPI3REF: 虚拟表对象
** 关键词: sqlite3_module {虚拟表模块}
**
** 这个结构，有时被称为“虚拟表模块”，
** 定义了[虚拟表]的实现。  
** 这个结构主要由模块的方法组成。
**
** ^通过填充这个结构的持久实例并将指向该实例的指针传递给
** [sqlite3_create_module()] 或 [sqlite3_create_module_v2()]来创建一个虚拟表模块。
** ^注册在被不同的模块替换或直到[数据库连接]关闭之前一直有效。
** 在它与任何数据库连接注册期间，该结构的内容不能更改。
*/
struct sqlite3_module {
  int iVersion;
  int (*xCreate)(sqlite3*, void *pAux,
			   int argc, const char *const*argv,
			   sqlite3_vtab **ppVTab, char**);
  int (*xConnect)(sqlite3*, void *pAux,
			   int argc, const char *const*argv,
			   sqlite3_vtab **ppVTab, char**);
  int (*xBestIndex)(sqlite3_vtab *pVTab, sqlite3_index_info*);
  int (*xDisconnect)(sqlite3_vtab *pVTab);
  int (*xDestroy)(sqlite3_vtab *pVTab);
  int (*xOpen)(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor);
  int (*xClose)(sqlite3_vtab_cursor*);
  int (*xFilter)(sqlite3_vtab_cursor*, int idxNum, const char *idxStr,
				int argc, sqlite3_value **argv);
  int (*xNext)(sqlite3_vtab_cursor*);
  int (*xEof)(sqlite3_vtab_cursor*);
  int (*xColumn)(sqlite3_vtab_cursor*, sqlite3_context*, int);
  int (*xRowid)(sqlite3_vtab_cursor*, sqlite3_int64 *pRowid);
  int (*xUpdate)(sqlite3_vtab *, int, sqlite3_value **, sqlite3_int64 *);
  int (*xBegin)(sqlite3_vtab *pVTab);
  int (*xSync)(sqlite3_vtab *pVTab);
  int (*xCommit)(sqlite3_vtab *pVTab);
  int (*xRollback)(sqlite3_vtab *pVTab);
  int (*xFindFunction)(sqlite3_vtab *pVtab, int nArg, const char *zName,
					   void (**pxFunc)(sqlite3_context*,int,sqlite3_value**),
					   void **ppArg);
  int (*xRename)(sqlite3_vtab *pVtab, const char *zNew);
  /* 上面的方法属于 sqlite_module 对象的版本1。
  ** 下面的方法适用于版本2及更高版本。 */
  int (*xSavepoint)(sqlite3_vtab *pVTab, int);
  int (*xRelease)(sqlite3_vtab *pVTab, int);
  int (*xRollbackTo)(sqlite3_vtab *pVTab, int);
};

/*
** CAPI3REF: 虚拟表索引信息
** 关键词: sqlite3_index_info
**
** sqlite3_index_info 结构及其子结构是作为 [虚拟表] 接口的一部分使用的，
** 用于向 [xBestIndex] 方法传递信息并接收其回复。**输入**下的字段是 xBestIndex 的输入，
** 只读。xBestIndex 将其结果插入到 **输出** 字段中。
**
** ^(aConstraint[] 数组记录了形式为：
**
** <blockquote>column OP expr</blockquote>
**
** 的 WHERE 子句约束，其中 OP 是 =, <, <=, >, 或 >=。)^(特定的运算符存储在 aConstraint[].op 中，
** 使用 [SQLITE_INDEX_CONSTRAINT_EQ | SQLITE_INDEX_CONSTRAINT_ 值] 之一。)^(列的索引存储在
** aConstraint[].iColumn 中。)^(如果右侧的 expr 可以计算（因此约束是可用的），则 aConstraint[].usable 为 TRUE；
** 如果不可计算，则为 FALSE。) ^
**
** ^优化器会自动反转形式为 "expr OP column" 的项，并对 WHERE 子句进行其他简化，
** 以尽可能多地将 WHERE 子句项转换为上述形式。^aConstraint[] 数组仅报告与正在查询的特定虚拟表相关的
** WHERE 子句项。
**
** ^关于 ORDER BY 子句的信息存储在 aOrderBy[] 中。^aOrderBy 中的每个项记录 ORDER BY 子句中的一列。
**
** [xBestIndex] 方法必须填充 aConstraintUsage[]，提供有关向 xFilter 传递哪些参数的信息。^如果 argvIndex>0，
** 则对应的 aConstraint[] 的右侧会被计算，并成为 argv 中的 argvIndex-th 项。^(如果 aConstraintUsage[].omit 为真，
** 则假定约束完全由虚拟表处理，SQLite 不会再次检查该约束。) ^
**
** ^idxNum 和 idxPtr 值被记录并传递给 [xFilter] 方法。^只有在 needToFreeIdxPtr 为真时，
** 才使用 [sqlite3_free()] 释放 idxPtr。
**
** ^orderByConsumed 表示 [xFilter]/[xNext] 的输出将以满足 ORDER BY 子句的正确顺序出现，
** 因此不需要单独的排序步骤。
**
** ^estimatedCost 值是对特定策略成本的估计。成本为 N 表示该策略的成本类似于对具有 N 行的 SQLite 表进行线性扫描的成本。
** 成本为 log(N) 表示操作的费用类似于对具有 N 行且具有唯一索引字段的 SQLite 表进行二分搜索的费用。
**
** ^estimatedRows 值是对策略将返回的行数的估计。
**
** 重要: estimatedRows 字段是在 SQLite 版本 3.8.2 中添加到 sqlite3_index_info 结构中的。
** 如果虚拟表扩展用于早于 3.8.2 的 SQLite 版本，尝试读取或写入 estimatedRows 字段的 结果是未定义的（但可能会使应用程序崩溃）。
** 因此，只有在 [sqlite3_libversion_number()] 返回大于或等于 3008002 的值时，才应使用 estimatedRows 字段。
*/
struct sqlite3_index_info {
	/* 输入 */
	int nConstraint;           /* aConstraint 中的条目数量 */
	struct sqlite3_index_constraint {
		int iColumn;              /* 约束左侧的列 */
		unsigned char op;         /* 约束操作符 */
		unsigned char usable;     /* 如果此约束可用则为真 */
		int iTermOffset;          /* 内部使用 - xBestIndex 应忽略 */
	} *aConstraint;            /* WHERE 子句约束表 */
	int nOrderBy;              /* ORDER BY 子句中的项数 */
	struct sqlite3_index_orderby {
		int iColumn;              /* 列号 */
		unsigned char desc;       /* 如果为 DESC 则为真，如果为 ASC 则为假 */
	} *aOrderBy;               /* ORDER BY 子句 */
	/* 输出 */
	struct sqlite3_index_constraint_usage {
		int argvIndex;           /* 如果 >0，约束是 xFilter 的 argv 的一部分 */
		unsigned char omit;      /* 不要为此约束编写测试代码 */
	} *aConstraintUsage;
	int idxNum;                /* 用于标识索引的编号 */
	char *idxStr;              /* 字符串，可能从 sqlite3_malloc 获取 */
	int needToFreeIdxStr;      /* 如果为真，则使用 sqlite3_free() 释放 idxStr */
	int orderByConsumed;       /* 如果输出已排序则为真 */
	double estimatedCost;           /* 使用此索引的估计成本 */
	/* 以下字段仅在 SQLite 3.8.2 及更高版本中可用 */
	sqlite3_int64 estimatedRows;    /* 返回的估计行数 */
};

/*
** CAPI3REF: 虚拟表约束操作符代码
**
** 这些宏定义了[sqlite3_index_info].aConstraint[].op字段允许的值。
** 每个值代表一个操作符，该操作符是使用[虚拟表]的查询的WHERE子句中约束项的一部分。
*/
#define SQLITE_INDEX_CONSTRAINT_EQ    2
#define SQLITE_INDEX_CONSTRAINT_GT    4
#define SQLITE_INDEX_CONSTRAINT_LE    8
#define SQLITE_INDEX_CONSTRAINT_LT    16
#define SQLITE_INDEX_CONSTRAINT_GE    32
#define SQLITE_INDEX_CONSTRAINT_MATCH 64

/**
** CAPI3REF: 注册虚拟表实现
** METHOD: sqlite3
**
** 这些例程用于注册新的[虚拟表模块]名称。
** 模块名称必须在创建使用该模块的新[虚拟表]之前以及在使用模块的现有[虚拟表]之前进行注册。
**
** 模块名称在由第一个参数指定的[数据库连接]上注册。
** 模块的名称由第二个参数给出。
** 第三个参数是指向[虚拟表模块]实现的指针。
** 第四个参数是一个任意的客户端数据指针，当创建或重新初始化新的虚拟表时，
** 该指针会传递到虚拟表模块的[xCreate]和[xConnect]方法中。
**
** sqlite3_create_module_v2()接口有一个第五个参数，
** 它是指向pClientData的析构函数的指针。
** 当SQLite不再需要pClientData指针时（如果它不为NULL），SQLite会调用析构函数。
** 如果调用sqlite3_create_module_v2()失败，也会调用析构函数。
** sqlite3_create_module()接口相当于带有NULL析构函数的sqlite3_create_module_v2()。
*/
SQLITE_API int SQLITE_STDCALL sqlite3_create_module(
  sqlite3 *db,               /* 连接到 SQLite 数据库以注册模块 */

  const char *zName,         // 模块的名称

  const sqlite3_module *p,   /* 模块的方法 */

  void *pClientData          /* 客户端数据用于 xCreate/xConnect */

);
SQLITE_API int SQLITE_STDCALL sqlite3_create_module_v2(
  sqlite3 *db,               /* 连接到 SQLite 数据库以注册模块 */
  const char *zName,         /* 模块名称 */
  const sqlite3_module *p,   /* 模块的方法 */
  void *pClientData,         /* 客户端数据用于 xCreate/xConnect */
  void(*xDestroy)(void*)     /* 模块析构函数 */
);

/*
** CAPI3REF: 虚拟表实例对象
** 关键词: sqlite3_vtab
**
** 每个[虚拟表模块]实现都使用这个对象的子类
** 来描述一个特定的[虚拟表]实例。
** 每个子类都将根据模块实现的具体需求进行定制。
** 这个超类的目的是定义所有模块实现共有的某些字段。
**
** ^虚拟表方法可以通过将来自[sqlite3_mprintf()]的字符串
** 赋值给zErrMsg来设置错误消息。方法应确保
** 在为zErrMsg分配新字符串之前，通过调用[sqlite3_free()]
** 释放任何先前的字符串。^在错误消息传递给客户端应用程序后，
** 该字符串将由sqlite3_free()自动释放，并且zErrMsg字段将被清零。
*/
struct sqlite3_vtab {
  const sqlite3_module* pModule;  /* 此虚拟表的模块 */
  int nRef;                       /* 打开的光标数量 */
  char* zErrMsg;                  /* 来自sqlite3_mprintf()的错误消息 */
  /* 虚拟表实现通常会添加额外的字段 */
};

/*
** CAPI3REF: 虚拟表游标对象
** 关键词: sqlite3_vtab_cursor {虚拟表游标}
**
** 每个虚拟表模块实现都使用以下结构的子类来描述指向虚拟表并用于遍历虚拟表的游标。
** 游标是使用模块的 [sqlite3_module.xOpen | xOpen] 方法创建的，并通过 [sqlite3_module.xClose | xClose] 方法销毁。
** 游标由模块的 [xFilter]、[xNext]、[xEof]、[xColumn] 和 [xRowid] 方法使用。
** 每个模块实现将根据其自身需求定义游标结构的内容。
**
** 这个超类存在是为了定义所有实现中游标共有的字段。
*/
struct sqlite3_vtab_cursor {
  sqlite3_vtab *pVtab;      /* 虚拟表的游标 */
  /* 虚拟表实现通常会添加额外的字段 */
};

/*
** CAPI3REF: 声明虚拟表的架构
**
** ^虚拟表模块的 [xCreate] 和 [xConnect] 方法调用此接口
** 来声明它们实现的虚拟表的格式（列的名称和数据类型）。
*/
SQLITE_API int SQLITE_STDCALL sqlite3_declare_vtab(sqlite3*, const char *zSQL);

/*
** CAPI3REF: 为虚拟表重载函数
** 方法: sqlite3
**
** ^虚拟表可以使用[virtual table module]的[xFindFunction]方法提供函数的替代实现。^
** ^但是，这些函数的全局版本必须存在才能被重载。^
**
** ^此API确保具有特定名称和参数数量的全局函数存在。^ ^如果在此API调用之前不存在这样的函数，则会创建一个新函数。^ ^新函数的实现总是会导致抛出异常。^
** 所以，新函数本身没有任何用处。^它的唯一目的是作为一个占位符函数，可以被[virtual table]重载。
*/
SQLITE_API int SQLITE_STDCALL sqlite3_overload_function(sqlite3*, const char *zFuncName, int nArg);

/*
** 上文定义的虚拟表机制的接口（回到与这个注释非常相似的注释）目前被认为是实验性的。
** 接口可能会以不兼容的方式进行更改。
** 如果这对您构成问题，请勿在此时期使用该接口。
**
** 当虚拟表机制稳定时，我们将声明接口固定，无限期支持，并删除此注释。
*/

/*
** CAPI3REF: 一个打开的BLOB的句柄
** 关键词：{BLOB句柄} {BLOB句柄}
**
** 此对象的实例代表一个可以执行[sqlite3_blob_open | 增量BLOB I/O]的打开的BLOB。
** ^此类对象由[sqlite3_blob_open()]创建，并由[sqlite3_blob_close()]销毁。
** ^可以使用[sqlite3_blob_read()]和[sqlite3_blob_write()]接口来读取或写入BLOB的小部分。
** ^[sqlite3_blob_bytes()]接口返回BLOB的字节大小。
*/
typedef struct sqlite3_blob sqlite3_blob;

/*
** CAPI3REF: 打开一个BLOB以进行增量I/O
** 方法: sqlite3
** 构造函数: sqlite3_blob
**
** ^(此接口打开位于数据库zDb中的表zTable的第iRow行、第zColumn列的BLOB的[BLOB句柄 | 句柄]；
** 换句话说，这是通过以下查询选择的相同BLOB：
**
** <pre>
**     SELECT zColumn FROM zDb.zTable WHERE [rowid] = iRow;
** </pre>)^
**
** ^(参数zDb不是包含数据库的文件名，而是数据库的符号名称。
** 对于附加的数据库，这是在[ATTACH]语句中出现AS关键字后的名称。
** 对于主数据库文件，数据库名称为"main"。对于TEMP表，数据库名称为"temp"。)^
**
** ^如果flags参数非零，则BLOB以读写访问方式打开。^如果flags参数为零，则BLOB以只读访问方式打开。
**
** ^(成功时，返回[SQLITE_OK]，新的[BLOB句柄]存储在*ppBlob中。
** 否则返回一个[错误代码]，除非错误代码是SQLITE_MISUSE，否则*ppBlob被设置为NULL。) ^
** 这意味着，只要API未被误用，在函数返回后调用[sqlite3_blob_close()]对*ppBlob始终是安全的。
**
** 如果以下任何一项为真，此函数将失败并返回SQLITE_ERROR：
** <ul>
**   <li> ^数据库zDb不存在^, 
**   <li> ^数据库zDb中不存在表zTable^, 
**   <li> ^表zTable是一个WITHOUT ROWID表^, 
**   <li> ^列zColumn不存在^,
**   <li> ^表中的行iRow不存在^,
**   <li> ^指定列的行iRow包含的值不是TEXT或BLOB值^,
**   <li> ^列zColumn是索引、PRIMARY KEY或UNIQUE约束的一部分，并且BLOB以读写访问方式打开^,
**   <li> ^([外键约束 | 外键约束]已启用，列zColumn是[子键]定义的一部分，并且BLOB以读写访问方式打开)^。
** </ul>
**
** ^除非返回SQLITE_MISUSE，否则此函数设置[数据库连接]错误代码和消息，可通过
** [sqlite3_errcode()]和[sqlite3_errmsg()]及相关函数访问。
**
** ^(如果BLOB句柄指向的行被[UPDATE]、[DELETE]或[ON CONFLICT]副作用修改，
** 则BLOB句柄被标记为“已过期”。
** 即使修改的是BLOB句柄未打开的列，只要行的任何列被更改，这也是正确的。) ^
** ^对已过期BLOB句柄的[sqlite3_blob_read()]和[sqlite3_blob_write()]调用将失败，返回代码为[SQLITE_ABORT]。
** ^(在BLOB过期之前写入BLOB的更改不会被BLOB的过期回滚。
** 如果事务继续完成，这些更改最终将提交。) ^
**
** ^使用[sqlite3_blob_bytes()]接口确定打开的blob的大小。^此接口不能更改blob的大小。
** 使用[UPDATE] SQL命令更改blob的大小。
**
** ^可以使用[sqlite3_bind_zeroblob()]和[sqlite3_result_zeroblob()]接口以及内置的[zeroblob] SQL函数创建一个
** 用零填充的blob，以使用增量blob接口进行读取或写入。
**
** 避免资源泄漏，每个打开的[BLOB句柄]最终都应该通过调用[sqlite3_blob_close()]释放。
*/
SQLITE_API int SQLITE_STDCALL sqlite3_blob_open(
  sqlite3*,
  const char *zDb,
  const char *zTable,
  const char *zColumn,
  sqlite3_int64 iRow,
  int flags,
  sqlite3_blob **ppBlob
);

/*
** CAPI3REF: 将 BLOB 句柄移动到新行
** METHOD: sqlite3_blob
**
** ^此函数用于移动现有的 BLOB 句柄，使其指向同一数据库表中的不同行。^新行由作为第二个参数传递的 rowid 值标识。
** 只能更改行。^BLOB 句柄打开的数据库、表和列保持不变。将现有的 BLOB 句柄移动到新行可能比关闭现有句柄并打开一个新句柄更快。
**
** ^（新行必须满足与 [sqlite3_blob_open()] 相同的条件 - 它必须存在，并且在指定的列中必须存储有 BLOB 或文本值。）^如果新行不在表中，或者它不包含 BLOB 或文本值，或者发生其他错误，
** 则返回 SQLite 错误代码，并且 BLOB 句柄被视为中止。
** ^在中止的 BLOB 句柄上对 [sqlite3_blob_read()]、[sqlite3_blob_write()] 或 [sqlite3_blob_reopen()] 的所有后续调用
** 都会立即返回 SQLITE_ABORT。^在中止的 BLOB 句柄上调用 [sqlite3_blob_bytes()] 始终返回零。
**
** ^此函数设置数据库句柄的错误代码和消息。
*/
SQLITE_API int SQLITE_STDCALL sqlite3_blob_reopen(sqlite3_blob *, sqlite3_int64);

/*
** CAPI3REF: 关闭 BLOB 句柄
** DESTRUCTOR: sqlite3_blob
**
** ^此函数关闭一个打开的 [BLOB 句柄]。^(该 BLOB 句柄会被无条件关闭。
** 即使此例程返回错误代码，句柄仍然会被关闭。) ^
**
** ^如果正在关闭的 blob 句柄是以读/写访问方式打开的，并且如果数据库处于自动提交模式，
** 并且没有其他打开的读/写 blob 句柄或活动的写语句，则当前事务将被提交。 ^如果在提交事务时发生错误，
** 将返回错误代码并回滚事务。 ^
**
** 使用不是 NULL 指针或打开的 blob 句柄作为参数调用此函数会导致未定义的行为。
** ^使用空指针（如 [sqlite3_blob_open()] 调用失败时返回的）调用此例程是安全的无操作。 ^
** 否则，如果此函数传递了一个有效的打开的 blob 句柄，在返回之前，sqlite3_errcode() 和 sqlite3_errmsg() 函数返回的值将被设置。
*/
SQLITE_API int SQLITE_STDCALL sqlite3_blob_close(sqlite3_blob *);

/*
** CAPI3REF: 返回打开的BLOB的大小
** METHOD: sqlite3_blob
**
** ^返回通过其唯一参数成功打开的[BLOB句柄]可访问的BLOB的字节大小。^
** 增量BLOB I/O例程只能读取或覆盖现有的BLOB内容；它们不能改变BLOB的大小。
**
** 此例程仅适用于通过之前的成功调用[sqlite3_blob_open()]创建的
** [BLOB句柄]，并且尚未通过[sqlite3_blob_close()]关闭。传递任何其他
** 指针到此例程会导致未定义且可能不希望的行为。
*/
SQLITE_API int SQLITE_STDCALL sqlite3_blob_bytes(sqlite3_blob *);

/*
** CAPI3REF: 从BLOB中逐步读取数据
** 方法: sqlite3_blob
**
** ^(此函数用于从打开的[BLOB句柄]中读取数据到调用者提供的缓冲区中。
** 从打开的BLOB中复制N个字节的数据到缓冲区Z，从偏移量iOffset开始。) ^
**
** ^如果偏移量iOffset小于BLOB末尾的N个字节，则返回[SQLITE_ERROR]，且不读取任何数据。
** ^如果N或iOffset小于零，则返回[SQLITE_ERROR]，且不读取任何数据。
** ^可以使用[sqlite3_blob_bytes()]接口确定BLOB的大小（因此N+iOffset的最大值）。
**
** ^尝试从已过期的[BLOB句柄]读取数据将导致错误代码为[SQLITE_ABORT]。
**
** ^(成功时，sqlite3_blob_read()返回SQLITE_OK。
** 否则，返回一个[错误代码]或一个[扩展错误代码]。)^
**
** 此例程仅在由之前的成功调用[sqlite3_blob_open()]创建的
** [BLOB句柄]上工作，并且尚未通过[sqlite3_blob_close()]关闭。
** 将任何其他指针传递给此例程将导致未定义且可能不希望的行为。
**
** 另见: [sqlite3_blob_write()]。
*/
SQLITE_API int SQLITE_STDCALL sqlite3_blob_read(sqlite3_blob *, void *Z, int N, int iOffset);

/**
** CAPI3REF: 分块写入数据到BLOB
** 方法: sqlite3_blob
**
** ^此函数用于从调用者提供的缓冲区向打开的[BLOB句柄]写入数据。N字节的数据从缓冲区Z复制到打开的BLOB中，从偏移量iOffset开始。^
**
** ^成功时，sqlite3_blob_write()返回SQLITE_OK。
** 否则，返回一个[错误代码]或[扩展错误代码]。^除非返回SQLITE_MISUSE，否则此函数会设置
** [数据库连接]错误代码和消息，可以通过[sqlite3_errcode()]和[sqlite3_errmsg()]及相关函数访问。 
**
** ^如果作为第一个参数传递的[BLOB句柄]没有以写入模式打开（[sqlite3_blob_open()]的标志参数为零），
** 此函数返回[SQLITE_READONLY]。^
**
** 此函数只能修改BLOB的内容；使用此API无法增加BLOB的大小。^如果偏移量iOffset距离BLOB末尾小于N字节，
** 返回[SQLITE_ERROR]，且不写入任何数据。BLOB的大小（因此N+iOffset的最大值）可以使用
** [sqlite3_blob_bytes()]接口确定。^如果N或iOffset小于零，返回[SQLITE_ERROR]，且不写入任何数据。^
**
** 尝试向已过期的[BLOB句柄]写入数据将失败，错误代码为[SQLITE_ABORT]。^在[BLOB句柄]过期之前进行的写入
** 不会因为句柄的过期而被回滚，尽管这些更改可能已被过期BLOB句柄的语句或其他独立语句覆盖。^
**
** 此例程仅在由之前的成功调用[sqlite3_blob_open()]创建且未被[sqlite3_blob_close()]关闭的
** [BLOB句柄]上工作。向此例程传递任何其他指针将导致未定义且可能不希望的行为。^
**
** 另见: [sqlite3_blob_read()]。
*/
SQLITE_API int SQLITE_STDCALL sqlite3_blob_write(sqlite3_blob *, const void *z, int n, int iOffset);

/*
** CAPI3REF: 虚拟文件系统对象
**
** 虚拟文件系统（VFS）是一个 [sqlite3_vfs] 对象，
** SQLite 使用它与底层操作系统进行交互。
** 大多数 SQLite 构建都带有一个适用于主机计算机的默认 VFS。
** 可以注册新的 VFS，也可以注销现有的 VFS。
** 提供了以下接口。
**
** ^sqlite3_vfs_find() 接口根据名称返回一个 VFS 的指针。
** ^名称区分大小写。
** ^名称是零终止的 UTF-8 字符串。
** ^如果没有匹配项，则返回一个空指针。
** ^如果 zVfsName 是 NULL，则返回默认 VFS。
**
** ^新的 VFS 使用 sqlite3_vfs_register() 注册。
** ^如果设置了 makeDflt 标志，每个新的 VFS 都会成为默认 VFS。
** ^可以多次注册相同的 VFS 而不会造成伤害。
** ^要使现有的 VFS 成为默认 VFS，请再次注册它，
** 并设置 makeDflt 标志。如果注册了两个名称相同的
** 不同 VFS，则行为是未定义的。如果使用 NULL 或空字符串
** 名称注册 VFS，则行为是未定义的。
**
** ^使用 sqlite3_vfs_unregister() 接口注销 VFS。
** ^(如果默认 VFS 被注销，则选择另一个 VFS 作为
** 默认 VFS。新 VFS 的选择是任意的。) ^
*/
SQLITE_API sqlite3_vfs *SQLITE_STDCALL sqlite3_vfs_find(const char *zVfsName);
SQLITE_API int SQLITE_STDCALL sqlite3_vfs_register(sqlite3_vfs*, int makeDflt);
SQLITE_API int SQLITE_STDCALL sqlite3_vfs_unregister(sqlite3_vfs*);

/*
** CAPI3REF: 互斥锁
**
** SQLite 核心使用这些例程进行线程同步。尽管它们旨在供 SQLite 内部使用，但链接到 SQLite 的代码也被允许使用这些例程中的任何一个。
**
** SQLite 源代码包含这些互斥锁例程的多种实现。在编译时会自动选择合适的实现。SQLite 核心中可用的实现包括：
**
** <ul>
** <li>   SQLITE_MUTEX_PTHREADS
** <li>   SQLITE_MUTEX_W32
** <li>   SQLITE_MUTEX_NOOP
** </ul>
**
** SQLITE_MUTEX_NOOP 实现是一组不进行实际锁定的例程，适用于单线程应用程序。SQLITE_MUTEX_PTHREADS 和 SQLITE_MUTEX_W32 实现分别适用于 Unix 和 Windows。
**
** 如果 SQLite 在编译时定义了 SQLITE_MUTEX_APPDEF 预处理器宏（使用 "-DSQLITE_MUTEX_APPDEF=1"），则库中不包含任何互斥锁实现。在这种情况下，应用程序必须在调用 sqlite3_initialize() 或任何其他调用 sqlite3_initialize() 的公共 sqlite3_ 函数之前，使用 sqlite3_config() 函数的 [SQLITE_CONFIG_MUTEX] 选项提供自定义的互斥锁实现。
**
** ^sqlite3_mutex_alloc() 例程分配一个新的互斥锁并返回指向它的指针。^如果无法分配请求的互斥锁，sqlite3_mutex_alloc() 例程返回 NULL。sqlite3_mutex_alloc() 的参数必须是以下整数常量之一：
**
** <ul>
** <li>  SQLITE_MUTEX_FAST
** <li>  SQLITE_MUTEX_RECURSIVE
** <li>  SQLITE_MUTEX_STATIC_MASTER
** <li>  SQLITE_MUTEX_STATIC_MEM
** <li>  SQLITE_MUTEX_STATIC_OPEN
** <li>  SQLITE_MUTEX_STATIC_PRNG
** <li>  SQLITE_MUTEX_STATIC_LRU
** <li>  SQLITE_MUTEX_STATIC_PMEM
** <li>  SQLITE_MUTEX_STATIC_APP1
** <li>  SQLITE_MUTEX_STATIC_APP2
** <li>  SQLITE_MUTEX_STATIC_APP3
** </ul>
**
** ^前两个常量（SQLITE_MUTEX_FAST 和 SQLITE_MUTEX_RECURSIVE）使 sqlite3_mutex_alloc() 创建一个新的互斥锁。^当使用 SQLITE_MUTEX_RECURSIVE 时，新互斥锁是递归的，但使用 SQLITE_MUTEX_FAST 时不一定如此。如果互斥锁实现不希望区分 SQLITE_MUTEX_RECURSIVE 和 SQLITE_MUTEX_FAST，则不需要区分。SQLite 只会在真正需要递归互斥锁的情况下请求递归互斥锁。如果主机平台上提供了更快的非递归互斥锁实现，互斥锁子系统可能会在响应 SQLITE_MUTEX_FAST 时返回这样的互斥锁。
**
** ^sqlite3_mutex_alloc() 的其他允许参数（除了 SQLITE_MUTEX_FAST 和 SQLITE_MUTEX_RECURSIVE 之外的任何参数）每个都返回指向静态预存在互斥锁的指针。^当前版本的 SQLite 使用了九个静态互斥锁。未来版本的 SQLite 可能会添加额外的静态互斥锁。静态互斥锁仅供 SQLite 内部使用。使用 SQLite 互斥锁的应用程序应仅使用由 SQLITE_MUTEX_FAST 或 SQLITE_MUTEX_RECURSIVE 返回的动态互斥锁。
**
** ^请注意，如果使用动态互斥锁参数（SQLITE_MUTEX_FAST 或 SQLITE_MUTEX_RECURSIVE），则 sqlite3_mutex_alloc() 每次调用都会返回不同的互斥锁。^对于静态互斥锁类型，每次调用返回的互斥锁是相同的。
**
** ^sqlite3_mutex_free() 例程释放先前分配的动态互斥锁。尝试释放静态互斥锁会导致未定义的行为。
**
** ^sqlite3_mutex_enter() 和 sqlite3_mutex_try() 例程尝试进入互斥锁。^如果另一个线程已经在互斥锁内，sqlite3_mutex_enter() 将阻塞，而 sqlite3_mutex_try() 将返回 SQLITE_BUSY。^sqlite3_mutex_try() 接口在成功进入时返回 [SQLITE_OK]。^（使用 SQLITE_MUTEX_RECURSIVE 创建的互斥锁可以由同一线程多次进入。在这种情况下，互斥锁必须被退出相同次数，其他线程才能进入。）^如果同一线程尝试多次进入任何非 SQLITE_MUTEX_RECURSIVE 的互斥锁，则行为是未定义的。
**
** ^（某些系统（例如 Windows 95）不支持 sqlite3_mutex_try() 实现的操作。在这些系统上，sqlite3_mutex_try() 将始终返回 SQLITE_BUSY。SQLite 核心仅将 sqlite3_mutex_try() 用作优化，因此这是可接受的行为。）^
**
** ^sqlite3_mutex_leave() 例程退出先前由同一线程进入的互斥锁。如果调用线程当前未进入互斥锁或互斥锁当前未分配，则行为是未定义的。
**
** ^如果 sqlite3_mutex_enter()、sqlite3_mutex_try() 或 sqlite3_mutex_leave() 的参数是 NULL 指针，则所有三个例程的行为都如同无操作。
**
** 另请参阅：[sqlite3_mutex_held()] 和 [sqlite3_mutex_notheld()]。
*/
SQLITE_API sqlite3_mutex *SQLITE_STDCALL sqlite3_mutex_alloc(int);
SQLITE_API void SQLITE_STDCALL sqlite3_mutex_free(sqlite3_mutex*);
SQLITE_API void SQLITE_STDCALL sqlite3_mutex_enter(sqlite3_mutex*);
SQLITE_API int SQLITE_STDCALL sqlite3_mutex_try(sqlite3_mutex*);
SQLITE_API void SQLITE_STDCALL sqlite3_mutex_leave(sqlite3_mutex*);

/*
** CAPI3REF: 互斥锁方法对象
**
** 此结构的实例定义了用于分配和使用互斥锁的低级例程。
**
** 通常，SQLite 提供的默认互斥锁实现已经足够，但应用程序可以选择为特殊部署或 SQLite 未提供合适实现的系统替换自定义实现。在这种情况下，应用程序创建并填充此结构的实例，将其与 [SQLITE_CONFIG_MUTEX] 选项一起传递给 sqlite3_config()。此外，此结构的实例可以用作输出变量，在使用 [SQLITE_CONFIG_GETMUTEX] 选项查询系统当前互斥锁实现时使用。
**
** ^此结构定义的 xMutexInit 方法作为系统初始化的一部分由 sqlite3_initialize() 函数调用。^对于每次有效的 [sqlite3_initialize()] 调用，SQLite 会精确调用一次 xMutexInit 例程。
**
** ^此结构定义的 xMutexEnd 方法作为系统关闭的一部分由 sqlite3_shutdown() 函数调用。此方法的实现应释放由互斥锁方法实现获取的所有未释放资源，尤其是由 xMutexInit 方法获取的资源。^对于每次 [sqlite3_shutdown()] 调用，xMutexEnd() 接口会精确调用一次。
**
** ^（此结构定义的其余七个方法（xMutexAlloc、xMutexFree、xMutexEnter、xMutexTry、xMutexLeave、xMutexHeld 和 xMutexNotheld）分别实现以下接口：
**
** <ul>
**   <li>  [sqlite3_mutex_alloc()] </li>
**   <li>  [sqlite3_mutex_free()] </li>
**   <li>  [sqlite3_mutex_enter()] </li>
**   <li>  [sqlite3_mutex_try()] </li>
**   <li>  [sqlite3_mutex_leave()] </li>
**   <li>  [sqlite3_mutex_held()] </li>
**   <li>  [sqlite3_mutex_notheld()] </li>
** </ul>)^
**
** 唯一的区别是，上述公共 sqlite3_XXX 函数会静默忽略任何传递 NULL 指针而不是有效互斥锁句柄的调用。此结构定义的方法的实现不需要处理这种情况，传递 NULL 指针而不是有效互斥锁句柄的结果是未定义的（即，如果传递 NULL 指针，提供导致段错误的实现是可以接受的）。
**
** xMutexInit() 方法必须是线程安全的。在同一进程中多次调用 xMutexInit() 且没有中间调用 xMutexEnd() 必须是无害的。第二次及后续调用 xMutexInit() 必须是无操作的。
**
** xMutexInit() 不得使用 SQLite 内存分配（[sqlite3_malloc()] 及其相关函数）。同样，xMutexAlloc() 不得为静态互斥锁使用 SQLite 内存分配。^然而，xMutexAlloc() 可以为快速或递归互斥锁使用 SQLite 内存分配。
**
** ^SQLite 将在调用 [sqlite3_shutdown()] 时调用 xMutexEnd() 方法，但前提是之前的 xMutexInit 调用返回了 SQLITE_OK。如果 xMutexInit 以任何方式失败，则期望它在返回之前自行清理。
*/
typedef struct sqlite3_mutex_methods sqlite3_mutex_methods;
struct sqlite3_mutex_methods {
  int (*xMutexInit)(void);
  int (*xMutexEnd)(void);
  sqlite3_mutex *(*xMutexAlloc)(int);
  void (*xMutexFree)(sqlite3_mutex *);
  void (*xMutexEnter)(sqlite3_mutex *);
  int (*xMutexTry)(sqlite3_mutex *);
  void (*xMutexLeave)(sqlite3_mutex *);
  int (*xMutexHeld)(sqlite3_mutex *);
  int (*xMutexNotheld)(sqlite3_mutex *);
};

/*
** CAPI3REF: 互斥锁验证例程
**
** sqlite3_mutex_held() 和 sqlite3_mutex_notheld() 例程旨在用于 assert() 语句内部。SQLite 核心除了在 assert() 内部外从不使用这些例程，建议应用程序遵循核心的做法。SQLite 核心仅在编译时启用 SQLITE_DEBUG 标志时提供这些例程的实现。外部互斥锁实现仅在定义了 SQLITE_DEBUG 且未定义 NDEBUG 时需要提供这些例程。
**
** 如果调用线程分别持有或不持有其参数中的互斥锁，这些例程应返回 true。
**
** 实现不需要提供实际工作的这些例程版本。如果实现不提供这些例程的工作版本，则至少应提供始终返回 true 的存根，以避免出现虚假的断言失败。
**
** 如果 sqlite3_mutex_held() 的参数是 NULL 指针，则该例程应返回 1。这似乎违反直觉，因为显然如果互斥锁不存在，则无法持有它。但互斥锁不存在的原因是构建未使用互斥锁。我们不希望包含 sqlite3_mutex_held() 调用的 assert() 失败，因此返回非零值是适当的做法。sqlite3_mutex_notheld() 接口在给定 NULL 指针时也应返回 1。
*/
#ifndef NDEBUG
SQLITE_API int SQLITE_STDCALL sqlite3_mutex_held(sqlite3_mutex*);
SQLITE_API int SQLITE_STDCALL sqlite3_mutex_notheld(sqlite3_mutex*);
#endif

/*
** CAPI3REF: 互斥锁类型
**
** [sqlite3_mutex_alloc()] 接口接受一个参数，
** 该参数是以下整数常量之一。
**
** 静态互斥锁的集合可能会因 SQLite 版本的不同而有所变化。
** 覆盖内置互斥锁逻辑的应用程序必须准备好适应额外的静态互斥锁。
*/
#define SQLITE_MUTEX_FAST             0
#define SQLITE_MUTEX_RECURSIVE        1
#define SQLITE_MUTEX_STATIC_MASTER    2
#define SQLITE_MUTEX_STATIC_MEM       3  /* sqlite3_malloc() */
#define SQLITE_MUTEX_STATIC_MEM2      4  /* 未使用 */
#define SQLITE_MUTEX_STATIC_OPEN      4  /* sqlite3BtreeOpen() */
#define SQLITE_MUTEX_STATIC_PRNG      5  /* sqlite3_random() */
#define SQLITE_MUTEX_STATIC_LRU       6  /* LRU 页面列表 */
#define SQLITE_MUTEX_STATIC_LRU2      7  /* 未使用 */
#define SQLITE_MUTEX_STATIC_PMEM      7  /* sqlite3PageMalloc() */
#define SQLITE_MUTEX_STATIC_APP1      8  /* 供应用程序使用 */
#define SQLITE_MUTEX_STATIC_APP2      9  /* 供应用程序使用 */
#define SQLITE_MUTEX_STATIC_APP3     10  /* 供应用程序使用 */

/**
 * CAPI3REF: 获取数据库连接的互斥锁
 * METHOD: sqlite3
 *
 * ^此接口返回一个指向[sqlite3_mutex]对象的指针，
 * 当[线程模式]为Serialized时，该对象序列化对[数据库连接]的访问。
 * ^如果[线程模式]为Single-thread或Multi-thread，则此例程返回一个NULL指针。
 */
SQLITE_API sqlite3_mutex *SQLITE_STDCALL sqlite3_db_mutex(sqlite3*);

/*
** CAPI3REF: 数据库文件的低级控制
** METHOD: sqlite3
**
** ^[sqlite3_file_control()] 接口直接调用与特定数据库关联的
** [sqlite3_io_methods] 对象的 xFileControl 方法，该数据库由第二个参数指定。
** ^数据库的名称对于主数据库为 "main"，对于 TEMP 数据库为 "temp"，
** 或者对于使用 [ATTACH] SQL 命令添加的数据库，为 AS 关键字后出现的名称。
** ^可以用 NULL 指针代替 "main" 来引用主数据库文件。
** ^此例程的第三个和第四个参数直接传递给 xFileControl 方法的
** 第二个和第三个参数。^xFileControl 方法的返回值成为此例程的返回值。
**
** ^当 op 参数为 SQLITE_FCNTL_FILE_POINTER 时，会将指向底层 [sqlite3_file] 
** 对象的指针写入第四个参数指向的空间。^SQLITE_FCNTL_FILE_POINTER 情况是
** 一个短路路径，实际上不会调用底层的 sqlite3_io_methods.xFileControl 方法。
**
** ^如果第二个参数 (zDbName) 与任何打开的数据库文件的名称不匹配，
** 则返回 SQLITE_ERROR。^此错误代码不会被记住，也不会通过 [sqlite3_errcode()]
** 或 [sqlite3_errmsg()] 重新调用。底层的 xFileControl 方法也可能返回 SQLITE_ERROR。
** 无法区分错误的 zDbName 和底层 xFileControl 方法返回的 SQLITE_ERROR。
**
** 另请参阅: [SQLITE_FCNTL_LOCKSTATE]
*/
SQLITE_API int SQLITE_STDCALL sqlite3_file_control(sqlite3*, const char *zDbName, int op, void*);

/*
** CAPI3REF: 测试接口
**
** ^sqlite3_test_control() 接口用于读取 SQLite 的内部状态，
** 并在测试时向 SQLite 注入故障。^第一个参数是一个操作码，
** 它决定了后续参数的数量、含义和操作。
**
** 此接口不供应用程序使用。它仅用于验证 SQLite 库的正确操作。
** 根据 SQLite 库的编译方式，此接口可能不存在。
**
** 操作码的详细信息、它们的含义、所需的参数以及它们的功能
** 都可能在不通知的情况下更改。与大多数 SQLite API 不同，
** 此函数不能保证在不同版本之间保持一致的操作。
*/
SQLITE_API int SQLITE_CDECL sqlite3_test_control(int op, ...);

/*
** CAPI3REF: 测试接口操作码
**
** 这些常量是作为[sqlite3_test_control()]的第一个参数使用的有效操作码参数。
**
** 这些参数及其含义可能会在没有任何通知的情况下发生变化。这些值仅用于测试目的。
** 应用程序不应使用这些参数或[sqlite3_test_control()]接口。
*/
#define SQLITE_TESTCTRL_FIRST                    5
#define SQLITE_TESTCTRL_PRNG_SAVE                5
#define SQLITE_TESTCTRL_PRNG_RESTORE             6
#define SQLITE_TESTCTRL_PRNG_RESET               7
#define SQLITE_TESTCTRL_BITVEC_TEST              8
#define SQLITE_TESTCTRL_FAULT_INSTALL            9
#define SQLITE_TESTCTRL_BENIGN_MALLOC_HOOKS     10
#define SQLITE_TESTCTRL_PENDING_BYTE            11
#define SQLITE_TESTCTRL_ASSERT                  12
#define SQLITE_TESTCTRL_ALWAYS                  13
#define SQLITE_TESTCTRL_RESERVE                 14
#define SQLITE_TESTCTRL_OPTIMIZATIONS           15
#define SQLITE_TESTCTRL_ISKEYWORD               16
#define SQLITE_TESTCTRL_SCRATCHMALLOC           17
#define SQLITE_TESTCTRL_LOCALTIME_FAULT         18
#define SQLITE_TESTCTRL_EXPLAIN_STMT            19  /* 未使用 */
#define SQLITE_TESTCTRL_NEVER_CORRUPT           20
#define SQLITE_TESTCTRL_VDBE_COVERAGE           21
#define SQLITE_TESTCTRL_BYTEORDER               22
#define SQLITE_TESTCTRL_ISINIT                  23
#define SQLITE_TESTCTRL_SORTER_MMAP             24
#define SQLITE_TESTCTRL_IMPOSTER                25
#define SQLITE_TESTCTRL_LAST                    25

/*
** CAPI3REF: SQLite 运行时状态
**
** ^这些接口用于检索有关 SQLite 性能的运行时状态信息，
** 并可选择重置各种高水位标记。^第一个参数是要测量的
** 特定参数的整数代码。^(可识别的整数代码的形式为
** [状态参数 | SQLITE_STATUS_...]。)^
** ^参数的当前值返回到 *pCurrent 中。
** ^最高记录值返回到 *pHighwater 中。^如果 resetFlag 为真，
** 则在 *pHighwater 写入后，最高记录值将被重置。
** ^(某些参数不记录最高值。对于这些参数，
** 不会向 *pHighwater 写入任何内容，并且 resetFlag 被忽略。)^
** ^(其他参数仅记录高水位标记而不记录当前值。
** 对于这些参数，不会向 *pCurrent 写入任何内容。)^
**
** ^sqlite3_status() 和 sqlite3_status64() 例程在成功时返回
** SQLITE_OK，在失败时返回非零的 [错误代码]。
**
** 如果当前值或高水位标记太大而无法用 32 位整数表示，
** 则 sqlite3_status() 返回的值是未定义的。
**
** 另请参阅: [sqlite3_db_status()]
*/
SQLITE_API int SQLITE_STDCALL sqlite3_status(int op, int *pCurrent, int *pHighwater, int resetFlag);
SQLITE_API int SQLITE_STDCALL sqlite3_status64(
  int op,
  sqlite3_int64 *pCurrent,
  sqlite3_int64 *pHighwater,
  int resetFlag
);


/*
** CAPI3REF: 状态参数
** 关键词: {状态参数}
**
** 这些整数常量指定了各种运行时状态参数，可以通过 [sqlite3_status()] 返回。
**
** <dl>
** [[SQLITE_STATUS_MEMORY_USED]] ^(<dt>SQLITE_STATUS_MEMORY_USED</dt>
** <dd>此参数是当前使用 [sqlite3_malloc()] 检出的内存量，无论是直接还是间接。该数字包括应用程序对 [sqlite3_malloc()] 的调用和 SQLite 库的内部内存使用。由 [SQLITE_CONFIG_SCRATCH] 控制的临时内存和由 [SQLITE_CONFIG_PAGECACHE] 控制的辅助页面缓存内存不包括在此参数中。返回的金额是 [sqlite3_mem_methods] 中 xSize 方法报告的分配大小的总和。</dd>)^
**
** [[SQLITE_STATUS_MALLOC_SIZE]] ^(<dt>SQLITE_STATUS_MALLOC_SIZE</dt>
** <dd>此参数记录了传递给 [sqlite3_malloc()] 或 [sqlite3_realloc()]（或它们的内部等效项）的最大内存分配请求。只有返回在 [sqlite3_status()] 的 *pHighwater 参数中的值才有意义。写入 *pCurrent 参数的值是未定义的。</dd>)^
**
** [[SQLITE_STATUS_MALLOC_COUNT]] ^(<dt>SQLITE_STATUS_MALLOC_COUNT</dt>
** <dd>此参数记录当前检出的单独内存分配的数量。</dd>)^
**
** [[SQLITE_STATUS_PAGECACHE_USED]] ^(<dt>SQLITE_STATUS_PAGECACHE_USED</dt>
** <dd>此参数返回使用 [SQLITE_CONFIG_PAGECACHE] 配置的 [页面缓存内存分配器] 的页面数。返回的值以页面为单位，不是以字节为单位。</dd>)^
**
** [[SQLITE_STATUS_PAGECACHE_OVERFLOW]] 
** ^(<dt>SQLITE_STATUS_PAGECACHE_OVERFLOW</dt>
** <dd>此参数返回由于无法由 [SQLITE_CONFIG_PAGECACHE] 缓冲区满足而被迫溢出到 [sqlite3_malloc()] 的页面缓存分配的字节数。返回的值包括因太大而溢出的分配（它们大于 [SQLITE_CONFIG_PAGECACHE] 的 "sz" 参数）和因页面缓存中没有剩余空间而溢出的分配。</dd>)^
**
** [[SQLITE_STATUS_PAGECACHE_SIZE]] ^(<dt>SQLITE_STATUS_PAGECACHE_SIZE</dt>
** <dd>此参数记录传递给 [页面缓存内存分配器] 的最大内存分配请求。只有返回在 [sqlite3_status()] 的 *pHighwater 参数中的值才有意义。写入 *pCurrent 参数的值是未定义的。</dd>)^
**
** [[SQLITE_STATUS_SCRATCH_USED]] ^(<dt>SQLITE_STATUS_SCRATCH_USED</dt>
** <dd>此参数返回使用 [SQLITE_CONFIG_SCRATCH] 配置的 [临时内存分配器] 的分配数量。返回的值以分配数为单位，不是以字节为单位。由于单个线程一次只能有一个临时分配，因此此参数还报告同时使用临时内存的线程数。</dd>)^
**
** [[SQLITE_STATUS_SCRATCH_OVERFLOW]] ^(<dt>SQLITE_STATUS_SCRATCH_OVERFLOW</dt>
** <dd>此参数返回由于无法由 [SQLITE_CONFIG_SCRATCH] 缓冲区满足而被迫溢出到 [sqlite3_malloc()] 的临时内存分配的字节数。返回的值包括因请求的分配过大而溢出的分配（即请求的分配大于 [SQLITE_CONFIG_SCRATCH] 的 "sz" 参数）和因没有临时缓冲区插槽可用而溢出的分配。</dd>)^
**
** [[SQLITE_STATUS_SCRATCH_SIZE]] ^(<dt>SQLITE_STATUS_SCRATCH_SIZE</dt>
** <dd>此参数记录传递给 [临时内存分配器] 的最大内存分配请求。只有返回在 [sqlite3_status()] 的 *pHighwater 参数中的值才有意义。写入 *pCurrent 参数的值是未定义的。</dd>)^
**
** [[SQLITE_STATUS_PARSER_STACK]] ^(<dt>SQLITE_STATUS_PARSER_STACK</dt>
** <dd>此参数记录最深的解析器堆栈。只有在 SQLite 编译时带有 [YYTRACKMAXSTACKDEPTH] 时才有意义。</dd>)^
** </dl>
**
** 可能会不时添加新的状态参数。
*/
#define SQLITE_STATUS_MEMORY_USED          0
#define SQLITE_STATUS_PAGECACHE_USED       1
#define SQLITE_STATUS_PAGECACHE_OVERFLOW   2
#define SQLITE_STATUS_SCRATCH_USED         3
#define SQLITE_STATUS_SCRATCH_OVERFLOW     4
#define SQLITE_STATUS_MALLOC_SIZE          5
#define SQLITE_STATUS_PARSER_STACK         6
#define SQLITE_STATUS_PAGECACHE_SIZE       7
#define SQLITE_STATUS_SCRATCH_SIZE         8
#define SQLITE_STATUS_MALLOC_COUNT         9

/*
** CAPI3REF: 数据库连接状态
** METHOD: sqlite3
**
** ^此接口用于检索关于单个[数据库连接]的运行时状态信息。^第一个参数是要查询的数据库连接对象。^第二个参数是一个整数常量，来自[SQLITE_DBSTATUS 选项]集合，决定了要查询的参数。[SQLITE_DBSTATUS 选项]集合可能会在SQLite的未来版本中增加。
**
** ^请求参数的当前值被写入*pCur，最高瞬时值被写入*pHiwtr。^如果resetFlg为真，则最高瞬时值将被重置为当前值。
**
** ^sqlite3_db_status()例程在成功时返回SQLITE_OK，在失败时返回非零[错误代码]。
**
** 另见：[sqlite3_status()] 和 [sqlite3_stmt_status()]。
*/
SQLITE_API int SQLITE_STDCALL sqlite3_db_status(sqlite3*, int op, int *pCur, int *pHiwtr, int resetFlg);

/*
** CAPI3REF: 数据库连接的状态参数
** 关键词: {SQLITE_DBSTATUS 选项}
**
** 这些常量是可以作为第二个参数传递给 [sqlite3_db_status()] 接口的可用整数 "动词"。
**
** 新的动词可能会在 SQLite 的未来版本中添加。现有的动词可能会被弃用。应用程序应检查 [sqlite3_db_status()] 的返回代码以确保调用成功。
** 如果调用了已弃用或不支持的动词，[sqlite3_db_status()] 接口将返回一个非零的错误代码。
**
** <dl>
** [[SQLITE_DBSTATUS_LOOKASIDE_USED]] ^(<dt>SQLITE_DBSTATUS_LOOKASIDE_USED</dt>
** <dd>此参数返回当前已检查出的 lookaside 内存槽的数量。</dd>)^
**
** [[SQLITE_DBSTATUS_LOOKASIDE_HIT]] ^(<dt>SQLITE_DBSTATUS_LOOKASIDE_HIT</dt>
** <dd>此参数返回使用 lookaside 内存满足的 malloc 尝试次数。只有高水位值是有意义的；当前值总是零。</dd>)^
**
** [[SQLITE_DBSTATUS_LOOKASIDE_MISS_SIZE]]
** ^(<dt>SQLITE_DBSTATUS_LOOKASIDE_MISS_SIZE</dt>
** <dd>此参数返回由于请求的内存量大于 lookaside 槽大小而失败的 malloc 尝试次数，这些尝试本可以使用 lookaside 内存满足。只有高水位值是有意义的；当前值总是零。</dd>)^
**
** [[SQLITE_DBSTATUS_LOOKASIDE_MISS_FULL]]
** ^(<dt>SQLITE_DBSTATUS_LOOKASIDE_MISS_FULL</dt>
** <dd>此参数返回由于所有 lookaside 内存已在使用中而失败的 malloc 尝试次数，这些尝试本可以使用 lookaside 内存满足。只有高水位值是有意义的；当前值总是零。</dd>)^
**
** [[SQLITE_DBSTATUS_CACHE_USED]] ^(<dt>SQLITE_DBSTATUS_CACHE_USED</dt>
** <dd>此参数返回与数据库连接关联的所有页缓存使用的堆内存的大致字节数。</dd>)^
** ^与 SQLITE_DBSTATUS_CACHE_USED 关联的高水位标记总是 0。
**
** [[SQLITE_DBSTATUS_SCHEMA_USED]] ^(<dt>SQLITE_DBSTATUS_SCHEMA_USED</dt>
** <dd>此参数返回与连接关联的所有数据库（主数据库、临时数据库和任何 [ATTACH] 的数据库）的架构存储使用的堆内存的大致字节数。</dd>)^ 
** ^即使由于启用了 [共享缓存模式] 而与其它数据库连接共享架构内存，也会报告架构使用的全部内存量。
** ^与 SQLITE_DBSTATUS_SCHEMA_USED 关联的高水位标记总是 0。
**
** [[SQLITE_DBSTATUS_STMT_USED]] ^(<dt>SQLITE_DBSTATUS_STMT_USED</dt>
** <dd>此参数返回与数据库连接关联的所有准备语句使用的堆内存和 lookaside 内存的大致字节数。</dd>)^
** ^与 SQLITE_DBSTATUS_STMT_USED 关联的高水位标记总是 0。
** </dd>
**
** [[SQLITE_DBSTATUS_CACHE_HIT]] ^(<dt>SQLITE_DBSTATUS_CACHE_HIT</dt>
** <dd>此参数返回发生的页缓存命中的次数。</dd>)^ ^与 SQLITE_DBSTATUS_CACHE_HIT 关联的高水位标记总是 0。
** </dd>
**
** [[SQLITE_DBSTATUS_CACHE_MISS]] ^(<dt>SQLITE_DBSTATUS_CACHE_MISS</dt>
** <dd>此参数返回发生的页缓存未命中的次数。</dd>)^ ^与 SQLITE_DBSTATUS_CACHE_MISS 关联的高水位标记总是 0。
** </dd>
**
** [[SQLITE_DBSTATUS_CACHE_WRITE]] ^(<dt>SQLITE_DBSTATUS_CACHE_WRITE</dt>
** <dd>此参数返回已写入磁盘的脏缓存条目的数量。具体来说，是在 wal 模式数据库中写入 wal 文件的页面数量，或在回滚模式数据库中写入数据库文件的页面数量。不包括作为事务回滚或数据库恢复操作一部分写入的任何页面。如果在写入页面到磁盘时发生 IO 或其他错误，对后续 SQLITE_DBSTATUS_CACHE_WRITE 请求的影响是未定义的。</dd>)^ ^与 SQLITE_DBSTATUS_CACHE_WRITE 关联的高水位标记总是 0。
** </dd>
**
** [[SQLITE_DBSTATUS_DEFERRED_FKS]] ^(<dt>SQLITE_DBSTATUS_DEFERRED_FKS</dt>
** <dd>如果所有外键约束（延迟或立即）都已解决，则此参数返回当前值为零。</dd>)^  ^高水位标记总是 0。
** </dd>
** </dl>
*/
#define SQLITE_DBSTATUS_LOOKASIDE_USED       0
#define SQLITE_DBSTATUS_CACHE_USED           1
#define SQLITE_DBSTATUS_SCHEMA_USED          2
#define SQLITE_DBSTATUS_STMT_USED            3
#define SQLITE_DBSTATUS_LOOKASIDE_HIT        4
#define SQLITE_DBSTATUS_LOOKASIDE_MISS_SIZE  5
#define SQLITE_DBSTATUS_LOOKASIDE_MISS_FULL  6
#define SQLITE_DBSTATUS_CACHE_HIT            7
#define SQLITE_DBSTATUS_CACHE_MISS           8
#define SQLITE_DBSTATUS_CACHE_WRITE          9
#define SQLITE_DBSTATUS_DEFERRED_FKS        10
#define SQLITE_DBSTATUS_MAX                 10   /* 最大已定义的 DBSTATUS */


/*
** CAPI3REF: 预备语句状态
** METHOD: sqlite3_stmt
**
** ^(每个预备语句都维护着各种
** [SQLITE_STMTSTATUS 计数器]，用于测量它执行特定操作的次数。)^ 这些计数器可以
** 用于监控预备语句的性能特征。例如，如果表步骤的数量大大超过
** 表搜索或结果行的数量，那通常表明预备语句使用的是全表扫描而不是
** 索引。  
**
** ^(这个接口用于从
** [预备语句] 中检索和重置计数器值。第一个参数是要查询的预备语句
** 对象。第二个参数是一个整数代码，用于指定要查询的特定
** [SQLITE_STMTSTATUS 计数器]。)^
** ^返回请求计数器的当前值。^
** ^如果 resetFlg 为真，那么在这次接口调用返回后，计数器将被重置为零。^
**
** 另见：[sqlite3_status()] 和 [sqlite3_db_status()]。
*/
SQLITE_API int SQLITE_STDCALL sqlite3_stmt_status(sqlite3_stmt*, int op,int resetFlg);

/*
** CAPI3REF: 预备语句的状态参数
** 关键词: {SQLITE_STMTSTATUS 计数器} {SQLITE_STMTSTATUS 计数器}
**
** 这些预处理宏定义了与 [sqlite3_stmt_status()] 接口相关联的计数器值的整型代码。
** 各种计数器的含义如下：
**
** <dl>
** [[SQLITE_STMTSTATUS_FULLSCAN_STEP]] <dt>SQLITE_STMTSTATUS_FULLSCAN_STEP</dt>
** <dd>^这是 SQLite 在全表扫描过程中向前步进的次数。
** 这个计数器的较大数值可能表明通过仔细使用索引可以改进性能。</dd>
**
** [[SQLITE_STMTSTATUS_SORT]] <dt>SQLITE_STMTSTATUS_SORT</dt>
** <dd>^这是发生的排序操作的次数。
** 这个计数器的非零值可能表明通过仔细使用索引可以改进性能。</dd>
**
** [[SQLITE_STMTSTATUS_AUTOINDEX]] <dt>SQLITE_STMTSTATUS_AUTOINDEX</dt>
** <dd>^这是插入到临时索引中的行数，这些索引是自动创建的，以帮助更快地执行连接。
** 这个计数器的非零值可能表明通过添加不需要每次运行语句时重新初始化的永久索引可以改进性能。</dd>
**
** [[SQLITE_STMTSTATUS_VM_STEP]] <dt>SQLITE_STMTSTATUS_VM_STEP</dt>
** <dd>^这是由预备语句执行的虚拟机操作次数，如果该次数小于或等于 2147483647。
** 虚拟机操作次数可以用作预备语句所做总工作的代理。
** 如果虚拟机操作次数超过 2147483647，则此语句状态代码返回的值是未定义的。
** </dd>
** </dl>
*/
#define SQLITE_STMTSTATUS_FULLSCAN_STEP     1
#define SQLITE_STMTSTATUS_SORT              2
#define SQLITE_STMTSTATUS_AUTOINDEX         3
#define SQLITE_STMTSTATUS_VM_STEP           4

/*
** CAPI3REF: 自定义页面缓存对象
**
** sqlite3_pcache 类型是不透明的。它由可插拔模块实现。
** SQLite核心对其大小或内部结构一无所知，并且除了持有和传递对象的指针之外，
** 从不直接处理 sqlite3_pcache 对象。
**
** 有关更多信息，请参见 [sqlite3_pcache_methods2]。
*/
typedef struct sqlite3_pcache sqlite3_pcache;

/*
** CAPI3REF: 自定义页面缓存对象
**
** sqlite3_pcache_page对象代表页面缓存中的一个单一页面。
** 页面缓存会分配此对象的实例。页面缓存的各种方法使用此对象的
** 指针作为参数或作为它们的返回值。
**
** 有关更多信息，请参见[sqlite3_pcache_methods2]。
*/
typedef struct sqlite3_pcache_page sqlite3_pcache_page;
struct sqlite3_pcache_page {
  void *pBuf;        /* 页面的内容 */
  void *pExtra;      /* 与页面关联的额外信息 */
};

/*
** CAPI3REF: 应用程序定义的页面缓存。
** 关键词: {页面缓存}
**
** ^(通过传递sqlite3_pcache_methods2结构的实例，[sqlite3_config]([SQLITE_CONFIG_PCACHE2], ...)接口可以注册一个替代的页面缓存实现。) ^
** 在许多应用程序中，SQLite分配的大部分堆内存用于页面缓存。
** 通过使用此API实现自定义页面缓存，应用程序可以更好地控制SQLite消耗的内存量，
** 内存分配和释放的方式，以及确定数据库文件的哪些部分被缓存以及缓存多长时间的政策。
**
** 替代的页面缓存机制是一种极端措施，只有最需要它的应用程序才需要。
** 对于大多数用途，建议使用内置的页面缓存。
**
** ^(sqlite3_pcache_methods2结构的内容在[sqlite3_config]调用中被SQLite复制到内部缓冲区中。
** 因此，应用程序可以在[sqlite3_config()]返回后丢弃参数。) ^
**
** [[xInit()页面缓存方法]]
** ^(xInit()方法在每次有效的[sqlite3_initialize()]调用时调用一次) ^
** (通常在进程的生命周期中只调用一次)。^(xInit()方法传递sqlite3_pcache_methods2.pArg值的副本。) ^
** xInit()方法的目的是设置自定义页面缓存实现所需的全球数据结构。
** ^(如果xInit()方法为NULL，则使用内置的默认页面缓存而不是应用程序定义的页面缓存。) ^
**
** [[xShutdown()页面缓存方法]]
** ^xShutdown()方法由[sqlite3_shutdown()]调用。
** 它可以在进程关闭前清理任何未完成的资源（如果需要）。
** ^xShutdown()方法可以是NULL。
**
** ^SQLite自动序列化对xInit方法的调用，因此xInit方法不需要是线程安全的。
** ^xShutdown方法仅从[sqlite3_shutdown()]调用，因此也不需要是线程安全的。
** 在多线程应用程序中，所有其他方法必须是线程安全的。
**
** ^SQLite永远不会在没有调用xShutdown()的情况下调用xInit()超过一次。
**
** [[xCreate()页面缓存方法]]
** ^SQLite调用xCreate()方法来构建一个新的缓存实例。
** SQLite通常为每个打开的数据库文件创建一个缓存实例，尽管这不是保证的。 ^
** 第一个参数szPage是缓存必须分配的页面的字节大小。 ^
** szPage总是2的幂。 ^
** 第二个参数szExtra是与每个页面缓存条目关联的额外字节数。 ^
** szExtra参数将是一个小于250的数字。 ^
** SQLite将使用每个页面上的额外szExtra字节来存储有关底层磁盘上数据库页面的元数据。
** 传递给szExtra的值取决于SQLite版本、目标平台和SQLite的编译方式。
** ^xCreate()的第三个参数bPurgeable，如果创建的缓存将用于缓存存储在磁盘上的文件数据库页面，则为true，或者如果用于内存数据库，则为false。
** 缓存实现不必根据bPurgeable的值做任何特殊处理；它只是建议性的。
** ^在bPurgeable为false的缓存上，SQLite将永远不会调用xUnpin()，除非是故意删除一个页面。
** ^换句话说，在bPurgeable设置为false的缓存上调用xUnpin()时，"丢弃"标志总是设置为true。
** ^因此，使用bPurgeable false创建的缓存永远不会包含任何未固定的页面。
**
** [[xCachesize()页面缓存方法]]
** ^(xCachesize()方法可由SQLite随时调用以设置缓存实例建议的最大缓存大小（存储的页面数）。
** 这是使用SQLite的"[PRAGMA cache_size]"命令配置的值。) ^ 与bPurgeable参数一样，实现不需要对此值做任何处理；它只是建议性的。
**
** [[xPagecount()页面缓存方法]]
** xPagecount()方法必须返回当前存储在缓存中的页面数，包括固定和未固定的页面。
** 
** [[xFetch()页面缓存方法]]
** xFetch()方法在缓存中定位一个页面，并返回与该页面关联的sqlite3_pcache_page对象的指针，或者返回NULL指针。
** 返回的sqlite3_pcache_page对象的pBuf元素将是指向用于存储单个数据库页面内容的szPage字节缓冲区的指针。
** sqlite3_pcache_page的pExtra元素将是指向SQLite为页面缓存中的每个条目请求的szExtra字节的额外存储的指针。
**
** 要获取的页面由键确定。 ^最小键值为1。 ^
** 使用xFetch检索后，页面被认为是“固定的”。
**
** 如果请求的页面已经在页面缓存中，则页面缓存实现必须返回一个指向页面缓冲区的指针，内容保持不变。
** 如果请求的页面不在缓存中，则缓存实现应使用createFlag参数的值来帮助确定应采取的操作：
**
** <table border=1 width=85% align=center>
** <tr><th> createFlag <th> 当页面不在缓存中时的行为
** <tr><td> 0 <td> 不分配新页面。返回NULL。
** <tr><td> 1 <td> 如果容易且方便，则分配新页面。否则返回NULL。
** <tr><td> 2 <td> 尽一切努力分配新页面。只有在分配新页面实际上不可能时才返回NULL。
** </table>
**
** ^(SQLite通常会在createFlag为0或1的情况下调用xFetch()。SQLite只有在之前的createFlag为1的调用失败后才会使用createFlag为2。) ^
** 在两次xFetch()调用之间，SQLite可能会尝试通过将固定页面的内容溢出到磁盘并同步操作系统磁盘缓存来取消固定一个或多个缓存页面。
**
** [[xUnpin()页面缓存方法]]
** ^SQLite调用xUnpin()并传递一个当前固定页面的指针作为第二个参数。
** 如果第三个参数discard非零，则页面必须从缓存中逐出。
** ^如果discard参数为零，则页面可以根据页面缓存实现的 discretion 丢弃或保留。
** 页面缓存实现可以在任何时候选择逐出未固定的页面。
**
** 缓存不得执行任何引用计数。无论对xFetch()调用多少次，单个xUnpin()调用都会取消固定页面。
**
** [[xRekey()页面缓存方法]]
** xRekey()方法用于更改与第二个参数传递的页面关联的键值。
** 如果缓存之前包含与newKey关联的条目，则必须丢弃它。 ^
** 任何先前与newKey关联的缓存条目保证未被固定。
**
** 当SQLite调用xTruncate()方法时，缓存必须丢弃所有现有的缓存条目，其页面号（键）大于或等于传递给xTruncate()的iLimit参数的值。
** 如果这些页面中的任何一个是固定的，它们将被隐式取消固定，这意味着它们可以安全地被丢弃。
**
** [[xDestroy()页面缓存方法]]
** ^xDestroy()方法用于删除xCreate()分配的缓存。
** 应该释放与指定缓存相关的所有资源。 ^调用xDestroy()方法后，SQLite认为[sqlite3_pcache*]句柄无效，并且不会将其用于任何其他sqlite3_pcache_methods2函数。
**
** [[xShrink()页面缓存方法]]
** ^当SQLite希望页面缓存释放尽可能多的堆内存时，它会调用xShrink()方法。
** 页面缓存实现不必释放任何内存，但行为良好的实现应尽力而为。
*/
typedef struct sqlite3_pcache_methods2 sqlite3_pcache_methods2;
struct sqlite3_pcache_methods2 {
  int iVersion;
  void *pArg;
  int (*xInit)(void*);
  void (*xShutdown)(void*);
  sqlite3_pcache *(*xCreate)(int szPage, int szExtra, int bPurgeable);
  void (*xCachesize)(sqlite3_pcache*, int nCachesize);
  int (*xPagecount)(sqlite3_pcache*);
  sqlite3_pcache_page *(*xFetch)(sqlite3_pcache*, unsigned key, int createFlag);
  void (*xUnpin)(sqlite3_pcache*, sqlite3_pcache_page*, int discard);
  void (*xRekey)(sqlite3_pcache*, sqlite3_pcache_page*, 
	  unsigned oldKey, unsigned newKey);
  void (*xTruncate)(sqlite3_pcache*, unsigned iLimit);
  void (*xDestroy)(sqlite3_pcache*);
  void (*xShrink)(sqlite3_pcache*);
};

/*
** 这是已经被废弃的 pcache_methods 对象，现在已经被 sqlite3_pcache_methods2 替换。
** 这个对象不再被 SQLite 使用。它仅在头文件中保留以保持向后兼容性。
*/
typedef struct sqlite3_pcache_methods sqlite3_pcache_methods;
struct sqlite3_pcache_methods {
  void *pArg;
  int (*xInit)(void*);
  void (*xShutdown)(void*);
  sqlite3_pcache *(*xCreate)(int szPage, int bPurgeable);
  void (*xCachesize)(sqlite3_pcache*, int nCachesize);
  int (*xPagecount)(sqlite3_pcache*);
  void *(*xFetch)(sqlite3_pcache*, unsigned key, int createFlag);
  void (*xUnpin)(sqlite3_pcache*, void*, int discard);
  void (*xRekey)(sqlite3_pcache*, void*, unsigned oldKey, unsigned newKey);
  void (*xTruncate)(sqlite3_pcache*, unsigned iLimit);
  void (*xDestroy)(sqlite3_pcache*);
};


/*
** CAPI3REF: 在线备份对象
**
** sqlite3_backup对象记录了正在进行中的在线备份操作的状态信息。
** ^sqlite3_backup对象是通过调用[sqlite3_backup_init()]创建的，
** 并通过调用[sqlite3_backup_finish()]销毁。
**
** 另见：[使用SQLite在线备份API]
*/
typedef struct sqlite3_backup sqlite3_backup;

/*
** CAPI3REF: 在线备份API。
**
** 备份API将一个数据库的内容复制到另一个数据库。
** 它可用于创建数据库的备份，或者用于在内存数据库和持久文件之间复制。
**
** 另见：[使用SQLite在线备份API]
**
** ^SQLite在整个备份操作期间会在目标数据库文件上保持一个写事务打开。
** ^源数据库只在被读取时才会被读锁住；它不会在整个备份操作期间持续被锁住。
** ^因此，可以在不阻止其他数据库连接在备份进行时对源数据库进行读写的情况下，
** 对活动的源数据库进行备份。
** 
** ^(要执行备份操作：
**   <ol>
**     <li><b>sqlite3_backup_init()</b> 被调用一次以初始化备份，
**     <li><b>sqlite3_backup_step()</b> 被调用一次或多次以在两个数据库之间传输数据，最后
**     <li><b>sqlite3_backup_finish()</b> 被调用以释放与备份操作相关的所有资源。
**   </ol>)^
** 对于每次成功的sqlite3_backup_init()调用，应该有且仅有一次sqlite3_backup_finish()调用。

** [[sqlite3_backup_init()]] <b>sqlite3_backup_init()</b>
**
** ^sqlite3_backup_init(D,N,S,M)的D和N参数分别是与目标数据库关联的
** [数据库连接]和数据库名称。
** ^数据库名称是"main"表示主数据库，"temp"表示临时数据库，或者是在
** [ATTACH]语句中AS关键字后面的名称表示附加的数据库。
** ^传递给sqlite3_backup_init(D,N,S,M)的S和M参数分别标识源数据库的
** [数据库连接]和数据库名称。
** ^源和目标[数据库连接]（参数S和D）必须不同，否则sqlite3_backup_init(D,N,S,M)
** 将会因错误而失败。
**
** ^如果目标数据库上已经有一个读或读写事务打开，调用sqlite3_backup_init()将失败，
** 返回SQLITE_ERROR。
**
** ^如果在sqlite3_backup_init(D,N,S,M)内部发生错误，则返回NULL，并在目标
** [数据库连接]D中存储错误代码和错误消息。
** ^可以使用[sqlite3_errcode()]、[sqlite3_errmsg()]和/或
** [sqlite3_errmsg16()]函数检索对sqlite3_backup_init()的失败调用的错误代码和消息。
** ^成功的sqlite3_backup_init()调用返回一个指向[sqlite3_backup]对象的指针。
** ^[sqlite3_backup]对象可以与sqlite3_backup_step()和sqlite3_backup_finish()函数一起使用
** 来执行指定的备份操作。
**
** [[sqlite3_backup_step()]] <b>sqlite3_backup_step()</b>
**
** ^函数sqlite3_backup_step(B,N)将在由[sqlite3_backup]对象B指定的源和目标数据库之间
** 复制最多N页。
** ^如果N为负数，则复制所有剩余的源页。
** ^如果sqlite3_backup_step(B,N)成功复制了N页并且还有更多页需要复制，则函数返回[SQLITE_OK]。
** ^如果sqlite3_backup_step(B,N)成功完成从源到目标的全部页面复制，则返回[SQLITE_DONE]。
** ^如果在运行sqlite3_backup_step(B,N)时发生错误，则返回一个[错误代码]。
** ^除了[SQLITE_OK]和[SQLITE_DONE]，调用sqlite3_backup_step()还可能返回[SQLITE_READONLY]、
** [SQLITE_NOMEM]、[SQLITE_BUSY]、[SQLITE_LOCKED]或一个
** [SQLITE_IOERR_ACCESS | SQLITE_IOERR_XXX]扩展错误代码。
**
** ^(sqlite3_backup_step()可能返回[SQLITE_READONLY]如果
** <ol>
** <li> 目标数据库以只读方式打开，或者
** <li> 目标数据库使用写前日志日志记录并且目标源页面大小不同，或者
** <li> 目标数据库是内存数据库并且目标源页面大小不同。
** </ol>)^
**
** ^如果sqlite3_backup_step()无法获取所需的文件系统锁，则调用
** [sqlite3_busy_handler | 忙处理函数]（如果已指定）。
** ^如果忙处理函数在锁可用之前返回非零，则向调用者返回[SQLITE_BUSY]。
** ^在这种情况下，可以稍后重试sqlite3_backup_step()调用。
** ^如果源[数据库连接]在调用sqlite3_backup_step()时正在写入源数据库，则立即返回[SQLITE_LOCKED]。
** ^在这种情况下，可以稍后重试sqlite3_backup_step()调用。
** ^(如果返回[SQLITE_IOERR_ACCESS | SQLITE_IOERR_XXX]、[SQLITE_NOMEM]或
** [SQLITE_READONLY]，则没有重试sqlite3_backup_step()调用的意义。
** 这些错误被视为致命的。)应用程序必须接受备份操作已失败，并将备份操作句柄传递给
** sqlite3_backup_finish()以释放相关资源。
**
** ^第一次调用sqlite3_backup_step()在目标文件上获取一个独占锁。
** ^独占锁直到调用sqlite3_backup_finish()或备份操作完成并且sqlite3_backup_step()返回[SQLITE_DONE]时才释放。
** ^每次调用sqlite3_backup_step()都会在sqlite3_backup_step()调用期间获取源数据库的一个[共享锁]。
** ^由于源数据库在调用sqlite3_backup_step()之间不会被锁住，因此备份过程中源数据库可能会被修改。
** ^如果源数据库被外部进程或通过除备份操作使用的以外的数据库连接修改，则备份将在下一次调用sqlite3_backup_step()时自动重新启动。
** ^如果源数据库是通过与备份操作使用的相同数据库连接修改的，则备份数据库将同时自动更新。
**
** [[sqlite3_backup_finish()]] <b>sqlite3_backup_finish()</b>
**
** 当sqlite3_backup_step()返回[SQLITE_DONE]，或者应用程序希望放弃备份操作时，
** 应用程序应通过传递给sqlite3_backup_finish()来销毁[sqlite3_backup]。
** ^sqlite3_backup_finish()接口释放与[sqlite3_backup]对象相关的所有资源。
** ^如果sqlite3_backup_step()尚未返回[SQLITE_DONE]，则任何在目标数据库上的活动写事务将被回滚。
** 调用sqlite3_backup_finish()后，[sqlite3_backup]对象无效，并且不能使用。
**
** ^sqlite3_backup_finish()返回的值是[SQLITE_OK]，如果没有sqlite3_backup_step()错误发生，
** 无论sqlite3_backup_step()是否完成。
** ^如果在任何先前的sqlite3_backup_step()调用期间发生了内存不足或IO错误，则sqlite3_backup_finish()返回相应的[错误代码]。
**
** ^sqlite3_backup_step()返回的[SQLITE_BUSY]或[SQLITE_LOCKED]不是永久错误，并且不影响sqlite3_backup_finish()的返回值。
**
** [[sqlite3_backup_remaining()]] [[sqlite3_backup_pagecount()]]
** <b>sqlite3_backup_remaining()和sqlite3_backup_pagecount()</b>
**
** ^sqlite3_backup_remaining()例程返回在最近一次sqlite3_backup_step()结束时仍需备份的页面数。
** ^sqlite3_backup_pagecount()例程返回在最近一次sqlite3_backup_step()结束时源数据库中的总页面数。
** ^(这些函数返回的值仅由sqlite3_backup_step()更新。
** 如果源数据库以改变源数据库大小或剩余页面数的方式被修改，则这些更改不会反映在sqlite3_backup_pagecount()和sqlite3_backup_remaining()的输出中，
** 直到下一次sqlite3_backup_step()调用之后。) ^
**
** <b>数据库句柄的并发使用</b>
**
** ^在备份操作正在进行或正在初始化时，源[数据库连接]可用于应用程序的其他目的。
** ^如果SQLite编译并配置为支持线程安全的数据库连接，则源数据库连接可以从其他线程中并发使用。
**
** 但是，应用程序必须保证在sqlite3_backup_init()调用之后和相应的sqlite3_backup_finish()调用之前，
** 不将目标[数据库连接]传递给任何其他API（任何线程）。SQLite目前不会检查应用程序是否错误地访问目标[数据库连接]，
** 因此不会报告错误代码，但操作可能会 malfunction。在备份进行时使用目标数据库连接也可能导致互斥锁死锁。
**
** 如果在[共享缓存模式]下运行，应用程序必须保证在备份运行时不会访问目标数据库使用的共享缓存。
** 实践中这意味着应用程序必须保证备份的磁盘文件不会被进程内的任何连接访问，
** 不仅仅是传递给sqlite3_backup_init()的特定连接。
**
** [sqlite3_backup]对象本身是部分线程安全的。多个线程可以安全地并发调用sqlite3_backup_step()多次。
** 但是，sqlite3_backup_remaining()和sqlite3_backup_pagecount() API严格来说不是线程安全的。
** 如果它们在与另一个线程同时调用sqlite3_backup_step()时被调用，则可能返回无效值。
*/
SQLITE_API sqlite3_backup *SQLITE_STDCALL sqlite3_backup_init(
  sqlite3 *pDest,                        /* 目标数据库句柄 */
  const char *zDestName,                 /* 目标数据库名称 */
  sqlite3 *pSource,                      /* 源数据库句柄 */
  const char *zSourceName                /* 源数据库名称 */
);
SQLITE_API int SQLITE_STDCALL sqlite3_backup_step(sqlite3_backup *p, int nPage);
SQLITE_API int SQLITE_STDCALL sqlite3_backup_finish(sqlite3_backup *p);
SQLITE_API int SQLITE_STDCALL sqlite3_backup_remaining(sqlite3_backup *p);
SQLITE_API int SQLITE_STDCALL sqlite3_backup_pagecount(sqlite3_backup *p);

/*
** CAPI3REF: 解锁通知
** 方法: sqlite3
**
** ^当以共享缓存模式运行时，如果无法获得共享缓存或共享缓存内个别表所需的锁定，数据库操作可能会因[SQLITE_LOCKED]错误而失败。参见[SQLite共享缓存模式]了解共享缓存锁定的描述。
** ^此API可用于注册一个回调，当当前持有必需锁的连接释放它时，SQLite将调用此回调。
** ^只有在库编译时定义了[SQLITE_ENABLE_UNLOCK_NOTIFY] C预处理器符号时，此API才可用。
**
** 另见：[使用SQLite解锁通知功能]。
**
** ^共享缓存锁定在数据库连接结束当前事务时释放，无论是提交还是回滚。
**
** ^当一个连接（称为被阻塞的连接）无法获得共享缓存锁定，并且向调用者返回SQLITE_LOCKED时，持有必需资源的数据库连接（阻塞连接）的身份将被内部存储。
** ^在应用程序收到SQLITE_LOCKED错误后，它可以调用sqlite3_unlock_notify()方法，并将被阻塞的连接句柄作为第一个参数来注册一个回调，当阻塞连接的当前事务结束时将被调用。
** ^回调从结束阻塞连接事务的[sqlite3_step]或[sqlite3_close]调用中调用。
**
** ^(如果在一个多线程应用程序中调用sqlite3_unlock_notify()，有可能在sqlite3_unlock_notify()被调用时，阻塞连接已经结束了其事务。如果发生这种情况，则指定的回调将立即从sqlite3_unlock_notify()的调用中调用。) ^
**
** ^如果被阻塞的连接试图在共享缓存表上获得写锁定，并且当前有多个其他连接持有同一表的读锁定，那么SQLite会任意选择其中一个其他连接作为阻塞连接。
**
** ^(被阻塞的连接最多只能注册一个解锁通知回调。如果当被阻塞的连接已经有一个已注册的解锁通知回调时调用sqlite3_unlock_notify()，则新的回调将替换旧的。) ^
** ^如果sqlite3_unlock_notify()的第二个参数为NULL指针，则取消任何现有的解锁通知回调。^被阻塞连接的解锁通知回调也可以通过使用[sqlite3_close()]关闭被阻塞的连接来取消。
**
** 解锁通知回调不是可重入的。如果应用程序在解锁通知回调内部调用任何sqlite3_xxx API函数，可能会导致崩溃或死锁。
**
** ^除非检测到死锁（见下文），否则sqlite3_unlock_notify()总是返回SQLITE_OK。
**
** <b>回调调用细节</b>
**
** 当注册解锁通知回调时，应用程序提供一个单一的void*指针，当回调被调用时传递给它。然而，回调函数的签名允许SQLite传递一个void*上下文指针数组给它。
** 传递给解锁通知回调的第一个参数是指向void*指针数组的指针，第二个是数组中的条目数。
**
** 当阻塞连接的事务结束时，可能有一个以上的被阻塞连接已注册了解锁通知回调。^如果有两个或更多的被阻塞连接指定了相同的回调函数，那么不会多次调用回调函数，而是将其调用一次，并将被阻塞连接指定的void*上下文指针集合捆绑成一个数组。
** 这使应用程序有机会优先处理与一组未阻塞数据库连接相关的任何操作。
**
** <b>死锁检测</b>
**
** 假设注册了解锁通知回调后，数据库在采取任何进一步操作之前等待回调发出（这是一个合理的假设），那么使用此API可能会导致应用程序死锁。例如，如果连接X正在等待连接Y的事务结束，而连接Y也在等待连接X的事务结束，那么两个连接都不会继续，系统可能会无限期地保持死锁状态。
**
** 为了避免这种情况，sqlite3_unlock_notify()执行死锁检测。^如果给定的sqlite3_unlock_notify()调用会使系统进入死锁状态，则返回SQLITE_LOCKED并且不注册解锁通知回调。如果连接A已注册连接B事务结束时的解锁通知回调，并且连接B也已注册连接A事务结束时的解锁通知回调，则系统被认为处于死锁状态。^间接死锁也会被检测到，所以如果连接B已注册连接C事务结束时的解锁通知回调，而连接C正在等待连接A，则系统也被认为处于死锁状态。^允许任意数量的间接级别。
**
** <b>"DROP TABLE"异常</b>
**
** 当[sqlite3_step()]调用返回SQLITE_LOCKED时，几乎总是应该调用sqlite3_unlock_notify()。然而，有一个例外。当执行"DROP TABLE"或"DROP INDEX"语句时，SQLite会检查是否有属于同一连接的当前正在执行的SELECT语句。如果有，则返回SQLITE_LOCKED。在这种情况下，没有"阻塞连接"，因此调用sqlite3_unlock_notify()会导致解锁通知回调立即被调用。如果应用程序然后重新尝试"DROP TABLE"或"DROP INDEX"查询，可能会结果无限循环。
**
** 解决这个问题的一种方法是检查sqlite3_step()调用返回的扩展错误代码。^(如果有阻塞连接，则扩展错误代码设置为SQLITE_LOCKED_SHAREDCACHE。否则，在特殊的"DROP TABLE/INDEX"情况下，扩展错误代码只是SQLITE_LOCKED。) ^
*/
SQLITE_API int SQLITE_STDCALL sqlite3_unlock_notify(
  sqlite3 *pBlocked,                          /* 等待连接 */
  void (*xNotify)(void **apArg, int nArg),    /* 回调函数调用 */
  void *pNotifyArg                            /* 传递给 xNotify 的参数 */
);


/*
** CAPI3REF: 字符串比较
**
** ^[sqlite3_stricmp()] 和 [sqlite3_strnicmp()] API 允许应用程序和扩展以不区分大小写的方式比较两个包含 UTF-8 字符串的缓冲区的内容，使用 SQLite 在比较标识符时内部使用的相同的“大小写不敏感”定义。
*/
SQLITE_API int SQLITE_STDCALL sqlite3_stricmp(const char *, const char *);
SQLITE_API int SQLITE_STDCALL sqlite3_strnicmp(const char *, const char *, int);

/**
 * CAPI3REF: 字符串模式匹配
 *
 * ^sqlite3_strglob(P, X) 接口在字符串 X 匹配模式 P 时返回零，
 * 如果字符串 X 不匹配模式 P，则返回非零值。^sqlite3_strglob(P, X) 中使用的
 * 模式匹配定义与 SQLite 使用的 SQL 方言中的 "X GLOB P" 操作符相同。
 * ^sqlite3_strglob(P, X) 函数是区分大小写的。
 *
 * 请注意，此例程在匹配时返回零，如果字符串不匹配，则返回非零值，
 * 这与 sqlite3_stricmp() 和 sqlite3_strnicmp() 的行为相同。
 */
SQLITE_API int SQLITE_STDCALL sqlite3_strglob(const char *zGlob, const char *zStr);

/*
** CAPI3REF: 错误日志接口
**
** ^[sqlite3_log()] 接口将消息写入由 [SQLITE_CONFIG_LOG] 选项在 [sqlite3_config()] 中建立的 [错误日志]。
** ^如果启用了日志记录，zFormat 字符串和后续参数将与 [sqlite3_snprintf()] 一起使用以生成最终的输出字符串。
**
** [sqlite3_log()] 接口旨在供扩展使用，例如虚拟表、排序函数和 SQL 函数。虽然没有任何东西可以阻止应用程序调用 sqlite3_log()，但这样做被认为是不良形式。
**
** zFormat 字符串不能为 NULL。
**
** 为了避免死锁和其他线程问题，sqlite3_log() 例程不会使用动态分配的内存。日志消息存储在堆栈上的固定长度缓冲区中。如果日志消息超过几百个字符，它将被截断为缓冲区的长度。
*/
SQLITE_API void SQLITE_CDECL sqlite3_log(int iErrCode, const char *zFormat, ...);

/**
** CAPI3REF: 写前日志提交钩子
** 方法: sqlite3
**
** ^[sqlite3_wal_hook()] 函数用于注册一个回调，每次在 wal 模式下向数据库提交数据时都会调用该回调。
**
** ^(在提交完成后，SQLite 会调用回调，此时与数据库关联的写锁已被释放)^，因此实现可以根据需要读取、写入或[检查点]数据库。
**
** ^当回调被调用时，传递给回调函数的第一个参数是注册回调时传递给 sqlite3_wal_hook() 的第三个参数的副本。^第二个参数是数据库句柄的副本。^第三个参数是被写入的数据库的名称 - 要么是 "main"，要么是 [ATTACH] 的数据库的名称。^第四个参数是写前日志文件中当前页面的数量，包括刚刚提交的那些。
**
** 回调函数通常应返回 [SQLITE_OK]。^如果返回错误代码，该错误将传播回 SQLite 代码库，导致触发回调的语句报告错误，尽管提交仍然会发生。如果回调返回 [SQLITE_ROW] 或 [SQLITE_DONE]，或者返回不对应任何有效 SQLite 错误代码的值，结果将是未定义的。
**
** 单个数据库句柄一次最多只能注册一个写前日志回调。^调用 [sqlite3_wal_hook()] 会替换之前注册的任何写前日志回调。^请注意，[sqlite3_wal_autocheckpoint()] 接口和 [wal_autocheckpoint pragma] 都会调用 [sqlite3_wal_hook()]，并且会覆盖任何之前的 [sqlite3_wal_hook()] 设置。
*/
SQLITE_API void *SQLITE_STDCALL sqlite3_wal_hook(
  sqlite3*, 
  int(*)(void *,sqlite3*,const char*,int),
  void*
);

/**
** CAPI3REF: 配置自动检查点
** 方法: sqlite3
**
** ^sqlite3_wal_autocheckpoint(D,N) 是一个围绕 sqlite3_wal_hook() 的包装器，
** 它使得在 [database connection] D 上的任何数据库在提交事务后自动 [checkpoint]，
** 如果 [write-ahead log] 文件中有 N 个或更多的帧。^将 nFrame 参数传递为零或
** 负值将完全禁用自动检查点。
**
** ^此函数注册的回调将替换使用 sqlite3_wal_hook() 注册的任何现有回调。
** ^同样，使用 sqlite3_wal_hook() 注册回调会禁用此函数配置的自动检查点机制。
**
** ^可以使用 [wal_autocheckpoint pragma] 从 SQL 调用此接口。
**
** ^由此机制启动的检查点是 PASSIVE 的 [sqlite3_wal_checkpoint_v2]。
**
** ^每个新的 [database connection] 默认启用自动检查点，阈值为 1000 或
** [SQLITE_DEFAULT_WAL_AUTOCHECKPOINT] 页。只有在发现默认设置对特定应用程序
** 不够优化时，才需要使用此接口。
*/
SQLITE_API int SQLITE_STDCALL sqlite3_wal_autocheckpoint(sqlite3 *db, int N);

/*
** CAPI3REF: 检查点数据库
** 方法: sqlite3
**
** ^(sqlite3_wal_checkpoint(D,X) 等同于
** [sqlite3_wal_checkpoint_v2](D,X,[SQLITE_CHECKPOINT_PASSIVE],0,0)。)^
**
** 简而言之，sqlite3_wal_checkpoint(D,X) 会导致数据库 X 在 [数据库连接] D 上的
** [预写日志] 中的内容被转移到数据库文件中，并重置预写日志。参见 [检查点]
** 文档以获取更多信息。
**
** 这个接口曾经是触发检查点的唯一方式。但后来添加了更强大、更灵活的
** [sqlite3_wal_checkpoint_v2()] 接口。保留此接口是为了向后兼容，并为需要手动
** 启动回调但不需 [sqlite3_wal_checkpoint_v2()] 的全部功能（及相应复杂性）的应用程序提供便利。
*/
SQLITE_API int SQLITE_STDCALL sqlite3_wal_checkpoint(sqlite3 *db, const char *zDb);

/**
 ** CAPI3REF: 检查点数据库
 ** METHOD: sqlite3
 **
 ** ^(sqlite3_wal_checkpoint_v2(D,X,M,L,C) 接口在模式 M 下对 [数据库连接] D 的数据库 X 运行检查点操作。
 ** 状态信息将写回到 L 和 C 指向的整数中。) ^
 ** ^(M 参数必须是有效的 [检查点模式]:) ^
 **
 ** <dl>
 ** <dt>SQLITE_CHECKPOINT_PASSIVE<dd>
 **   ^在不等待任何数据库读者或写者完成的情况下，尽可能多地检查帧，然后如果日志中的所有帧都已检查点，则同步数据库文件。 ^在 SQLITE_CHECKPOINT_PASSIVE 模式下，[忙碌处理程序回调] 永远不会被调用。 ^另一方面，如果存在并发的读者或写者，被动模式可能会留下未完成的检查点。
 **
 ** <dt>SQLITE_CHECKPOINT_FULL<dd>
 **   ^此模式会阻塞（调用 [sqlite3_busy_handler|忙碌处理程序回调]），直到没有数据库写者并且所有读者都在读取最新的数据库快照。 ^然后检查日志文件中的所有帧并同步数据库文件。 ^此模式在挂起时会阻塞新的数据库写者，但允许新的数据库读者继续无阻碍地操作。
 **
 ** <dt>SQLITE_CHECKPOINT_RESTART<dd>
 **   ^此模式与 SQLITE_CHECKPOINT_FULL 相同，只是检查日志文件后，它会阻塞（调用 [忙碌处理程序回调]），直到所有读者都在从数据库文件中读取。 ^这确保下一个写者将从日志文件的开始处重新启动。 ^与 SQLITE_CHECKPOINT_FULL 一样，此模式在挂起时会阻塞新的数据库写者尝试，但不会阻碍读者。
 **
 ** <dt>SQLITE_CHECKPOINT_TRUNCATE<dd>
 **   ^此模式与 SQLITE_CHECKPOINT_RESTART 相同，只是成功返回之前将日志文件截断为零字节。
 ** </dl>
 **
 ** ^如果 pnLog 不为 NULL，则 *pnLog 被设置为日志文件中的总帧数，或者如果由于错误或数据库不在 [WAL 模式] 下而无法运行检查点，则设置为 -1。 ^如果 pnCkpt 不为 NULL，则 *pnCkpt 被设置为日志文件中已检查点的总帧数（包括在调用函数之前已经检查点的任何帧），或者如果由于错误或数据库不在 WAL 模式下而无法运行检查点，则设置为 -1。 ^请注意，在成功完成 SQLITE_CHECKPOINT_TRUNCATE 后，日志文件将被截断为零字节，因此 *pnLog 和 *pnCkpt 都将被设置为零。
 **
 ** ^所有调用都会在数据库文件上获取一个独占的 "检查点" 锁。 ^如果同时有其他进程正在运行检查点操作，则无法获取该锁，并返回 SQLITE_BUSY。 ^即使配置了忙碌处理程序，在这种情况下也不会被调用。
 **
 ** ^SQLITE_CHECKPOINT_FULL、RESTART 和 TRUNCATE 模式还会在数据库文件上获取独占的 "写者" 锁。 ^如果无法立即获取写者锁，并且配置了忙碌处理程序，则会调用它并重试写者锁，直到忙碌处理程序返回 0 或成功获取锁。 ^在等待数据库读者时也会调用忙碌处理程序，如上所述。 ^如果忙碌处理程序在获取写者锁之前返回 0 或在等待数据库读者时返回 0，则检查点操作将从该点继续，就像 SQLITE_CHECKPOINT_PASSIVE 一样 - 尽可能多地检查帧而不阻塞进一步的检查。 ^在这种情况下返回 SQLITE_BUSY。
 **
 ** ^如果参数 zDb 为 NULL 或指向一个空字符串，则尝试对连接到 [数据库连接] db 的所有 WAL 数据库执行指定的操作。 在这种情况下，输出参数 *pnLog 和 *pnCkpt 的值是未定义的。 ^如果在处理一个或多个附加的 WAL 数据库时遇到 SQLITE_BUSY 错误，仍然会尝试对任何剩余的附加数据库执行操作，并在结束时返回 SQLITE_BUSY。 ^如果在处理附加数据库时发生任何其他错误，则放弃处理并立即将错误代码返回给调用者。 ^如果在处理附加数据库时没有遇到错误（SQLITE_BUSY 或其他），则返回 SQLITE_OK。
 **
 ** ^如果数据库 zDb 是不在 WAL 模式下的附加数据库的名称，则返回 SQLITE_OK，并且 *pnLog 和 *pnCkpt 都设置为 -1。 ^如果 zDb 不是 NULL（或空字符串）并且不是任何附加数据库的名称，则返回 SQLITE_ERROR 给调用者。
 **
 ** ^除非返回 SQLITE_MISUSE，
 ** sqlite3_wal_checkpoint_v2() 接口
 ** 会设置由 [sqlite3_errcode()] 和 [sqlite3_errmsg()] 查询的错误信息。
 **
 ** ^可以使用 [PRAGMA wal_checkpoint] 命令从 SQL 调用此接口。
 */
SQLITE_API int SQLITE_STDCALL sqlite3_wal_checkpoint_v2(
  sqlite3 *db,                    /* 数据库句柄 */
  const char *zDb,                /* 附加数据库的名称（或 NULL） */
  int eMode,                      /* SQLITE_CHECKPOINT_* 值 */
  int *pnLog,                     /* 输出：WAL 日志的帧数 */
  int *pnCkpt                     /* 输出：已检查点的总帧数 */
);

/*
** CAPI3REF: 检查点模式值
** KEYWORDS: {checkpoint mode}
**
** 这些常量定义了作为第三个参数传递给 [sqlite3_wal_checkpoint_v2()] 接口的
** "检查点模式" 的所有有效值。有关每个检查点模式的含义，请参阅
** [sqlite3_wal_checkpoint_v2()] 的文档。
*/
#define SQLITE_CHECKPOINT_PASSIVE  0  /* 尽可能不阻塞地执行操作 */
#define SQLITE_CHECKPOINT_FULL     1  /* 等待写入者，然后执行检查点 */
#define SQLITE_CHECKPOINT_RESTART  2  /* 类似 FULL，但同时等待读取者 */
#define SQLITE_CHECKPOINT_TRUNCATE 3  /* 类似 RESTART，但还会截断 WAL */

/*
** CAPI3REF: 虚拟表接口配置
**
** 此函数可由虚拟表实现的 [xConnect] 或 [xCreate] 方法调用，
** 以配置虚拟表接口的各个方面的特性。
**
** 如果在此函数在 xConnect 或 xCreate 虚拟表方法上下文之外被调用，
** 则行为是未定义的。
**
** 目前，使用此函数可配置的选项只有一个。（参见 [SQLITE_VTAB_CONSTRAINT_SUPPORT]。）
** 以后可能会添加更多选项。
*/
SQLITE_API int SQLITE_CDECL sqlite3_vtab_config(sqlite3*, int op, ...);

/*
** CAPI3REF: 虚拟表配置选项
**
** 这些宏定义了[sqlite3_vtab_config()]接口的各种选项，[虚拟表]实现可以使用这些选项来定制和优化它们的行为。
**
** <dl>
** <dt>SQLITE_VTAB_CONSTRAINT_SUPPORT
** <dd>支持形如[sqlite3_vtab_config](db,SQLITE_VTAB_CONSTRAINT_SUPPORT,X)的调用，其中X是一个整数。如果X为零，则调用[xCreate]或[xConnect]方法并调用[sqlite3_vtab_config()]的[虚拟表]不支持约束。在这种配置下（这也是默认配置），如果[xUpdate]方法的调用返回[SQLITE_CONSTRAINT]，则整个语句将被回滚，就像在用户的SQL语句中指定了[ON CONFLICT | OR ABORT]一样，而不管实际指定的ON CONFLICT模式是什么。
**
** 如果X不为零，则虚拟表实现保证，如果[xUpdate]返回[SQLITE_CONSTRAINT]，它将在对内部或持久数据结构进行任何修改之前这样做。如果[ON CONFLICT]模式是ABORT、FAIL、IGNORE或ROLLBACK，SQLite能够回滚语句或数据库事务，并根据需要放弃或继续处理当前的SQL语句。如果ON CONFLICT模式是REPLACE，并且[xUpdate]方法返回[SQLITE_CONSTRAINT]，SQLite将其处理为ON CONFLICT模式为ABORT。
**
** 必须处理OR REPLACE的虚拟表实现必须在[xUpdate]方法中这样做。如果调用[sqlite3_vtab_on_conflict()]函数指示当前的ON CONFLICT策略是REPLACE，虚拟表实现应在xUpdate回调中静默地替换适当的行并返回SQLITE_OK。或者，如果这是不可能的，它可能返回SQLITE_CONSTRAINT，在这种情况下，SQLite回退到OR ABORT约束处理。
** </dl>
*/
#define SQLITE_VTAB_CONSTRAINT_SUPPORT 1

/*
** CAPI3REF: Determine The Virtual Table Conflict Policy
**
** 这个函数只能在调用 [virtual table] 实现的 [xUpdate] 方法的调用中调用。
** 返回的值是 [SQLITE_ROLLBACK]、[SQLITE_IGNORE]、[SQLITE_FAIL]、[SQLITE_ABORT] 或 [SQLITE_REPLACE] 中的一个，
** 根据触发对 [virtual table] 的 [xUpdate] 方法调用的 SQL 语句的 [ON CONFLICT] 模式。
*/
SQLITE_API int SQLITE_STDCALL sqlite3_vtab_on_conflict(sqlite3 *);

/*
** CAPI3REF: 冲突解决模式
** 关键词: {冲突解决模式}
**
** 这些常量由 [sqlite3_vtab_on_conflict()] 返回，以通知 [虚拟表] 实现当前正在评估的 SQL 语句的 [ON CONFLICT] 模式。
**
** 注意，[SQLITE_IGNORE] 常量也用作 [sqlite3_set_authorizer()] 回调的潜在返回值，而 [SQLITE_ABORT] 也是一个 [结果代码]。
*/
#define SQLITE_ROLLBACK 1
/* #define SQLITE_IGNORE 2 // 也被 sqlite3_authorizer() 回调使用 */
#define SQLITE_FAIL     3
/* #define SQLITE_ABORT 4  // 也是一个错误码 */
#define SQLITE_REPLACE  5

/*
** CAPI3REF: 预备语句扫描状态操作码
** 关键词: {scanstatus 选项}
**
** 以下常量可用于[sqlite3_stmt_scanstatus(S,X,T,V)]接口的T参数。每个常量指定了sqlite3_stmt_scanstatus()返回的不同指标。
**
** 当返回给V的值是字符串时，由预备语句S管理的空间将自动在S被最终化时释放。
**
** <dl>
** [[SQLITE_SCANSTAT_NLOOP]] <dt>SQLITE_SCANSTAT_NLOOP</dt>
** <dd>^指向T参数的[sqlite3_int64]变量将被设置为X-th循环运行的总次数。</dd>
**
** [[SQLITE_SCANSTAT_NVISIT]] <dt>SQLITE_SCANSTAT_NVISIT</dt>
** <dd>^指向T参数的[sqlite3_int64]变量将被设置为X-th循环所有迭代检查的总行数。</dd>
**
** [[SQLITE_SCANSTAT_EST]] <dt>SQLITE_SCANSTAT_EST</dt>
** <dd>^指向T参数的"double"变量将被设置为查询计划器对X-th循环每次迭代输出的平均行数的估计。如果查询计划器的估计是准确的，那么这个值将接近NVISIT/NLOOP的商，并且所有先前循环具有相同SELECTID的此值的乘积将是当前循环的NLOOP值。</dd>
**
** [[SQLITE_SCANSTAT_NAME]] <dt>SQLITE_SCANSTAT_NAME</dt>
** <dd>^指向T参数的"const char *"变量将被设置为包含用于X-th循环的索引或表名的以零终止的UTF-8字符串。</dd>
**
** [[SQLITE_SCANSTAT_EXPLAIN]] <dt>SQLITE_SCANSTAT_EXPLAIN</dt>
** <dd>^指向T参数的"const char *"变量将被设置为包含X-th循环的[EXPLAIN QUERY PLAN]描述的以零终止的UTF-8字符串。</dd>
**
** [[SQLITE_SCANSTAT_SELECTID]] <dt>SQLITE_SCANSTAT_SELECT</dt>
** <dd>^指向T参数的"int"变量将被设置为X-th循环的"select-id"。select-id标识循环是哪个查询或子查询的一部分。主查询的select-id为零。select-id的值与[EXPLAIN QUERY PLAN]查询的第一列输出的值相同。</dd>
** </dl>
*/
#define SQLITE_SCANSTAT_NLOOP    0
#define SQLITE_SCANSTAT_NVISIT   1
#define SQLITE_SCANSTAT_EST      2
#define SQLITE_SCANSTAT_NAME     3
#define SQLITE_SCANSTAT_EXPLAIN  4
#define SQLITE_SCANSTAT_SELECTID 5

/*
** CAPI3REF: 预备语句扫描状态
** METHOD: sqlite3_stmt
**
** 此接口返回有关pStmt的预测和测量性能的信息。高级应用程序可以使用此
** 接口来比较预测和测量性能，并在发现差异时发出警告和/或重新运行 [ANALYZE]。
**
** 由于此接口预期很少使用，因此只有在使用 [SQLITE_ENABLE_STMT_SCANSTATUS]
** 编译时选项编译SQLite时才可用。
**
** "iScanStatusOp" 参数确定要返回的状态信息。 "iScanStatusOp" 必须是
** [scanstatus 选项] 之一，否则此接口的行为是未定义的。
** ^请求的测量值将被写入由 "pOut" 参数指向的变量中。
** 参数 "idx" 标识要检索统计信息的特定循环。
** 循环从零开始编号。 ^如果 idx 超出范围 - 小于零或大于或等于用于实现
** 语句的总循环数 - 则返回非零值，并且 pOut 指向的变量保持不变。
**
** ^并非所有语句的所有循环都有统计信息可用。 ^在存在没有可用统计信息的循环的情况下，
** 此函数的行为就像循环不存在一样 - 它返回非零并保持 pOut 指向的变量不变。
**
** 另见：[sqlite3_stmt_scanstatus_reset()]
*/
SQLITE_API int SQLITE_STDCALL sqlite3_stmt_scanstatus(
  sqlite3_stmt *pStmt,      /* 需要信息的预处理语句 */
  int idx,                  /* 要报告的循环索引 */
  int iScanStatusOp,        /* 所需的信息。SQLITE_SCANSTAT_* */
  void *pOut                /* 结果写入此处 */
);     

/*
** CAPI3REF: 清零扫描状态计数器
** 方法: sqlite3_stmt
**
** ^清零所有与 [sqlite3_stmt_scanstatus()] 相关的事件计数器。
**
** 此API仅在库以预处理器符号 [SQLITE_ENABLE_STMT_SCANSTATUS] 定义的情况下可用。
*/
SQLITE_API void SQLITE_STDCALL sqlite3_stmt_scanstatus_reset(sqlite3_stmt*);


/*
** 撤销将浮点类型转换为整型的操作，
** 以适应在不支持浮点运算的处理器上进行构建。
*/
#ifdef SQLITE_OMIT_FLOATING_POINT
# undef double
#endif

#ifdef __cplusplus
}  /* 结束 'extern "C"' 块 */
#endif
#endif /* _SQLITE3_H_ */

/*
** 2010年8月30日
**
** 作者放弃对此源代码的版权。代替法律声明，这里有一句祝福：
**
**    愿你行善而不是作恶。
**    愿你原谅自己并原谅他人。
**    愿你自由分享，永不取多予少。
**
*************************************************************************
*/

#ifndef _SQLITE3RTREE_H_
#define _SQLITE3RTREE_H_


#ifdef __cplusplus
extern "C" {
#endif

typedef struct sqlite3_rtree_geometry sqlite3_rtree_geometry;
typedef struct sqlite3_rtree_query_info sqlite3_rtree_query_info;

/* RTree使用的双精度数据类型取决于编译时选项SQLITE_RTREE_INT_ONLY。 */
#ifdef SQLITE_RTREE_INT_ONLY
  typedef sqlite3_int64 sqlite3_rtree_dbl;
#else
  typedef double sqlite3_rtree_dbl;
#endif

/*
** 注册一个名为 zGeom 的几何回调函数，该函数可以作为 R-树几何查询的一部分使用，
** 具体用法如下：
**
**   SELECT ... FROM <rtree> WHERE <rtree col> MATCH $zGeom(... 参数 ...)
*/
SQLITE_API int SQLITE_STDCALL sqlite3_rtree_geometry_callback(
  sqlite3 *db,
  const char *zGeom,
  int (*xGeom)(sqlite3_rtree_geometry*, int, sqlite3_rtree_dbl*,int*),
  void *pContext
);

/*
** 指向以下类型的结构体的指针作为第一个参数传递给使用 rtree_geometry_callback() 注册的回调函数。
*/
struct sqlite3_rtree_geometry {
  void *pContext;                 /* 复制的 pContext 传递给 s_r_g_c() 函数 */
  int nParam;                     /* 数组 aParam[] 的大小 */
  sqlite3_rtree_dbl *aParam;      /* 参数传递给 SQL 几何函数 */
  void *pUser;                    /* 实现回调的用户数据 */
  void (*xDelUser)(void *);       /* 被SQLite调用以清理pUser */
};

/*
** 注册一个名为 zScore 的第二代几何回调，它可以作为 R-树几何查询的一部分使用，
** 具体用法如下：
**
**   SELECT ... FROM <rtree> WHERE <rtree col> MATCH $zQueryFunc(... params ...)
*/
SQLITE_API int SQLITE_STDCALL sqlite3_rtree_query_callback(
  sqlite3 *db,
  const char *zQueryFunc,
  int (*xQueryFunc)(sqlite3_rtree_query_info*),
  void *pContext,
  void (*xDestructor)(void*)
);


/*
** 一个指向以下类型的结构体的指针作为参数传递给使用
** sqlite3_rtree_query_callback() 注册的得分几何回调函数。
**
** 注意，此结构体的前5个字段与sqlite3_rtree_geometry相同。
** 这个结构体是sqlite3_rtree_geometry的子类。
*/
struct sqlite3_rtree_query_info {
	void *pContext;                   /* 注册函数时的 pContext */
	int nParam;                       /* 函数参数的数量 */
	sqlite3_rtree_dbl *aParam;        /* 函数参数的值 */
	void *pUser;                      /* 回调函数可以根据需要使用此字段 */
	void (*xDelUser)(void*);          /* 用于释放 pUser 的函数 */
	sqlite3_rtree_dbl *aCoord;        /* 要检查的节点或条目的坐标 */
	unsigned int *anQueue;            /* 队列中待处理条目的数量 */
	int nCoord;                       /* 坐标的数量 */
	int iLevel;                       /* 当前节点或条目的层级 */
	int mxLevel;                      /* 树中最大的 iLevel 值 */
	sqlite3_int64 iRowid;             /* 当前条目的 Rowid */
	sqlite3_rtree_dbl rParentScore;   /* 父节点的分数 */
	int eParentWithin;                /* 父节点的可见性 */
	int eWithin;                      /* 输出：可见性 */
	sqlite3_rtree_dbl rScore;         /* 输出：在此写入分数 */
	/* 以下字段仅在 3.8.11 及更高版本中可用 */
	sqlite3_value **apSqlParam;       /* 参数的原始 SQL 值 */
};

/*
** 允许的值用于 sqlite3_rtree_query.eWithin 和 .eParentWithin。
*/
#define NOT_WITHIN       0   /* 对象完全在查询区域之外 */
#define PARTLY_WITHIN    1   /* 对象部分覆盖查询区域 */
#define FULLY_WITHIN     2   /* 对象完全包含在查询区域内 */


#ifdef __cplusplus
}  /* 结束 'extern "C"' 块 */
#endif

#endif  /* ifndef _SQLITE3RTREE_H_ */

