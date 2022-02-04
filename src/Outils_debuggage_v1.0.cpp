
// écriture dans le log (idem printf); %d pour un entier ; %f pour un float (ou un double) en écriture décimale ; %.2f pour un float (ou un double) en écriture décimale avec 2 chifres derrière la virgule ; %E pour la notation scientifique ; %s pour un string

if (sc.Index == 100 )
{
	SCString message ;
	message.Format("%d", var) ;  
	sc.AddMessageToLog(message, 0) ;
}



SCString message ;
message.Format("INQ100 : %f  // SpreadMoyen : %f", INQ100SansCorrection[index], SpreadMoyen[index]) ;  
sc.AddMessageToLog(message, 0) ;


// date de l'index au format YY.MMJJ  // La date retournée correspond à la date à l'instant t dans le fuseau horaire choisi, et pas au "trading day".
int Year, Month, Day ;
DATE_TO_YMD(DATE_PART(sc.BaseDateTimeIn[index]), Year, Month, Day);
float Date = Year-2000 + float(Month)/100 + float(Day)/10000 ;


		
		
// RAZ d'IntradayBarIndex lors du changement de jour
if  (  sc.BaseDateTimeIn.TimeAt(sc.Index) < sc.BaseDateTimeIn.TimeAt(sc.Index-1)   )  
{ }




// STRINGS

	// additionner 2 SCString : "Blabla1" + "Blabla2" = "Blabla1Blabla2"
	SCString  Message1 ;
	SCString  Message2 ;
	Message1.Format("Blabla1") ;
	Message2.Format("Blabla2") ;
	Message1.Append (Message2) ;

		
		
	// conversion d'un std::string en SCString :
	std :: string MyString = "Some text here" ;
	SCString MySCString ;
	const char* adress = &(test[0])  ;
	MySCString.Format(adress) ;
	// code plus court :
	std :: string MyString = "Some text here" ;
	SCString MySCString ;
	MySCString.Format( &(test[0]) ) ;


	
	// ajout de l'horaire dans le log
	SCString message ;
	int Hour, Minute, Second;
	TIME_TO_HMS(sc.BaseDateTimeIn[index].GetTimeWithoutMilliseconds(), Hour, Minute, Second);  // TIME_TO_HMS nécessite une time_value (un entier) en entrée ; GetTimeWithoutMilliseconds convertit un SCDateTime en time_value (un entier).
	float time = float(Hour)*100 + float(Minute) + float(Second)/100;  // tracé au format 1234.16 pour 12h34m16s   //  si autoloop, remplacer index par sc.Index
	message.Format("time = %.2f // %f", time, AutreVariable) ;  
	sc.AddMessageToLog(message, 0) ;

	// ajout de l'horaire dans le log (autre méthode)
	SCString message ;
	int Hour, Minute, Second;
	TIME_TO_HMS(sc.BaseDateTimeIn[index].GetTimeWithoutMilliseconds(), Hour, Minute, Second);
	SCString message ;
	message.Format("!! ARRET CALCUL !! MaxBarresCalculees (%d barres) a ete depasse a %dh%dm%ds", MaxBarresCalculees, Hour, Minute, Second) ;  



	