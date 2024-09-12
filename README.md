# DLL-Injection
**Código que injeta uma DLL no espaço virtual de um processo via thread remota**.

**Uso**: `./main.exe mylib.dll exemplo.exe`

Para confirmar que funcionou pode-se usar um debugger ou simplesmente escrever uma função DLLMain na DLL e tratar os casos de PROCESS_ATTACH e PROCEESS_DETACH. 

Só funciona em casos onde o processo-alvo já tem importado a biblioteca kernel32.dll (absoluta maioria), pois é ela que possui a função LoadLibraryA. 
