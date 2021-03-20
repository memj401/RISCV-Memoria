/*Trabalho 1 de OAC - Implementação de Funções de acesso à Memória do RISCV RV321 em Linguagem C
 *Aluno: Marcos Eduardo Monteiro Junqueira
 *Matrícula: 18/0023691
 *Turma: 01A
 *IDE Utilizada: Sublime Text 3
 *Sistema Operacional Utilizado: Windows 10
 *Compilador Utilizado: GCC
 */

#include <stdio.h>
#include <stdint.h>
#define MEM_SIZE 4096
int32_t mem[MEM_SIZE];

int32_t lw(uint32_t address, uint32_t kte){
	int32_t indice;

	if (address % 4 != 0){
		
		printf("Endereco Invalido\n"); // Verificação se o endereço é válido
		return 0;
	}

	if (kte % 4 != 0){
		
		printf("Deslocamento Invalido\n"); // Verificação se o deslocamento é válido
		return 0;
	}

	indice = address + kte; // Pelas validações anteriores, garante-se que o índice será sempre divisível por 4
	indice /=4;
	
	if (indice > MEM_SIZE-1 || indice < 0){
		
		printf("Endereco Fora da Memoria\n");
		return 0;
	}
	return mem[indice];
};

int32_t lb(uint32_t address, int32_t kte){
	int32_t palavra;
	int32_t resultado;
	int32_t mascara = 0x000000FF; // Numero 32-bit utilizado para isolamento do byte desejado, no caso de kte = 0 (Byte menos significativo)
	
	if (address % 4  != 0 || address/4 > (MEM_SIZE - 1) || address/4 < 0 ){
		
		printf("Endereco Invalido\n");
		return 0;
	}

	if (kte < 0 || kte > 3){
		
		printf("Byte Selecionado Invalido\n");
		return 0;
	}
	
	palavra = lw(address, 0);
	mascara = mascara << (kte*8); // Conversão da mascara para 0x0000FF00, 0x00FF0000 ou 0xFF000000 dependendo do byte desejado
	resultado = palavra & mascara; // AND bit a bit para separação do byte
	resultado  = resultado >> (kte*8); // Shift para a direita do byte desejado
	resultado = (int8_t) resultado; // Casting para um byte a fim estender o sinal do resultado
	
	return resultado;

};

int32_t lbu(uint32_t address, int32_t kte){
	int32_t palavra;
	int32_t resultado;
	int32_t mascara = 0x000000FF; // Numero 32-bit utilizado para isolamento do byte desejado, no caso de kte = 0 (Byte menos significativo)
	
	if (address % 4  != 0 || address/4 > (MEM_SIZE - 1) || address/4 < 0 ){
		
		printf("Endereco Invalido\n");
		return 0;
	}

	if (kte < 0 || kte > 3){
		
		printf("Byte Selecionado Invalido\n");
		return 0;
	}

	palavra = lw(address, 0);
	mascara =mascara << (kte*8); // Conversão da mascara para 0x0000FF00, 0x00FF0000 ou 0xFF000000 dependendo do byte desejado
	resultado = palavra & mascara; // AND bit a bit para separação do byte
	resultado = resultado >> (kte*8); // Shift para a direita do byte desejado
	
	/*No caso de se desejar o byte mais significativo, o shift para a direita estende o bit mais significativo da palavra, 
	*podendo ser 1 no caso de valores iguais ou superiores a 0x8000000000, assim necessitando de uma possível correção do sinal*/
	if (kte == 3){ 
		mascara = 0x000000FF; // Máscara para correção do sinal
		resultado = resultado & mascara; // Operação AND para correção do sinal
	}
	
	return resultado;
};

void sw(uint32_t address, int32_t kte, int32_t dado){
	int32_t indice = address + kte; // Cálculo do índice da memória utilizado anteriormente
	indice /=4;

	if (address % 4 != 0){
		
		printf("Endereco Invalido\n");

	}
	else if (kte % 4 != 0){
		
		printf("Deslocamento Invalido\n");
	}
	
	else if (indice > (MEM_SIZE-1) || indice < 0){
		
		printf("Endereco Fora da Memoria\n");
	}
	else {
		mem[indice] = dado;
	}
	
};

void sb(uint32_t address, int32_t kte, int8_t dado){
	int32_t dado_32bit;
	int32_t mascara_dado =  0x000000FF; // Numero 32-bit utilizado para isolamento do byte desejado, no caso de kte = 0 (Byte menos significativo)
	int32_t mascara_palavra = 0xFFFFFFFF; // Numero 32-bit utilizado para exclusão do byte desejado
	int32_t offset = 0x000000FF; // Valor a ser retirado da máscara da palavra
	int32_t palavra; 

	if (address % 4 != 0 || address/4 > (MEM_SIZE-1) || address/4 < 0){
		
		printf("Endereco Invalido\n");

	}
	else if (kte < 0 || kte > 3){
		
		printf("Byte Selecionado Invalido\n");
	}
	
	else {
		dado_32bit = (int32_t) dado; // Conversão do dado para um número de 32 bits
		dado_32bit = dado_32bit << (kte*8); // Shift do byte para a posição desejada
		mascara_dado = mascara_dado << (kte*8); // Conversão da máscara para o valor apropriado
		dado_32bit = dado_32bit & mascara_dado; // AND bit a bit para isolar o byte necessário
		palavra = mem[address/4]; // Acessar a palavra pelo calculo de índice utilizado anteriormente 
		offset = offset << (8*kte); //Alinhamento do offset com a posição do byte desejado
		mascara_palavra -= offset; // Mudança do valor da máscara a fim de retirar o byte desejado da palavra
		palavra = palavra & mascara_palavra; // AND bit a bit para retirar o byte necessario
		palavra += dado_32bit; // Operação de soma entre a palavra e o byte fornecido, resultando na alteração do byte escolhido para o valor desejado
		mem[address/4] = palavra; // Salvando a palavra atualizada na memória
	}	
};

