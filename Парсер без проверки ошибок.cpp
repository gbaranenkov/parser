// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Node
{    
    std::vector <Node*> children;
    Node* parent = NULL;
    int id = 0;
    std::string Node_Name;
    std::string Node_Data;
};

void is_digital()
{
    for (int i = 'a'; i < 'z'; ++i)
    {
        std::cout << i;

        //return true;
    }
}

void printtree(Node* node, std::fstream& temp, int n = 0)
{    
    for (size_t i = 0; i < n; ++i)
    {
        temp << ' ';
    }

    temp << node->id;
    temp << ' ';
    if (node->parent != NULL)
    {
        temp << node->parent->id;
    }
    temp << ' ';
    temp << node->Node_Name;
    temp << ' '; 
    if (node->Node_Data == "")
    {
        for (auto var : node->children)
        {
            temp << var->id;
            temp << ' ';
        }

    }
    temp << node->Node_Data;
    temp << '\n';
    
    for (auto var : node->children)
    {
       printtree(var, temp, n + 2);
    }
}

void delete_tree(Node* root)
{
    for (auto var : root->children)
    {
        delete_tree(var);
    }

    if (root->children.size() == 0)
    {
        delete root;
    }
}

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "rus");

    //is_digital();
/*

    return 0;
}
*/
    int count = 1; //счетчик id узла

   // std::string input_file;// = "input_file.txt";
    //std::string output_file;// = "output_file.txt";
    
    std::ifstream file_in (input_file);
    std::fstream file_out (output_file);

    if (!file_in.is_open())
    {
        std::cout << "Не удалось открыть файл для чтения!\n";
        return 1;
    }
    
    std::string buffer; //строка для считывания текстового файла
    
    std::getline(file_in, buffer, '\0');
        
    file_in.close();
    
    std::vector <std::string> vec; //вектор для записи обработанных значений
    bool flag_quote = false;    //флаг для получения значения в кавычках
    std::string temp = "";      //временная переменная для передачи значений в вектор


    //разбиваем строку на элементы
    
    for (size_t i = 0; i < buffer.length(); ++i) 
    {        
        if (buffer[i] == '{')  
        {
            vec.push_back("{");
            continue;
        }
        else if (buffer[i] == '}')
        {
            vec.push_back("}");
            continue;
        }
        else if (buffer[i] == '\"')
        {            
            flag_quote = true;            
        }
        else if (buffer[i] == '\n')
        {
            continue;
        }

        if (flag_quote == true)
        {            
            if (buffer[i] == '\"' && flag_quote == true && temp != "") //получаем значения в кавычках (значения узлов)
            {
                temp.push_back(buffer[i]);
                vec.push_back(temp);
                flag_quote = false;
                temp = "";
            }
            else temp.push_back(buffer[i]);            
        } 
        else
        {
            if (buffer[i] != ' ' && buffer[i] != '=')           //получаем значения без кавычек (имя узла)
            {
                temp.push_back(buffer[i]);
            }
            
            if (buffer[i] == '=')
            {
                vec.push_back(temp);
                temp = "";
            }
        }
             
    }
    

    Node* root = NULL;                                          // создаем указатель на корень дерева    
    Node* current_parent = NULL;                               //создаем указатель на текущего родителя
    Node* current_node = NULL;                                 //создаем указатель на текущий узел


    for (size_t i = 0; i < vec.size(); ++i) //обходим вектор для создания дерева
    {
        
        if (vec[i][0] == '{')                                    // { означает создание нового узла-потомка предыдущего
        {
            Node* new_node = new Node;                           // cоздаем узел
            current_parent = current_node;                       // предыдущий узел - новый текущий родитель
            new_node->parent = current_parent;                   // текущий родитель - родитель созданного узла            
            current_parent->children.push_back(new_node);        // добавление в список потомков родителя
            current_node = new_node;                             // перенос указателя текущего узла из предыдущего в созданный
            new_node->id = count;
            count++;
        }

        else if (vec[i][0] == '}')
        {
            current_node = current_node->parent;                
            current_parent = current_parent->parent;            
        }

        else if (vec[i][0] == '\"')
        {
            current_node->Node_Data = vec[i];
        }

        else if (vec[i][0] != '{' && vec[i][0] != '\"' && vec[i][0] != '}') // нашли имя узла
        {
            //if (vec[i][0] != '')
            is_digital(vec[i][0]);
            if (current_parent == NULL) //если у узла нет родителей, значит нашли имя корня
            {
                Node* new_node = new Node;                              //создаем узел
                new_node->Node_Name = vec[i];                           //пишем имя узла
                current_node = new_node;                    //ставим в указатель на текущий узел указатель на созданный узел
                new_node->parent = NULL;                   //добавляем в текущий узел указатель на родителя
                root = new_node;                           //ставим в корень указатель на созданный узел 
                new_node->id = count;
                count++;
            }
            else  //если нашли последующее имя узла (после {)
            {
                if (current_node->Node_Name == "")
                {
                    current_node->Node_Name = vec[i];                  //записывает имя в узел
                }
                else
                {
                    Node* new_node = new Node;                             //создаем новый узел
                    new_node->parent = current_parent;                     // добавляем в текущий узел указатель на родителя
                    current_node = new_node;                               // перенос указателя текущего узла из предыдущего в созданный
                    new_node->Node_Name = vec[i];
                    current_parent->children.push_back(current_node);      // добавление в список потомков узла-родителя
                    new_node->id = count;
                    count++;
                }
             }
        }
    }

    if (!file_out.is_open())
    {
        std::cout << "Не удалось открыть файл для записи!\n";
        return 1;
    }
        
    printtree(root, file_out);

    file_out.close();

    delete_tree(root);
    


    return 0;
}
*/