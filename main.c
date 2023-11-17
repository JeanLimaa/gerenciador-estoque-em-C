#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int loadProducts(struct Product product[])
{
    FILE *file = fopen("../estoque.csv", "r");

    if (file == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    // Ignora a primeira linha (cabeçalho)
    fscanf(file, "%*[^\n]\n");

    // Lê os dados já existentes do arquivo
    int count = 0;
    while (fscanf(file, "%d\t%s\tR$%f\t%d\n", &product[count].id, product[count].name, &product[count].price, &product[count].quantity) == 4)
    {
        count++;
    }

    fclose(file);

    return count; // Retorna a quantidade de elementos
}

void saveProducts(struct Product products[], int count)
{
    FILE *file = fopen("../estoque.csv", "w");
    if (file == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    // Verifica se o arquivo está vazio
    fseek(file, 0, SEEK_END); // Posiciona o cursor no final do arquivo
    if (ftell(file) == 0)
    {
        // Se o arquivo estiver vazio, escrever o cabeçalho
        fprintf(file, "Id\tNome\tPreco\tQuantidade");
    }

    // Escreve os novos dados
    for (int i = 0; i < count; i++)
    {
        fprintf(file, "\n%d\t%s\tR$%.2f\t%d", products[i].id = i + 1, products[i].name, products[i].price, products[i].quantity);
    }

    fclose(file);
}

void listProducts(struct Product products[], int count)
{
    printf("Lista de Produtos:\n");
    printf("Id\tNome\tPreço\tQuantidade\n");

    for (int i = 0; i < count; i++)
    {
        printf("%d\t%s\tR$%.2f\t%d\n", products[i].id, products[i].name, products[i].price, products[i].quantity);
    }
}

void deleteProduct(struct Product products[], int *count, int index)
{
    if (index >= 0 && index < *count)
    {
        for (int i = index; i < *count - 1; i++)
        {
            strcpy(products[i].name, products[i + 1].name);
            products[i].price = products[i + 1].price;
            products[i].quantity = products[i + 1].quantity;
        }
        (*count)--;
        saveProducts(products, *count);
        printf("Produto excluído com sucesso!\n");
    }
    else
    {
        printf("Índice inválido. Produto não encontrado.\n");
    }
}

void editProduct(struct Product products[], int count, int index)
{
    if (index >= 0 && index < count)
    {
        printf("Novo nome do produto: ");
        scanf("%s", products[index].name);
        printf("Novo preço: ");
        scanf("%f", &products[index].price);
        printf("Nova quantidade: ");
        scanf("%d", &products[index].quantity);

        saveProducts(products, count);
        printf("Produto editado com sucesso!\n");
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

    //Verifica se o ID é valido
    if (productId <= 0 || productId > count)
    {
        printf("ID de produto inválido.\n");
        return;
    }

    printf("Digite a quantidade a ser vendida: ");
    scanf("%d", &quantity);

    //Verifica se a quantidade de itens digitada existe em estoque
    if (quantity <= 0 || quantity > products[productId - 1].quantity)
    {
        printf("Quantidade inválida ou insuficiente em estoque.\n");
        return;
    }

    // Atualiza a quantidade em estoque
    products[productId - 1].quantity -= quantity;

    // Calcula o valor total da venda
    float totalSale = quantity * products[productId - 1].price;

    // Adiciona o valor ao saldo do vendedor
    seller->balance += totalSale;

    printf("Venda realizada com sucesso! Total: R$ %.2f\n", totalSale);
}

void consultBalance(struct Seller *seller)
{
    printf("\n///////////////////////////////\n\n");
    printf("O seu saldo atual é: R$ %.2f\n", seller->balance);
    printf("\n///////////////////////////////\n");
}

int main()
{
    struct Product products[MAX_PRODUCTS];
    int productCount = loadProducts(products);
    int choice;
    struct Seller seller;
    seller.balance = 0;

    do
    {
        printf("\n1. Cadastrar novo produto\n");
        printf("2. Listar produtos\n");
        printf("3. Excluir produto\n");
        printf("4. Editar produto\n");
        printf("5. Vender produto\n");
        printf("////////////////////\n");
        printf("6. Consultar saldo\n");
        printf("0. Sair\n");
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
                printf("Preço: ");
                scanf("%f", &products[productCount].price);
                printf("Quantidade: ");
                scanf("%d", &products[productCount].quantity);

                productCount++;
                saveProducts(products, productCount);

                printf("Produto cadastrado com sucesso!\n");
            }
            else
            {
                printf("Limite máximo de produtos atingido.\n");
            }
            break;
        case 2:
            // listar os produtos
            listProducts(products, productCount);
            break;
        case 3:
            // Excluir produto
            printf("Digite o ID do produto a ser excluído: ");
            int deleteID;
            scanf("%d", &deleteID);
            deleteProduct(products, &productCount, deleteID - 1);
            break;
        case 4:
            // Editar produto
            printf("Digite o ID do produto a ser editado: ");
            int editID;
            scanf("%d", &editID);
            editProduct(products, productCount, editID - 1);
            break;
        case 5:
            //vender o produto
            sellProduct(products, productCount, &seller);
            break;
        case 6:
            //consultar o saldo
            consultBalance(&seller);
            break;
        case 0:
            // sair
            printf("Saindo do programa.\n");
            break;
        default:
            //caso digite um inexistente: 
            printf("Opção inválida. Tente novamente.\n");
        }
    } while (choice != 0);

    return 0;
}
