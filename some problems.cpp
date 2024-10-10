
#include <iostream>
#include <vector>
#include <limits>
#include <string>
#include <fstream>
#include <locale>
#include <codecvt>
#include <unordered_map>
#include <random>
#include <windows.h>

using namespace std;


struct dominoT {
    int leftDots;
    int RightDots;
};
//Problem Number 7 Domino chain
bool DominoChainhelper(vector<dominoT>& domino, vector<bool>& used, int leftright,vector<dominoT>&chain) {
    bool dominoUsed = true;
    for (bool u : used) {
        if (!u) {
            dominoUsed = false;
            break;
        }
    }
    if (dominoUsed) return true;


    for (int i = 0; i < domino.size(); i++) {
        if (!used[i]) {
            if (domino[i].leftDots == leftright) {
                used[i] = true;
                chain.push_back(domino[i]);
                if (DominoChainhelper(domino, used, domino[i].RightDots, chain)) {
                    return true;
                }
                chain.pop_back();
                used[i] = false;
            }
        }
    }

    return false;
}

bool Formchain(vector<dominoT>& domino,vector<dominoT>&chain) {
    for (int i = 0; i < domino.size(); i++) {
        vector<bool> used(domino.size(), false);
        used[i] = true;
        chain.push_back(domino[i]);
        if (DominoChainhelper(domino, used, domino[i].RightDots,chain)) {
            return true;
        }
        chain.pop_back();
    }
    return false;
}

void print(vector<dominoT>& chain) {
    for (int i = 0; i < chain.size(); i++) {
        cout << chain[i].leftDots << "|" << chain[i].RightDots;
        if (i != chain.size() - 1)
            cout << " - ";
    }
    cout << endl;
}

bool valid(int& n, bool Dots = false) {
    if (cin.fail() ||(Dots &&(n < 0 || n > 6))) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
    return true;
}

void Rundomino() {
    int n;

    do {
        cout << "Enter the number of dominos (positive integer): ";
        cin >> n;

        if (!valid(n)) {
            cout << "Invalid please, enter a positive integer" << endl;
        }
    } while (!valid(n));

    vector<dominoT> dominoSet;


    cout << "Enter the left and right dots for each domino:" << endl;
    for (int i = 0; i < n; i++) {
        dominoT domino;
        cout << "domino " << i + 1 << " left: ";
        while (true) {
            cin >> domino.leftDots;
            if (valid(domino.leftDots, true)) break;
            cout << "Invalid please, enter an integer between 0 and 6 for left dots: ";
        }

        cout << "domino " << i + 1 << " right: ";
        while (true) {
            cin >> domino.RightDots;
            if (valid(domino.RightDots, true)) break;
            cout << "Invalid please, enter an integer between 0 and 6 right dots: ";
        }

        dominoSet.push_back(domino);
    }
    vector<dominoT> chain;
    if (!Formchain(dominoSet, chain)) {
        cout << "No valid domino chain can be formed." << endl;
    } else {
        print(chain);
    }

}

//Problem Number 10 Message alerting
unordered_map<wstring, vector<wstring>> loadalternativetable(const string& filename) {
    unordered_map<wstring, vector<wstring>> alternativetable;
    wifstream file(filename);
    file.imbue(locale(locale(), new codecvt_utf8<wchar_t>));

    if (file.is_open()) {
        try {
            wstring key;
            while (file >> key) {
                vector<wstring> alternatives;
                wstring alternative;
                while (file.peek() != '\n' && file >> alternative) {
                    alternatives.push_back(alternative);
                }
                alternativetable[key] = alternatives;
            }
            file.close();
        }
        catch (const exception& e)
        {
            wcerr << "Error loading loadup table: " << e.what() << endl;
            file.close();
            return unordered_map<wstring, vector<wstring>>();
        }
    }
    else
    {
        wcerr << "Couldn't open load table file" << endl;
        return unordered_map<wstring, vector<wstring>>();
    }
    return alternativetable;
}

wstring randomalternative(vector<wstring> & alternatives){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(0, alternatives.size()-1);
    return alternatives[distr(gen)];
}


wstring replacemessage(wstring& message,unordered_map<wstring, vector<wstring>>& alternativetable){
    wstring result = message;
    for(auto& [word, alternatives] : alternativetable){
        size_t pos = 0;
        while((pos = result.find(word, pos)) != wstring::npos){
            result.replace(pos, word.length(), randomalternative(alternatives));
            pos += word.length();
        }
    }
    return result;
}

