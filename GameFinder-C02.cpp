//MARIANA BISSARO WEISS, 2281
//GIOVANNA GAMA FERNANDINHO, 1087
//VITORIA FATIMA SILVA DIAS, 1090 
//MARIA EDUARDA SILVA SOUZA, 2264
//JOAO VITOR DE MELLO RIBEIRO, 737

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream> 
using namespace std;

void mostrarMenu(); //mostra o menu
void mostrarCategorias(); //mostra as categorias de jogos
void mostrarJogosCategoria(int escolhaCategoria); //mostra os jogos dentro de cada categoria
void mostrarJogosPorAvaliacao(); //mostra a categoria de jogos por avaliacao
void limparCarrinhoArquivo(); //limpa o arquivo do carrinho quando sai do aplicativo
int obterEntradaInteira(); //impede que o codigo se repita caso o usuario colocar uma string e nao um int no cin
void adicionarAoCarrinho(const string& nomeJogo);//adiciona os jogos ao carrinho
void verCarrinho(); //permite ver os itens no arquivo
void removerDoCarrinho(); //funcao para remover um jogo do carrinho

//funcao para verificar se o jogo ja existe no carrinho
bool jogoExisteEmArquivo(string& nomeJogo, const string& nomeArquivo);

//converte string para float manualmente (para nota de avaliacao no void mostrarJogosPorAvaliacao)
float stringToFloat(const string& str) {
    float resultado = 0.0f;
    float divisor = 1.0f;
    bool parteDecimal = false;

    for (int i = 0; i < str.length(); ++i) {
        if (str[i] >= '0' && str[i] <= '9') {
            if (!parteDecimal) {
                resultado = resultado * 10.0f + (str[i] - '0');
            } else {
                divisor *= 10.0f;
                resultado = resultado + (str[i] - '0') / divisor;
            }
        } else if (str[i] == '.') {
            parteDecimal = true;
        }
    }
    return resultado;
}

//função para impedir o bug de repeticao de mensagem de menu, quando o usuario nao coloca uma int 
int obterEntradaInteira() {
    int escolha;
    while (!(cin >> escolha)) {
        cout << "Opcao invalida. Escolha novamente: ";
        cin.clear(); //limpa o estado do erro
        cin.ignore(1000, '\n'); //ignora ate 1000 caracteres até o \n
    }
    return escolha;
}

bool jogoExisteEmArquivo(const string& nomeJogo, const string& nomeArquivo){
	ifstream arquivo("carrinho.txt");
	if(!arquivo.is_open()){
		return false;
	}
	
	string linha;
	while(getline(arquivo, linha)){
		if(linha == nomeJogo){
			arquivo.close();
			return true;
		}
	}
	
	arquivo.close();
	return false;
}

// CONSTANTES
const int MAX_JOGOS_POR_CATEGORIA = 5; // limite para categorias regulares
const int MAX_JOGOS_AVALIACAO = 35;    // limite para jogos lidos do arquivo de avaliações
const int MAX_CARRINHO = 35;           // limite para o carrinho

// VARIAVEIS GLOBAIS
string carrinho[MAX_CARRINHO];
int contagemCarrinho = 0;

//DEFININDO OS VOID's

void mostrarMenuPrincipal() 
{
    cout << "--- Menu Principal ---" << endl;
    cout << "1. Procurar jogos por categoria" << endl;
    cout << "2. Ver meu carrinho" << endl;
    cout << "3. Sair" << endl;
    cout << "Escolha uma opcao: ";
}

void mostrarCategorias() 
{
    cout << "\n--- Escolha uma Categoria ---" << endl;
    cout << "1. FPS" << endl;
    cout << "2. RPG" << endl; 
    cout << "3. Acao e Aventura" << endl;
    cout << "4. Terror" << endl;
    cout << "5. Plataforma" << endl;
    cout << "6. Souls Like" << endl;
    cout << "7. Simulacao" << endl;
    cout << "8. Por Avaliacao" << endl;
    cout << "Escolha uma categoria: ";
}