int main(){
	//Testes Sugeridos
	printf("Testes Sugeridos\n\n");
	sb(0, 0, 0x04); 
	sb(0, 1, 0x03);
	sb(0, 2, 0x02); 
	sb(0, 3, 0x01);
	sb(4, 0, 0xFF); 
	sb(4, 1, 0xFE); 
	sb(4, 2, 0xFD); 
	sb(4, 3, 0xFC);
	sw(12, 0, 0xFF);
	sw(16, 0, 0xFFFF);
	sw(20, 0, 0xFFFFFFFF);
	sw(24, 0, 0x80000000);
	
	for (int i = 0; i < 7; ++i){
		
		printf("Memoria[%d] = %08x\n", i, mem[i]);
	}
	
	printf("\n");
	
	for (int i = 0; i < 4; ++i){
		
		printf("lb: %08x\n", lb(4,i));
		printf("lbu: %08x\n",lbu(4,i));
	}
	
	printf("\n");
	
	for (int i = 3; i < 6 ; ++i){
		
		printf("Memoria[%d] =  %08x\n", i, lw(4*i,0));
	}
	printf("\n");

	//Testes Realizados pelo Aluno
	
	// 1° Teste - Validação de Entrada das Funções
	printf("Teste Validacao das Entradas\n\n"); 

	printf("Teste lw\n\n"); 
	lw(3,0);
	lw(0,5);
	lw(15,-3);
	lw(-4,0);
	lw(4,-12);
	lw(20000,0);
	lw(0,20000);
	printf("\n");
	
	printf("Teste lb\n\n"); 
	lb(5,0);
	lb(20000,0);
	lb(-4,0);
	lb(4,-1);
	lb(4,4);
	printf("\n");

	printf("Teste lbu\n\n"); 
	lbu(5,0);
	lbu(20000,0);
	lbu(-4,0);
	lbu(4,-1);
	lbu(4,4);
	printf("\n");
	
	printf("Teste sw\n\n"); 
	sw(3,0,0xFF000000);
	sw(0,5,0xFF000000);
	sw(-4,0,0xFF000000);
	sw(4,-12,0xFF000000);
	sw(20000,0,0xFF000000);
	sw(0,20000,0xFF000000);
	printf("\n");
	
	printf("Teste sb\n\n"); 
	sb(5,0,0xFF);
	sb(20000,0,0xFF);
	sb(-4,0,0xFF);
	sb(4,-1,0xFF);
	sb(4,4,0xFF);
	printf("\n");

	// 2° Teste - Teste de Funcionamento das Funções
	
	printf("Teste Funcionamento lw\n\n"); 
	printf("lw(20,0): %08x = lw(16,4): %08X = lw(24,-4): %08x\n\n",lw(20,0),lw(16,4),lw(24,-4) ); //Teste  o Endereçamento de lw
	
	printf("Teste Funcionamento lb e lbu\n\n");

	sw(8,0,0x9788394A);

	for (int i = 0; i < 4; ++i){
		
		printf("lb: %08x\n",lb(8,i));
		printf("lbu: %08x\n", lbu(8,i));
	}
	
	printf("\n");

	printf("Teste Funcionamento sw\n\n"); //Teste  do Endereçamento de sw
	sw(12,0,0x12345678);
	printf("sw(12,0,0x12345678) = %08x\n",mem[3]);
	sw(8,4,0x87654321);
	printf("sw(8,4,0x87654321) = %08x\n", mem[3]);
	sw(16,-4,0xFFFFFFFF);
	printf("sw(16,-4,0xFFFFFFFF) = %08x\n", mem[3]);

	printf("\n");

	printf("Teste Funcionamento sb\n\n"); 
	printf("Crescente (Substituicao por 0xAA):\n\n");
	
	for (int i = 0; i < 4; ++i){
		
		printf("Antes: %08x\n", mem[2]);
		sb(8,i,0xAA);
		printf("Depois: %08x\n", mem[2]);
		printf("\n");
	}

	printf("\n");

	printf("Decrescente (Substituicao por 0xAA):\n\n");
	
	for (int i = 3; i >= 0; i--){

		printf("Antes: %08x\n", mem[1]);
		sb(4,i,0xAA);
		printf("Depois: %08x\n", mem[1]);
		printf("\n");
	}
	return 0;
}