
byte rows[] = {10,16,9,4,5,3};


byte cols[] = {2,6,7,8};
const int rowCount = sizeof(rows)/sizeof(rows[0]);
const int colCount = sizeof(cols)/sizeof(cols[0]);

byte keys[colCount][rowCount];

void setup() {
	Serial.begin(9600);
  delay(500);

	for(int x=0; x<colCount; x++) {
		Serial.print(cols[x]); Serial.println(" as input");
		pinMode(cols[x], INPUT);
	}

	for (int x=0; x<rowCount; x++) {
		Serial.print(rows[x]); Serial.println(" as input-pullup");
		pinMode(rows[x], INPUT_PULLUP);
	}
		
}

void readMatrix() {

  for (int rowIndex=0; rowIndex < rowCount; rowIndex++) {
    byte curRow = rows[rowIndex];
    pinMode(curRow, OUTPUT);
    digitalWrite(curRow, LOW);

    for (int colIndex = 0; colIndex<colCount; colIndex++) {
      byte colRow = cols[colIndex];
      pinMode(colRow, INPUT_PULLUP);
      keys[colIndex][rowIndex] = digitalRead(colRow);
      pinMode(colRow, INPUT);
    }
    pinMode(curRow, INPUT);
  }  
}

void printMatrix() {
	for (int rowIndex=0; rowIndex < rowCount; rowIndex++) {
		if (rowIndex < 10)
			Serial.print(F("0"));
		Serial.print(rowIndex); Serial.print(F(": "));

		for (int colIndex=0; colIndex < colCount; colIndex++) {	
			Serial.print(keys[colIndex][rowIndex]);
			if (colIndex < colCount)
				Serial.print(F(", "));
		}	
		Serial.println("");
	}
	Serial.println("");
}

void loop() {
	readMatrix();
	if (Serial.read()=='!')
		printMatrix();
}