//variavel escolhaCategoria é o numero correspondente a categoria no menu
void mostrarJogosCategoria(int escolhaCategoria) 
{
	//procura os arquivos com os seguintes nomes para usar no sistema
	string nomesArquivosCategorias[] = { 
        "",
        "fps.txt",
        "rpg.txt",
        "acao_aventura.txt",
        "terror.txt",
        "plataforma.txt",
        "soulslike.txt",
        "simulacao.txt"
    };
    
	//denomina as categorias de jogos que irao aparecer no menu
    string nomesCategorias[] = { 
        "",
        "FPS",
        "RPG",
        "Acao e Aventura",
        "Terror",
        "Plataforma",
        "Souls Like",
        "Simulacao"
    };
    
	//impede que o usuario escolha um numero que nao corresponde a leitura
    if (escolhaCategoria < 1 || escolhaCategoria > 7) { 
        cout << "Categoria invalida." << endl;
        return;
    }
    
    //definindo o nome do arquivo ao nome das categorias
	string nomeArquivoCategoria = nomesArquivosCategorias[escolhaCategoria];
    string categoriaNome = nomesCategorias[escolhaCategoria];
    
    //variavel bool que define que o usuario vai poder escolher mais de um jogo na categoria se quiser
    bool continuarNaCategoria = true; 
    while (continuarNaCategoria) { //se a bool é verdadeira
        ifstream arquivoEntrada(nomeArquivoCategoria.c_str());
        if (!arquivoEntrada.is_open()) { //se o arquivo de entrada nao abrir, mostra essa mensagem
            cout << "Erro ao abrir o arquivo da categoria " << categoriaNome << ". Verifique se existe." << endl;
            return;
        }
    
 		cout << "\n--- Jogos em " << categoriaNome << " ---" << endl;
        string jogos[MAX_JOGOS_POR_CATEGORIA]; //vetor com variavel global
        int contagemJogosLidos = 0; //0 jogos lidos no inicio da aplicacao
        string linha; //pega cada linha em que o jogo esta escrito
        
        //Mostra na tela os jogos dentro do arquivo escolhido
        while (getline(arquivoEntrada, linha) && contagemJogosLidos < MAX_JOGOS_POR_CATEGORIA) {
            jogos[contagemJogosLidos] = linha;
            cout << (contagemJogosLidos + 1) << ". " << jogos[contagemJogosLidos] << endl;
            contagemJogosLidos++;
        }
        arquivoEntrada.close();
        
        if (contagemJogosLidos == 0) { //se nao tem nenhum jogo no arquivo categoria
            cout << "Nenhum jogo encontrado nesta categoria." << endl;
            return;
        }
		
		//adiciona o jogo ao carrinho do usuario
        cout << "Escolha um jogo para adicionar ao carrinho (1 a " << contagemJogosLidos << ") ou 0 para voltar: ";
        int escolhaJogo;
        escolhaJogo = obterEntradaInteira();
        
   	   if (escolhaJogo >= 1 && escolhaJogo <= contagemJogosLidos) {
            adicionarAoCarrinho(jogos[escolhaJogo - 1]); //menos 1 para nao contar o 0
        } else if (escolhaJogo == 0) { //se seleciona 0, vola ao menu
            continuarNaCategoria = false;
            cout << "Voltando ao menu principal." << endl;
        } else { //se seleciona qualqquer outro numero, que nao seja 0 ou os numeros referentes ao jogos na categoria
            cout << "Escolha de jogo invalida." << endl;
        }
        
        //se continuar na categoria verdadeiro e a escolha de jogo foi diferente de 0, o usuario pode escolher se pega mais algum joo daquela mesma categoria
        if (continuarNaCategoria && escolhaJogo != 0) {
            cout << "\nDeseja escolher mais algum jogo da categoria '" << categoriaNome << "'?" << endl;
            cout << "1. Sim" << endl;
            cout << "2. Nao (Voltar ao Menu Principal)" << endl;
            cout << "Escolha: ";
            int escolhaContinuar;
            escolhaContinuar = obterEntradaInteira();
            if (escolhaContinuar == 2) {
                continuarNaCategoria = false;
            } else if (escolhaContinuar != 1) {
                cout << "Opcao invalida. Voltando ao Menu Principal." << endl;
                continuarNaCategoria = false;
            }
        }
    	
}
}

//para o void mostrarJogoPorAvaliacao para armazenar o nome do jogo e sua avaliação
struct JogoAvaliacao 
{
    string nome;
    float avaliacao;
};

