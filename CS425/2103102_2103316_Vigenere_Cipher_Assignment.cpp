/*

    CODING ASSIGNMENT : VIGENERE CIPHER
    TEAM MEMBERS : 
        1. NAME    : Abhinav Reddy Gutha
           ROLL NO : 2103102
        2. NAME    : Nalamolu Nagarjuna Viswanath
           ROLL NO : 2103316

*/

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<cmath>
using namespace std;

const double f[] = {0.08167, 0.01492, 0.02782, 0.04253, 0.12702, 0.02228, 0.02015, 0.06094, 0.06966, 0.00153, 0.00772, 0.04025, 0.02406, 0.06749, 0.07507, 0.01929, 0.00095, 0.05987, 0.06327, 0.09056, 0.02758, 0.00978, 0.02360, 0.00150, 0.01974, 0.00074};

double cosineSimilarity(const vector<double>& a, const vector<double>& b) {
    // We get the cosine value of the angle between the input vectors a and b here.
    double dotProduct = 0.0;
    double normA = 0.0;
    double normB = 0.0;
    for (size_t i = 0; i < a.size(); ++i) {
        dotProduct += a[i] * b[i];
        normA += a[i] * a[i];
        normB += b[i] * b[i];
    }
    return dotProduct / (sqrt(normA) * sqrt(normB));
}

int getKey (vector<int> arr) {
    // We will brute force all posssibilities from 3 to 100 and check which number divided the most of the distance values present in the array.
    vector<int>values;
    for (int i = 3; i < 100; i++) {
        int curr = 0;
        for (int x : arr) {
            if (x % i == 0) curr++;
        }
        values.push_back(curr);
    }
    int m = 1;
    int curr = 0;
    for (int i = 0; i < values.size(); i++) {
        if (curr <= values[i]) {
            curr = values[i];
            m = i + 3;
        }
    }
    return m;
}

string preProcess(string s) {
    // This function will return string which only contains characters from A to Z.
    string ans = "";
    for (int i = 0; i < s.length(); i++) {
        if ((s[i] - 'A' >= 0) && (s[i] - 'A' < 26)) ans += s[i];
    }
    return ans;
}



int caesarCipher(string s) {
    vector<double> freq(26, 0);
    for (char c : s) {      // We store the frequencies here.
        if (isupper(c)) {
            int x = c - 'A';
            freq[x]++;
        }
    }

    int sum = 0;
    for (double x : freq) {
        sum += x;
    }
    for (int i = 0; i < 26; i++) {
        freq[i] = freq[i] / sum;    // We will find the relative frequency here.
    }

    vector<vector<double> > frequencies;
    frequencies.push_back(freq);
    for (int i = 1; i < 26; i++) {      // We will generate 26 different vectors by shifting our current vector generated from the text.
        vector<double> curr = freq;
        vector<double> temp(26, 0);
        temp[0] = curr[25];
        for (int j = 1; j < 26; j++) {
            temp[j] = curr[j - 1];
        }
        frequencies.push_back(temp);
        freq = temp;
    }

    vector<double> cosineValues;
    vector<double> frequency;
    for (int i = 0; i < 26; i++) {
        frequency.push_back(f[i]);
    }
    for (const auto& f : frequencies) {     // We apply the cosine similarity here and store the cosine values in the array.
        double cosine = cosineSimilarity(f, frequency); 
        cosineValues.push_back(cosine);
    }

    double maxCosine = 0;
    int k = 0;
    for (int i = 0; i < cosineValues.size(); i++) {     // We check which index has the highest cosine value. The index which has the highest index will be our key.
        if (maxCosine < cosineValues[i]) {
            maxCosine = cosineValues[i];
            k = i;
        }
    }
    k = 26 - k;         // We wiil take 26 - k as we have shifted the frequency vector from forward direction.
    if (k == 26) k = 0;
    char x = 'A' + k;   // This will help us print the key.
    cout << x;
    return k;
}

int keyLength(string s) {
    string t = preProcess(s);       // This function will return string which only contains characters from A to Z.
    int key = 2;
    double val = 0;
    int n = t.length();
    for (int k = 3; k <=100; k++){
        double avg = 0;
        int count = 0;
        for (int i = 0; i < k; i++) {
            int idx = i;
            string temp1 = "";
            while (idx < n) {           // This loop will help us to divide the string based on the key we have.
                temp1 += t[idx];
                idx += k;
            }
            vector<double> freq(26, 0);
            
            for (char c : temp1) {      // We store the frequencies here.
                if (isupper(c)) {
                    int x = c - 'A';
                    freq[x]++;
                    count++;
                }
            }
            

            int sum = 0;
            for (double x : freq) {
                sum += x;
            }
            for (int l = 0; l < 26; l++) {
                freq[l] = freq[l] / (double)sum;    // We will find the relative frequency here.
            }
            for (double x : freq){
                avg += x*x;                     // We find the sum of squares of relative frequencies here.
            }
            
        }
        avg /= k;           // We take the average of all summations across k different partitions of the string
        if (val < avg){     // If this value is near to actual frequency square summation and greater, then we replace older key with the keyent key.
            val = avg;
            key = k;
        }
        
    }
    return key;
}

