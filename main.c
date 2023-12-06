#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_PRODUCTS 100
#define MAX_NAME_LENGTH 50

struct Product
{
    int id;
    char name[MAX_NAME_LENGTH];
    float price;
    int quantity;
};

struct Seller
{
    float balance;
};

void welcomeMessage()
{
    printf("\n\n------------------------------------------------------------------\n\n");
    printf("*******************************************************************\n\n");
    printf("            Bem-vindo ao seu Gerenciador de estoque!              \n");
    printf("                   by: Vision Softwares\n\n"); // nome ficticio
    printf("*******************************************************************\n");
    printf("------------------------------------------------------------------\n\n");
}

void showMessage(const char *message)
{
    printf("\n\n////////////////////////////////////////////\n\n");
    printf("%s", message);
    printf("\n\n////////////////////////////////////////////\n\n");
}

int loadProducts(struct Product product[], struct Seller *seller)
{
    //startBalanceZero(&seller);
    FILE *file = fopen("./estoque.csv", "r");
    
     //caso não tenha nada no arquivo o "balance" inicial é 0
    fseek(file, 0, SEEK_END); // Posiciona o cursor no fim do arquivo
    if (ftell(file) == 0) seller->balance = 0;
    fseek(file, 0, SEEK_SET); // Posiciona o cursor no início do arquivo

    //fclose(file);
    if (file == NULL)
    {
        printf("\n--------------------\nO arquivo não existe. Criando um novo...\n-----------------------\n");
        FILE *file = fopen("./estoque.csv", "w");
        if (file == NULL)
        {
            printf("Erro ao abrir o arquivo, no load.\n");
            exit(1);
        }
        else
        {
            fclose(file);
            return 0;
        }
    }

    // Ignora a primeira linha (cabeçalho)
    fscanf(file, "%*[^\n]\n");

    int count = 0;
    // Lê os dados já existentes do arquivo
    // na primeira linha pós cabeçalho escrever o saldo do balance
    if(count == 0)
    {
        if(fscanf(file, "%d\t%s\tR$%f\t%d\t%f\n", &product[count].id, product[count].name, &product[count].price, &product[count].quantity, &seller->balance) == 5){
            count++;
        }
    } 

    while (fscanf(file, "%d\t%s\tR$%f\t%d\n", &product[count].id, product[count].name, &product[count].price, &product[count].quantity) == 4)
    {
        count++;
    }
    
    fclose(file);

    return count; // Retorna a quantidade de elementos
}

void saveProducts(struct Product products[], int count, struct Seller *seller)
{
    FILE *file = fopen("./estoque.csv", "r");
/*     //caso não tenha nada no arquivo o "balance" inicial é 0
    fseek(file, 0, SEEK_END); // Posiciona o cursor no fim do arquivo
    if (ftell(file) == 0) seller->balance = 0;
    fseek(file, 0, SEEK_SET); // Posiciona o cursor no início do arquivo
    fclose(file); */

    file = fopen("./estoque.csv", "w");

    if (file == NULL)
    {
        printf("Erro ao abrir o arquivo, ao salvar.\n");
        exit(1);
    }

    fseek(file, 0, SEEK_END); // Posiciona o cursor no final do arquivo
    if (ftell(file) == 0)
    {
        // Se o arquivo estiver vazio, escrever o cabeçalho e define o balnce inicialmente como 0
        fprintf(file, "Id\tNome\tPreco\tQuantidade\tSaldoT");
    }

    // Escreve os novos dados
    for (int i = 0; i < count; i++)
    {
        if(i == 0)
        {
            //se i for igual a 0, ele estará na linha onde contém o saldo e quero escrever isso.
            fprintf(file, "\n%d\t%s\tR$%.2f\t%d\t%f", products[i].id = i + 1, products[i].name, products[i].price, products[i].quantity, seller->balance);
        } else{
            fprintf(file, "\n%d\t%s\tR$%.2f\t%d", products[i].id = i + 1, products[i].name, products[i].price, products[i].quantity);
        }
    }

    fclose(file);

    showMessage("Produto cadastrado com sucesso!");
}

void auxSaveProducts(struct Seller *seller)
{
    printf("\n** (Caso não tenha, digite 0 - será subtraido no valor do saldo total) **\n");
    printf("** Esse é o valor do custo TOTAL que você teve com o produto **\n");

    float itemPrice;
    do {
        printf("Preço de custo: ");
        scanf("%f", &itemPrice);

        if (itemPrice < 0) {
            printf("O preço não pode ser negativo. Tente novamente.\n");
        }
    } while (itemPrice < 0);
    
    seller->balance -= itemPrice;
}

void showProducts(struct Product products[], int count)
{
    printf("\n\n////////////////////////////////////////////\n\n");
    printf("Lista de Produtos:\n\n");
    printf("Id\t\t\tNome\t\t\tPreço\t\t\tEm estoque\n");

    //iterar sobre todos os produtos e exibir um a um
    for (int i = 0; i < count; i++)
    {
        printf("%d\t\t\t%s\t\t\tR$%.2f\t\t\t%d\n", products[i].id, products[i].name, products[i].price, products[i].quantity);
    }
    printf("\n////////////////////////////////////////////\n\n");
}

void deleteProduct(struct Product products[], int *count, int index)
{
    struct Seller seller;

    if (index >= 0 && index < *count)
    {
        // loop for define o indice digitado como sendo a variavel "i";
        // count - 1, para iterar sobre todos os elementos - 1;
        // i++, para iterar sobre todos os outros elementos depois dele
        for (int i = index; i < *count - 1; i++)
        {
            strcpy(products[i].name, products[i + 1].name);
            products[i].price = products[i + 1].price;
            products[i].quantity = products[i + 1].quantity;
        }
        (*count)--;
        saveProducts(products, *count, &seller);

        showMessage("Produto excluído com sucesso!");
    }
    else
    {
        showMessage("Índice inválido. Produto não encontrado.");
    }
}

