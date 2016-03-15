#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

//constants
const int zmienna = 10;
const int RulesFileCounter = 4;
const int FactsFileCounter = 11;

//structures
struct Fact{
	char name[4];
	char value[6];
	int rulesRefIndex[4];//as RulesFileCounter
};
struct SuperRule {
	struct Fact first;
	char sign[3];
	struct Fact second;
	char sign2[3];
	struct Fact result;
};



//functions
void getFacts(FILE *fileName, struct Fact facts[10]) {
	char temp[3];
	int i = 0;
	for (i = 0; i < FactsFileCounter; i++) {
		fscanf(fileName, "%s", &facts[i].name);
		fscanf(fileName, "%s", &temp);
		fscanf(fileName, "%s", &facts[i].value);
		//HOLY GRAIL !!!
		for (int k = 0; k < RulesFileCounter; k++){
			if (k == 0)
				facts[i].rulesRefIndex[k] = 1;
			else
				facts[i].rulesRefIndex[k] = -1;
		}
	}
}
int findFactIndex2(struct Fact facts[], struct Fact toFind) {
	for (int i = 0; i < FactsFileCounter; i++) {
		//printf("Compare %s with %s and the result is %d", facts[i].name, toFind.name, strcmp(facts[i].name, toFind.name));
		if (!strcmp(facts[i].name, toFind.name))/*enter if when condition is 1, so ! to invert 0*/ {
			return i;
		}
	}
}
void showRulesWithFacts(struct SuperRule superrule[]) {
	int len = RulesFileCounter - 1;
	for (int i = 0; i < len; i++) {
		printf("%s %s %s %s %s where:", superrule[i].first.name, superrule[i].sign, superrule[i].second.name, superrule[i].sign2, superrule[i].result.name);
		printf("%s=%s [", superrule[i].first.name, superrule[i].first.value);
		for (int j = 1; j < len; j++){
			printf(" %d, ", superrule[i].first.rulesRefIndex[j]);
		}
		printf("] ");
		printf("\t%s=%s [", superrule[i].second.name, superrule[i].second.value);
		for (int j = 1; j < len; j++) {
			printf(" %d, ", superrule[i].second.rulesRefIndex[j]);
		}
		printf("] ");
		printf("\t%s=%s [", superrule[i].result.name, superrule[i].result.value);
		for (int j = 1; j < len; j++) {
			printf(" %d, ", superrule[i].result.rulesRefIndex[j]);
		}
		printf("] \n");
	}
}
void addToStack(int toAdd, int whichRule, char whichChar[], struct SuperRule superrules[]) {
	//first element is stack pointer
	if (whichChar == "first") {
		superrules[whichRule].first.rulesRefIndex[superrules[whichRule].first.rulesRefIndex[0]++] = toAdd;
	}
	else if (whichChar == "second") {
		superrules[whichRule].second.rulesRefIndex[superrules[whichRule].second.rulesRefIndex[0]++] = toAdd;
	}
	else if (whichChar == "result") {
		superrules[whichRule].result.rulesRefIndex[superrules[whichRule].result.rulesRefIndex[0]++] = toAdd;
	}
}
void removeFromStack(int toRemove, int whichRule, char whichChar[], struct SuperRule superrules[]) {
	//first element is stack pointer
	//first find where this element is
	int removeIndex;
	for (int i = 1; i < RulesFileCounter; i++){
		if(superrules[whichRule].first.rulesRefIndex[i]==toRemove){
			removeIndex = i;
			break;
		}
	}
	//now remove this element and move the rest to the left
	if (whichChar == "first") {
		superrules[whichRule].first.rulesRefIndex[removeIndex] = -1;//remove
		for (int i = removeIndex; i < RulesFileCounter; i++){
			superrules[whichRule].first.rulesRefIndex[i] = superrules[whichRule].first.rulesRefIndex[i + 1];//move
		}
		superrules[whichRule].first.rulesRefIndex[0]--;
	}
	else if (whichChar == "second") {
		superrules[whichRule].second.rulesRefIndex[removeIndex] = -1;//remove
		for (int i = removeIndex; i < RulesFileCounter; i++) {
			superrules[whichRule].second.rulesRefIndex[i] = superrules[whichRule].second.rulesRefIndex[i + 1];//move
		}
		superrules[whichRule].second.rulesRefIndex[0]--;
	}
	else if (whichChar == "result") {
		superrules[whichRule].result.rulesRefIndex[removeIndex] = -1;//remove
		for (int i = removeIndex; i < RulesFileCounter; i++) {
			superrules[whichRule].result.rulesRefIndex[i] = superrules[whichRule].result.rulesRefIndex[i + 1];//move
		}
		superrules[whichRule].result.rulesRefIndex[0]--;
	}
}
void getRulesFromFile3(FILE *fileName, struct SuperRule superrules[], struct Fact facts[]) {
	struct Fact tempFact;
	int index = 0, j = 0, i=0;

	for (i = 0; i < RulesFileCounter; i++) {//for all lines in file
		fscanf(fileName, "%s", &tempFact.name);
		index = findFactIndex2(facts, tempFact);
		superrules[i].first = facts[index];
		addToStack(i, i, "first", superrules);
		//superrules[i].first.rulesRefIndex[j] = i;
		fscanf(fileName, "%s", &superrules[i].sign);
		fscanf(fileName, "%s", &tempFact.name);
		index = findFactIndex2(facts, tempFact);
		superrules[i].second = facts[index];
		addToStack(i, i, "second", superrules);
		//superrules[i].second.rulesRefIndex[j] = i;
		fscanf(fileName, "%s", &superrules[i].sign2);
		fscanf(fileName, "%s", &tempFact.name);
		index = findFactIndex2(facts, tempFact);
		superrules[i].result = facts[index];
		addToStack(i, i, "result", superrules);
		//superrules[i].result.rulesRefIndex[j] = i;
		j++;
	}
}
void findInRules(char find[], struct SuperRule superrules[], int found[]) {
	int index = 0, i = 0, j = 0,k=0;
	for (i = 0; i < RulesFileCounter-1; i++)
	{
		if (!strcmp(superrules[i].first.name, find) || !strcmp(superrules[i].second.name, find)) {
			found[j++] = i;
			printf("index=%d\n", i);
		}
	}
}
void foundReset(int found[]) {
	int i = 0;
	for (i = 0; i < RulesFileCounter; i++) {
		found[i] = -1;
	}
}