void mostrarJogosPorAvaliacao()
{
	ifstream arquivoEntrada("avaliacoes.txt");
	if (!arquivoEntrada.is_open()) {
        cout << "Erro ao abrir o arquivo de avaliacoes. Verifique se existe." << endl;
        return;
    }
    //as avaliacoes dos jogos foram tiradas do Metascore
    JogoAvaliacao jogosAvaliados[MAX_JOGOS_AVALIACAO]; //define a struct
    int contagemJogosAvaliados = 0;
    string linha; //define cada linha do arquivo
    
    //le os jogos e suas avaliacoes
    while (getline(arquivoEntrada, linha) && contagemJogosAvaliados < MAX_JOGOS_AVALIACAO) {
        stringstream ss(linha); //separa a linha pelo delimitador ;
        string nomeJogo, strAvaliacao;
		//le o nome do jogo e a avaliacao
        getline(ss, nomeJogo, ';'); //le o nome ate o ;
        getline(ss, strAvaliacao); //continua a ler a partir do ;

        if (!nomeJogo.empty() && !strAvaliacao.empty()) { //verifica se exziste dados
            jogosAvaliados[contagemJogosAvaliados].nome = nomeJogo;
            jogosAvaliados[contagemJogosAvaliados].avaliacao = stringToFloat(strAvaliacao);
            contagemJogosAvaliados++;
        }
    }
    arquivoEntrada.close();

    if (contagemJogosAvaliados == 0) {
        cout << "Nenhum jogo encontrado." << endl;
        return;
    }

    // mostra os jogos e a avaliacao
    bool continuarNestaLista = true;
    while (continuarNestaLista) {
        cout << "\n--- Jogos por Avaliacao ---" << endl;
        for (int i = 0; i < contagemJogosAvaliados; ++i) {
            cout << (i + 1) << ". " << jogosAvaliados[i].nome << " (Avaliacao: "
                 << fixed << setprecision(1) << jogosAvaliados[i].avaliacao << ")" << endl;
        }

        cout << "Escolha um jogo para adicionar ao carrinho (1 a " << contagemJogosAvaliados << ") ou 0 para voltar: ";
        int escolhaJogo = obterEntradaInteira();
		//se escolhe um jogo valido
        if (escolhaJogo >= 1 && escolhaJogo <= contagemJogosAvaliados) {
            adicionarAoCarrinho(jogosAvaliados[escolhaJogo - 1].nome); //adiciona no carrinho 
        } else if (escolhaJogo == 0) { //volta ao menu
            continuarNestaLista = false;
            cout << "Voltando ao menu principal." << endl;
            break;
        } else { //se digitar errado, a lista reaparece
            cout << "Escolha de jogo invalida." << endl;
            continue;
        }
		
		//faz o loop da lista se o usuario quiser escolher mais um dessa lista
        cout << "\nDeseja escolher mais algum jogo desta lista de avaliacao?" << endl;
        cout << "1. Sim" << endl;
        cout << "2. Nao (Voltar ao Menu Principal)" << endl;
        cout << "Escolha: ";
        int escolhaContinuar = obterEntradaInteira();
        if (escolhaContinuar == 2) {
            continuarNestaLista = false;
        } else if (escolhaContinuar != 1) {
            cout << "Opcao invalida. Voltando ao Menu Principal." << endl;
            continuarNestaLista = false;
        }
    }
}

void adicionarAoCarrinho(const string& nomeJogo)
{
	//verifica se o jogo ja esta no carrinho usando a funcao
	if(jogoExisteEmArquivo(nomeJogo, "carrinho.txt"))
	{
		cout << "'" << nomeJogo << "'" << " ja esta no seu carrinho!" << endl;
		return;
	}
	
	if (contagemCarrinho < MAX_CARRINHO) {
        carrinho[contagemCarrinho] = nomeJogo;
        contagemCarrinho++;
		
		//arquivo de saida que coloca os itens 
        ofstream arquivoSaida("carrinho.txt", ios::app); //app para adicionar itens, nao sobreescrever
        if (arquivoSaida.is_open()) {
            arquivoSaida << nomeJogo << endl; //adiciona o nome do jogo escolhido no arquivo carrinho
            arquivoSaida.close();
            cout << "'" << nomeJogo << "' adicionado ao carrinho com sucesso!" << endl;
        } else {
            cout << "Erro ao abrir o arquivo do carrinho para escrita." << endl;
        }
    } else {
        cout << "Carrinho cheio! Nao e possivel adicionar mais jogos. Limite de " << MAX_CARRINHO << " jogos." << endl;
    }
}

void verCarrinho() {
    cout << "\n--- Seu Carrinho ---" << endl;
    ifstream arquivoEntrada("carrinho.txt"); //le o arquivo de entrada carrinho, que foi definido no void adicionarAoCarrinho
    if (!arquivoEntrada.is_open()) {
        cout << "Seu carrinho esta vazio ou houve um erro ao abrir o arquivo." << endl;
        return;
    }

    string jogoNoCarrinho;
    int item = 1; //numero que indica o jogo
    bool carrinhoVazio = true;
	//enquanto tiver itens no carrinho, ele mostra o nome do jogo
    while (getline(arquivoEntrada, jogoNoCarrinho)) {
        cout << item << ". " << jogoNoCarrinho << endl;
        item++; 
        carrinhoVazio = false;
    }
    arquivoEntrada.close();

    if (carrinhoVazio) {
        cout << "Seu carrinho esta vazio." << endl;
    }
    cout << "--------------------" << endl;
    
    if (!carrinhoVazio) {
        int opcaoRemover;
        cout << "Deseja remover algum jogo do carrinho? " << endl;
        cout << "1. Sim" << endl;
        cout << "2. Nao" << endl;
        cout << "Escolha: ";
        opcaoRemover = obterEntradaInteira();
        if (opcaoRemover == 1) {
            removerDoCarrinho();
        } else {
            cout << "Voltando ao menu." << endl;
        }
    }
    
}

