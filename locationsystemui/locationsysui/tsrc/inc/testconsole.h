#include <QtTest/QtTest>
 
#ifndef S60UNITTEST_H_
#define S60UNITTEST_H_
 
#define QTEST_MAIN_S60(TestObject) \
int main(int argc, char *argv[]) \
{ \
char *new_argv[3]; \
QApplication app(argc, argv); \
\
QString str = "C:\\data\\" + QFileInfo(QCoreApplication::applicationFilePath()).baseName() + ".log"; \
QByteArray   bytes  = str.toAscii(); \
\
char arg1[] = "-o"; \
\
new_argv[0] = argv[0]; \
new_argv[1] = arg1; \
new_argv[2] = bytes.data(); \
\
TestObject tc; \
return QTest::qExec(&tc, 3, new_argv); \
}
#endif /* S60UNITTEST_H_ */