void editProduct(struct Product products[], int count, int index)
{
    int editChoice = 0;
    struct Seller seller;

    // condição para editar somente se o indice digitado por válido
    if (index >= 0 && index < count)
    {
        // perguntando o que ele deseja editar
        printf("\nO que deseja editar?\n\n1. Nome\n2. Preço\n3. Quantidade em estoque\n0. Voltar\n\nDigite a opção: ");
        scanf("%d", &editChoice);

        switch (editChoice)
        {
        case 1:
            printf("Novo nome do produto: ");
            scanf("%s", products[index].name);
            break;
        case 2:
            printf("Novo preço: ");
            scanf("%f", &products[index].price);
            break;
        case 3:
            printf("Nova quantidade: ");
            scanf("%d", &products[index].quantity);
            break;
        case 0:
            printf("\n\nVoltando...\n\n");
            break;
        default:
            showMessage("Opção inválida!");
        }

        // caso o número digitado esteja entre as opções validas
        if (editChoice > 0 && editChoice < 4)
        {
            saveProducts(products, count, &seller);
            showMessage("Produto editado com sucesso!");
        }
    }
    else
    {
        printf("Índice inválido. Produto não encontrado.\n");
    }
}

void sellProduct(struct Product products[], int count, struct Seller *seller)
{
    int productId, quantity;

    printf("Digite o ID do produto que deseja vender: ");
    scanf("%d", &productId);

    // Verifica se o ID é valido
    if (productId <= 0 || productId > count)
    {
        printf("ID de produto inválido.\n");
        return;
    }

    printf("Digite a quantidade a ser vendida: ");
    scanf("%d", &quantity);

    // Verifica se a quantidade de itens digitada existe em estoque
    if (quantity <= 0 || quantity > products[productId - 1].quantity)
    {
        printf("\n\n** Quantidade inválida ou insuficiente em estoque. **\n\n");
        return;
    }

    // Atualiza a quantidade em estoque
    products[productId - 1].quantity -= quantity;

    // Calcula o valor total da venda
    float totalSale = quantity * products[productId - 1].price;

    // Adiciona o valor ao saldo do vendedor
    seller->balance += totalSale;
    printf("valor: %f\n", seller->balance);
    saveProducts(products, count, seller);
    
    printf("\n\n////////////////////////////////////////////\n\n");
    printf("Venda realizada com sucesso! Total: R$ %.2f", totalSale);
    printf("\n\n////////////////////////////////////////////\n\n");
}

void consultBalance(struct Seller *seller)
{
    printf("\n\n////////////////////////////////////////////\n\n");
    printf("O seu saldo atual é: R$ %.2f", seller->balance);
    printf("\n\n////////////////////////////////////////////\n\n");
}

int main()
{
    setlocale(LC_ALL, "Portuguese");
    welcomeMessage();
    // definição de estruturas e varivaveis
    struct Product products[MAX_PRODUCTS];
    struct Seller seller;
    seller.balance = 0;
    int productCount = loadProducts(products, &seller);
    int choice;

    // loop do-while com um switch case o qual é feito para escolher as opções, enquanto o usuario não encerrar o programa
    do
    {
        printf("\n* Digite o número da ação que deseja *\n");
        printf("\n1. Cadastrar novo produto\n");
        printf("2. Exibir produtos\n");
        printf("3. Excluir produto\n");
        printf("4. Editar produto\n");
        printf("5. Vender produto\n");
        printf("6. Consultar saldo\n");
        printf("0. Sair\n\n");
        printf("Escolha uma opção: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            if (productCount < MAX_PRODUCTS)
            {
                // cadastrar produto
                printf("Nome do produto: ");
                scanf("%s", products[productCount].name);
                printf("Preço (R$): ");
                scanf("%f", &products[productCount].price);
                printf("Quantidade: ");
                scanf("%d", &products[productCount].quantity);

                productCount++;
                auxSaveProducts(&seller);
                saveProducts(products, productCount, &seller);
            }
            else
            {
                printf("Limite máximo de produtos atingido.\n");
            }
            break;
        case 2:
            // listar os produtos
            showProducts(products, productCount);
            break;
        case 3:
            // chamando a função de exibir produtos para poder visualizar os ids antes de excluir
            showProducts(products, productCount);

            // Excluir produto
            printf("Digite o ID do produto a ser excluído: ");
            int deleteID;
            scanf("%d", &deleteID);
            deleteProduct(products, &productCount, deleteID - 1);
            break;
        case 4:
            // chamando a função de exibir produtos para poder visualizar os ids antes de editar
            showProducts(products, productCount);

            // Editar produto
            printf("Digite o ID do produto a ser editado: ");
            int editID;
            scanf("%d", &editID);
            editProduct(products, productCount, editID - 1);
            break;
        case 5:
            // chamando a função de exibir produtos para poder visualizar os ids antes de vender
            showProducts(products, productCount);
            // vender o produto
            sellProduct(products, productCount, &seller);
            break;
        case 6:
            // consultar o saldo
            consultBalance(&seller);
            break;
        case 0:
            // sair
            printf("Saindo do programa.\n");
            break;
        default:
            // caso digite um inexistente:
            printf("Opção inválida. Tente novamente.\n");
        }
    } while (choice != 0);

    return 0;
}