void removerDoCarrinho(){
	ifstream arquivoEntrada("carrinho.txt");
    if (!arquivoEntrada.is_open()) {
        cout << "Erro ao abrir o carrinho para leitura." << endl;
        return;
    }

    //le todos os jogos do arquivo
    string jogos[MAX_CARRINHO];
    int totalJogos = 0;
    string linha;
    while (getline(arquivoEntrada, linha) && totalJogos < MAX_CARRINHO) {
        jogos[totalJogos++] = linha;
    }
    arquivoEntrada.close();

    if (totalJogos == 0) {
        cout << "Seu carrinho esta vazio." << endl;
        return;
    }

    //mostra os jogos com índice
    cout << "\n--- Selecione um jogo para remover ---" << endl;
    for (int i = 0; i < totalJogos; ++i) {
        cout << (i + 1) << ". " << jogos[i] << endl;
    }

    cout << "Digite o numero do jogo que deseja remover ou 0 para cancelar: ";
    int escolhaRemover = obterEntradaInteira();

    if (escolhaRemover == 0) {
        cout << "Remocao cancelada. Voltando ao menu." << endl;
        return;
    }

    if (escolhaRemover < 1 || escolhaRemover > totalJogos) {
        cout << "Opcao invalida. Nenhum jogo removido." << endl;
        return;
    }

    //remove o jogo do vetor e reescreve o arquivo
    ofstream arquivoSaida("carrinho.txt", ios::out | ios::trunc);
    if (!arquivoSaida.is_open()) {
        cout << "Erro ao abrir o carrinho para reescrita." << endl;
        return;
    }
	
	//reescreve os jogos do carrinho, MENOS o que foi removido
    for (int i = 0; i < totalJogos; ++i) {
    	//verifica se o indice atual nao e do jogo que se deseja remover
        if (i != (escolhaRemover - 1)) {
            arquivoSaida << jogos[i] << endl; //escreve o jogo atual no carrinho (nao o removido)
        }
    }
    arquivoSaida.close();

    cout << "'" << jogos[escolhaRemover - 1] << "' foi removido do carrinho com sucesso!" << endl;

    //atualiza a contagem de jogos no carrinho
    contagemCarrinho = totalJogos - 1;
}

//quando o usuario sai (aperta 3 no menu), o carrinho é limpado para um novo login
void limparCarrinhoArquivo() {
    ofstream arquivoSaida("carrinho.txt", ios::out | ios::trunc);
    if (arquivoSaida.is_open()) {
        arquivoSaida.close();
    }
    contagemCarrinho = 0;
}

int main()
{
	//Mostrara o menu e o nome dos participantes
	cout << "--- PROJETO FEITO POR ---" << endl;
	cout << endl;
	cout << "--- MARIANA BISSARO WEISS, 2281 ---"  << endl;
	cout << "--- GIOVANNA GAMA FERNANDINHO, 1087 ---"  << endl;
	cout << "--- VITORIA FATIMA SILVA DIAS, 1090 ---"  << endl;
	cout << "--- MARIA EDUARDA SILVA SOUZA, 2264 ---"  << endl;
	cout << "--- JOAO VITOR DE MELLO RIBEIRO, 737 ---"  << endl;
	cout << endl;
	int escolhaMenuPrincipal;
	
    do {
        mostrarMenuPrincipal();
        escolhaMenuPrincipal = obterEntradaInteira();

        switch (escolhaMenuPrincipal) {
            case 1: // procurar jogos por categoria
                mostrarCategorias();
                int escolhaCategoria;
                escolhaCategoria = obterEntradaInteira();
                if (escolhaCategoria == 8) {
                    mostrarJogosPorAvaliacao();
                } else {
                    mostrarJogosCategoria(escolhaCategoria);
                }
                break;
            case 2: // ver carrinho
                verCarrinho();
                break;
            case 3: // sair
                cout << "Saindo do programa." << endl;
                limparCarrinhoArquivo();
                break;
            default:
                cout << "Opcao invalida. Tente novamente." << endl;
                break;
        }
        cout << endl;
    } while (escolhaMenuPrincipal != 3);

    return 0;
}