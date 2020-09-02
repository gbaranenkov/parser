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

void printtree(Node* node, std::ofstream& temp, int n = 0)
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

bool check_data_string(const std::string& str1, const std::string& str2) //проверка данных узла на корректность
{
    for (size_t i = 1; i < (str1.size()) - 1; ++i)
    {
        if (str2.find(str1[i]))
        {
            return false;
        }
    }
    return true;
}

bool check_name_string(const std::string& str1, const std::string& str2) //проверка имени узла на содержание букв и символа '_'
{    
    for (size_t i = 0; i < str1.size(); ++i)
    {
        int fl = str2.find(str1[i]);
        if(fl == -1)
        {
            return false;
        }
    }
    return true;
}

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "rus");

    int count = 1; //счетчик id узла
    std::string output_file_name;
    if (argc < 2)
    {        
        std::cout << "Not argument" << '\n';
        return 1;
    }

    if (argc == 2)
    {
        std::cout << "Файл для записи создан автоматически: \"auto_output_file.txt\"" << '\n';
        output_file_name = "auto_output_file.txt";
    }

    if (argc == 3)
    {
        std::string file_out_name(argv[2]);
        output_file_name = file_out_name;
    }

    std::fstream file_in(argv[1]);
    std::ofstream file_out(output_file_name);
    
    if (!file_in.is_open())
    {
        std::cout << "Не удалось открыть файл для чтения!\n";
        return 1;
    }
    
    std::string buffer; //строка для считывания текстового файла
    
    std::getline(file_in, buffer, '\0');
        
    file_in.close();
    
    bool flag_quote = false;    //флаг для получения значения в кавычках


    std::vector <std::string> vec; //вектор для записи обработанных значений
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
        else if (buffer[i] == '\n' && flag_quote == false) //если перенос строки не внутри кавычек
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
            else if ((buffer[i] == '\n' || buffer[i] == '\0') && flag_quote == true) //если перенос строки внутри кавычек
            {
                std::cout << "Неверный формат данных!\n";
                return 1;
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
                vec.push_back(temp); //записываем имя узла
                vec.push_back("="); //после узла всегда символ = 
                temp = "";
            }
        }             
    }
    
    //проверка ошибок формата данных

    std::string Node_Name_alphabet = "_0123456789";
    for (char i = 'A'; i <= 'Z'; ++i)
    {
        Node_Name_alphabet.push_back(i);
    }
    for (char i = 'a'; i <= 'z'; ++i)
    {
        Node_Name_alphabet.push_back(i);
    }

    std::string Node_Data_incorrect_sym = {'\n', '\"'};


    for (size_t i = 0; i < vec.size(); ++i)
    {
        if (isdigit(vec[i][0])) //проверка формата данных имени узла (первый символ - не цифра)
         {
            std::cout << "Неверный формат данных!\n";
            return 1;
        }

        else if (vec[i][0] == '{' || vec[i][0] == '}')// || vec [i] == "=") 
        {
            continue;
        }

        else if (vec[vec.size()-1] != "}") //последний символ }
        {
            std::cout << "Неверный формат данных!\n";
            return 1;
        }

        else if (vec[i] == "=")
        {
            if (vec[i + 1][0] != '\"' && vec[i + 1][0] != '{') //после = всегда { или "
            {
                std::cout << "Неверный формат данных!\n";
                return 1;
            }
            else continue;
        }

        else if (vec[i][0] == '\"') //проверка значения узла
        {
            if (check_data_string(vec[i], Node_Data_incorrect_sym))
            {
                std::cout << "Неверный формат данных!\n";
                return 1;
            }
        }
        else //проверка имени узла
        {
            if (!check_name_string(vec[i], Node_Name_alphabet))
            {
                std::cout << "Неверный формат данных!\n";
                return 1;
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
        else if(vec[i] == "=")
        {
            continue;
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
