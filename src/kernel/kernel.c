char *const screen = (char *)0xb8000;
const char greeting[] = "AmorFatiOS v0.0.1";

void _start(void)
{
    for (int i = 0; i < sizeof(greeting); i++)
    {
        screen[i * 2] = greeting[i];
    }
    while (1);
}
