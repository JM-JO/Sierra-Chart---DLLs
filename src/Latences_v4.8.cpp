#include <chrono>
#include "sierrachart.h"

SCDLLName("Latences")

/* 
v1.0 (2019/10/2) : affichage de DureeCycle dans la fenêtre sc.GraphRegion.
v1.1 (2019/10/10) : affichage dans le log du delta entre l'horaire local et l'horodatage du dernier tick (ce n'est pas le lag, car le dernier tick a pu arriver depuis plusieurs secondes).
v1.2 (2019/10/11) : affichage dans le log uniquement quand des ticks arrivent.
v1.3 (2019/10/11) : ajout d'IntradayIndex.
v1.4 (2019/10/11) : écriture dans un fichier .csv.
v1.5 (2019/10/11) : noms de fichier différents pour les différents flux.
v1.6 (2019/10/11) : le nom de fichier est géré par une variable SCString persistante ; ce SCString contient : le nom de l'instrument + son UT + la date de la séance. Les en-têtes de colonne sont ajoutés à la création du fichier .csv.
v1.7 (2019/10/13) : création de la fonction ConversionFloat qui convertit un float en string avec un séparateur décimal "," pour le Excel français.
v1.8 (2019/10/14) : ajout d'un message dans le log pour indiquer que l'acquisition de données a lieu dans le fichier XYZ.csv.
v2.0 (2019/11/3) : création de la study "Duree cycle chart".
v2.1 (2019/11/3) : gestion de sc.CalculationPrecedence.   !! Warning !! Attention à toujours cliquer le bouton Apply ou le bouton OK deux fois quand on change l'input de CalculationPrecedence sinon il n'est pas pris en compte correctement.
v2.2 (2019/11/4) : ajout de l'input StudyONOFF. Création de la study "Duree cycle chart - Study de controle" : cette study permet de mettre ON ou OFF toutes les studies "DUREE CYCLE CHART".
v2.3 (2020/1/15) : la study scsf_LatenceFluxTicks gère automatiquement le décalage horaire entre l'heure locale (PC ou VPS) et l'heure du graphique Sierra.
v2.4 (2020/1/16) : ajout d'un test " sc.ChartIsDownloadingHistoricalData() " en complément du test " NotAFullRecalculation " pour ne pas avoir les ticks téléchargés lorsque le real-time démarre.
v2.5 (2020/1/17) : réécriture du code avec variables en anglais dans le but de diffuser celui-ci sur Sierra.
v2.6 (2020/1/21) : codage d'une fonction qui extraie les lignes d'un fichier texte avec getline.  Fonction effacée et reportée dans IPC_v0.0.cpp.
v2.7 (2020/02/03) : on remplace system_clock par steady_clock => c'est idiot, car dans ce DLL on a besoin du temps absolu de system_clock.
v2.8 (2020/02/04) : utilisation de QueryPerformanceCounter pour mesurer des durées.
v2.9 (2020/02/12) : on distingue les symboles des indices ("$..."), des futures avec la fonction Compare(). Les autres symboles sont considérés comme des actions.
v3.0 (2020/02/13) : on ajoute sc.Bid, sc.BidSize, sc.Ask, sc.AskSize dans le fichier d'écriture (uniquement pour le dernier tick du paquet). A chaque changement de seconde (LocalTime), on indique le nb de paquets reçus dans la seconde précédente.
v3.1 (2020/03/17) : utilisation de QueryPerformanceCounter pour mesurer des durées de scsf_DureeCycleChart.
v3.2 (2020/06/17) : dans la study "Latence flux ticks", on met en forme le bid et le ask.
v3.3 (2020/06/17) : dans la study "Latence flux ticks", on ajoute l'input "Periode echantillonnage Ask/Bid (ms) (0=OFF)".  !! DLL non finalisé au profit de la version v3.4 qui est plus aggressive, et qui crée moins de data inutile.
v3.4 (2020/06/17) : dans la study "Latence flux ticks", on ajoute l'input "Echantillonnage Ask/Bid (yes/no)" qui acquière le bid-ask s'il est différent du dernier bid-ask connu.
v3.5 (2020/06/18) : dans la study "Latence flux ticks", on lance un TriggerRecalculate si Delta_Seconde est négatif. Lorsqu'on change de jour (différent de la date du fichier .csv) pdt une acquisition, il faut lancer un TriggerRecalculate pour débuter un nouveau fichier .csv. Car le DecalageHoraire calculé offline est souvent faux (typiquement les fois où on se connecte (F11) après avoir ouvert le chartbook), car il est calculé sur le dernier tick (il date souvent de la veille pour les flux indice ou action).
v3.6 (2020/06/18) : dans la study "Latence flux ticks", on ajoute le motif d'écriture : "new tick", "ask up", "ask down", etc...
v3.7 (2020/06/29) : modifs mineures dans scsf_DureeCycleChart ; modifs sur instructions liées à SCDateTime.
v3.8 (2020/08/26) : ajout des millisecondes officielles. Le DeltaTimeStamp correspond dorénavant à la vraie différence de TimeStamp.
v3.9 (2020/08/29) : lorsque EchantillonnageAskBid_Input = OFF, on ne trace plus aucun élément lié au Bid-Ask.
v4.1 (2020/09/04) : création de l'indicateur qui retourne le lag du flux de data sur le graphe 16-ticks = HoraireLocal - BaseDataEndDateTime.
v4.2 (2020/11/05) : modification de l'indicateur qui retourne le lag du flux de data pour qu'il trace un graphe du lag.
v4.3 (2020/11/17) : modifications mineures.
v4.4 (2020/11/19) : modification de l'indicateur qui retourne la latence pour qu'il trace un graphe du lag.
v4.5 (2020/11/19) : correction d'une erreur de calcul de MM_Lag.
v4.6 (2020/11/20) : ajout de MM_Lag_TickRapide et MM_Lag_TickLent (resp. dernier et premier tick d'un paquet de tick).
v4.7 (2020/11/21) : résolution du bug des valeurs de lag très élevées lors de la connection au serveur de data.
v4.8 (2020/12/10) : résolution du bug de scsf_DureeCycleChart (durée vs instance0 fausse ; cause = PersistentIntFast vs PersistentInt).



Correction de bugs à faire :
- scsf_LatencyDataFeed est obsolète (précision des ticks = 1 seconde) => tenir compte des millisecondes des ticks pour le calcul de la vrai latence.


Amélioration possible de "latence flux ticks" :
- pour indice : retourner l'EMA(10) du DeltaTimeStamp.



Amélioration possible de "DUREE CYCLE CHART" :
- lorsque les lignes sont tracées en quasi-doublon dans le log, supprimer la première ligne du log (mettre un input qui gère le doublon)





SOMMAIRE DES STRUCTURES ET FONCTIONS :
Texte_RelativePosition_UTAMAOA
DrawText_RelativePosition_UTAMAOA
Texte_UTAMAOA_UDD
DrawText_UTAMAOA_UDD
 */


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Texte_UTAMAOA : s_UseTool   // Texte bas centré
{											
	Texte_UTAMAOA (SCString texte, int tailleTexte,  COLORREF couleur, int graphRegion, int Line_Number) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
	{
		Clear(); 
		AddMethod = UTAM_ADD_OR_ADJUST ;			
		LineNumber = Line_Number ;
		DrawingType = DRAWING_TEXT;
		Text = texte ;		
		BeginDateTime = 1 ;     // To use a value relative to the left side of the chart for BeginDateTime rather than an absolute Date Time value, specify an integer value from 1 to 150
		UseRelativeVerticalValues = true ;   // If UseRelativeVerticalValues is set to 1 or a nonzero value, then BeginValue is a value relative to the Chart Region itself. It is a percentage. Where 1 = 1%.
		BeginValue = 50 ;   // position verticale entre 0 et 100
		FontSize = tailleTexte ; 
		Color = couleur ;
		Region = graphRegion ; 
		FontBold = 1 ;
		TextAlignment = DT_VCENTER | DT_LEFT;  // options disponibles : DT_TOP, DT_VCENTER, DT_BOTTOM, DT_CENTER, DT_LEFT, DT_RIGHT
		FontBackColor = RGB(200,200,200) ;
	} 
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct HLine : s_UseTool  // HLine crée un objet qui contient mes paramètres choisis pour une ligne horizontale  (hérite de toutes les données public et protected de s_UseTool)
	{
		HLine(float niveau, int epaisseur, COLORREF rgb, int graphRegion) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
		{
			Clear(); 
			DrawingType = DRAWING_HORIZONTALLINE;
			LineStyle = LINESTYLE_SOLID;
			AddMethod = UTAM_ADD_ALWAYS;	
			BeginValue = niveau;
			LineWidth = epaisseur;
			Color = rgb; 
			Region = graphRegion ;
			DrawUnderneathMainGraph = true ;
		} 
	};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
void DrawHLine (float monNiveau, int monEpaisseur, COLORREF maCouleur, s_sc* ptrsc)
	{
		//Création d'instance de HLine et appel de sc.UseTool
		HLine objetLigne(monNiveau, monEpaisseur, maCouleur, ptrsc->GraphRegion);
		ptrsc->UseTool(objetLigne); // c'est sc.UseTool qui est appelé
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Marqueur = MARKER_POINT, MARKER_DASH, MARKER_SQUARE, MARKER_STAR, MARKER_PLUS, MARKER_X, MARKER_ARROWUP, MARKER_ARROWDOWN, MARKER_ARROWRIGHT, MARKER_ARROWLEFT, MARKER_TRIANGLEUP, MARKER_TRIANGLEDOWN, MARKER_TRIANGLERIGHT, MARKER_TRIANGLELEFT, MARKER_DIAMOND

struct Marker_UTAMAOA : s_UseTool    // crée un Marker pour être affiché en coordonnées (indice,prix)
{											
	Marker_UTAMAOA (int marqueur, int index, float prix, int tailleObjet, COLORREF couleur, int graphRegion/* , int Line_Number */) : s_UseTool()    
	{
		Clear(); 
		AddMethod = UTAM_ADD_OR_ADJUST ;		
		// LineNumber = Line_Number ;		
		DrawingType = DRAWING_MARKER;
		MarkerType = marqueur ;
		BeginIndex = index ;
		BeginValue = prix ;
		LineWidth = 1 ;
		MarkerSize = tailleObjet ;
		Color = couleur ;
		Region = graphRegion ; 
		DrawUnderneathMainGraph = true ;
	} 
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawMarker_UTAMAOA (int marqueur, int index, float prix, int tailleObjet, COLORREF couleur, int graphRegion, s_sc* ptrsc)  
{
	//Création d'instance de Marker_UTAMAOA et appel de sc.UseTool
	Marker_UTAMAOA objetMarker(marqueur, index, prix, tailleObjet, couleur, graphRegion);
	ptrsc->UseTool(objetMarker); 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool FileExists (const std::string& name)             // https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
 {
    std::ifstream f(name.c_str());
    return f.good();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string ConversionFloat(float number)   // fonction qui convertit un float en string avec un séparateur décimal "," pour le Excel français.
{
	std::string MyString ;
	std::ostringstream oss;  // ostringstream = output stream class to operate on strings
	
	// Conversion of the float number into a string
	if (number <= 10000 )
	{
		oss << number;       // par défaut number est converti en oss avec 6 chiffres (le 7è saute)    // pour le modifier : http://www.cplusplus.com/reference/iomanip/setprecision/    http://www.cplusplus.com/reference/ios/ios_base/precision/
	}
	else		
	{
		oss << std::fixed;		// std::fixed garantit que les chiffres avant la virgule ne sont pas tronqués.  En complément de std::fixed, std::setprecision donne le nb de chiffres après la virgule.
		oss << std::setprecision(2) << number;     // 2 chiffres après la virgule
	}
	MyString = oss.str() ;
	
	// Replacement of '.' by ','    
	std::replace( MyString.begin(), MyString.end(), '.', ',');

	return MyString ;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_LatencyDataFeed(SCStudyInterfaceRef sc) 
{
	// Références /////////////////////////////////////////////////////////////////////////////////////////////
	SCInputRef EchantillonnageAskBid_Input = sc.Input[0];                   
	
	
	// Références /////////////////////////////////////////////////////////////////////////////////////////////
	int& Clicks_PreviousCycle = sc.GetPersistentIntFast(1);  
	int& PreviousUpdateLastIndex = sc.GetPersistentIntFast(3);  
	int& IndexRAZIntraday = sc.GetPersistentIntFast(4);  
	int& Nb_Paquets_Intraseconde = sc.GetPersistentIntFast(5);  
	int& DecalageHoraire = sc.GetPersistentIntFast(6);  	
	__int64& ElapsedTicks_PreviousCycle = sc.GetPersistentInt64(1);  
	SCString& NameLogFile = sc.GetPersistentSCString(1);
	float& AskPrecedent = sc.GetPersistentFloatFast(1);
	float& BidPrecedent = sc.GetPersistentFloatFast(2);
	SCDateTime& ComputerLocalTime_PaquetPrecedent = sc.GetPersistentSCDateTimeFast(1);  
	SCDateTime& DateFichierCSV = sc.GetPersistentSCDateTimeFast(2);  


	// Variables /////////////////////////////////////////////////////////////////////////////////////////////
	int NotAFullRecalculation = 1 ;
	int const LastIndex = sc.ArraySize-1 ;
	s_sc* const ptrsc = &sc;      // c'est un pointeur constant sur un s_sc   // attention : " const s_sc* " est un pointeur sur un s_sc constant
	SCString Texte ; 
	
	
	// Bloc sc.SetDefaults /////////////////////////////////////////////////////////////////////////////////////////////
	if (sc.SetDefaults)
	{
		sc.GraphName = "Latence flux ticks (acquisition de data.csv)" ;   
		sc.GraphRegion = 1 ;  
		sc.ValueFormat = 0 ;
		sc.AutoLoop = 0 ;  
		sc.DisplayStudyName = 1;
		sc.DisplayStudyInputValues = 1;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 0 ;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		
		sc.ResetAllScales = 1 ; 

		EchantillonnageAskBid_Input.Name = "Echantillonnage Ask/Bid" ;
		EchantillonnageAskBid_Input.SetYesNo(0);
	
		return ;	
	} // fin sc.Defaults
	
	
	
	// Get the millisecond part   ///////////////     https://stackoverflow.com/questions/21470315/odd-behaviour-using-chronohigh-resolution-clocknow    !! il faut utiliser system_clock (temps exact), et ne pas utiliser steady_clock (qui est high_resolution_clock a priori) ou high_resolution_clock (temps relatif au démarrage de SC)
	int Millisecondes_Local ;
	{
		using namespace std;
		using namespace std::chrono;
		system_clock::time_point now = system_clock::now();    	 //  https://stackoverflow.com/questions/27136854/c11-actual-system-time-with-milliseconds
		system_clock::duration tp = now.time_since_epoch();
		tp -= duration_cast<seconds>(tp); 										  // on soustrait le time_point arrondi à la seconde au time_point non arrondi => il ne reste que les ms  // tp est exprimé en nanosecondes
		Millisecondes_Local =   tp.count() / 1000000 ;
	}	

	
	// Conversion de l'heure du PC en SCDateTime /////////////////////////////
	SCDateTime ComputerLocalTime ;
	{
		// Temps du PC (local time)
		time_t now = time(0) ;
		tm *ltm = localtime(&now) ;    // cf https://www.tutorialspoint.com/cplusplus/cpp_date_time.htm
		
		// Extraction des membres de ltm
		int Annee = 1900 + ltm->tm_year ;
		int Mois = 1 + ltm->tm_mon ;
		int JourDuMois = ltm->tm_mday ;
		int Heure = ltm->tm_hour ;
		int Minute = ltm->tm_min ;
		int Seconde = ltm->tm_sec ;

		// Conversion en objet SCDateTime
		ComputerLocalTime.SetDateTimeYMDHMS_MS(Annee, Mois, JourDuMois, Heure, Minute, Seconde, Millisecondes_Local) ;   
	}
	
	
	
	// bloc effectué uniquement en cas de FullRecalculation, ou lorsqu'une nouvelle séance démarre
	if ( sc.UpdateStartIndex == 0  or   (  sc.BaseDateTimeIn.TimeAt(LastIndex) >= 64800    and   sc.BaseDateTimeIn.TimeAt(PreviousUpdateLastIndex) < 64800   )    )
	{
		// Initialisations
		sc.GraphRegion = 1 ;  		
		IndexRAZIntraday = 0 ;
		Nb_Paquets_Intraseconde = 0 ;
		
		if ( sc.UpdateStartIndex == 0 ) 
		{ 
			NotAFullRecalculation = 0 ;  			 //  ce bloc n'est parcouru qu'en cas de FullRecalculation
			PreviousUpdateLastIndex = 999999999 ;
		}
		
		
		// Recherche de IndexRAZIntraday
		SCString ChartSymbol = sc.GetChartSymbol(sc.ChartNumber) ;
		enum Symbol_Type { Indetermine, Indice, Action, Future } ThisSymbol ;
		if ( ChartSymbol.Compare("$",1)==0 ) // le Symbol est un indice            // The Compare function makes a comparison between the SCString object and StringToCompare and returns 0 if they are the same. An optional NumChars can be supplied which will compare only the first NumChars between the strings, otherwise the entire string is compared.
		{
			ThisSymbol = Indice ;
			if ( sc.BaseDateTimeIn[LastIndex].GetDate() != ComputerLocalTime.GetDate() )   // le LastIndex date de la veille
			{ IndexRAZIntraday = LastIndex + 1 ;  }
			else
			{
				for ( int index = LastIndex ; index != 0 ; index -- )
				{
					// recherche du début de la séance  
					if  (  sc.BaseDateTimeIn.TimeAt(index) <  sc.BaseDateTimeIn.TimeAt(index-1)  )   
					{
						IndexRAZIntraday = index ;
						break ;
					}
				}
				
			}
		}
		else if ( ChartSymbol.Compare("ES",2)==0 or ChartSymbol.Compare("NQ",2)==0 or ChartSymbol.Compare("MES",3)==0 or ChartSymbol.Compare("MNQ",3)==0 )  // le Symbol est un future
		{
			ThisSymbol = Future ;
			for ( int index = LastIndex ; index != 0 ; index -- )
			{
				// recherche du précédent changement de séance  // 64800 est le nb de secondes à 18h   
				if  (  sc.BaseDateTimeIn.TimeAt(index) >= 64800    and   sc.BaseDateTimeIn.TimeAt(index-1) < 64800   )   
				{
					IndexRAZIntraday = index ;
					break ;
				}
			}
		}
		else // le Symbol est une action
		{
			ThisSymbol = Action ;
			if ( sc.BaseDateTimeIn[LastIndex].GetDate() != ComputerLocalTime.GetDate() )   // le LastIndex date de la veille
			{ IndexRAZIntraday = LastIndex + 1 ;  }
			else
			{
				for ( int index = LastIndex ; index != 0 ; index -- )
				{
					// recherche du début de la séance  
					if  (  sc.BaseDateTimeIn.TimeAt(index) <  sc.BaseDateTimeIn.TimeAt(index-1)  )   
					{
						IndexRAZIntraday = index ;
						break ;
					}
				}
			}
		}
		
		// Création du nom du fichier .csv pour l'écriture des data de la séance   // Le format du fichier est "Latence flux - [Symbol] - [UT] - [Date séance]" 
		{
			// Debut du nom de fichier
			NameLogFile.Format("Latence flux - ") ;
			
			// Nom du symbol
			SCString SymbolAppend ;
			if 			( ChartSymbol.Compare("NQ-",3) == 0 ) { SymbolAppend = "Fut NQ100" ;  }
			else if 	( ChartSymbol.Compare("ES-",3) == 0 ) { SymbolAppend = "Fut SP500" ;  }
			else if 	( ChartSymbol == "$IUXX" ) 					{ SymbolAppend = "Ind NQ100" ;  }
			else if 	( ChartSymbol == "$SPX" ) 					{ SymbolAppend = "Ind SP500" ;  }
			else 																	{ SymbolAppend = ChartSymbol ;  }
			NameLogFile.Append(SymbolAppend) ;
			
			// GetBarPeriodParameters
			NameLogFile.AppendFormat(" - UT ") ;
			n_ACSIL :: s_BarPeriod BarPeriod ;
			sc.GetBarPeriodParameters(BarPeriod) ;
			int QuantiteUnite = BarPeriod.IntradayChartBarPeriodParameter1 ; // pour de l'UT 20s, QuantiteUnite = 20
			NameLogFile.AppendFormat("%d", QuantiteUnite) ;
			SCString Unite ;  // seconde ou tick
			if ( BarPeriod.IntradayChartBarPeriodType == IBPT_DAYS_MINS_SECS ) { Unite = " sec - " ; }   else if ( BarPeriod.IntradayChartBarPeriodType == IBPT_NUM_TRADES_PER_BAR ) { Unite = " tick - " ; }  else { Unite = " UT_n.d. - " ; }
			NameLogFile.Append(Unite) ;
			
			// Ajout de la date : YYYY-MM-DD
			int Year, Month, Day ;
			SCDateTime DateSeance ;  
			if ( ThisSymbol == Future )  		{ DateSeance = sc.BaseDateTimeIn[IndexRAZIntraday] + SCDateTime::DAYS(1) ;  }    // on rajoute 1 jour à IndexRAZIntraday pour tomber sur le jour de la séance  // en future, jour de la séance = jour du settlement
			else 											{ DateSeance = ComputerLocalTime ;  }  // le Symbol est un indice ou una action : on utilise la date du PC car c'est trop risqué d'utiliser un index (la date serait fausse si la FullRecalculation était effectuée juste avant 9h30)
			DateSeance.GetDateYMD(Year, Month, Day) ;
			NameLogFile.AppendFormat("%04d-%02d-%02d", Year, Month, Day) ;
			
			// Pour indiquer des données issues d'un replay                // Note : lorsqu'on stoppe le replay, la study est recalculée et, un fichier .csv est créé s'il n'existait pas encore. Ce fichier ne comporte pas la mention "Data issu d'un replay".
			if ( sc.ReplayStatus != REPLAY_STOPPED ) // un replay est en cours ou en pause
			{ NameLogFile.AppendFormat(" - Data issu d'un replay") ;  }
			
		
			// Fin du du nom de fichier
			NameLogFile.AppendFormat(".csv") ;
			
			// Envoi de ce nom dans le log
			SCString message ;
			message.Format("Ecriture en cours dans le fichier ''%s''", NameLogFile.GetChars() ) ;    // the GetChars() function on the SCString to be able to access the internal C++ character pointer which is necessary when using this class with the SCString Format() and AppendFormat() functions.
			sc.AddMessageToLog(message,0) ;
		}
		
		// Création des noms de colonne s'il s'agit de la première ouverture du fichier
		std::string string_NomFichierLog = NameLogFile.GetChars(); // conversion SCString en string
		if ( ! FileExists(string_NomFichierLog) )  // le fichier n'existe pas
		{ 
			// Déclaration du flux d'écriture
			std::ofstream myStream ;
			myStream.open(string_NomFichierLog, std::ofstream::out | std::ofstream::app) ;        
			if ( myStream.is_open() )
			{	 
				// remplissage du fichier .csv avec  : en-têtes pour l'acquisition des ticks
				myStream << "IntradayIndex" << ";" << "TimeStamp officiel" << ";" << "Millisec officiel" << ";" << "Lag (ms)" << ";" << "Refresh SC (ms)"<< ";" << "Prix" << ";" << "Volume" << ";" << "TimeStamp local" << ";" << "Millisec local" << ";" << "Date du PC" <<  ";" << "Paquets /s" ;

				// remplissage du fichier .csv avec  : en-têtes pour l'acquisition des Bid-Ask
				if ( EchantillonnageAskBid_Input.GetYesNo() )
				{ myStream << ";" << "Motif"<< ";"<< "Ask" << ";" << "Bid" << ";" << "Ask-Bid" << ";"  << "AskSize" << ";"  << "BidSize" ;  }

				// remplissage du fichier .csv avec : retout chariot
				myStream << "\n" ; 
			}  
			else {	SCString message ;  message.Format("Ouverture du fichier a la creation : erreur") ;  sc.AddMessageToLog(message, 1) ;  }
		}

		// Calcul du décalage horaire = Horaire_Du_PC - DernierHoraireDuMarché
		SCDateTime Delta = ComputerLocalTime - sc.BaseDateTimeIn[LastIndex] ;
		DecalageHoraire = round ( Delta.GetTimeInSeconds() / 3600.0f ) ;   // valeur convertie en heures      
	}  // fin sc.UpdateStartIndex== 0

	
	// Mesure du Refresh entre deux appels du DLL
	int Refresh ;
	{ 
		// Calcul de DureeCycle avec QPC :    // https://docs.microsoft.com/fr-fr/windows/win32/sysinfo/acquiring-high-resolution-time-stamps
		LARGE_INTEGER ElapsedTicks_PresentCycle, Frequency;
		QueryPerformanceFrequency(&Frequency); 
		QueryPerformanceCounter(&ElapsedTicks_PresentCycle);    // Ticks 
		float Refresh_ms = (ElapsedTicks_PresentCycle.QuadPart  - ElapsedTicks_PreviousCycle)*1000.0 / Frequency.QuadPart ;
		ElapsedTicks_PreviousCycle = ElapsedTicks_PresentCycle.QuadPart  ;
		Refresh = round(Refresh_ms) ;
		
		// Création d'instance de Texte_UTAMAOA et appel de sc.UseTool
		Texte.Format("Refresh : %03d ms / %d ms", Refresh, sc.ChartUpdateIntervalInMilliseconds) ;  
		
		Texte_UTAMAOA objetTexte(Texte, 8, RGB(0,127,255), sc.GraphRegion, 1000001);   // 1000001 est le Line_Number spécifique pour DrawTempsDeCalculs
		sc.UseTool(objetTexte); 
	}
	

	if ( NotAFullRecalculation  and  ! sc.ChartIsDownloadingHistoricalData(sc.ChartNumber) )   // les calculs étant faits en live, les latences du flux ne peuvent être calculées que sur les ticks obtenus en temps réel
	{
		if ( LastIndex > PreviousUpdateLastIndex )   // ça veut dire que de nouveaux ticks sont arrivés
		{

			// Déclaration du flux d'écriture
			std::ofstream myStream ;
			myStream.open(NameLogFile, std::ofstream::out | std::ofstream::app) ;
			if ( myStream.is_open() ) 	{	/* SCString message ;  message.Format("Ouverture du fichier : OK") ;  sc.AddMessageToLog(message, 0) ; */  }  else {	SCString message ;  message.Format("Ouverture du fichier : erreur") ;  sc.AddMessageToLog(message, 0) ;  }
			
			
			// Changement de seconde (LocalTime) depuis le précédent paquet reçu ? 
			bool NouvelleSeconde_LocalTime = 0 ;
			if ( ComputerLocalTime.GetTimeInSecondsWithoutMilliseconds() != ComputerLocalTime_PaquetPrecedent )
			{
				NouvelleSeconde_LocalTime = 1 ;
				ComputerLocalTime_PaquetPrecedent = ComputerLocalTime.GetTimeInSecondsWithoutMilliseconds() ;
			}
			else Nb_Paquets_Intraseconde ++ ;
			
			
			// Ecriture dans le .csv pour chaque tick reçu
			for (int index = PreviousUpdateLastIndex + 1 ; index < LastIndex + 1 ; index++)                 // début manual looping  
			{
				
				// Différence de temps entre l'heure locale du PC et le tick réceptionné  
				int DeltaTimeStamp ;
				SCDateTime HoraireTick = sc.BaseDateTimeIn[index] ;
				{
					SCDateTime Delta = ComputerLocalTime - HoraireTick - SCDateTime::HOURS(DecalageHoraire) ;  
					DeltaTimeStamp = Delta.GetTimeInSecondsWithoutMilliseconds() * 1000  +  Delta.GetMillisecond() ;  			// Secondes de Delta + millisecondes de Delta
				}
				
				// DeltaTimeStamp est très négatif 
				if ( DeltaTimeStamp < -1000 )		// typiquement lorsqu'on ouvre Sierra et qu'on reconnecte avec F11 
				{	
					sc.RecalculateChart(sc.ChartNumber);  
					SCString message ;  message.Format("DeltaTimeStamp (%dms) est tres negatif  =>  RecalculateChart", DeltaTimeStamp) ;  sc.AddMessageToLog(message, 1) ; 
				}
				
				// IntradayIndex
				int IntradayIndex = index - IndexRAZIntraday ;
				
				// remplissage du fichier .csv avec : IntradayIndex / datation officielle (horaire en SCDateTime) / DeltaTimeStamp en ms / Refresh de SC (ms) / prix / volume  / datation locale (horaire en SCDateTime) / ms de la datation locale / datation locale (jour en Date Value)
				myStream << IntradayIndex << ";" << ConversionFloat(HoraireTick.GetTimeAsSCDateTime().GetTimeInSecondsWithoutMilliseconds()/86400.0) << ";" << HoraireTick.GetMillisecond() << ";" << DeltaTimeStamp << ";" << Refresh << ";" << ConversionFloat(sc.Close[index]) << ";" << sc.Volume[index] << ";" << ConversionFloat(ComputerLocalTime.GetTimeAsSCDateTime().GetTimeInSecondsWithoutMilliseconds()/86400.0) << ";" << Millisecondes_Local << ";" << ComputerLocalTime.GetDate() << ";" ; 
				
				// remplissage du fichier .csv avec : "paquets /s"  (uniquement pour le premier tick de la nouvelle seconde)
				if ( NouvelleSeconde_LocalTime  and  (index == (PreviousUpdateLastIndex+1) ) )			// la seconde a changé
				{
					myStream << Nb_Paquets_Intraseconde  << ";" ;
					Nb_Paquets_Intraseconde = 1 ;
				}
				else // on n'écrit rien (nécessite tout de même un ";")
				{
					myStream << ";" ;
				}
				
				// pour le dernier tick du paquet, remplissage du fichier .csv avec : motif, ask, bid, spread, asksize, bidsize
				if ( EchantillonnageAskBid_Input.GetYesNo() )
				{
					if ( index == LastIndex )  { myStream  << "tick" << ";" << ConversionFloat(sc.Ask) << ";" << ConversionFloat(sc.Bid) << ";" << ConversionFloat(round((sc.Ask-sc.Bid)*1000)/1000)  << ";" << sc.AskSize << ";"  << sc.BidSize ;  }    // sans le round, les valeurs écrites se terminent par 0000001 ou 9999999
					else { myStream  << "tick" ;   }
				}
				
				// remplissage du fichier .csv avec : retout chariot
				myStream << "\n" ;
			}
		}
		else if ( EchantillonnageAskBid_Input.GetYesNo() )		// pas de nouveau tick => on acquière le askbid le cas échéant
		{
			float DeltaAsk = sc.Ask-AskPrecedent ;
			float DeltaBid = sc.Bid-BidPrecedent ;
			
			if (  fabs(DeltaAsk) > 0.1  or  fabs(DeltaBid) > 0.1  )			// le ask ou le bid a changé
			{
				// création de motif
				std::string motif ;
				{
					if ( DeltaAsk < -0.1 )
					{
						if ( DeltaBid < -0.1 )
						{
							motif = "ask et bid down" ;
						}
						else if ( DeltaBid > 0.1 )
						{
							motif = "ask down, bid up" ;
						}
						else // Bid ne varie pas
						{
							motif = "ask down" ;
						}					
					}
					else if ( DeltaAsk > 0.1 )
					{
						if ( DeltaBid < -0.1 )
						{
							motif = "ask up, bid down" ;
						}
						else if ( DeltaBid > 0.1 )
						{
							motif = "ask et bid up" ;
						}
						else // Bid ne varie pas
						{
							motif = "ask up" ;
						}
					}
					else // Ask ne varie pas
					{
						if ( DeltaBid < -0.1 )
						{
							motif = "bid down" ;
						}
						else if ( DeltaBid > 0.1 )
						{
							motif = "bid up" ;
						}
						else // Bid ne varie pas
						{
							motif = "erreur motif" ;
						}
					}
				}
				
				// déclaration du flux d'écriture
				std::ofstream myStream ;
				myStream.open(NameLogFile, std::ofstream::out | std::ofstream::app) ;
				if ( myStream.is_open() ) 	{	/* SCString message ;  message.Format("Ouverture du fichier : OK") ;  sc.AddMessageToLog(message, 0) ; */  }  else {	SCString message ;  message.Format("Ouverture du fichier : erreur") ;  sc.AddMessageToLog(message, 0) ;  }
				
				// remplissage du fichier .csv avec : IntradayIndex / datation officielle (horaire en SCDateTime) / ms officiel / DeltaTimeStamp en ms / Refresh de SC (ms) / prix / volume  / datation locale (horaire en SCDateTime) / ms de la datation locale / datation locale (jour en Date Value)
				myStream /* << IntradayIndex */ << ";" /* << ConversionFloat(HoraireTick.GetTimeAsSCDateTime()) */ << ";" /* << ms officiel */ << ";" /* << DeltaTimeStamp */ << ";" << Refresh << ";" /* << ConversionFloat(sc.Close[index]) */ << ";" /* << sc.Volume[index] */ << ";" << ConversionFloat(ComputerLocalTime.GetTimeAsSCDateTime().GetTimeInSecondsWithoutMilliseconds()/86400.0) << ";" << Millisecondes_Local << ";" << ComputerLocalTime.GetDate() << ";" /* << Nb_Paquets_Intraseconde */  << ";" ;

				// remplissage du fichier .csv avec : motif, ask, bid, spread, asksize, bidsize
				myStream  << motif << ";" << ConversionFloat(sc.Ask) << ";" << ConversionFloat(sc.Bid) << ";" << ConversionFloat(round((sc.Ask-sc.Bid)*1000)/1000)  << ";" << sc.AskSize << ";"  << sc.BidSize << ";" ; 
				
				// remplissage du fichier .csv avec : retout chariot
				myStream << "\n" ;
			}
		}
		
		// modif des variables persistantes pour l'échantillonnage du ask/bid
		AskPrecedent = sc.Ask ; 
		BidPrecedent = sc.Bid ;
	}
	
	
	// On store le LastIndex pour le prochain update
	if ( ! sc.ChartIsDownloadingHistoricalData(sc.ChartNumber) )
	{ PreviousUpdateLastIndex = LastIndex ;  }
	else 
	{ PreviousUpdateLastIndex = 999999999 ;  }
	
	// lorsque je suis déconnecté d'IB, pour les backtests :
	// if ( sc.ServerConnectionState != SCS_CONNECTED )    // en mettant cette ligne en commentaire le rafraichissement est de sc.ChartUpdateIntervalInMilliseconds   // sinon le rafraichissement ne se fait qu'à l'arrivée d'un nouveau tick, et à une période >= sc.ChartUpdateIntervalInMilliseconds 
	{	sc.UpdateAlways = 1 ;  }
	
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_DureeCycleChart(SCStudyInterfaceRef sc) 
{
	
	SCInputRef StudyONOFF_Input = sc.Input[0];                       // input modifié par la study "Duree cycle chart - Study de controle"
	SCInputRef CalculationPrecedence_Input = sc.Input[1];
	
	
	int& Cette_instance_est_au_moins_la_deuxieme = sc.GetPersistentIntFast(0); 
	int& ClicksDuPrecedentCycle = sc.GetPersistentInt(1);      // numéro appelé par d'autres instances de la study "Duree cycle chart" , conférer DureeCalculsDuChart 		// ne doit pas être une PersistentIntFast
	int& ClicksDuCycleMoins2 = sc.GetPersistentIntFast(2);  
	int& ClicksDuCycleMoins3 = sc.GetPersistentIntFast(3);  
	int& ClicksDuCycleMoins4 = sc.GetPersistentIntFast(4);  
	int& StudyIDPremiereInstance = sc.GetPersistentIntFast(5);  
	int& StudyID_StudyDeControle = sc.GetPersistentIntFast(6);   
	int& DureeCalculsDuChart_CycleMoins1 = sc.GetPersistentIntFast(11);  
	int& DureeCalculsDuChart_CycleMoins2 = sc.GetPersistentIntFast(12);  
	int& DureeCalculsDuChart_CycleMoins3 = sc.GetPersistentIntFast(13);  
	int& DureeCalculsDuChart_CycleMoins4 = sc.GetPersistentIntFast(14);  
	int& DureeCalculsDuChart_CycleMoins5 = sc.GetPersistentIntFast(15);  
	int& DureeCalculsDuChart_CycleMoins6 = sc.GetPersistentIntFast(16);  
	int& DureeCalculsDuChart_CycleMoins7 = sc.GetPersistentIntFast(17);  
	int& DureeCalculsDuChart_CycleMoins8 = sc.GetPersistentIntFast(18);  
	int& DureeCalculsDuChart_CycleMoins9 = sc.GetPersistentIntFast(19);  
	
	__int64& ElapsedTicks_PreviousCycle = sc.GetPersistentInt64(1);  
	
	const int ChartNumber_StudyDeControle = 3 ;  // on veut que la study "Duree cycle chart - Study de controle" soit sur le chart #3
	
	
	
	// Bloc sc.SetDefaults /////////////////////////////////////////////////////////////////////////////////////////////
	if (sc.SetDefaults)
	{
		sc.GraphName = "DUREE CYCLE CHART" ;                    // nom appelé par d'autres instances de la study "Duree cycle chart" , conférer DureeCalculsDuChart            // nom appelé par la study "Duree cycle chart - Study de controle"
		sc.AutoLoop = 0 ;  
		sc.CalculationPrecedence = STD_PREC_LEVEL  ; 
		sc.DisplayStudyName = 0;
		sc.DisplayStudyInputValues = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 0 ;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		sc.ResetAllScales = 0 ; 
		
		sc.HideStudy = 1;
		
		StudyONOFF_Input.Name = "Study is ON" ;
		StudyONOFF_Input.SetYesNo(1);
		CalculationPrecedence_Input.Name = "Calculation Precedence" ;
		CalculationPrecedence_Input.SetCustomInputStrings("STD_PREC_LEVEL;LOW_PREC_LEVEL ;VERY_LOW_PREC_LEVEL"); 
		CalculationPrecedence_Input.SetCustomInputIndex(0);
	
		return ;	
		
	} // fin sc.Defaults
	
	
	
	if ( StudyONOFF_Input.GetYesNo() )
	{
		
		if ( sc.UpdateStartIndex == 0 )
		{
			// modification de sc.CalculationPrecedence
			sc.CalculationPrecedence = CalculationPrecedence_Input.GetIndex() ;            // !! Warning !! Attention à toujours cliquer le bouton Apply ou le bouton OK deux fois quand on change l'input de CalculationPrecedence sinon il n'est pas pris en compte correctement.
			
			// on récupère la StudyID de la première instance de "Duree cycle chart"                
			StudyIDPremiereInstance = sc.GetStudyIDByName(sc.ChartNumber, "DUREE CYCLE CHART", 0);       //  WARNING : ce test est très long lorsque les numéros d'ordre des deux studies sont très différents : 7ms de durée d'exécution pour cette seule ligne sur le graphe #6, si les deux studies "DUREE CYCLE CHART" sont en première et dernière position
			
			// On checke si cette instance est la deuxième ou plus de la study "DUREE CYCLE CHART"
			if ( StudyIDPremiereInstance != sc.StudyGraphInstanceID )   // on vérifie que la study n'est pas en train de s'appeler elle-même
			{ Cette_instance_est_au_moins_la_deuxieme = 1 ;  }
			else 
			{ Cette_instance_est_au_moins_la_deuxieme = 0 ;  }
		
			// on récupère la StudyID de la study de contrôle sur le chart #13
			StudyID_StudyDeControle = sc.GetStudyIDByName(ChartNumber_StudyDeControle, "Duree cycle chart - Study de controle", 0);  
			if ( StudyID_StudyDeControle == 0 )  {	SCString message ;  message.Format("!! ERREUR !! La Study ''Duree cycle chart - Study de controle'' n'a pas ete trouvee sur le chart #%d (StudyID==0).", ChartNumber_StudyDeControle) ;  sc.AddMessageToLog(message, 1) ;  }
			
		}
		
		
		// DureeCycle
		{ 
			// Calcul de DureeCycle avec QPC :    // https://docs.microsoft.com/fr-fr/windows/win32/sysinfo/acquiring-high-resolution-time-stamps
			LARGE_INTEGER ElapsedTicks_PresentCycle, Frequency;
			QueryPerformanceFrequency(&Frequency); 
			QueryPerformanceCounter(&ElapsedTicks_PresentCycle);    // Ticks 
			
			// int ClicksDuPresentCycle = ElapsedTicks_PresentCycle.QuadPart * 1000.0 / Frequency.QuadPart ;   // variables exprimées en millisecondes
			int ClicksDuPresentCycle = ElapsedTicks_PresentCycle.QuadPart * 10000.0 / Frequency.QuadPart ;   // variables exprimées en dixièmes de millisecondes
			int DureeCycle = ClicksDuPresentCycle - ClicksDuPrecedentCycle ; 
			float Duree4Cycles = ClicksDuPresentCycle - ClicksDuCycleMoins4 ;

			// Construction du SCString message
			SCString message ;
			message.Format("Cycle = %04.0f ms | MM4 = %04.0f ms", DureeCycle/10.0, round(Duree4Cycles/4.0/10.0)) ;  		// variables exprimées en dixièmes de millisecondes, converties en ms
			if 			( sc.CalculationPrecedence == 2 )  { SCString StringAjoute ;  StringAjoute.Format(" | VERY_LOW_PREC_LEVEL") ;  message.Append(StringAjoute) ;  }
			else if 	( sc.CalculationPrecedence == 0 )  { SCString StringAjoute ;  StringAjoute.Format(" |            STD_PREC_LEVEL") ;  message.Append(StringAjoute) ;  }
			else if 	( sc.CalculationPrecedence == 1 )  { SCString StringAjoute ;  StringAjoute.Format(" |           LOW_PREC_LEVEL") ;  message.Append(StringAjoute) ;  }
			else 															{ SCString StringAjoute ;  StringAjoute.Format(" | bug de PREC_LEVEL") ;  message.Append(StringAjoute) ;  }
			
			if ( Cette_instance_est_au_moins_la_deuxieme )
			{
				// Récupération des Clicks de l'instance de la study appelée en premier par le chart
				int& ClicksDuDebutDuCycle = sc.GetPersistentIntFromChartStudy(sc.ChartNumber, StudyIDPremiereInstance, 1);                      // int& GetPersistentIntFromChartStudy(int ChartNumber, int StudyID, int Key);
				int DureeCalculsDuChart = ClicksDuPresentCycle - ClicksDuDebutDuCycle ;
				float DureeMoyenneCalculsDuChart = DureeCalculsDuChart_CycleMoins9 + DureeCalculsDuChart_CycleMoins8 + DureeCalculsDuChart_CycleMoins7 + DureeCalculsDuChart_CycleMoins6 + DureeCalculsDuChart_CycleMoins5 + DureeCalculsDuChart_CycleMoins4 + DureeCalculsDuChart_CycleMoins3 + DureeCalculsDuChart_CycleMoins2 + DureeCalculsDuChart_CycleMoins1 + DureeCalculsDuChart ; 
				DureeMoyenneCalculsDuChart /= 10 ;  // on fait la moyenne sur 10 calculs.
				SCString StringAjoute ;  StringAjoute.Format(" | Duree des calculs (vs instance0) = %.1f ms | MM10 = %.1f ms", DureeCalculsDuChart/10.0, DureeMoyenneCalculsDuChart/10.0) ;  message.Append(StringAjoute) ;		// variables exprimées en dixièmes de millisecondes, converties en ms
				
				// Report des DureeCalculsDuChart des cycles précédents
				DureeCalculsDuChart_CycleMoins9 = DureeCalculsDuChart_CycleMoins8 ;
				DureeCalculsDuChart_CycleMoins8 = DureeCalculsDuChart_CycleMoins7 ;
				DureeCalculsDuChart_CycleMoins7 = DureeCalculsDuChart_CycleMoins6 ;
				DureeCalculsDuChart_CycleMoins6 = DureeCalculsDuChart_CycleMoins5 ;
				DureeCalculsDuChart_CycleMoins5 = DureeCalculsDuChart_CycleMoins4 ;
				DureeCalculsDuChart_CycleMoins4 = DureeCalculsDuChart_CycleMoins3 ;
				DureeCalculsDuChart_CycleMoins3 = DureeCalculsDuChart_CycleMoins2 ;
				DureeCalculsDuChart_CycleMoins2 = DureeCalculsDuChart_CycleMoins1 ;
				DureeCalculsDuChart_CycleMoins1 = DureeCalculsDuChart ;
			}
			else // cette instance est la première sur ce chart
			{
				if ( StudyID_StudyDeControle != 0 )
				{
					int& ClickDuPrecedentChart = sc.GetPersistentIntFromChartStudy(ChartNumber_StudyDeControle, StudyID_StudyDeControle, 0);                      // int& GetPersistentIntFromChartStudy(int ChartNumber, int StudyID, int Key);
					SCString StringAjoute ;  StringAjoute.Format(" |          Latence depuis precedent chart = %.0f ms         ", (ClicksDuPresentCycle-ClickDuPrecedentChart)/10.0) ;  message.Append(StringAjoute) ;		// variables exprimées en dixièmes de millisecondes, converties en ms
				}
			}
			
			// AddMessageToLog
			sc.AddMessageToLog(message,0) ;
			
			// Report des clicks des cycles précédents
			ClicksDuCycleMoins4 = ClicksDuCycleMoins3 ;
			ClicksDuCycleMoins3 = ClicksDuCycleMoins2 ;
			ClicksDuCycleMoins2 = ClicksDuPrecedentCycle ;
			ClicksDuPrecedentCycle = ClicksDuPresentCycle ;
			
			// Report des clicks dans la variable persistante "ClickDuPrecedentChart" de la study "Duree cycle chart - Study de controle"
			if ( StudyID_StudyDeControle != 0 )
			{ sc.SetPersistentIntForChartStudy(ChartNumber_StudyDeControle, StudyID_StudyDeControle, 0, ClicksDuPresentCycle) ;  }        // void SetPersistentIntForChartStudy(int ChartNumber, int StudyID, int Key, int Value);
			
		}
	
	}
	
	// lorsque je suis déconnecté du serveur, pour les backtests :
	if ( sc.ServerConnectionState != SCS_CONNECTED )  sc.UpdateAlways = 1 ; 
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_DureeCycleChart_StudyDeControle(SCStudyInterfaceRef sc) 
{
	
	SCInputRef AllStudiesON_Input = sc.Input[0];
	SCInputRef AllStudiesOFF_Input = sc.Input[1];
	SCInputRef RecalculateAllCharts_Input = sc.Input[2];
	
	
	int& ClickDuPrecedentChart = sc.GetPersistentInt(0);   	// cette référence n'est utilisée que pour stocker des valeurs transmises par les studies "DUREE CYCLE CHART"		// ne doit pas être une PersistentIntFast
	

	
	// Bloc sc.SetDefaults /////////////////////////////////////////////////////////////////////////////////////////////
	if (sc.SetDefaults)
	{
		sc.GraphName = "Duree cycle chart - Study de controle" ;             // nom appelé par la study "Duree cycle chart - Study de controle" (cf StudyID_StudyDeControle)
		sc.AutoLoop = 0 ;  
		sc.DisplayStudyName = 0;
		sc.DisplayStudyInputValues = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 0 ;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		// sc.ResetAllScales = 0 ; 
		sc.HideStudy = 1;
		
		AllStudiesON_Input.Name = "Turn all Studies ON" ;
		AllStudiesON_Input.SetYesNo(0);
		AllStudiesOFF_Input.Name = "Turn all Studies OFF" ;
		AllStudiesOFF_Input.SetYesNo(0);
		RecalculateAllCharts_Input.Name = "Recalculate all Charts" ;
		RecalculateAllCharts_Input.SetYesNo(0) ;  
	
		return ;	
		
	} // fin sc.Defaults
	
	
	
	// Bascule de toutes les studies en ON
	if ( AllStudiesON_Input.GetYesNo() )
	{
		AllStudiesON_Input.SetYesNo(0) ;   // pour RAZ l'input
		
		// On détermine parmi les 35 premières StudyID lesquelles s'appellent "DUREE CYCLE CHART" pour les 30 premiers charts
		SCString StudyName ;
		for ( int ChartNumber = 1 ; ChartNumber < 31 ; ChartNumber ++ )
		{
			for ( int StudyID = 1 ; StudyID < 36 ; StudyID ++ )  
			{
				StudyName = sc.GetStudyNameFromChart(ChartNumber, StudyID);
				if ( StudyName.Compare("DUREE CYCLE CHART",0) == 0 )      // returns 0 if they are the same
				{
					// On met "DUREE CYCLE CHART" en ON
					sc.SetChartStudyInputInt(ChartNumber, StudyID, 0, 1) ;   // int SetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int IntegerValue);   
				}		
			}
		}
	}
	
	// Bascule de toutes les studies en OFF
	if ( AllStudiesOFF_Input.GetYesNo() )
	{
		AllStudiesOFF_Input.SetYesNo(0) ;   // pour RAZ l'input
		
		// On détermine parmi les 35 premières StudyID lesquelles s'appellent "DUREE CYCLE CHART" pour les 30 premiers charts
		SCString StudyName ;
		for ( int ChartNumber = 1 ; ChartNumber < 31 ; ChartNumber ++ )
		{
			for ( int StudyID = 1 ; StudyID < 36 ; StudyID ++ )  
			{
				StudyName = sc.GetStudyNameFromChart(ChartNumber, StudyID);
				if ( StudyName.Compare("DUREE CYCLE CHART",0) == 0 )      // returns 0 if they are the same
				{
					// On met "DUREE CYCLE CHART" en ON
					sc.SetChartStudyInputInt(ChartNumber, StudyID, 0, 0) ;   // int SetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int IntegerValue);   
				}		
			}
		}
	}
	
	// Recalcule tous les charts ; nécessaire pour initialiser quotidiennement les studies "DUREE CYCLE CHART" afin de récupérer les variables persistantes suivantes :  Cette_instance_est_au_moins_la_deuxieme, StudyIDPremiereInstance et StudyID_StudyDeControle
	if ( RecalculateAllCharts_Input.GetYesNo() )
	{	
		RecalculateAllCharts_Input.SetYesNo(0) ;   // pour RAZ l'input
		
		for ( int ChartNumber = 1 ; ChartNumber < 31 ; ChartNumber ++ )
		{
			sc.RecalculateChart(ChartNumber);          // int RecalculateChart(int ChartNumber);
		}
	}
	
}
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_DebugCompteurMessageLog(SCStudyInterfaceRef sc)   // cette petite study ajoute un message dans le log à chaque appel du DLL. Le message est un compteur du nombre d'appels.
{
	
	int& CompteurLog = sc.GetPersistentIntFast(0);   
	
	// Bloc sc.SetDefaults /////////////////////////////////////////////////////////////////////////////////////////////
	if (sc.SetDefaults)
	{
		sc.GraphName = "Debug compteur message log" ;             
		sc.AutoLoop = 0 ;  
		sc.CalculationPrecedence = STD_PREC_LEVEL  ; 
		sc.DisplayStudyName = 0;
		sc.DisplayStudyInputValues = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 0 ;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		sc.ResetAllScales = 0 ; 
		
		sc.HideStudy = 1;
		
		return ;	
		
	} // fin sc.Defaults
	

	SCString message ;
	message.Format("Compteur Log = %d", CompteurLog ) ;  
	
	// AddMessageToLog
	sc.AddMessageToLog(message,0) ;

	CompteurLog ++ ;
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_LatencyDataFeedDuringTrading(SCStudyInterfaceRef sc) 
{
	// Références /////////////////////////////////////////////////////////////////////////////////////////////
	SCSubgraphRef Lag = sc.Subgraph[0];									// en millisecondes			// appelé par la study "Robot micro-flash-krachs"
	SCSubgraphRef Refresh = sc.Subgraph[1];							// en millisecondes			// appelé par la study "Robot micro-flash-krachs"
	SCSubgraphRef MM_Lag = sc.Subgraph[2];							// en millisecondes
	SCSubgraphRef MM_Refresh = sc.Subgraph[3];					// en millisecondes
	SCSubgraphRef MM_Lag_TickRapide = sc.Subgraph[4];		// en millisecondes
	SCSubgraphRef MM_Lag_TickLent = sc.Subgraph[5];			// en millisecondes
	
	
	// Inputs /////////////////////////////////////////////////////////////////////////////////////////////
	SCInputRef Affichage_Texte_DeltaTimeStamp = sc.Input[0];  
	SCInputRef PeriodeMM = sc.Input[1];  
	
	
	// Références /////////////////////////////////////////////////////////////////////////////////////////////
	int& DecalageHoraire = sc.GetPersistentIntFast(0);  	
	int& Clicks_PreviousCycle = sc.GetPersistentIntFast(1);  
	int& PreviousUpdateIndex = sc.GetPersistentIntFast(2);  
	int& PreviousUpdateIndex_Volume = sc.GetPersistentIntFast(3);  
	int& FirstIndex_Acquisition = sc.GetPersistentIntFast(4);  
	int& NbAppelsDLL = sc.GetPersistentIntFast(5);  
	int& DeltaTimeStamp_1 = sc.GetPersistentIntFast(11);  	
	int& DeltaTimeStamp_2 = sc.GetPersistentIntFast(12);  	
	int& DeltaTimeStamp_3 = sc.GetPersistentIntFast(13);  	
	int& DeltaTimeStamp_4 = sc.GetPersistentIntFast(14);  	
	int& DeltaTimeStamp_5 = sc.GetPersistentIntFast(15);  	
	int& DeltaTimeStamp_6 = sc.GetPersistentIntFast(16);  	
	int& DeltaTimeStamp_7 = sc.GetPersistentIntFast(17);  	
	int& DeltaTimeStamp_8 = sc.GetPersistentIntFast(18);  	
	int& DeltaTimeStamp_9 = sc.GetPersistentIntFast(19);  	
	__int64& ElapsedTicks_PreviousCycle = sc.GetPersistentInt64(1);  
	SCDateTime& ComputerLocalTime_PaquetPrecedent = sc.GetPersistentSCDateTimeFast(1);  


	// Variables /////////////////////////////////////////////////////////////////////////////////////////////
	int const LastIndex = sc.ArraySize-1 ;
	s_sc* const ptrsc = &sc;      // c'est un pointeur constant sur un s_sc   // attention : " const s_sc* " est un pointeur sur un s_sc constant
	
	
	// Bloc sc.SetDefaults /////////////////////////////////////////////////////////////////////////////////////////////
	if (sc.SetDefaults)
	{
		sc.GraphName = "Latence flux ticks (durant trading)" ;   // nom appelé par la study "Robot micro-flash-krachs"
		sc.GraphRegion = 3 ;  
		sc.ValueFormat = 0 ;
		sc.AutoLoop = 0 ;  
		sc.DisplayStudyName = 1;
		sc.DisplayStudyInputValues = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 0 ;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		sc.ResetAllScales = 0 ; 
		
		sc.MaintainAdditionalChartDataArrays = 1;  // sc.MaintainAdditionalChartDataArrays also causes the sc.BaseDataEndDateTime[] array to be filled in and maintained.
		
		Affichage_Texte_DeltaTimeStamp.Name = "Affichage Texte DeltaTimeStamp" ;
		Affichage_Texte_DeltaTimeStamp.SetYesNo(false) ;
		PeriodeMM.Name = "PeriodeMM (nb paquets)" ;
		PeriodeMM.SetInt(100) ;
		
		for (int SubgraphIndex = 0; SubgraphIndex <= 5; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].LineWidth = 1 ;
			sc.Subgraph[SubgraphIndex].DrawStyle = DRAWSTYLE_LINE ;
			sc.Subgraph[SubgraphIndex].LineStyle = LINESTYLE_SOLID ;
			sc.Subgraph[SubgraphIndex].LineLabel = /* LL_DISPLAY_NAME | */ LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
			sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre
			sc.Subgraph[SubgraphIndex].DrawZeros = false ;
		}

		Lag.Name = "Lag (ms)"; 
		Lag.PrimaryColor = RGB(0,0,0);		
		Refresh.Name = "Refresh (ms)"; 
		Refresh.PrimaryColor = RGB(0,0,250);		
		Refresh.DrawStyle = DRAWSTYLE_X ;
		Refresh.LineWidth = 2;
		Refresh.LineLabel = 0 ;		// on n'afiche rien
		MM_Lag.Name = "MM Lag (ms)"; 
		MM_Lag.PrimaryColor = COLOR_DARKORANGE ;		
		MM_Refresh.Name = "MM Refresh (ms)"; 
		MM_Refresh.PrimaryColor = RGB(0,0,250);		
		MM_Lag_TickRapide.Name = "MM Refresh Tick Rapide (ms)"; 
		MM_Lag_TickRapide.PrimaryColor = RGB(0,200,0);		
		MM_Lag_TickLent.Name = "MM Refresh Tick Lent (ms)"; 
		MM_Lag_TickLent.PrimaryColor = RGB(250,0,0);		

		return ;	
	} // fin sc.Defaults
	
	
	
	// Get the millisecond part   ///////////////     https://stackoverflow.com/questions/21470315/odd-behaviour-using-chronohigh-resolution-clocknow    !! il faut utiliser system_clock (temps exact), et ne pas utiliser steady_clock (qui est high_resolution_clock a priori) ou high_resolution_clock (temps relatif au démarrage de SC)
	int Millisecondes_Local ;
	{
		using namespace std;
		using namespace std::chrono;
		system_clock::time_point now = system_clock::now();    	 //  https://stackoverflow.com/questions/27136854/c11-actual-system-time-with-milliseconds
		system_clock::duration tp = now.time_since_epoch();
		tp -= duration_cast<seconds>(tp); 										  // on soustrait le time_point arrondi à la seconde au time_point non arrondi => il ne reste que les ms  // tp est exprimé en nanosecondes
		Millisecondes_Local =   tp.count() / 1000000 ;
	}	

	
	// Conversion de l'heure du PC en SCDateTime /////////////////////////////
	SCDateTime ComputerLocalTime ;
	{
		// Temps du PC (local time)
		time_t now = time(0) ;
		tm *ltm = localtime(&now) ;    // cf https://www.tutorialspoint.com/cplusplus/cpp_date_time.htm
		
		// Extraction des membres de ltm
		int Annee = 1900 + ltm->tm_year ;
		int Mois = 1 + ltm->tm_mon ;
		int JourDuMois = ltm->tm_mday ;
		int Heure = ltm->tm_hour ;
		int Minute = ltm->tm_min ;
		int Seconde = ltm->tm_sec ;

		// Conversion en objet SCDateTime
		ComputerLocalTime.SetDateTimeYMDHMS_MS(Annee, Mois, JourDuMois, Heure, Minute, Seconde, Millisecondes_Local) ;   
	}
	
	

	
	if ( sc.UpdateStartIndex == 0 )
	{
		// Initialisations
		if (sc.ChartNumber == 6 ) sc.GraphRegion = 2 ;  		
		else if (sc.ChartNumber == 13 ) sc.GraphRegion = 3 ;  		
		FirstIndex_Acquisition = LastIndex + 1 ;
		NbAppelsDLL = 1 ;

		// Calcul du décalage horaire = Horaire_Du_PC - DernierHoraireDuMarché
		SCDateTime Delta = ComputerLocalTime - sc.BaseDateTimeIn[LastIndex] ;
		DecalageHoraire = round ( Delta.GetTimeInSeconds() / 3600.0f ) ;   // valeur convertie en heures      
		
		// Tracé du graphe Lag si on est en 1-tick
		n_ACSIL::s_BarPeriod BarPeriod ;
		sc.GetBarPeriodParameters(BarPeriod) ;
		if ( BarPeriod.IntradayChartBarPeriodType == IBPT_NUM_TRADES_PER_BAR  and  BarPeriod.IntradayChartBarPeriodParameter1 == 1 )
		{ Lag.DrawStyle =  DRAWSTYLE_LINE ;  }
		else  { Lag.DrawStyle = DRAWSTYLE_IGNORE ;  }
		
		// Tracé de la MM
		if ( PeriodeMM.GetInt() == 0 )  MM_Lag.DrawStyle = DRAWSTYLE_IGNORE ;  else MM_Lag.DrawStyle = DRAWSTYLE_LINE ; 
		
		// Tracé de lignes horizontales
		s_sc* const ptrsc = &sc;  
		COLORREF Gris = RGB(200,200,200) ;
		DrawHLine (-20, 1, Gris, ptrsc) ;
		DrawHLine (-10, 1, Gris, ptrsc) ;
		DrawHLine (0, 3, Gris, ptrsc) ; 			// void DrawHLine (float monNiveau, int monEpaisseur, COLORREF maCouleur, s_sc* ptrsc)
		DrawHLine (10, 1, Gris, ptrsc) ;
		DrawHLine (20, 1, Gris, ptrsc) ;
		DrawHLine (30, 1, Gris, ptrsc) ;
		DrawHLine (40, 1, Gris, ptrsc) ;
		DrawHLine (50, 2, Gris, ptrsc) ;
		DrawHLine (60, 1, Gris, ptrsc) ;
		DrawHLine (80, 1, Gris, ptrsc) ;
		DrawHLine (100, 2, Gris, ptrsc) ;
		DrawHLine (150, 1, Gris, ptrsc) ;
		DrawHLine (200, 2, Gris, ptrsc) ;
		DrawHLine (300, 1, Gris, ptrsc) ;
		DrawHLine (400, 1, Gris, ptrsc) ;
		DrawHLine (500, 2, Gris, ptrsc) ;
		DrawHLine (1000, 2, Gris, ptrsc) ;
	}  // fin sc.UpdateStartIndex== 0

	
	// Mesure du Refresh entre deux appels du DLL
	LARGE_INTEGER ElapsedTicks_PresentCycle, Frequency;
	QueryPerformanceFrequency(&Frequency); 
	QueryPerformanceCounter(&ElapsedTicks_PresentCycle);    // Ticks 
	if ( sc.UpdateStartIndex != 0  and  LastIndex != PreviousUpdateIndex )
	{ 
		// Calcul de DureeCycle avec QPC :    // https://docs.microsoft.com/fr-fr/windows/win32/sysinfo/acquiring-high-resolution-time-stamps
		float Refresh_ms = (ElapsedTicks_PresentCycle.QuadPart  - ElapsedTicks_PreviousCycle)*1000.0 / Frequency.QuadPart ;
		Refresh[LastIndex] = round(Refresh_ms) ;
		if ( Refresh[LastIndex] == 0 )  Refresh[LastIndex] = 0.001 ;		// DrawZeros = false
	}
	ElapsedTicks_PreviousCycle = ElapsedTicks_PresentCycle.QuadPart  ;
	
	
	// manual looping sur les ticks qui viennent d'arriver
	if ( NbAppelsDLL > 4 )		//	on exclue les premier appel du DLL après un Recalculate et un Download
	{
		for ( int index = PreviousUpdateIndex+1 ;  index <= LastIndex ;  index++ )            
		{
			// Différence de temps entre l'heure locale du PC et le tick réceptionné  
			int DeltaTimeStamp ;		// en millisecondes
			SCDateTime HoraireTick = sc.BaseDataEndDateTime[index] ;
			{
				SCDateTime Delta = ComputerLocalTime - HoraireTick - SCDateTime::HOURS(DecalageHoraire) ;  
				DeltaTimeStamp = Delta.GetTimeInSecondsWithoutMilliseconds() * 1000  +  Delta.GetMillisecond() ;  			// Secondes de Delta + millisecondes de Delta
				Lag[index] = DeltaTimeStamp ;
				if ( DeltaTimeStamp == 0 )  Lag[index] = 0.001 ;		// DrawZeros = false
			}
			
			// Calcul des MM
			if ( index == LastIndex  and  PeriodeMM.GetInt() > 1 )
			{
				int CompteurNbLag = 0 ; 		// on compte les ticks
				int CompteurNbRefresh = 0 ; 	// on compte les refresh de valeur non nulle
				int SommeLag = 0 ;
				int SommeRefresh = 0 ;
				int SommeRefresh_TickRapide = 0 ;
				int SommeRefresh_TickLent = 0 ;
				for ( int index2 = index ;  index2 > FirstIndex_Acquisition ; -- index2 )			// on parcoure les ticks en partant de index, et on s'arrête à FirstIndex_Acquisition, ou on breake lorsque CompteurNbRefresh atteind PeriodeMM
				{
					// on somme les refreshs
					if ( Refresh[index2] != 0 )
					{
						CompteurNbRefresh ++ ;
						SommeRefresh += Refresh[index2] ;
						SommeRefresh_TickRapide += Lag[index2] ;
						if ( index2 != LastIndex )  SommeRefresh_TickLent += Lag[index2+1] ;
					}
					// quand on a sommé un nb de refreshs égal à PeriodeMM
					if ( CompteurNbRefresh == PeriodeMM.GetInt() )
					{
						MM_Lag[index] = (float) SommeLag / CompteurNbLag ;
						MM_Refresh[index] = SommeRefresh / CompteurNbRefresh ;
						MM_Lag_TickRapide[index] = SommeRefresh_TickRapide / CompteurNbRefresh ;
						MM_Lag_TickLent[index] = SommeRefresh_TickLent / ( CompteurNbRefresh-1) ;
						break ;		// on sort du calcul des MM
					}
					// on somme les lags (après le break)
					{
						CompteurNbLag ++ ;
						SommeLag += Lag[index2] ;
					}
				}
			}			
		}
	}
	
	
	
	if ( Affichage_Texte_DeltaTimeStamp.GetYesNo() )
	{
		// On teste si un nouveau tick est arrivé
		int LastIndex_Volume = sc.Volume[LastIndex] ;		// (pertinent pour un graphe en X-ticks avec X > 1)
		if ( LastIndex != PreviousUpdateIndex  or  LastIndex_Volume != PreviousUpdateIndex_Volume )
		{
			// Différence de temps entre l'heure locale du PC et le tick réceptionné  
			int DeltaTimeStamp ;		// en millisecondes
			SCDateTime HoraireLastTick = sc.BaseDataEndDateTime[LastIndex] ;
			{
				SCDateTime Delta = ComputerLocalTime - HoraireLastTick - SCDateTime::HOURS(DecalageHoraire) ;  
				DeltaTimeStamp = Delta.GetTimeInSecondsWithoutMilliseconds() * 1000  +  Delta.GetMillisecond() ;  			// Secondes de Delta + millisecondes de Delta
			}
			
			// Calculs de DeltaTimeStamp_min10 et DeltaTimeStamp_max10
			using namespace std;
			int DeltaTimeStamp_min10 = min(min(min(min(min(min(min(min(min(DeltaTimeStamp,DeltaTimeStamp_1), DeltaTimeStamp_2), DeltaTimeStamp_3), DeltaTimeStamp_4), DeltaTimeStamp_5), DeltaTimeStamp_6), DeltaTimeStamp_7), DeltaTimeStamp_8), DeltaTimeStamp_9) ;
			int DeltaTimeStamp_moy10 = round ( ( DeltaTimeStamp + DeltaTimeStamp_1 + DeltaTimeStamp_2 + DeltaTimeStamp_3 + DeltaTimeStamp_4 + DeltaTimeStamp_5 + DeltaTimeStamp_6 + DeltaTimeStamp_7 + DeltaTimeStamp_8 + DeltaTimeStamp_9 ) /100 ) *10 ;
			int DeltaTimeStamp_max10 = max(max(max(max(max(max(max(max(max(DeltaTimeStamp,DeltaTimeStamp_1), DeltaTimeStamp_2), DeltaTimeStamp_3), DeltaTimeStamp_4), DeltaTimeStamp_5), DeltaTimeStamp_6), DeltaTimeStamp_7), DeltaTimeStamp_8), DeltaTimeStamp_9) ;
			
			// Création d'instance de Texte_UTAMAOA et appel de sc.UseTool
			SCString Texte ; 
			Texte.Format("Lag (HoraireLocal - LastTick) : %03d ms | min10 = %03d ms | moy10 = %03d ms | max10 = %03d ms", DeltaTimeStamp, DeltaTimeStamp_min10, DeltaTimeStamp_moy10, DeltaTimeStamp_max10) ;  
			Texte_UTAMAOA objetTexte(Texte, 8, RGB(0,127,255), sc.GraphRegion, 1000001);   // 1000001 est le Line_Number spécifique
			sc.UseTool(objetTexte);
			
			// DeltaTimeStamp est très négatif 
			if ( DeltaTimeStamp < -1000 )		// typiquement lorsqu'on ouvre Sierra et qu'on reconnecte avec F11 
			{	
				sc.RecalculateChart(sc.ChartNumber);  
				SCString message ;  message.Format("DeltaTimeStamp (%dms) est tres negatif  =>  RecalculateChart", DeltaTimeStamp) ;  sc.AddMessageToLog(message, 1) ; 
			}
			
			// On store le LastIndex_Volume pour le prochain update
			PreviousUpdateIndex_Volume = LastIndex_Volume ;
			
			// On store les valeurs précédentes de DeltaTimeStamp
			DeltaTimeStamp_9 = DeltaTimeStamp_8 ;
			DeltaTimeStamp_8 = DeltaTimeStamp_7 ;
			DeltaTimeStamp_7 = DeltaTimeStamp_6 ;
			DeltaTimeStamp_6 = DeltaTimeStamp_5 ;
			DeltaTimeStamp_5 = DeltaTimeStamp_4 ;
			DeltaTimeStamp_4 = DeltaTimeStamp_3 ;
			DeltaTimeStamp_3 = DeltaTimeStamp_2 ;
			DeltaTimeStamp_2 = DeltaTimeStamp_1 ;
			DeltaTimeStamp_1 = DeltaTimeStamp ;
		}
	}
	
	
	// On store le LastIndex pour le prochain update
	PreviousUpdateIndex = LastIndex ;
	
	// NbAppelsDLL incrémenté
	if ( sc.ChartIsDownloadingHistoricalData(sc.ChartNumber) )  NbAppelsDLL = 0 ;
	else ++ NbAppelsDLL ;
	
	
	// lorsque je suis déconnecté d'IB, pour les backtests :
	// if ( sc.ServerConnectionState != SCS_CONNECTED )    // en mettant cette ligne en commentaire le rafraichissement est de sc.ChartUpdateIntervalInMilliseconds   // sinon le rafraichissement ne se fait qu'à l'arrivée d'un nouveau tick, et à une période >= sc.ChartUpdateIntervalInMilliseconds 
	{	sc.UpdateAlways = 1 ;  }
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if 0		// on utilise ces instructions pour accéder au temps écoulé :
	LARGE_INTEGER Frequency ;  QueryPerformanceFrequency(&Frequency) ; 
	LARGE_INTEGER ElapsedTicks_t0 ;  QueryPerformanceCounter(&ElapsedTicks_t0) ;  
	LARGE_INTEGER ElapsedTicks ;

	QueryPerformanceCounter(&ElapsedTicks) ;  
	float DureeCalculDLL = ( ElapsedTicks.QuadPart - ElapsedTicks_t0.QuadPart ) *10000 / Frequency.QuadPart ;     // calculé en dixièmes de millisecondes
	{ SCString message ;  message.Format("Duree Calcul DLL = %.1fms", DureeCalculDLL/10) ;  sc.AddMessageToLog(message, 0) ;  }				

#endif