void RunMessageAlerting() {
    unordered_map<wstring, vector<wstring>> alternativetable = loadalternativetable("alternativetable.txt");
    if (alternativetable.empty()) {
        wcerr << "Failed to load lookup table" << endl;
        return;
    }

    string messageFile;
    cout << "Please enter the name of the message file (e.g., message.txt): ";
    getline(cin, messageFile);

    wifstream inputfile(messageFile);
    inputfile.imbue(locale(locale(), new codecvt_utf8<wchar_t>));

    if (!inputfile.is_open()) {
        wcerr << "Error opening message file: " << messageFile.c_str() << endl;
        return;
    }

    wstring message((istreambuf_iterator<wchar_t>(inputfile)), istreambuf_iterator<wchar_t>());
    inputfile.close();

    wstring alteredmessage = replacemessage(message, alternativetable);

    wofstream outputfile("altered_message.txt");
    outputfile.imbue(locale(locale(), new codecvt_utf8<wchar_t>));

    if (outputfile.is_open()) {
        outputfile << alteredmessage;
        outputfile.close();
        wcout << "Message is altered and saved to 'altered_message.txt'" << endl;
    }
    else
    {
        wcerr << "Error opening the output file" << endl;
    }
}

//Problem Number 4 Prime selection
void primeSelection(int N){
    if(N < 2){
        cout << "There is no prime numbers smaller than 2" << endl;
        return;
    }

    vector<bool>prime(N+1, true);

    prime[0] = prime[1] = false;

    for(int p = 2; p*p <= N;p++){
        if(prime[p]){
            for(int i = p*p;i <= N;i+=p){
                prime[i] = false;
            }
        }
    }
    for(int p = 2; p <= N;p++){
        if(prime[p])
            cout << p << " ";
    }

}

void RunPrime() {
    int N;
    while (true) {
        cout << "Please, enter a positive integer (greater than or equal to 2): ";
        cin >> N;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid please, enter a valid positive integer" << endl;
            continue;
        }
        break;
    }

    primeSelection(N);

}

// Problem Number 1 String
void remove_first_spaces(string &words)
{
    size_t start = words.find_first_not_of(' ');

    if (start != string::npos)
    {
        words = words.substr(start);
    }

    else
    {
        words.clear();
    }

}

bool captalize(string &words)
{
    if(words.empty()){
        cerr << "Error, string is empty " << endl;
        return false;
    }
    remove_first_spaces(words);

    for (int i = 0; i < words.length(); i++) {
        words[i] = tolower(words[i]);
    }
    if (isalpha(words[0])) {
        words[0] = toupper(words[0]);
    } else {
        cerr << "Error: First character isn't a letter" << endl;
        return false;
    }
    return true;
}
bool spaces(string &words, string &result){
    if(words.empty()){
        cerr << "Error, string is empty" << endl;
        return false;
    }

    bool space = false;
    result.clear();

    for(char c : words) {
        if(c == ' ') {
            if(!space) {
                result += ' ';
                space = true;
            }
        }
        else {
            result += c;
            space = false;
        }
    }

    return true;
}

void RunStringchanging(){
    string words;
    string result;

    cout << "Please, enter a string: ";
    getline(cin,words);

    if (!captalize(words)) {
        cerr << "Error, invalid input" << endl;
    }
    if (!spaces(words, result)) {
        cerr << "Error, invalid input" << endl;
    }
    cout << result << endl;

}

void displayMenu() {
    cout << "\n--- Menu ---\n";
    cout << "1. Prime number selection\n";
    cout << "2. String changing (capitalize and remove extra spaces)\n";
    cout << "3. Domino chain problem\n";
    cout << "4. Message Altering to Avoid Censorship\n";
    cout << "5. Exit\n";
}

int main() {
    int choice;

    while (true) {
        displayMenu();
        cout << "Please select an option (1-5): ";

        while (true) {
            cin >> choice;
            if (cin.fail() || choice < 1 || choice > 5) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid choice, please enter a number between 1 and 5: ";
            } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
        }
        switch (choice) {
            case 1:
                RunPrime();
                break;
            case 2:
                RunStringchanging();
                break;
            case 3:
                Rundomino();
                break;
            case 4:
                RunMessageAlerting();
                break;
            case 5:
                cout << "Exiting the program." << endl;
                return 0;
            default:
                cout << "Invalid choice, please select again." << endl;
        }
    }
    return 0;
}
