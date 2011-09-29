1.  To get the socket code to compile in Visual Studio 2008, click Project >> Properties >> Linker >> Input, and then in the Additional Dependencies box, add "wsock32.lib" without the quotes. This brings in the windows socket library, and you will get linker errors without it.
2.  Also, the VS 2008 Feature Pack is required.
3.  Current code requires that server is running