//obsolete and redundant shit
void rulesRefIndexReset(struct SuperRule superrules[]) {
	int p, q;
	for (p = 0; p < RulesFileCounter; p++) {
		for (q = 0; q < RulesFileCounter; q++) {
			superrules[p].first.rulesRefIndex[q] = -1;
			superrules[p].second.rulesRefIndex[q] = -1;
			superrules[p].result.rulesRefIndex[q] = -1;
		}
	}
}
void showFacts(struct Fact facts[]) {
	for (int i = 0; i < FactsFileCounter; i++){
		printf("Name: %s, RefVals: ", facts[i].name);
		for (int j = 0; j < FactsFileCounter; j++){
			printf("%d ", facts[i].rulesRefIndex[j]);
		}
		printf("Value: %d\n", facts[i].value);
	}
}


//main
int main() {
	/*ZALOZENIA
	1. Dane sa poprawne na wejsciu
	2. Dane sa w formacie: a && b => c , to jest, pojedyncze znaki odpowiadajace czemustam oddzielone spacja od operatorow logicznych.
	3. Pojedyncze znaki, a, nie ma ab.
	4. Dane wprowadzane do pliku, ktory wczytywany jest do programu
	5. Tylko 5 stringow na linie
	6. Plik rules.txt musi konczyc sie pusta linia
	*/
	struct Fact facts[11];//as FactsFileCounter
	struct SuperRule superrules[4];//as RulesFileCounter
	//struct SuperRuleFact [4];
	char toFind[3];
	int found[11] = { -1 };//as FactsFileCounter

	FILE *plik1 = fopen("rules.txt", "r");
	FILE *plik2 = fopen("facts.txt", "r");
	getFacts(plik2, facts);
	//dodac & do facts i gdzies na gorze tez a odwolania przez *
	getRulesFromFile3(plik1, superrules, facts);
//	addToStack(1, 0, "first", superrules);
	showRulesWithFacts(superrules);
	printf("\n");
	showFacts(facts);


	fclose(plik1);
	fclose(plik2);

	
	//findInRules("a", superrules, found);

	return 0;
}