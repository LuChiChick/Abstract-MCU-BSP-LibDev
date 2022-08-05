//纯虚数函数服务处理函数，没这玩意无法编译链接
extern "C" void __cxa_pure_virtual()
{
    //调试用死循环
    while (1)
        ;
}