string Decrypter(string s) { 

    cout << "The Key used to encrypt the given information is: ";
    // int key = keyLength1(s);     // This function will return the length of the key.
    int key = keyLength(s);
    // int key = 260;
    cout << endl;
    int n = s.length();
    vector<int> extraChars;
    for (int i = 0; i < n; i++) {
        if (s[i] - 'A' < 0 || s[i] - 'A' >= 26) {
            extraChars.push_back(i);        // This operation will store all other characters which are not from the alphabets A to Z.
        }
    }
    string t = preProcess(s);       // This function will return string which only contains characters from A to Z.
    string ans = "";
    vector<string> temp;
    for (int i = 0; i < key; i++) {
        int idx = i;
        string temp1 = "";
        while (idx < n) {           // This loop will help us to divide the string based on the key we have.
            temp1 += t[idx];
            idx += key;
        }
        int k = caesarCipher(temp1);    // This function will apply caesar cipher and return the key of length 1 corresponding to the text running in that loop.
        string decrypt = "";
        for (char c : temp1) {
            if (isupper(c)) {
                int x = (c - 'A' - k + 26) % 26;
                char y = 'A' + x;
                decrypt += y;
            } else {
                decrypt += c;
            }
        }
        temp.push_back(decrypt);        // With the key we got from caesar cipher, we decode the corresponding text here.
    }
    int curr = 0;
    int idx1 = 0;
    for (int i = 0; i < n; i++) {       // We will use this loop to add the extra characters which we have not used in our process.
        int f = 0;
        for (int j = 0; j < extraChars.size(); j++) {
            if (i == extraChars[j]) { 
                ans += s[i];
                f = 1;
                break;
            }
        }
        if (f == 0) {
            ans += temp[curr][idx1];
            curr++;
            if (curr == key) {
                curr = 0;
                idx1++;
            }
        }
    }
    return ans;
}

int main() {
    string filename;
    cout << "Enter the name of the file containing the cipher text: ";
    getline(cin, filename);         // This line will take the file name as input from the user.
    // Make sure that the input given contains alphabets starting from A to Z and only in upper case letters.
    ifstream inputFile(filename); 
    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return 1;
    }

    string s;
    getline(inputFile, s); 
    inputFile.close(); 
    string ans = Decrypter(s);      // Decrypter function will return the decoded string corresponding to the encoded text given.
    // This will also print the key used to encode the input text.
    cout << "\n\nDecrypted text is: \n" << ans << endl; 

    return 0;
}

/*  THIS IS ANOTHER APPROACH WROTE BELOW WHICH FINDS LENGTH OF THE KEY USING KRASKI'S METHOD */

// int keyLength1(string s) {
//     vector<vector<vector<int> > > tri_grams(26, vector<vector<int> > (26, vector<int>(26, 0)));
//     string t = preProcess(s);           // This function will return string which only contains characters from A to Z.
//     int n = t.length();
//     for (int i = 0; i < n-2; i++) {     // We store all possible trigrams from the input text.
//         int a1 = t[i] - 'A';
//         int a2 = t[i+1] - 'A';
//         int a3 = t[i+2] - 'A';
//         tri_grams[a1][a2][a3]++;
//     }
//     int m1 = 0;
//     int m2 = 0;
//     int m3 = 0;
//     int max = 0;
//     for (int i = 0; i < 26; i++) {      // We will get the trigram which has the highest frequency in the input text.
//         for (int j = 0; j < 26; j++) {
//             for (int k = 0; k < 26; k++) {
//                 if (tri_grams[i][j][k] > max) {
//                     max = tri_grams[i][j][k];
//                     m1 = i;
//                     m2 = j;
//                     m3 = k;
//                 }
//             }
//         }
//     }
//     vector<int> distances;
//     int first_idx = 0;
//     for (int i = 0; i < n-2; i++) {     // We get the first index of the maximum occuring tri gram.
//         int a1 = t[i] - 'A';
//         int a2 = t[i+1] - 'A';
//         int a3 = t[i+2] - 'A';
//         if (a1 == m1 && a2 == m2 && a3 == m3) {
//             first_idx = i;
//             break;
//         }       
//     }
//     for (int i = first_idx + 1; i < n-2; i++) {     // We get the distance from the current index of the trigram to it's first index and store it in distance array.
//         int a1 = t[i] - 'A';
//         int a2 = t[i+1] - 'A';
//         int a3 = t[i+2] - 'A';
//         int dist;
//         if (a1 == m1 && a2 == m2 && a3 == m3) {
//             dist = i - first_idx;
//             distances.push_back(dist);
//         }       
//     }
//     int key = getKey(distances);            // This will help us to get the key length.
//     return key;
// }

//  END OF CODE // 
