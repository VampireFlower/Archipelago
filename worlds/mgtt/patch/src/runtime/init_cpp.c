void __init_cpp() {
    extern void (*ctors)();
    auto func = &ctors; 
    
    while (*func)
        (*func++)();
}