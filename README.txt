MODO DE UTILIZAÇÃO:

-> Compactação
	Para compactar os dados, o programa deve ser chamado, via linha de comando, da
	seguinte forma: nome_programa arq_origem.txt arq_destino.bin.

		Ex: GGzip Arquivo_Teste.txt Arquivo_teste.bin

	O programa gera duas saidas:
		Compactação 1: (Tamanho do codigo obtido)/(Tamanho do codigo original)
		Compactação 2: (Tamanho do arquivo obtido)/(Tamanho do arquivo original)

-> Descompactação
	Para descompactar, o programa deve ser chamado da seguinte forma:
	nome_programa arq_compactado.bin.

		Ex: GGzip Arquivo_teste.bin

Desenvolvido a fim de cumprimento da Atividade Acadêmica de Estrutura de Dados I - UFRRJ por Gabriel Antonio, Gabriela Almeida e Vinícius Ayres.