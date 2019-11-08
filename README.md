# extios

这个库是对标准输入输出操作的扩展，可以对ANSI、宽字符、UTF-8、UTF-16、UTF-32编码的字符串进行输入输出操作。适用于Windows和Linux。

    #include <extios/extiostream.h>
    #include <string>

    int main(void)
    {
        auto text = std::u32string(U"你好骚啊👌");
        extios::u32cout << text << std::endl;

        std::u16string text2;
        extios::u16cin >> text2;
        extios::u16cout << text2 << std::endl;

        return 0;
    }

使用`extios::u32cout`对象可以输出UTF-32编码的字符串，使用`extios::u16cin`对象可以将控制台/终端输入的字符串保存为UTF-16编码的字符串。

类似的对象有以下这些：

+ `extios::cout`：专用于输出ANSI字符串
+ `extios::wcout`：专用于输出宽字符字符串
+ `extios::u8cout`：专用于输出UTF-8编码的字符串
+ `extios::u16cout`：专用于输出UTF-16编码的字符串
+ `extios::u32cout`：专用于输出UTF-32编码的字符串
+ `extios::cin`：专用于输入ANSI字符串
+ `extios::wcin`：专用于输入宽字符字符串
+ `extios::u8cin`：专用于输入UTF-8编码的字符串
+ `extios::u16cin`：专用于输入UTF-16编码的字符串
+ `extios::u32cin`：专用于输入UTF-32编码的字符串
+ `extios::wcin`：专用于输入宽字符字符串
