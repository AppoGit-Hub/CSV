#include "Global.hpp"

typedef struct model Model;
struct model {
	int move;
	double v_accs[600];
};

void convertFiletoTable(std::ifstream& accessFilePattern, std::array<Model, NUMBER_MOVEMENTS>& pModelsMovements);
int indexOfMinValue(std::array<double, NUMBER_MOVEMENTS> arrayValue, int numberValue);
void displayResultsForEachClass(std::array < uint64_t, NUMBER_MOVEMENTS> estimateClasses, std::array < uint64_t, NUMBER_MOVEMENTS> realClasses);

void evaluationTestSet() {
	std::ifstream pattern_file(PATTERN_FILENAME);
	std::ifstream testset_file(TRAINSET_FILENAME);
	if (!pattern_file.is_open() || !testset_file.is_open()) {
		std::cout << "Couldnt open file : " << CHECK_FILENAME << " or " << TRAINSET_FILENAME << std::endl;
	}
	else {
		std::array<Model, NUMBER_MOVEMENTS> modelsMovements;
		std::array < uint64_t, NUMBER_MOVEMENTS> estimateClasses;
		std::array < uint64_t, NUMBER_MOVEMENTS> realClasses;
		estimateClasses.fill(0);
		realClasses.fill(0);

		convertFiletoTable(pattern_file, modelsMovements);

		std::string line;
		std::getline(testset_file, line);// for the header 

		while (std::getline(testset_file, line)) {
			int gender, realMovement, index;
			char delimiter;
			
			std::array<double, NUMBER_MOVEMENTS> distancesCloseModels;

			std::istringstream oneMovementTested(line);
			oneMovementTested >> realMovement >> delimiter >> gender >> delimiter >> index >> delimiter;
			realClasses[realMovement - 1]++;

			int iMovement = 0;
			while (iMovement < NUMBER_MOVEMENTS) {
				int iVectAccel = 0;
				double totalSumOfSquaredDifference = 0;
				double v_acc;

				oneMovementTested.clear();
				oneMovementTested.str(line);
				oneMovementTested >> realMovement >> delimiter >> gender >> delimiter >> index >> delimiter;

				while (oneMovementTested >> v_acc >> delimiter && iVectAccel < 60)
				{
					//std::cout << v_acc << " ("<< iVectAccel << ") ";
					double difference = v_acc - modelsMovements[iMovement].v_accs[iVectAccel];
					totalSumOfSquaredDifference += pow(difference, 2);
					iVectAccel++;
				}
				// todo : eliminer plus tard 
				//std::cout << std::endl;
				//std::cout << "mouvement : " << iMovement + 1 << std::endl;
				//std::cout << std::endl;
				distancesCloseModels[iMovement] = sqrt(totalSumOfSquaredDifference);
				iMovement++;
			}
			int iEstimatedMovement = indexOfMinValue(distancesCloseModels, NUMBER_MOVEMENTS) ;
			std::cout << " real : " << realMovement << " estimate : " << iEstimatedMovement + 1 << std::endl;
			if(iEstimatedMovement + 1 == realMovement )
				estimateClasses[iEstimatedMovement]++;
		}
		displayResultsForEachClass(estimateClasses,realClasses);
	}
}


void convertFiletoTable(std::ifstream& accessFilePattern, std::array<Model, NUMBER_MOVEMENTS>& modelsMovements) {
	std::string line;
	int iMovement = 0;

	std::string header;
	std::getline(accessFilePattern, header);

	while (iMovement < NUMBER_MOVEMENTS &&	std::getline(accessFilePattern, line) ) {
		
		std::istringstream oneModelMovement(line);
		int iVAcc = 0;
		char delimiter;

		int moveCurrent;
		oneModelMovement >> moveCurrent >> delimiter;
		Model& modelAccurent = modelsMovements[moveCurrent - 1];
		modelAccurent.move = moveCurrent;
		
		while (oneModelMovement >> modelAccurent.v_accs[iVAcc] >> delimiter	&& iVAcc < TRAINSET_COLUMNS) {		
			iVAcc++;
		}
		iMovement++;

	}
}

int indexOfMinValue(std::array<double, NUMBER_MOVEMENTS> arrayValue,int numberValue) {
	double minValue = arrayValue[0];
	int iMinValue = 0;

	for (int iValueAccurent = 1; iValueAccurent < numberValue; iValueAccurent++) {
		if (arrayValue[iValueAccurent] < minValue) {
			minValue = arrayValue[iValueAccurent];
			iMinValue = iValueAccurent;
		}
	}

	for (int iArray = 0; iArray < 6; iArray++) {
		std::cout << iArray << " : " << arrayValue[iArray] << std::endl;
		std::cout << std::endl;
	}
	return iMinValue;

}
	
void displayResultsForEachClass(std::array < uint64_t, NUMBER_MOVEMENTS> estimateClasses, std::array < uint64_t, NUMBER_MOVEMENTS> realClasses) {
	double pourcentResult; 
	int totalEstimate = 0 , totalReal = 0;
	std::cout << "results of evaluation : " << std::endl;
	std::cout << " NUM NB_E NB_R % " << std::endl;
	for (int iMovement = 0; iMovement < NUMBER_MOVEMENTS; iMovement++) {
		pourcentResult = (static_cast<double> (estimateClasses[iMovement]) / ((realClasses[iMovement] == 0) ? 1 : realClasses[iMovement])) * 100;
		std::cout << " " << iMovement + 1 << "   " << estimateClasses[iMovement] << "    " << realClasses[iMovement] << "  " << pourcentResult << "%" << std::endl;
		totalEstimate += estimateClasses[iMovement];
		totalReal += realClasses[iMovement];
	}
	std::cout << "total real : " << totalReal << " estimate total : " << (static_cast<double> (totalEstimate)  / totalReal) * 100 << " % " << std::endl;
	std::cout << "FIN (results of evaluation) " << std::endl;
} 
