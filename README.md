# DLL-Injection
**Código que injeta uma DLL no espaço virtual de um processo via thread remota**.

Para usar é só trocar o caminho dos arquivos na main, sendo primeiro a DLL e segundo o executável. 
Para confirmar que funcionou pode-se usar um debugger ou simplesmente escrever uma função DLLMain na DLL e tratar os casos de PROCESS_ATTACH e PROCEESS_DETACH. 
