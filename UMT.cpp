#include <cstring>
#include <iostream>

using namespace std;

int strongPassword(string password) {
	// len is the lenght of the password
	int len = password.length();
	// in these variables we will compute what is missing ( a digit, an uppercase letter or a lowercase letter)
	// in the beginning we suppose that the password has a digit, an uppercase and a lowercase
	int hasUppercase=1, hasLowercase=1, hasDigit=1;
	// I'm assuming that the password has no more than 1000 characters
	// this is an array that will hold the number of duplicates
	int duplicates[1000] = { 0 };
	int j;

	// we parse the password
	for (int i = 0; i < len;) {
		// when we find a digit/uppercase letter/lowercase letter we mark it with 0 in the corresponding variable
		if (password[i] >= 'a' && password[i] <= 'z')
			hasLowercase = 0;
		else if (password[i] >= 'A' && password[i] <= 'Z')
			hasUppercase = 0;
		else if (password[i] >= '0' && password[i] <= '9')
			hasDigit = 0;

		//we compute the number of duplicate characters
		j = i;
		while (i < len && password[j] == password[i]) {
			duplicates[j] ++;
			i++;
		}
	}

	// we compute how many characters are missing to be a strong password
	int missingCharacters = hasLowercase + hasUppercase + hasDigit;

	// if we don't have enough characters in the password
	if (len < 6) {
		// even if we add those missing characters we still don't have enough characters in the password
		if (missingCharacters <= 6 - len)
			// we have to add to the password 6-len characters (which will include those missing characters) and that will be
			//the answer
			return 6 - len;
		// but if we add those missing characters and our password has > 6 characters, it is okay and we don't have
		//to add anything else
		else	
			return missingCharacters;
	}
	//if the password has more than 6 characters
	else {
		//in the variable "answer" we will compute the final number of steps needed
		int answer = 0;
		int over20 = 0;
		// if the password contains more than 20 characters we need to delete len-20 characters
		// for now, that will be the number of steps we need to make
		if (len > 20) {
			over20 = len - 20;
			answer = over20;
		}
		
		// we can notice that 1 delete can resolve 3 duplicate characters, 1 insert can resolve 4 duplicate characters
		//and 1 replace can resolve 5 duplicate characters
		// so, the best case is to replace a character 
		// if we generalize it, m replaces can modify a duplicate series of characters of lenght 3m+2
		
		//so, we try to make every series of duplicates an 3m+2 multiple by verifying if:
		//		-> it is a multiple of 3m we delete 1 character to become a 3m+2 multiple
		// and we also update the number of characters we need to delete so that the string will have less than 20 characters

		for (int i = 0; i < len && over20 > 0; i++) {
			if (duplicates[i] >= 3 && duplicates[i] % 3 == 0) {
				duplicates[i]--;
				over20--;
			}
		}

		//		-> it is a multiple of 3m+1 we delete 2 characters to become a 3m+2 multiple
		// and we also update the number of characters we need to delete so that the string will have less than 20 characters
		for (int i = 0; i < len && over20 > 0; i++) {
			if (duplicates[i] >= 3 && duplicates[i] % 3 == 1) {
				duplicates[i] -= min(over20,2);
				over20 -= 2;
			}
		}

		// after we've deleted some characters to make the password the appropriate lenght we will probably need to do some
		//replacements too, to make sure that there are no more than 2 repeating characters in a row
		// we count them in the "replaces" variable
		int replaces = 0;
		for (int i = 0; i < len; i++) {
			// if we still need to eliminate some characters
			if (over20 > 0 && duplicates[i] >= 3) {
				// we will need to eliminate a number "need" of characters such that there will remain at most 2 duplicates
				int need = duplicates[i] - 2;
				if (over20 < need) {
					duplicates[i] -= over20;
				}
				else
					duplicates[i] -= need;
				over20 -= need;
			}
			// the replaces needed to fix the repeating characters is actually duplicates[i]/3 (because we took it to
			//the form 3m+2)
			if (duplicates[i] >= 3) {
				replaces += duplicates[i] / 3;
			}
		}
		// if the number of replaces is bigger than the number of missing characters we add the replaces to the result
		if (replaces > missingCharacters)
			answer += replaces;
		// otherwise we add the number of missing characters to the result because a missing character can be seen as a replace
		else
			answer += missingCharacters;
		return answer;
	}
}


int main()
{
	string password;
	// I assume that the password has no spaces in it
	cout << "Please insert a password: ";
	cin >> password;
    cout<<"The number of steps required is: "<<strongPassword(password);
	return 0;
}
