/* 
v1.0 (2020/2/7) : on part de la study Overlay_INQ100_v2.2 ("Overlay indice NQ100 UT 1s"), qu'on expurge pour ne garder que Spread_UT_1s (issu de future UT 1s moins indice UT 1s), SpreadMoyen (EMA 3000 ticks) et OverlayINQ100.
v1.1 (2020/2/7) : introduction du Subgraph "Ecart" qui est l'écart entre le prix typique du future et OverlayINQ100.  EcartBis est l'écart entre Spread_UT_1s et SpreadMoyen.
v1.2 (2020/2/7) : calcul du OverlayINQ100_Interpole, issu d'une interpolation a posteriori de OverlayINQ100. Note : cette study est  configurée pour tracer OverlayINQ100_Interpole sur le prix du future.
v1.3 (2020/2/7) : introduction du Subgraph "Ecart_PostInterpolation" qui est l'écart entre le prix typique du future et OverlayINQ100.
v1.4 (2020/2/7) : introduction des Subgraph "Ecart_P" et "Ecart_N" qui sont la partie positive et la parte négative de "Ecart_PostInterpolation".
v1.5 (2020/2/7) : introduction des Subgraph "Ecart_P_EMA", "Ecart_N_EMA" et "PeriodeEMA_Ecart" (PeriodeEMA_Spread est la période classique).
v1.6 (2020/2/8) : introduction des Subgraph "Ecart_P_Affichage", "Ecart_N_Affichage", "Ecart_P_EMA_Affichage", "Ecart_N_EMA_Affichage"  : valeur de base multipliée par une cste pour disposer d'une bonne échelle d'affichage.
v1.7 (2020/2/8) : calcul de l'EMA de Ecart_PostInterpolation et son affichage.
v1.8 (2020/2/8) : calcul du RSI' (borné entre -100 et +100).
v1.9 (2020/2/8) : ajout d'un input pour tracer les lignes du RSI' suracheté et survendu.
v2.0 (2020/2/9) : simplification du code.
v2.1 (2020/2/9) : ajout de Ecart_Absolu (valeur absolue de Ecart_PostInterpolation), Ecart_Absolu_EMA et Ecart_Absolu_EMA_Affichage.
!! les calculs de SpreadMoyen antérieurs à v3.0 sont faux : SpreadMoyen n'était calculé que dans la première barre de la seconde.
v3.0 (2020/2/10) : SpreadMoyen est dorénavant calculé pour chaque barre. Gestion auto de X de X-ticks.
v3.1 (2020/2/10) : les valeurs affichées sont cappées entre -100/+100. Introduction de l'input Seuil_Ecart_Absolu_Input (vaut 1.0). Introduction de RSI_Composite = Ecart_Absolu_EMA * RSI.
v3.2 (2020/2/10) : nouvelle formule pour RSI_Composite = (Ecart_Absolu_EMA/Seuil_Ecart_Absolu_Input)² * RSI. le Seuil_Ecart_Absolu_Input est de 0.9.
v3.3 (2020/2/11) : débuggage du code sur le démarrage à 9h30m15s.
v4.0 (2020/2/11) : ajout de marqueurs sur le graphique du prix pour signaler les achats/ventes.
v4.1 (2020/2/11) : comptage des positions (longues + courtes / gagnantes + perdantes ) et des gains. Pour en déduire : GainMoyen (Long/Court/Total), GainTotal (Long/Court/Total), Gain/Perte.
v4.2 (2020/2/12) : ajout de GainMax et PerteMax.
v4.3 (2020/2/13) : ajout d'un subgraphe GainTotal (qui sera appelé par les studies "Affichage Gain Cumule (sur le meme chart)" et "Affichage Gain Cumule (sur un chart externe)". 
v4.4 (2020/2/14) : pas d'ouverture de position avant 9h31, ni après 15h45. Création study scsf_PHPBDynamiquesForFuture avec 9 déciles pour le range. Ajout de DrawTool_Input.
v4.5 (2020/2/14) : Ecart_Absolu_EMA est cappé à une valeur max (Ecart_Absolu_EMA_ValeurMAX). Ajout de PositionDansRange à scsf_PHPBDynamiquesForFuture.
v4.6 (2020/2/16) : RSI_Composite est supprimé. Création de RSI_Composite_Entree et RSI_Composite_Sortie.  RSI_Composite_Entree = RSI * f(Ecart_Absolu_EMA) * g(PositionDansRange) .  RSI_Composite_Sortie = RSI * g(PositionDansRange) .
v4.7 (2020/2/17) : calcul de l'écart type et de l'IC.
v4.8 (2020/2/18) : créer 4 inputs pour valeurs max de de Bonus_PositionDansRange : pour entrer/sortir, cadran favorable/défavorable. Favorable signifie PHJ pour PL, ou PBJ pour PC.
v4.9 (2020/2/18) : ajout d'un input pour Diviseur_RSI_Extremum. Ajout d'un input pour Glissement (0.5pt).
v5.0 (2020/2/18) : après un calcul complet la StudyPrincipale écrit les résultats dans un fichier .csv.
v5.1 (2020/2/19) : la study principale est recalculée à chaque appel avec un jeu d'inputs différents provenant de la study scsf_VariationParametresTestes.
v5.2 (2020/2/19) : correction de bugs. La study est fonctionnelle.
v5.3 (2020/2/20) : modifs mineures.




Notes : 	- quand je parle de "Spread", je parle du spread entre le future et l'indice (spread qui vaut plusieurs dizaines de points à 3 mois de la maturité).
			- quand je parle d' "Ecart", je parle de l'écart entre le future et l'OverlayINQ100 (écart qui vaut 0 en moyenne, mais qui peut monter à 2 ou 3 pts lors d'un rallye de quelques minutes).
			- Excel est limité à 1048000 lignes.

Améliorations à apporter :
	- 
	


	
	
 */

 
 
#include "sierrachart.h"  // tous les autres headers doivent être placés avant sierrachart.h
SCDLLName("Suivi tendance spread future-indice")


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct LigneHorizontale : s_UseTool  // LigneHorizontale crée un objet qui contient mes paramètres choisis pour une ligne horizontale  (hérite de toutes les données public et protected de s_UseTool)
{
	LigneHorizontale(float niveau, int epaisseur, COLORREF couleur, int graphRegion) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
	{
	Clear(); 
	DrawingType = DRAWING_HORIZONTALLINE;
	LineStyle = LINESTYLE_SOLID;
	AddMethod = UTAM_ADD_ALWAYS;	
	BeginValue = niveau;
	LineWidth = epaisseur;
	Color = couleur; 
	Region = graphRegion ;   // 0 par défaut
	} 
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawHLine (float niveau, int epaisseur, COLORREF couleur, s_sc* ptrsc, int graphRegion)
{
	//Création d'instance de LigneHorizontale et appel de sc.UseTool
	LigneHorizontale objetLigneHorizontale(niveau, epaisseur, couleur, graphRegion);
	ptrsc->UseTool(objetLigneHorizontale); // c'est sc.UseTool qui est appelé
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Marker : s_UseTool    // crée un Marker pour être affiché en coordonnées (indice,prix)
{											
	Marker (int marqueur, int index, float prix, int tailleObjet, COLORREF couleur,  int graphRegion) : s_UseTool()     
	{
		Clear(); 
		AddMethod = UTAM_ADD_ALWAYS;			
		DrawingType = DRAWING_MARKER;
		MarkerType = marqueur ;
		BeginIndex = index ;
		BeginValue = prix ;
		MarkerSize = tailleObjet ;
		Color = couleur ;
		Region = graphRegion ; 
	} 
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawMarker (int marqueur, int index, float prix, int tailleObjet, COLORREF couleur, int graphRegion, s_sc* ptrsc)  
{
	//Création d'instance de Marker et appel de sc.UseTool
	Marker objetMarker(marqueur, index, prix, tailleObjet, couleur, graphRegion);
	ptrsc->UseTool(objetMarker); 
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Texte : s_UseTool    // crée un texte avec le SCString "texte" pour être affiché en coordonnées (indice,prix)
{											
	Texte(SCString texte, int index, float prix, int tailleTexte,  COLORREF couleur, int graphRegion) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
	{
		Clear(); 
		AddMethod = UTAM_ADD_ALWAYS;			
		DrawingType = DRAWING_TEXT;
		Text = texte ;		
		BeginIndex = index ;
		BeginValue = prix ;
		FontSize = tailleTexte ;
		Color = couleur ;
		Region = graphRegion ; 
		FontBold = 1 ;
		TextAlignment = DT_VCENTER | DT_CENTER;  // options disponibles : DT_TOP, DT_VCENTER, DT_BOTTOM, DT_CENTER, DT_LEFT, DT_RIGHT
	} 
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawText (SCString texte, int index, float prix, int tailleTexte, COLORREF couleur, int graphRegion, s_sc* ptrsc)  	    // DrawText n'est utilisé que pour les affichages qui ne se mettent pas à jour à chaque appel de la fonction
{
	//Création d'instance de Texte et appel de sc.UseTool
	Texte objetTexte(texte, index, prix, tailleTexte, couleur, graphRegion);
	ptrsc->UseTool(objetTexte); 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_StudyPrincipale(SCStudyInterfaceRef sc) 
{
	
	SCSubgraphRef OverlayINQ100 = sc.Subgraph[0]; 
	SCFloatArrayRef INQ100SansCorrection = sc.Subgraph[0].Arrays[0] ;  			
	SCSubgraphRef Spread_UT_1s = sc.Subgraph[1]; 											// c'est le spread instantanné (issu de future UT 1s moins indice UT 1s)
	SCSubgraphRef SpreadMoyen = sc.Subgraph[2]; 											// c'est la moyenne sur N ticks
	SCSubgraphRef Ecart = sc.Subgraph[3]; 														// c'est l'écart entre le prix typique de la barre et OverlayINQ100
	SCSubgraphRef EcartBis = sc.Subgraph[4]; 													// EcartBis est l'écart entre Spread_UT_1s et SpreadMoyen
	SCSubgraphRef OverlayINQ100_Interpole = sc.Subgraph[5]; 
	SCSubgraphRef Ecart_PostInterpolation_EMA_Affichage = sc.Subgraph[11]; 	// on utilise l'index 11 pour que ce graphe soit tracé par dessus les graphes avec DRAWSTYLE_FILL_TO_ZERO
	SCFloatArrayRef Ecart_PostInterpolation = sc.Subgraph[11].Arrays[0]; 			// c'est l'écart entre le prix typique de la barre et OverlayINQ100_Interpole
	SCFloatArrayRef Ecart_PostInterpolation_EMA = sc.Subgraph[11].Arrays[1]; 					
	SCSubgraphRef Ecart_P_Affichage = sc.Subgraph[7]; 									
	SCFloatArrayRef Ecart_P = sc.Subgraph[7].Arrays[0] ; 									// partie positive de Ecart_PostInterpolation
	SCSubgraphRef Ecart_N_Affichage = sc.Subgraph[8]; 									
	SCFloatArrayRef Ecart_N = sc.Subgraph[8].Arrays[0] ; 									// partie négative de Ecart_PostInterpolation
	SCSubgraphRef Ecart_P_EMA_Affichage = sc.Subgraph[9]; 									
	SCFloatArrayRef Ecart_P_EMA = sc.Subgraph[9].Arrays[0] ; 							// EMA de Ecart_P
	SCSubgraphRef Ecart_N_EMA_Affichage = sc.Subgraph[10]; 									
	SCFloatArrayRef Ecart_N_EMA = sc.Subgraph[10].Arrays[0] ; 						// EMA de Ecart_N
	SCSubgraphRef RSI = sc.Subgraph[12]; 									
	SCSubgraphRef Ecart_Absolu_EMA_Affichage = sc.Subgraph[13]; 									
	SCFloatArrayRef Ecart_Absolu_EMA = sc.Subgraph[13].Arrays[0] ; 						
	SCFloatArrayRef Ecart_Absolu = sc.Subgraph[13].Arrays[1] ; 							// valeur absolue de Ecart_PostInterpolation
	SCSubgraphRef Ecart_Absolu_EMA_AffichageBis = sc.Subgraph[14]; 									
	SCSubgraphRef RSI_Composite_Entree = sc.Subgraph[15]; 							//    = RSI * f(Ecart_Absolu_EMA) * g(PositionDansRange)
	SCSubgraphRef GainCumule = sc.Subgraph[16]; 											// appelé par les studies "Affichage Gain Cumule (sur le meme chart)" et "Affichage Gain Cumule (sur un chart externe)"
	SCSubgraphRef RSI_Composite_Sortie = sc.Subgraph[17]; 							//    = RSI * g(PositionDansRange)
	SCSubgraphRef RSI_Extremum_Affichage = sc.Subgraph[18]; 						// RSI_Extremum divisé par 2
	SCFloatArrayRef RSI_Extremum = sc.Subgraph[18].Arrays[0];

	
	SCInputRef ExternalChartNumber = sc.Input[0];																							
	SCInputRef PeriodeEMA_Spread_Input = sc.Input[1];																				// modifié par scsf_VariationParametresTestes
	SCInputRef PeriodeEMA_Ecart_Input = sc.Input[2];																					// modifié par scsf_VariationParametresTestes
	SCInputRef ChartInputsSpread = sc.Input[3]; 
	SCInputRef RSI_Surachat_Input = sc.Input[4]; 																						// modifié par scsf_VariationParametresTestes
	SCInputRef Seuil_Ecart_Absolu_Input = sc.Input[5]; 																				// modifié par scsf_VariationParametresTestes
	SCInputRef Exposant_Input = sc.Input[6]; 																								// modifié par scsf_VariationParametresTestes
	SCInputRef Diviseur_RSI_Extremum_Input = sc.Input[8]; 																			// modifié par scsf_VariationParametresTestes
	SCInputRef ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable_Input = sc.Input[9]; 				// modifié par scsf_VariationParametresTestes
	SCInputRef ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable_Input = sc.Input[10]; 			// modifié par scsf_VariationParametresTestes
	SCInputRef ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable_Input = sc.Input[11]; 				// modifié par scsf_VariationParametresTestes
	SCInputRef ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable_Input = sc.Input[12]; 			// modifié par scsf_VariationParametresTestes
	SCInputRef Glissement_Input = sc.Input[13]; 
	SCInputRef TriggerRecalculate_Input = sc.Input[14]; 
	SCInputRef NameLogFile_Input = sc.Input[15]; 
	SCInputRef DrawTool_Input = sc.Input[16]; 
	

	int& HoraireDebutTrace_OverlayINQ100 = sc.GetPersistentInt(1) ;  	// time value en secondes
	int& HoraireDebutTrading = sc.GetPersistentInt(2) ;  						// time value en secondes
	int& HoraireFinTrading = sc.GetPersistentInt(3) ;  							// time value en secondes
	int& HoraireFinSeance = sc.GetPersistentInt(4) ;  							// time value en secondes
	int& IndexDebutSecondePrecedente = sc.GetPersistentInt(5) ; 
	int& StudyID_PositionDansRange = sc.GetPersistentInt(6) ; 
	int& DrawTool = sc.GetPersistentInt(7) ; 
	int& Compteur_Variations = sc.GetPersistentInt(8) ; 						// modifié par scsf_VariationParametresTestes


	float& alphaEMA_Spread = sc.GetPersistentFloat(1);  
	float& betaEMA_Spread = sc.GetPersistentFloat(2);  
	float& alphaEMA_Ecart = sc.GetPersistentFloat(3);  
	float& betaEMA_Ecart = sc.GetPersistentFloat(4);  
	float& ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable = sc.GetPersistentFloat(5) ; 
	float& ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable = sc.GetPersistentFloat(6) ; 
	float& ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable = sc.GetPersistentFloat(7) ; 
	float& ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable = sc.GetPersistentFloat(8) ; 
	float& Diviseur_RSI_Extremum = sc.GetPersistentFloat(9) ; 
	float& Glissement = sc.GetPersistentFloat(10) ; 
	float& Exposant = sc.GetPersistentFloat(11) ; 
	
	
	

	const float Zoom_Ecart = 20 ;
	const float Zoom_Ecart_EMA = 40 ;
	const float Ecart_Absolu_EMA_ValeurMAX = 1.5 ;
	
	const int graphRegion_Price = 0 ;  // region du tracé du prix
	const int TailleTableaux = 10000 ;  

	int PremierePosition = 1 ;
	int EnPosition = 0 ;  //  +1 pour longue   et   -1 pour courte
	int EcrireResultatsDansMessageLog = 0 ;
	int Nb_PL_Gagnantes = 0 ;
	int Nb_PL_Perdantes = 0 ;
	int Nb_PC_Gagnantes = 0 ;
	int Nb_PC_Perdantes = 0 ;
	
	
	float Prix_Ouverture_Position ;
	float Prix_Fermeture_Position ;
	float Gains_PL = 0 ;
	float Pertes_PL = 0 ;   // float < 0
	float Gains_PC = 0 ;
	float Pertes_PC = 0 ;	// float < 0
	float GainMax = 0 ;
	float PerteMax = 0 ;
	float Bonus_PositionDansRange_Entree ;
	float Bonus_PositionDansRange_Sortie ;
	float Tableau_Gains_PL[TailleTableaux] ;   // gains et pertes, indifféremment
	float Tableau_Gains_PC[TailleTableaux] ;

	
	SCDateTime GainMax_DateTime ;
	SCDateTime PerteMax_DateTime ;

	
	SCString Texte ;
	
	s_sc* const ptrsc = &sc; 
	
	COLORREF rouge = RGB(250,0,0);
	COLORREF vert = RGB(0,200,0);
	COLORREF noir = RGB(0,0,0);
	
	
	if (sc.SetDefaults)
	{
		sc.GraphName = "SUIVI TENDANCE spread future-indice" ;     // appelé par les studies "Affichage Gain Cumule (sur le meme chart)" et "Affichage Gain Cumule (sur un chart externe)"   // appelé par scsf_VariationParametresTestes
		// sc.GraphRegion = 1 ;  
		sc.ScaleRangeType = SCALE_AUTO;
		sc.ValueFormat = 2;
		sc.AutoLoop = 0;  
		sc.CalculationPrecedence = LOW_PREC_LEVEL  ;    // doit être calculé après PH/PB pour récupérer PositionDansRange
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 1 ;  // global, puis préciser lesquels sont display avec DisplayNameValueInDataLine
		sc.DisplayStudyInputValues = 1;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;	
		
		
		// Inputs
		ExternalChartNumber.Name = "ChartNumber du INQ100 en UT 1s";
		ExternalChartNumber.SetChartNumber(4);
		PeriodeEMA_Spread_Input.Name = "PeriodeEMA Spread Future-Indice (ticks)";
		PeriodeEMA_Spread_Input.SetInt(3000);
		PeriodeEMA_Ecart_Input.Name = "PeriodeEMA Ecart Future-Overlay (ticks)";
		PeriodeEMA_Ecart_Input.SetInt(3000);
		RSI_Surachat_Input.Name = "Niveau Surachat du RSI";
		RSI_Surachat_Input.SetFloat(60);
		Seuil_Ecart_Absolu_Input.Name = "Seuil de l'ecart absolu";
		Seuil_Ecart_Absolu_Input.SetFloat(1.1);
		Exposant_Input.Name = "Exposant de EcartAbsolu/Seuil" ;
		Exposant_Input.SetFloat(2.0);
		DrawTool_Input.Name = "On dessine les Tools ?";
		DrawTool_Input.SetYesNo(false);
		ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable_Input.Name = "ValeurMaxBonus Entree Cadran Favorable" ;
		ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable_Input.SetFloat(2.0);			//	Une valeur de 1.0 signifie 100% de la valeur du RSI (ie RSI non modifié)   // Une valeur de 2.0 signifie que le RSI a été multiplié par 2.0 sur le PHJ sur PL		// Une valeur de 0 signifie interdiction de PL sur le PHJ
		ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable_Input.Name = "ValeurMinBonus Entree Cadran DeFavorable" ;
		ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable_Input.SetFloat(0.0);		//	Une valeur de 1.0 signifie 100% de la valeur du RSI (ie RSI non modifié)   // Une valeur de 0.0 signifie que le RSI a été multiplié par 0.0 sur le PBJ sur PL		// Une valeur de 2 signifie interdiction de PL sur le PBJ
		ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable_Input.Name = "ValeurMaxBonus Sortie Cadran Favorable" ;
		ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable_Input.SetFloat(2.0);
		ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable_Input.Name = "ValeurMinBonus Sortie Cadran DeFavorable" ;
		ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable_Input.SetFloat(0.0);
		Diviseur_RSI_Extremum_Input.Name = "Diviseur RSI Extremum" ;
		Diviseur_RSI_Extremum_Input.SetFloat(2.0);
		Glissement_Input.Name = "Glissement (pts)" ;
		Glissement_Input.SetFloat(0.5);
		TriggerRecalculate_Input.Name = "TriggerRecalculate_Input";
		TriggerRecalculate_Input.SetInt(0);
		NameLogFile_Input.Name = "NameLogFile";
		NameLogFile_Input.SetString("void");
		
		


		
		// Subgraphs parameters
		for (int SubgraphIndex = 0; SubgraphIndex <= 20; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].DrawStyle = DRAWSTYLE_LINE ;
			sc.Subgraph[SubgraphIndex].LineStyle = LINESTYLE_SOLID ;
			//sc.Subgraph[SubgraphIndex].LineLabel = LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE ;  // affiche le nom de l'indicateur retourné et sa valeur
			sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre
			sc.Subgraph[SubgraphIndex].DrawZeros = false ;
			sc.Subgraph[SubgraphIndex].DisplayNameValueInDataLine = false ;
		}
		
		OverlayINQ100.Name = "Indice+Spread"; 
		OverlayINQ100.LineWidth = 1;
		OverlayINQ100.PrimaryColor = RGB(176,176,0);		
		OverlayINQ100.DrawStyle = DRAWSTYLE_IGNORE ;
		
		Spread_UT_1s.Name = "Spread UT 1s (issu de future UT 1s moins indice UT 1s)"; 
		Spread_UT_1s.LineWidth = 1;
		Spread_UT_1s.PrimaryColor = RGB(0, 0, 255);
		Spread_UT_1s.DrawStyle = DRAWSTYLE_IGNORE;
		
		SpreadMoyen.Name = "SpreadMoyen (EMA)"; 
		SpreadMoyen.LineWidth = 2;
		SpreadMoyen.PrimaryColor = RGB(0, 0, 255);
		SpreadMoyen.DrawStyle = DRAWSTYLE_IGNORE;

		Ecart.Name = "Ecart entre le prix typique du future et OverlayINQ100"; 
		Ecart.LineWidth = 1;
		Ecart.PrimaryColor = RGB(0, 0, 0);
		Ecart.DrawZeros = true ;
		Ecart.DrawStyle = DRAWSTYLE_IGNORE;
		
		EcartBis.Name = "EcartBis (Spread_UT_1s - SpreadMoyen)"; 
		EcartBis.LineWidth = 1;
		EcartBis.PrimaryColor = RGB(0, 0, 255);
		EcartBis.DrawZeros = true ;
		EcartBis.DrawStyle = DRAWSTYLE_IGNORE;
		
		OverlayINQ100_Interpole.Name = "OverlayINQ100_Interpole"; 
		OverlayINQ100_Interpole.LineWidth = 1;
		OverlayINQ100_Interpole.PrimaryColor = RGB(0,255,255);		
		OverlayINQ100_Interpole.DrawStyle = DRAWSTYLE_IGNORE ;

		Ecart_PostInterpolation_EMA_Affichage.Name = "EMA de l'ecart entre le prix typique du future et OverlayINQ100_Interpole"; 
		Ecart_PostInterpolation_EMA_Affichage.LineWidth = 1;
		Ecart_PostInterpolation_EMA_Affichage.PrimaryColor = RGB(0, 0, 0);
		Ecart_PostInterpolation_EMA_Affichage.DrawZeros = true ;
		Ecart_PostInterpolation_EMA_Affichage.DrawStyle = DRAWSTYLE_IGNORE;

		Ecart_P_Affichage.Name = "Partie positive de Ecart_PostInterpolation (x20)"; 
		Ecart_P_Affichage.LineWidth = 1;
		Ecart_P_Affichage.PrimaryColor = RGB(0, 230, 0);
		Ecart_P_Affichage.DrawZeros = true ;
		Ecart_P_Affichage.DrawStyle = DRAWSTYLE_FILL_TO_ZERO;
		// Ecart_P_Affichage.DrawStyle = DRAWSTYLE_IGNORE;

		Ecart_N_Affichage.Name = "Partie negative de Ecart_PostInterpolation (x20)"; 
		Ecart_N_Affichage.LineWidth = 1;
		Ecart_N_Affichage.PrimaryColor = RGB(255, 0, 0);
		Ecart_N_Affichage.DrawZeros = true ;
		Ecart_N_Affichage.DrawStyle = DRAWSTYLE_FILL_TO_ZERO;
		// Ecart_N_Affichage.DrawStyle = DRAWSTYLE_IGNORE;

		Ecart_P_EMA_Affichage.Name = "EMA de la partie positive de Ecart_PostInterpolation (x40)"; 
		Ecart_P_EMA_Affichage.LineWidth = 1;
		Ecart_P_EMA_Affichage.PrimaryColor = RGB(0, 180, 0);
		Ecart_P_EMA_Affichage.DrawZeros = true ;
		// Ecart_P_EMA_Affichage.DrawStyle = DRAWSTYLE_IGNORE;

		Ecart_N_EMA_Affichage.Name = "EMA de la partie negative de Ecart_PostInterpolation (x40)"; 
		Ecart_N_EMA_Affichage.LineWidth = 1;
		Ecart_N_EMA_Affichage.PrimaryColor = RGB(200, 0, 0);
		Ecart_N_EMA_Affichage.DrawZeros = true ;
		// Ecart_N_EMA_Affichage.DrawStyle = DRAWSTYLE_IGNORE;

		RSI.Name = "RSI"; 
		RSI.LineWidth = 2;
		RSI.PrimaryColor = RGB(0, 0, 255);
		RSI.DrawZeros = true ;
		// RSI.DrawStyle = DRAWSTYLE_IGNORE;
		RSI.LineLabel =  LL_DISPLAY_VALUE | LL_VALUE_ALIGN_CENTER | LL_VALUE_ALIGN_VALUES_SCALE  ;
		RSI.DisplayNameValueInDataLine = true ;

		Ecart_Absolu_EMA_Affichage.Name = "EMA de l'ecart absolu entre le prix typique du future et OverlayINQ100_Interpole (x40)"; 
		Ecart_Absolu_EMA_Affichage.LineWidth = 2;
		Ecart_Absolu_EMA_Affichage.PrimaryColor = RGB(255, 128, 0);
		Ecart_Absolu_EMA_Affichage.DrawZeros = true ;
		// Ecart_Absolu_EMA_Affichage.DrawStyle = DRAWSTYLE_IGNORE;
		Ecart_Absolu_EMA_Affichage.LineLabel =  LL_DISPLAY_VALUE | LL_VALUE_ALIGN_CENTER | LL_VALUE_ALIGN_VALUES_SCALE  ;

		Ecart_Absolu_EMA_AffichageBis.Name = "-EMA de l'ecart absolu entre le prix typique du future et OverlayINQ100_Interpole (x40)"; 
		Ecart_Absolu_EMA_AffichageBis.LineWidth = 2;
		Ecart_Absolu_EMA_AffichageBis.PrimaryColor = RGB(255, 128, 0);
		Ecart_Absolu_EMA_AffichageBis.DrawZeros = true ;
		// Ecart_Absolu_EMA_AffichageBis.DrawStyle = DRAWSTYLE_IGNORE;
		Ecart_Absolu_EMA_AffichageBis.LineLabel =  LL_DISPLAY_VALUE | LL_VALUE_ALIGN_CENTER | LL_VALUE_ALIGN_VALUES_SCALE  ;

		RSI_Composite_Entree.Name = "RSI Composite Entree"; 
		RSI_Composite_Entree.LineWidth = 3;
		RSI_Composite_Entree.PrimaryColor = RGB(0, 0, 0);
		RSI_Composite_Entree.DrawZeros = true ;
		// RSI_Composite_Entree.DrawStyle = DRAWSTYLE_IGNORE;
		RSI_Composite_Entree.LineLabel =  LL_DISPLAY_VALUE | LL_VALUE_ALIGN_CENTER | LL_VALUE_ALIGN_VALUES_SCALE  ;
		RSI_Composite_Entree.DisplayNameValueInDataLine = true ;
		
		GainCumule.Name = "Gain Cumule"; 
		GainCumule.LineWidth = 3;
		GainCumule.PrimaryColor = RGB(0, 0, 0);
		GainCumule.DrawZeros = true ;
		GainCumule.DrawStyle = DRAWSTYLE_IGNORE;
		GainCumule.LineLabel =  LL_DISPLAY_VALUE | LL_VALUE_ALIGN_CENTER | LL_VALUE_ALIGN_VALUES_SCALE  ;
		
		RSI_Composite_Sortie.Name = "RSI Composite Sortie"; 
		RSI_Composite_Sortie.LineWidth = 1;
		// RSI_Composite_Sortie.LineStyle = LINESTYLE_DOT ;
		RSI_Composite_Sortie.PrimaryColor = RGB(0, 0, 0);
		RSI_Composite_Sortie.DrawZeros = true ;
		// RSI_Composite_Sortie.DrawStyle = DRAWSTYLE_IGNORE;
		RSI_Composite_Sortie.LineLabel =  LL_DISPLAY_VALUE | LL_VALUE_ALIGN_CENTER | LL_VALUE_ALIGN_VALUES_SCALE  ;
		RSI_Composite_Sortie.DisplayNameValueInDataLine = true ;

		RSI_Extremum_Affichage.Name = "RSI Extremum (divise)"; 
		RSI_Extremum_Affichage.LineWidth = 2;
		RSI_Extremum_Affichage.DrawStyle = DRAWSTYLE_DASH ;
		RSI_Extremum_Affichage.PrimaryColor = RGB(0, 0, 255);
		RSI_Extremum_Affichage.DrawZeros = false ;
		// RSI_Extremum_Affichage.DrawStyle = DRAWSTYLE_IGNORE;
		// RSI_Extremum_Affichage.LineLabel =  LL_DISPLAY_VALUE | LL_VALUE_ALIGN_CENTER | LL_VALUE_ALIGN_VALUES_SCALE  ;
		RSI_Extremum_Affichage.DisplayNameValueInDataLine = true ;
		
		
		return ;	
	}
	
	
	
	if ( sc.UpdateStartIndex == 0  or  TriggerRecalculate_Input.GetInt() )
	{
		// Zeroing tools
		sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   // all non user drawn Chart Drawings on the specified ChartNumber will be deleted. The LineNumber parameter in this case is ignored and should be 0. 		
		
		//Initialisations
		// sc.GraphRegion = 1;  
		OverlayINQ100[0] = 0 ;
		OverlayINQ100_Interpole[0] = 0 ;
		INQ100SansCorrection[0] = 0 ;
		Ecart_P[0] = 0 ;
		Ecart_N[0] = 0 ;
		Ecart_Absolu[0] = 0 ;
		Ecart_P_EMA[0] = 1 ;  // on initialise avec une valeur élevée pour éviter des valeurs initiales de RSI trop élevées
		Ecart_N_EMA[0] = 1 ;
		RSI[0] = 0 ;
		GainCumule[0] = 0 ;
		HoraireDebutTrace_OverlayINQ100 = HMS_TIME(9,30,15);
		HoraireDebutTrading = HMS_TIME(9,31,0);
		HoraireFinTrading = HMS_TIME(15,45,0);
		HoraireFinSeance = HMS_TIME(16,0,0);
		IndexDebutSecondePrecedente = 0 ;
		EcrireResultatsDansMessageLog = 1 ;
		DrawTool = DrawTool_Input.GetYesNo() ;
		ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable = ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable_Input.GetFloat() ;
		ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable = ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable_Input.GetFloat() ;
		ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable = ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable_Input.GetFloat() ;
		ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable = ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable_Input.GetFloat() ;
		Diviseur_RSI_Extremum = Diviseur_RSI_Extremum_Input.GetFloat() ;
		Glissement = Glissement_Input.GetFloat() ;
		Exposant = Exposant_Input.GetFloat() ;

		
		
		// Récupération du X dans le X-Ticks
		int xTicks=1 ;
		n_ACSIL::s_BarPeriod BarPeriod;
		sc.GetBarPeriodParameters(BarPeriod);
		if ( BarPeriod.IntradayChartBarPeriodType == IBPT_NUM_TRADES_PER_BAR )   // on vérifie que le graphe est bien en XTicks
		{	xTicks = BarPeriod.IntradayChartBarPeriodParameter1; 	}  
		else // message d'erreur dans le log
		{  SCString message ;   message.Format("!! ERREUR !! Le chart #%d n'est pas en ticks, impossible de tracer l'indicateur ''SUIVI TENDANCE spread future-indice''", sc.ChartNumber) ;    sc.AddMessageToLog(message,1) ; }
		
		// periodeEMA_Spread
		float periodeEMA_Spread = float( PeriodeEMA_Spread_Input.GetInt() ) / xTicks ;   
		alphaEMA_Spread = 2 / (periodeEMA_Spread+1) ;
		betaEMA_Spread = 1 - alphaEMA_Spread ;

		// periodeEMA_Ecart
		float periodeEMA_Ecart = float ( PeriodeEMA_Ecart_Input.GetInt() ) / xTicks ;   
		alphaEMA_Ecart = 2 / (periodeEMA_Ecart+1) ;
		betaEMA_Ecart = 1 - alphaEMA_Ecart ;
		
		// Récupération de la studyID du Spread_UT_1s sur le graphe de l'indice
		int StudyID_Spread_UT_1s = sc.GetStudyIDByName(ExternalChartNumber.GetChartNumber(), "Spread Future-Indice pour tracer sous l'indice", 0) ;
		if  (StudyID_Spread_UT_1s == 0 )
		{ SCString message ;  message.Format("!! ERREUR !! Le chart source #%d ne contient pas le spread Future-Indice", ExternalChartNumber.GetChartNumber()) ; sc.AddMessageToLog(message,1) ; }
		else
		{
			// Création des ChartStudySubgraphValues pour Spread_UT_1s
			ChartInputsSpread.SetChartStudySubgraphValues(ExternalChartNumber.GetChartNumber(), StudyID_Spread_UT_1s, 1);
		}
		
		// Récupération de la studyID pour PositionDansRange
		StudyID_PositionDansRange = sc.GetStudyIDByName(sc.ChartNumber, "PH PB du future", 0) ; 
		if  (StudyID_PositionDansRange == 0 )
		{ SCString message ;  message.Format("!! ERREUR !! Le chart #%d ne contient pas la study ''PH PB du future''", sc.ChartNumber) ; sc.AddMessageToLog(message,1) ; }

		
		// Tracé lignes horizontales
		/* DrawHLine (500, 1, RGB(0,0,255), &sc, sc.GraphRegion) ;			
		DrawHLine (450, 1, RGB(0,0,255), &sc, sc.GraphRegion) ;			
		DrawHLine (400, 1, RGB(0,0,255), &sc, sc.GraphRegion) ;			
		DrawHLine (350, 1, RGB(0,0,255), &sc, sc.GraphRegion) ;			
		DrawHLine (300, 1, RGB(0,0,255), &sc, sc.GraphRegion) ;			
		DrawHLine (250, 1, RGB(0,0,255), &sc, sc.GraphRegion) ;			
		DrawHLine (200, 1, RGB(0,0,255), &sc, sc.GraphRegion) ;			
		DrawHLine (150, 1, RGB(0,0,255), &sc, sc.GraphRegion) ;			
		DrawHLine (120, 1, RGB(0,0,255), &sc, sc.GraphRegion) ;			
		DrawHLine (100, 1, RGB(0,0,255), &sc, sc.GraphRegion) ;			
		DrawHLine (80, 1, RGB(0,0,255), &sc, sc.GraphRegion) ;			
		DrawHLine (60, 1, RGB(0,0,255), &sc, sc.GraphRegion) ;			
		DrawHLine (40, 1, RGB(0,0,255), &sc, sc.GraphRegion) ;			
		DrawHLine (20, 1, RGB(0,0,255), &sc, sc.GraphRegion) ;			 */
		DrawHLine (0, 3, RGB(0,0,255), &sc, sc.GraphRegion) ;			// void DrawHLine (float niveau, int epaisseur, COLORREF couleur, s_sc* ptrsc, int graphRegion)
		/* DrawHLine (-20, 1, RGB(0,0,255), &sc, sc.GraphRegion) ;			
		DrawHLine (-40, 1, RGB(0,0,255), &sc, sc.GraphRegion) ;			
		DrawHLine (-60, 1, RGB(0,0,255), &sc, sc.GraphRegion) ;			
		DrawHLine (-80, 1, RGB(0,0,255), &sc, sc.GraphRegion) ;			
		DrawHLine (-100, 1, RGB(0,0,255), &sc, sc.GraphRegion) ;			
		DrawHLine (-120, 1, RGB(0,0,255), &sc, sc.GraphRegion) ;			
		DrawHLine (-150, 1, RGB(0,0,255), &sc, sc.GraphRegion) ;			
		DrawHLine (-200, 1, RGB(0,0,255), &sc, sc.GraphRegion) ;			
		DrawHLine (-250, 1, RGB(0,0,255), &sc, sc.GraphRegion) ;			
		DrawHLine (-300, 1, RGB(0,0,255), &sc, sc.GraphRegion) ;			
		DrawHLine (-350, 1, RGB(0,0,255), &sc, sc.GraphRegion) ;			
		DrawHLine (-400, 1, RGB(0,0,255), &sc, sc.GraphRegion) ;			
		DrawHLine (-450, 1, RGB(0,0,255), &sc, sc.GraphRegion) ;			
		DrawHLine (-500, 1, RGB(0,0,255), &sc, sc.GraphRegion) ;			 */
		DrawHLine (RSI_Surachat_Input.GetFloat(), 3, RGB(0,180,0), &sc, sc.GraphRegion) ;			
		DrawHLine (-RSI_Surachat_Input.GetFloat(), 3, RGB(200,0,0), &sc, sc.GraphRegion) ;			
		DrawHLine (Seuil_Ecart_Absolu_Input.GetFloat()*Zoom_Ecart_EMA, 1, RGB(255,128,0), &sc, sc.GraphRegion) ;			
		DrawHLine (-Seuil_Ecart_Absolu_Input.GetFloat()*Zoom_Ecart_EMA, 1, RGB(255,128,0), &sc, sc.GraphRegion) ;			
		
	}
	
	
	// on récupère le cours du INQ100 dans ExternalChartBaseData
	SCGraphData ExternalChartBaseData;
	sc.GetChartBaseData(ExternalChartNumber.GetChartNumber(), ExternalChartBaseData);
	
	// on récupère l'array de SpreadExternalArray   (c'est le Spread_UT_1s instantanné)
	SCFloatArray SpreadExternalArray;
	sc.GetStudyArrayFromChartUsingID(ChartInputsSpread.GetChartStudySubgraphValues(), SpreadExternalArray);   //   void GetStudyArrayFromChartUsingID(const s_ChartStudySubgraphValues& ChartStudySubgraphValues, SCFloatArrayRef SubgraphArray);
	
	// on récupère l'array de PositionDansRange dans le Subgraph[12] de "PH PB du future" sur le présent Chart 
	SCFloatArray PositionDansRange ;
	sc.GetStudyArrayUsingID(StudyID_PositionDansRange, 12, PositionDansRange);

	
	// Tracé des subgraphes
	for (int index = sc.UpdateStartIndex * (1-TriggerRecalculate_Input.GetInt()) ; index < sc.ArraySize; index++)   // début autoloop          // début à 0 en cas de TriggerRecalculate=1
	{
		RSI_Composite_Sortie[index] = PositionDansRange[index] ;
		
		int OpenTimeValue = sc.BaseDateTimeIn[index].GetTimeWithoutMilliseconds()  ; //GetTimeWithoutMilliseconds retourne une time_value
		int PreviousOpenTimeValue = sc.BaseDateTimeIn[index-1].GetTimeWithoutMilliseconds()  ; 
		
		if ( OpenTimeValue >=  HoraireDebutTrace_OverlayINQ100 and OpenTimeValue < HoraireFinSeance )  // entre 9h30m15s et 16h00
		{
			if  ( OpenTimeValue  !=  PreviousOpenTimeValue )   // début de nouvelle seconde
			{ 
				// on récupère l'index sur le chart INQ100 en UT 1s
				int ExternalChartIndex = sc.GetContainingIndexForDateTimeIndex(ExternalChartNumber.GetChartNumber(), index); 
				
				
				INQ100SansCorrection[index] = ExternalChartBaseData[SC_OPEN][ExternalChartIndex] ;	
				Spread_UT_1s[index] = SpreadExternalArray[ExternalChartIndex] ;	
				SpreadMoyen[index] = betaEMA_Spread*SpreadMoyen[index-1] + alphaEMA_Spread*Spread_UT_1s[index] ;
				OverlayINQ100[index] = INQ100SansCorrection[index] + SpreadMoyen[index] ;  
				Ecart[index] = sc.BaseDataIn[SC_HLC][index] - OverlayINQ100[index] ;   // prix typique - OverlayINQ100
				EcartBis[index] = Spread_UT_1s[index] - SpreadMoyen[index] ;

				// Calcul sur les chandeliers de la seconde précédente écoulée (valeurs de i inférieures ou égales à index) des valeurs dépendantes de l'interpolation
				if ( index - IndexDebutSecondePrecedente != 0 )
				{
					float Pente = ( OverlayINQ100[index] - OverlayINQ100[IndexDebutSecondePrecedente] ) / ( index - IndexDebutSecondePrecedente ) ;     // pente de la droite tracée entre les valeurs de OverlayINQ100 à IndexDebutSecondePrecedente et index
					for ( int i = IndexDebutSecondePrecedente+1 ; i <= index ; i++ )
					{						
						// Calcul des subgraphes
						OverlayINQ100_Interpole[i] = OverlayINQ100[IndexDebutSecondePrecedente] + (i-IndexDebutSecondePrecedente) * Pente ;     
						Ecart_PostInterpolation[i] = sc.BaseDataIn[SC_HLC][i] - OverlayINQ100_Interpole[i] ;
						Ecart_Absolu[i] = fabs(Ecart_PostInterpolation[i]) ; 
						Ecart_P[i] = max (0, Ecart_PostInterpolation[i]) ; 
						Ecart_N[i] = min (0, Ecart_PostInterpolation[i]) ; 
						Ecart_PostInterpolation_EMA[i] = betaEMA_Ecart*Ecart_PostInterpolation_EMA[i-1]+ alphaEMA_Ecart*Ecart_PostInterpolation[i] ;
						Ecart_Absolu_EMA[i] = min ( betaEMA_Ecart*Ecart_Absolu_EMA[i-1]+ alphaEMA_Ecart*Ecart_Absolu[i], Ecart_Absolu_EMA_ValeurMAX) ;
						Ecart_P_EMA[i] = min ( betaEMA_Ecart*Ecart_P_EMA[i-1]+ alphaEMA_Ecart*Ecart_P[i], Ecart_Absolu_EMA_ValeurMAX) ;
						Ecart_N_EMA[i] = max (betaEMA_Ecart*Ecart_N_EMA[i-1]+ alphaEMA_Ecart*Ecart_N[i], -Ecart_Absolu_EMA_ValeurMAX) ;
						RSI[i] = (Ecart_P_EMA[i]-(-Ecart_N_EMA[i]))/(Ecart_P_EMA[i]+(-Ecart_N_EMA[i]))*100 ;      // RSI' = (H-B)/(H+B)   avec H et B des valeurs positives
						Ecart_P_Affichage[i] = min ( Ecart_P[i] * Zoom_Ecart , 100 );
						Ecart_N_Affichage[i] = max ( Ecart_N[i] * Zoom_Ecart, -100 ) ;
						Ecart_PostInterpolation_EMA_Affichage[i] = Ecart_PostInterpolation_EMA[i] * Zoom_Ecart_EMA ;
						Ecart_Absolu_EMA_Affichage[i] = min ( Ecart_Absolu_EMA[i] * Zoom_Ecart_EMA , 100 ) ;
						Ecart_Absolu_EMA_AffichageBis[i] = -Ecart_Absolu_EMA_Affichage[i] ;
						Ecart_P_EMA_Affichage[i] = min ( Ecart_P_EMA[i] * Zoom_Ecart_EMA , 100 ) ;
						Ecart_N_EMA_Affichage[i] = max ( Ecart_N_EMA[i] * Zoom_Ecart_EMA , -100 ) ;
						
						// Bonus_PositionDansRange		// Le Bonus_PositionDansRange doit être de 1.00000 par construction lorsque PositionDansRange=0.5 (milieu).   Il varie de manière linéaire à partir de 1.00000 lorsque PositionDansRange varie.
						if ( RSI[i] > 0  and  PositionDansRange[i] > 0.5 )				// PL et CadranFavorable pour PL
						{ Bonus_PositionDansRange_Entree = 1 + (PositionDansRange[i]-0.5)*2*(ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable-1) ;  Bonus_PositionDansRange_Sortie = 1 + (PositionDansRange[i]-0.5)*2*(ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable-1) ;  }  
						else if ( RSI[i] < 0  and  PositionDansRange[i] < 0.5 )		// PC et CadranFavorable pour PC
						{ Bonus_PositionDansRange_Entree = 1 + (0.5-PositionDansRange[i])*2*(ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable-1) ;  Bonus_PositionDansRange_Sortie = 1 + (0.5-PositionDansRange[i])*2*(ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable-1) ;  }  
						else if ( RSI[i] > 0  and  PositionDansRange[i] <= 0.5 )		// PL et CadranDefavorable pour PL
						{ Bonus_PositionDansRange_Entree = ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable + PositionDansRange[i]*2*(1-ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable) ;  Bonus_PositionDansRange_Sortie = ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable + 2*PositionDansRange[i]*(1-ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable) ;  }  
						else 	// ( RSI[i] < 0  and  PositionDansRange[i] > 0.5 )		// PC et CadranDefavorable pour PL
						{ Bonus_PositionDansRange_Entree = ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable + (1-PositionDansRange[i])*2*(1-ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable) ;  Bonus_PositionDansRange_Sortie = ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable + (1-PositionDansRange[i])*2*(1-ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable) ;  }
						
						// RSI_Composite
						RSI_Composite_Entree[i] = max ( -100, min ( 100, RSI[i] * Bonus_PositionDansRange_Entree * pow((Ecart_Absolu_EMA[i]/Seuil_Ecart_Absolu_Input.GetFloat()), Exposant ) ) )  ;
						RSI_Composite_Sortie[i]  = max ( -100, min ( 100, RSI[i] * Bonus_PositionDansRange_Sortie ) )  ;
					}
				}
				IndexDebutSecondePrecedente = index ;
				
				// Premier chandelier de la séance => initialisation des valeurs des EMA et des valeurs qui seront utilisées à index+1
				if ( OpenTimeValue ==  HoraireDebutTrace_OverlayINQ100 )
				{
					OverlayINQ100_Interpole[index] = OverlayINQ100[index] ;
					Ecart_PostInterpolation[index] = sc.BaseDataIn[SC_HLC][index] - OverlayINQ100_Interpole[index] ;
					Ecart_Absolu[index] = fabs(Ecart_PostInterpolation[index]) ; 
					Ecart_P[index] = max (0, Ecart_PostInterpolation[index]) ; 
					Ecart_N[index] = min (0, Ecart_PostInterpolation[index]) ; 
					Ecart_PostInterpolation_EMA[index] = betaEMA_Ecart*0+ alphaEMA_Ecart*Ecart_PostInterpolation[index] ;
					Ecart_Absolu_EMA[index] = betaEMA_Ecart*0 + alphaEMA_Ecart*Ecart_Absolu[index] ;
					Ecart_P_EMA[index] = 1 ;  // on initialise avec une valeur élevée pour éviter des valeurs initiales de RSI trop élevées
					Ecart_N_EMA[index] = -1 ;
					RSI[index] = 0 ;
					Ecart_P_Affichage[index] = min ( Ecart_P[index] * Zoom_Ecart , 100 );
					Ecart_N_Affichage[index] = max ( Ecart_N[index] * Zoom_Ecart, -100 ) ;
					Ecart_PostInterpolation_EMA_Affichage[index] = Ecart_PostInterpolation_EMA[index] * Zoom_Ecart_EMA ;
					Ecart_Absolu_EMA_Affichage[index] = min ( Ecart_Absolu_EMA[index] * Zoom_Ecart_EMA , 100 ) ;
					Ecart_Absolu_EMA_AffichageBis[index] = -Ecart_Absolu_EMA_Affichage[index] ;
					Ecart_P_EMA_Affichage[index] = min ( Ecart_P_EMA[index] * Zoom_Ecart_EMA , 100 ) ;
					Ecart_N_EMA_Affichage[index] = max ( Ecart_N_EMA[index] * Zoom_Ecart_EMA , -100 ) ;
					RSI_Composite_Entree[index] = 0 ;
					RSI_Composite_Sortie[index] = 0 ;
				}
			}
			else   // pas de changement de seconde  		  // => pas de calcul sur OverlayINQ100_Interpole, ni Ecart_PostInterpolation  		// => calcul de SpreadMoyen
			{
				Spread_UT_1s[index] = Spread_UT_1s[index-1] ;   
				SpreadMoyen[index] = betaEMA_Spread*SpreadMoyen[index-1] + alphaEMA_Spread*Spread_UT_1s[index] ;
				OverlayINQ100[index] = OverlayINQ100[index-1] ;
				INQ100SansCorrection[index] = INQ100SansCorrection[index-1] ;
				if ( OpenTimeValue >= HoraireDebutTrace_OverlayINQ100 ) { Ecart[index] = sc.BaseDataIn[SC_HLC][index] - OverlayINQ100[index] ;  } else { Ecart[index] = 0 ;   }
				EcartBis[index] = Spread_UT_1s[index] - SpreadMoyen[index] ;
			} 
		}
		else // avant 9h30m15s ou après 16h00
		{
			SpreadMoyen[index] = SpreadMoyen[index-1] ;   // on reporte le dernier SpreadMoyen de la séance jusqu'à la séance suivante
			Spread_UT_1s[index] = 0 ;
			INQ100SansCorrection[index] = 0 ;
			OverlayINQ100[index] = 0 ;  
			OverlayINQ100_Interpole[index] = 0 ;
			Ecart[index] = 0 ;
			EcartBis[index] = 0 ;
			Ecart_PostInterpolation[index] = 0 ;
			Ecart_Absolu[index] = 0 ;
			Ecart_P[index] = 0 ;
			Ecart_N[index] = 0 ;
			Ecart_PostInterpolation_EMA[index] = 0 ;
			Ecart_Absolu_EMA[index] = 0 ;
			Ecart_P_EMA[index] = 0 ;  
			Ecart_N_EMA[index] = 0 ;
			Ecart_P_Affichage[index] = 0 ;
			Ecart_N_Affichage[index] = 0 ;
			Ecart_PostInterpolation_EMA_Affichage[index] = 0 ;
			Ecart_Absolu_EMA_Affichage[index] = 0 ;
			Ecart_Absolu_EMA_AffichageBis[index] = 0 ;
			Ecart_P_EMA_Affichage[index] = 0 ;
			Ecart_N_EMA_Affichage[index] = 0 ;
			RSI[index] = 0 ;
			RSI_Composite_Entree[index] = 0 ;
			RSI_Composite_Sortie[index] = 0 ;
			
			IndexDebutSecondePrecedente = 99999999 ;
		}
	} // fin autoloop
	

	// ProBackTest
	float RSI_Surachat = RSI_Surachat_Input.GetFloat() ;
	for (int index = sc.UpdateStartIndex * (1-TriggerRecalculate_Input.GetInt()); index < sc.ArraySize; index++)   // début autoloop          // début à 0 en cas de TriggerRecalculate=1
	{
		int OpenTimeValue = sc.BaseDateTimeIn[index].GetTimeWithoutMilliseconds()  ; // GetTimeWithoutMilliseconds retourne une time_value
		if ( OpenTimeValue >=  HoraireDebutTrading  and  OpenTimeValue < HoraireFinTrading )  // entre 9h31 et 15h45
		{
			
			// ouverture de nouvelle position lorsque le RSI_Composite_Entree dépasse RSI_Surachat (et que la condition de sortie n'est pas déjà validée 
			if ( EnPosition == 0  and  fabs(RSI_Composite_Entree[index]) > RSI_Surachat  and  fabs(RSI_Composite_Entree[index-1]) <= RSI_Surachat   and  fabs(RSI_Composite_Sortie[index]) > fabs(RSI[index]) / Diviseur_RSI_Extremum )  
			{
				if ( PremierePosition )   // on ne comptabilise jamais la première position car le SpreadMoyen est foireux au premier open du premier jour
				{ PremierePosition = 0 ;  }
				else
				{				
					RSI_Extremum[index] = RSI[index] ;
					Prix_Ouverture_Position = sc.BaseDataIn[SC_HLC][index] ;
					Texte.Format("%.2f", sc.BaseDataIn[SC_HLC][index]) ; 
					
					if ( RSI_Composite_Entree[index] > 0 )
					{
						EnPosition = 1 ;
						if (DrawTool)
						{
							DrawMarker (MARKER_TRIANGLEUP, index, Prix_Ouverture_Position, 10, vert, graphRegion_Price, ptrsc) ; 		// void DrawMarker (int marqueur, int index, float prix, int tailleObjet, COLORREF couleur, int graphRegion, s_sc* ptrsc) 
							DrawText (Texte, index, Prix_Ouverture_Position - 1.5, 9, vert, graphRegion_Price, ptrsc) ;									// void DrawText (SCString texte, int index, float prix, int tailleTexte, COLORREF couleur, int graphRegion, s_sc* ptrsc)  	 
						}
					}				
					else
					{
						EnPosition = -1 ;				
						if (DrawTool)
						{
							DrawMarker (MARKER_TRIANGLEDOWN, index, Prix_Ouverture_Position, 10, rouge, graphRegion_Price, ptrsc) ; 		
							DrawText (Texte, index, Prix_Ouverture_Position + 1.5, 9, rouge, graphRegion_Price, ptrsc) ;		
						}
					}
				}
			}
			
			// GainCumule est recopié
			GainCumule[index] = GainCumule[index-1] ;
			
			
			// gestion de position ouverte
			{
				if ( EnPosition == 1 )  // une position longue est ouverte
				{
					// calcul de RSI_Extremum
					if ( RSI[index] > RSI_Extremum[index-1] )   // nouveau plus haut
					{ RSI_Extremum[index] = RSI[index] ;  }
					else
					{ RSI_Extremum[index] = RSI_Extremum[index-1] ;  }

					// cloture de position
					if ( RSI_Composite_Sortie[index] < RSI_Extremum[index] / Diviseur_RSI_Extremum )   
					{
						Prix_Fermeture_Position = sc.BaseDataIn[SC_HLC][index] ;
						float Gain = Prix_Fermeture_Position-Prix_Ouverture_Position ;  
						GainCumule[index] += Gain - Glissement ;
						
						if (Gain > 0 )
						{ 
							Texte.Format("Gain : %.2f", Gain) ;  
							Nb_PL_Gagnantes ++ ;
							Gains_PL += Gain - Glissement ;
							if ( Gain > GainMax ) { GainMax = Gain, GainMax_DateTime = sc.BaseDateTimeIn[index] ;  }
						}
						else
						{ 
							Texte.Format("Perte : %.2f", Gain) ;  
							Nb_PL_Perdantes ++ ;
							Pertes_PL += Gain - Glissement ;		// float < 0
							if ( Gain < PerteMax ) { PerteMax = Gain, PerteMax_DateTime = sc.BaseDateTimeIn[index] ;  }
						}
						
						if (DrawTool)  
						{
							DrawText (Texte, index, Prix_Fermeture_Position - 2.5, 9, noir, graphRegion_Price, ptrsc) ;		
							DrawMarker (MARKER_SQUARE, index, Prix_Fermeture_Position, 5, rouge, graphRegion_Price, ptrsc) ; 	
							Texte.Format("%.2f", Prix_Fermeture_Position) ; 
							DrawText (Texte, index, Prix_Fermeture_Position - 1.5, 9, rouge, graphRegion_Price, ptrsc) ;		
						}

						int Nb_PL = Nb_PL_Gagnantes + Nb_PL_Perdantes ;
						if ( Nb_PL < TailleTableaux ) Tableau_Gains_PL[Nb_PL] = Gain - Glissement ;
						RSI_Extremum[index] = RSI_Extremum[index-1] ;
						EnPosition = 0 ;
					}
				}
				else if ( EnPosition == -1 )   // une position courte est ouverte
				{
					// calcul de RSI_Extremum
					if ( RSI[index] < RSI_Extremum[index-1] )   // nouveau plus bas
					{ RSI_Extremum[index] = RSI[index] ;  }
					else
					{ RSI_Extremum[index] = RSI_Extremum[index-1] ;  }

					// cloture de position
					if ( RSI_Composite_Sortie[index] > RSI_Extremum[index] / Diviseur_RSI_Extremum )   
					{
						Prix_Fermeture_Position = sc.BaseDataIn[SC_HLC][index] ;
						float Gain = Prix_Ouverture_Position-Prix_Fermeture_Position ;  
						GainCumule[index] += Gain - Glissement ;
						
						if (Gain > 0 )
						{ 
							Texte.Format("Gain : %.2f", Gain) ;  
							Nb_PC_Gagnantes ++ ;
							Gains_PC += Gain - Glissement ;
							if ( Gain > GainMax ) { GainMax = Gain, GainMax_DateTime = sc.BaseDateTimeIn[index] ;  }
						} 
						else
						{ 
							Texte.Format("Perte : %.2f", Gain) ;  
							Nb_PC_Perdantes ++ ;
							Pertes_PC += Gain - Glissement ;   // float < 0
							if ( Gain < PerteMax ) { PerteMax = Gain, PerteMax_DateTime = sc.BaseDateTimeIn[index] ;  }
						}
						
						if (DrawTool)
						{
							DrawText (Texte, index, Prix_Fermeture_Position + 2.5, 9, noir, graphRegion_Price, ptrsc) ;		
							DrawMarker (MARKER_SQUARE, index, Prix_Fermeture_Position, 5, vert, graphRegion_Price, ptrsc) ; 	
							Texte.Format("%.2f", Prix_Fermeture_Position) ;  
							DrawText (Texte, index, Prix_Fermeture_Position + 1.5, 9, vert, graphRegion_Price, ptrsc) ;		
						}

						int Nb_PC = Nb_PC_Gagnantes + Nb_PC_Perdantes ;
						if ( Nb_PC < TailleTableaux ) Tableau_Gains_PC[Nb_PC] = Gain - Glissement ;						
						RSI_Extremum[index] = RSI_Extremum[index-1] ;
						EnPosition = 0 ;
					}
				}
				else if ( EnPosition == 0 )   // pas de position ouverte
				{
					RSI_Extremum[index] = 0 ;
				}
			}
		}
		else  // pré-market ou post-market
		{ 
			// annulation d'une position qui n'aurait pas été refermée durant la séance précédente
			EnPosition = 0 ;  
			
			// GainCumule est recopié
			GainCumule[index] = GainCumule[index-1] ;
			RSI_Extremum[index] = 0 ;
		}
		
		RSI_Extremum_Affichage[index] = RSI_Extremum[index] / Diviseur_RSI_Extremum ;
		
	}  // fin autoloop
	
	



	if ( EcrireResultatsDansMessageLog )
	{
		
		int Nb_Positions_Gagnantes = Nb_PL_Gagnantes + Nb_PC_Gagnantes ;
		int Nb_Positions_Perdantes = Nb_PL_Perdantes + Nb_PC_Perdantes ; 
		int Nb_PL = Nb_PL_Gagnantes + Nb_PL_Perdantes ;
		int Nb_PC = Nb_PC_Gagnantes + Nb_PC_Perdantes ;
		int Nb_Positions = Nb_Positions_Gagnantes + Nb_Positions_Perdantes ;
		float Gains_Positions = Gains_PL + Gains_PC ;
		float Pertes_Positions = Pertes_PL + Pertes_PC ;
		float Gain_Net_Total = Gains_Positions + Pertes_Positions ;
		float Gain_Net_PL = Gains_PL+Pertes_PL ;
		float Gain_Net_PC = Gains_PC+Pertes_PC ;
		float Gain_Moyen = Gain_Net_Total / Nb_Positions ;
		float Gain_Moyen_PL = Gain_Net_PL / Nb_PL ;
		float Gain_Moyen_PC = Gain_Net_PC / Nb_PC ;
		float PF = -Gains_Positions/Pertes_Positions ;
		float PF_PL = -Gains_PL/Pertes_PL ; 
		float PF_PC = -Gains_PC/Pertes_PC ;
		float ET, ET_PL, ET_PC ;		// écarts-type
		float IC, IC_PL, IC_PC ;		// indices de criticité
		SCString message ; 


		if ( Nb_PL >= TailleTableaux  or  Nb_PC >= TailleTableaux )
		{ 	message.Format("|| !! WARNING !!  La variable TailleTableaux (vaut %d) est insuffisante ||", TailleTableaux) ;  sc.AddMessageToLog(message,1) ;  }
		else
		{
			// écarts-type PL
			float Distance_Carre_Gains_PL = 0 ;
			for (int i=1 ; i <= Nb_PL ; i++ )  { Distance_Carre_Gains_PL += pow((Tableau_Gains_PL[i]-Gain_Moyen_PL), 2) ;  }   // somme des carrés de l'écart des trades à la moyenne
			ET_PL = sqrt(Distance_Carre_Gains_PL/Nb_PL) ;
			IC_PL = sqrt(Nb_PL) * Gain_Moyen_PL / ET_PL ;
			
			// écarts-type PC
			float Distance_Carre_Gains_PC = 0 ;
			for (int i=1 ; i <= Nb_PC ; i++ )  { Distance_Carre_Gains_PC += pow((Tableau_Gains_PC[i]-Gain_Moyen_PC), 2) ;  }   // somme des carrés de l'écart des trades à la moyenne
			ET_PC = sqrt(Distance_Carre_Gains_PC/Nb_PC) ;
			IC_PC = sqrt(Nb_PC) * Gain_Moyen_PC / ET_PC ;
			
			// écarts-type global
			ET = sqrt( (Distance_Carre_Gains_PL+Distance_Carre_Gains_PC) / Nb_Positions ) ;
			IC = sqrt(Nb_Positions) * Gain_Moyen / ET ;
		}
		
		// Ecriture dans le messagelog de Sierra
		message.Format("|| ***** | PeriodeEMA (Spread) : %d t | PeriodeEMA (Ecart) : %d t | Seuil RSI : %.0f | Seuil Ecart Absolu : %.2f pts | Exposant : %.2f pts | Diviseur RSI Extremum : %.1f | Glissement : %.2f pt | ***** ||",  PeriodeEMA_Spread_Input.GetInt(), PeriodeEMA_Ecart_Input.GetInt(), RSI_Surachat_Input.GetFloat(), Seuil_Ecart_Absolu_Input.GetFloat(), Exposant, Diviseur_RSI_Extremum, Glissement_Input.GetFloat() ) ;  sc.AddMessageToLog(message,0) ;  
		message.Format("|| ***** | ValeurMaxBonus Entree Cadran Favorable : %.1f | ValeurMaxBonus Entree Cadran Defavorable : %.1f | ValeurMaxBonus Sortie Cadran Favorable : %.1f | ValeurMaxBonus Sortie Cadran Defavorable : %.1f | ***** ||",  ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable_Input.GetFloat(), ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable_Input.GetFloat(), ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable_Input.GetFloat(), ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable_Input.GetFloat()  ) ;  sc.AddMessageToLog(message,0) ;  
		message.Format("|| Nb Positions : %d | Nb Gagnantes : %d | Nb Perdantes : %d | %%Gagnantes : %.0f%% ||", Nb_Positions, Nb_Positions_Gagnantes, Nb_Positions_Perdantes, ((float)Nb_Positions_Gagnantes)/Nb_Positions*100) ;  sc.AddMessageToLog(message,0) ;  
		message.Format("|| Gain Net (total) : %.0f pts   | Gains : %.0f pts      | Pertes : %.0f pts      |                        Gain Moyen : %.2f pts     | PF (total) : %.2f  | ET = %.2f pts | IC = %.1f  ||", Gain_Net_Total, Gains_Positions, Pertes_Positions, Gain_Moyen, PF, ET, IC) ;  sc.AddMessageToLog(message,0) ;  
		message.Format("|| Gain Net (long) : %.0f pts  | Gains PL : %.0f pts  | Pertes PL : %.0f pts | Nb PL : %d  | Gain Moyen PL : %.2f pts  | PF (long) : %.2f  | ET = %.2f pts | IC = %.1f ||", 	Gain_Net_PL, Gains_PL, Pertes_PL, Nb_PL, Gain_Moyen_PL, PF_PL, ET_PL, IC_PL ) ;  sc.AddMessageToLog(message,0) ;  
		message.Format("|| Gain Net (court) : %.0f pts | Gains PC : %.0f pts | Pertes PC : %.0f pts | Nb PC : %d | Gain Moyen PC : %.2f pts | PF (court) : %.2f | ET = %.2f pts | IC = %.1f ||", Gain_Net_PC, Gains_PC, Pertes_PC, Nb_PC, Gain_Moyen_PC, PF_PC, ET_PC, IC_PC ) ;  sc.AddMessageToLog(message,0) ;  
		message.Format("|| Gain Max : %.1f pts | ", GainMax ) ; int Year, Month, Day, Hour, Minute, Second ;  GainMax_DateTime.GetDateTimeYMDHMS(Year, Month, Day, Hour, Minute, Second) ;  message.AppendFormat("%04d-%02d-%02d %02d:%02d ||", Year, Month, Day, Hour, Minute) ;  sc.AddMessageToLog(message,0) ;  
		message.Format("|| Perte Max : %.1f pts | ", PerteMax ) ;  PerteMax_DateTime.GetDateTimeYMDHMS(Year, Month, Day, Hour, Minute, Second) ;  message.AppendFormat("%04d-%02d-%02d %02d:%02d ||", Year, Month, Day, Hour, Minute) ;  sc.AddMessageToLog(message,0) ;  
		
		
		// Ecriture dans fichier .csv
		{
			// Déclaration du flux d'écriture
			SCString NameLogFile = NameLogFile_Input.GetString() ;
			std::ofstream myStream ;
			myStream.open(NameLogFile, std::ofstream::out | std::ofstream::app) ;
			if ( myStream.is_open() ) 	{	/* SCString message ;  message.Format("Ouverture du fichier : OK") ;  sc.AddMessageToLog(message, 0) ; */  }  else {	SCString message ;  message.Format("Ouverture du fichier .csv : erreur") ;  sc.AddMessageToLog(message, 0) ;  }
			
			// remplissage du fichier .csv avec : 
			//					"Compteur" << ";" 		<< "Nb Positions" << ";" << "Gain Moyen (pt)" << ";" 				<< "Gain Net (pts)"    << ";" 		<< "PF" << ";" 						  << "IC" << ";" 						   << "Nb Positions" << ";" << "Gain Moyen (pt)" << ";" 						<< "Gain Net (pts)" << ";" 			 << "PF" << ";"    						  << "IC" << ";" 				  << "Nb Positions" << ";" << "Gain Moyen (pt)" << ";" 							<< "Gain Net (pts)" << ";" 			<< "PF"      << ";" 						  << "IC"     << ";" 						<< "Spread" 									   << ";" << "Ecart" 									   << ";" << "RSI" 									   << ";" << "Ecart Abs" 									   << ";" << "Exposant" << ";"  << "Diviseur RSI Extr" 		<< ";" << "ValeurMaxBonus Entree Cadran Favorable" 												     << ";" << "ValeurMinBonus Entree Cadran Defavorable" 															<< ";" << "ValeurMaxBonus Sortie Cadran Favorable" 														<< ";" << "ValeurMinBonus Sortie Cadran Defavorable" 													  << ";" << "Glissement" << ";" << "\n" ; 
			myStream << Compteur_Variations << ";" << Nb_Positions << ";" << round(Gain_Moyen*10)/10 << ";" << round(Gain_Net_Total) << ";" << round(PF*10)/10   << ";"  << round(IC*10)/10   << ";"    << Nb_PL           << ";" << round(Gain_Moyen_PL*10)/10    << ";" << round(Gain_Net_PL)    << ";" << round(PF_PL*10)/10 << ";" << round(IC_PL*10)/10 << ";" << Nb_PC  << ";" << round(Gain_Moyen_PC*10)/10  << ";" << round(Gain_Net_PC)    << ";" << round(PF_PC*10)/10 << ";" << round(IC_PC*10)/10 << ";" << PeriodeEMA_Spread_Input.GetInt() << ";" << PeriodeEMA_Ecart_Input.GetInt() << ";" << RSI_Surachat_Input.GetFloat() << ";" << Seuil_Ecart_Absolu_Input.GetFloat() << ";" << Exposant   << ";" << Diviseur_RSI_Extremum << ";" << ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable_Input.GetFloat() << ";" <<  ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable_Input.GetFloat()   << ";" << ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable_Input.GetFloat() << ";" << ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable_Input.GetFloat() << ";" << Glissement   << "\n" ;
		}
	}
	
	// Ce DLL ne sera intégralement recalculé au prochain appel que si la study externe réamorce le TriggerRecalculate_Input
	TriggerRecalculate_Input.SetInt(0) ;
	
	// pour les backtests :
	{	sc.UpdateAlways = 1 ;  }


}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_AffichageGainCumule_MemeChart(SCStudyInterfaceRef sc) 
{
	
	SCSubgraphRef GainCumule = sc.Subgraph[0]; 										
	
	int& StudyID = sc.GetPersistentInt(0); 
	
	s_sc* const ptrsc = &sc; 
	
	SCFloatArray Array_Externe ;
	
	
	if (sc.SetDefaults)
	{
		sc.GraphName = "Affichage Gain Cumule (sur le meme chart)" ; 
		// sc.GraphRegion = 1 ;  
		sc.ScaleRangeType = SCALE_AUTO;
		sc.ValueFormat = 2;
		sc.CalculationPrecedence = VERY_LOW_PREC_LEVEL  ;  // la study "SUIVI TENDANCE spread future-indice" doit être calculée précédemment
		sc.AutoLoop = 0;  
		sc.DisplayStudyName = 1;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 1 ;
		sc.DisplayStudyInputValues = 0;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;	
		
		// Subgraphs parameters
		for (int SubgraphIndex = 0; SubgraphIndex <= 0; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].DrawStyle = DRAWSTYLE_LINE ;
			sc.Subgraph[SubgraphIndex].LineStyle = LINESTYLE_SOLID ;
			//sc.Subgraph[SubgraphIndex].LineLabel = LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE ;  // affiche le nom de l'indicateur retourné et sa valeur
			sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre
			sc.Subgraph[SubgraphIndex].DrawZeros = false ;
		}

		GainCumule.Name = "Gain Cumule"; 
		GainCumule.LineWidth = 2;
		GainCumule.PrimaryColor = RGB(0, 0, 0);
		GainCumule.DrawZeros = true ;
		// GainCumule.DrawStyle = DRAWSTYLE_IGNORE;
		GainCumule.LineLabel =  LL_DISPLAY_VALUE | LL_VALUE_ALIGN_CENTER | LL_VALUE_ALIGN_VALUES_SCALE  ;
		
		return ;	
	}
	
	
	
	if ( sc.UpdateStartIndex == 0 )
	{
		// Récupération de la StudyID
		StudyID = sc.GetStudyIDByName(sc.ChartNumber, "SUIVI TENDANCE spread future-indice", 0) ; 
		if  (StudyID == 0 )
		{ SCString message ;  message.Format("!! ERREUR !! Le chart #%d ne contient pas la study ''SUIVI TENDANCE spread future-indice''", sc.ChartNumber) ; sc.AddMessageToLog(message,1) ; }
		
		// Tracé lignes horizontales
		DrawHLine (100, 1, RGB(0,0,255), &sc, sc.GraphRegion) ;			
		DrawHLine (0, 2, RGB(0,0,255), &sc, sc.GraphRegion) ;			// void DrawHLine (float niveau, int epaisseur, COLORREF couleur, s_sc* ptrsc, int graphRegion)
		DrawHLine (-100, 1, RGB(0,0,255), &sc, sc.GraphRegion) ;			
	}
	
	
	// on récupère les valeurs de GainCumule dans le Subgraph[16] de "SUIVI TENDANCE spread future-indice" sur le présent Chart 
	sc.GetStudyArrayUsingID(StudyID, 16, Array_Externe);

	for (int index = sc.UpdateStartIndex; index < sc.ArraySize; index++)   // début autoloop          
	{
		GainCumule[index] = Array_Externe[index] ; 
	}  // fin autoloop
	
	
	if ( GainCumule[sc.ArraySize-1] == 0  and  sc.UpdateStartIndex == 0 )
	{
		SCString message ;
		message.Format(" !! WARNING !!  Si la study ''Affichage Gain Cumule sur le meme chart'' n'affiche rien c'est vraisemblablement parce qu'elle appelle StudyID=%d comme study source", StudyID ) ;  sc.AddMessageToLog(message,1) ;  
	}
	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_AffichageGainCumule_ChartExterne(SCStudyInterfaceRef sc) 
{
	
	SCSubgraphRef GainCumule = sc.Subgraph[0]; 										
	
	SCInputRef ExternalChartNumber = sc.Input[0]; 
	SCInputRef ExternalChartInputs = sc.Input[1];  
	
	s_sc* const ptrsc = &sc; 
	
	int StudyID ;
	
	
	
	
	if (sc.SetDefaults)
	{
		sc.GraphName = "Affichage Gain Cumule (sur un chart externe)" ; 
		// sc.GraphRegion = 1 ;  
		sc.ScaleRangeType = SCALE_AUTO;
		sc.ValueFormat = 2;
		sc.AutoLoop = 0;  
		sc.DisplayStudyName = 1;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 1 ;
		sc.DisplayStudyInputValues = 0;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;	
		
		
		// Inputs
		ExternalChartNumber.Name = "ChartNumber du calcul du SpreadIndex";
		ExternalChartNumber.SetChartNumber(6);
		
		
		// Subgraphs parameters
		for (int SubgraphIndex = 0; SubgraphIndex <= 0; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].DrawStyle = DRAWSTYLE_LINE ;
			sc.Subgraph[SubgraphIndex].LineStyle = LINESTYLE_SOLID ;
			//sc.Subgraph[SubgraphIndex].LineLabel = LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE ;  // affiche le nom de l'indicateur retourné et sa valeur
			sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre
			sc.Subgraph[SubgraphIndex].DrawZeros = false ;
		}

		GainCumule.Name = "Gain Cumule"; 
		GainCumule.LineWidth = 2;
		GainCumule.PrimaryColor = RGB(0, 0, 0);
		GainCumule.DrawZeros = true ;
		// GainCumule.DrawStyle = DRAWSTYLE_IGNORE;
		GainCumule.LineLabel =  LL_DISPLAY_VALUE | LL_VALUE_ALIGN_CENTER | LL_VALUE_ALIGN_VALUES_SCALE  ;
		
		return ;	
	}
	
	
	
	if ( sc.UpdateStartIndex == 0 )
	{
		// Récupération de la StudyID
		StudyID = sc.GetStudyIDByName(ExternalChartNumber.GetChartNumber(), "SUIVI TENDANCE spread future-indice", 0) ; 
		if  (StudyID == 0 )
		{ SCString message ;  message.Format("!! ERREUR !! Le chart source #%d ne contient pas la study ''SUIVI TENDANCE spread future-indice''", ExternalChartNumber.GetChartNumber()) ; sc.AddMessageToLog(message,1) ; }
		else	{	ExternalChartInputs.SetChartStudySubgraphValues(ExternalChartNumber.GetChartNumber(), StudyID, 16);	}	   // SCInputRef sc.Input[].SetChartStudySubgraphValues(int ChartNumber, int StudyID, int SubgraphIndex);
		
		// Tracé lignes horizontales
		DrawHLine (100, 1, RGB(0,0,255), &sc, sc.GraphRegion) ;			
		DrawHLine (0, 2, RGB(0,0,255), &sc, sc.GraphRegion) ;			// void DrawHLine (float niveau, int epaisseur, COLORREF couleur, s_sc* ptrsc, int graphRegion)
		DrawHLine (-100, 1, RGB(0,0,255), &sc, sc.GraphRegion) ;			
	}
	
	
	// on récupère les valeurs du GainCumule dans le Subgraph adéquat
	SCFloatArray Array_Externe ;
	sc.GetStudyArrayFromChartUsingID(ExternalChartInputs.GetChartStudySubgraphValues(), Array_Externe);   //   void GetStudyArrayFromChartUsingID(const s_ChartStudySubgraphValues& ChartStudySubgraphValues, SCFloatArrayRef SubgraphArray);


	for (int index = sc.UpdateStartIndex; index < sc.ArraySize; index++)   // début autoloop          
	{
		// on récupère l'index du nouveau chandelier sur le chart d'où provient le calcul (UT 1-tick)
		int ExternalIndex = sc.GetContainingIndexForDateTimeIndex(ExternalChartNumber.GetChartNumber(), index); 
		
		GainCumule[index] = Array_Externe[ExternalIndex] ; 
	}  // fin autoloop
	
	
	if ( GainCumule[sc.ArraySize-1] == 0  and  sc.UpdateStartIndex == 0 )
	{
		SCString message ;
		message.Format(" !! WARNING !!  Si la study ''Affichage Gain Cumule (source : chart externe)'' n'affiche rien c'est vraisemblablement parce qu'elle appelle StudyID=%d comme study source", StudyID ) ;  sc.AddMessageToLog(message,1) ;  
	}
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Rectangle2 : s_UseTool             // Attention à ne pas nommer cette structure Rectangle => erreur de compilation, le terme est probablement réservé pour un autre usage
{
	Rectangle2(int beginIndex, int endIndex, float niveau1, float niveau2, COLORREF maCouleur, s_sc* ptrsc) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
	{
		Clear(); 
		DrawingType = DRAWING_RECTANGLEHIGHLIGHT;
		AddMethod = UTAM_ADD_ALWAYS;	
		BeginIndex = beginIndex ;
		EndIndex = endIndex ;
		BeginValue = niveau1 ;
		EndValue = niveau2 ;
		LineWidth = 0;    // For the Rectangle Drawing types, in order to see the outline it is necessary to set the LineWidth to 1 or greater. Otherwise, the outline will not be visible.
		SecondaryColor = maCouleur;    // Le membre "Color" correspond au contour du rectangle   // SecondaryColor corespond au fill
		TransparencyLevel = 90;
		Region = ptrsc->GraphRegion ; 
	} 
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void creationRectangle (int beginIndex, int endIndex, float niveau1, float niveau2, COLORREF maCouleur, s_sc* ptrsc)
{
	//Création d'instance de Rectangle2 et appel de sc.UseTool
	Rectangle2 objet(beginIndex, endIndex, niveau1, niveau2, maCouleur, ptrsc);
	ptrsc->UseTool(objet); 
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_PHPBDynamiquesForFuture(SCStudyInterfaceRef sc) 
{
	
	SCSubgraphRef PlusHautJour = sc.Subgraph[1];    
	SCSubgraphRef PlusBasJour  = sc.Subgraph[2];    
	SCSubgraphRef Decile1 = sc.Subgraph[3];    
	SCSubgraphRef Decile2 = sc.Subgraph[4];    
	SCSubgraphRef Decile3 = sc.Subgraph[5];    
	SCSubgraphRef Decile4 = sc.Subgraph[6];    
	SCSubgraphRef Decile5 = sc.Subgraph[7];    
	SCSubgraphRef Decile6 = sc.Subgraph[8];    
	SCSubgraphRef Decile7 = sc.Subgraph[9];    
	SCSubgraphRef Decile8 = sc.Subgraph[10];    
	SCSubgraphRef Decile9 = sc.Subgraph[11];    
	SCSubgraphRef PositionDansRange = sc.Subgraph[12];    // comprise entre 0 (sur le PBJ) et 1 (sur le PHJ)    // appelé par scsf_StudyPrincipale
	
	// SCInputRef DZInput = sc.Input[1]; 
	SCInputRef TraceVectorielONOFF = sc.Input[2];
	SCInputRef InversionRougeVert = sc.Input[3];
	
	float& DZ = sc.GetPersistentFloat(1);  
	
	int& FirstIndex = sc.GetPersistentInt(1);  
	int& LastIndex = sc.GetPersistentInt(2);  



	if (sc.SetDefaults)
	{
		sc.GraphName = "PH PB du future" ;   // scsf_StudyPrincipale
		sc.ScaleRangeType = SCALE_SAMEASREGION;
		sc.ValueFormat = 24;    // Pourcentage avec deux décimales		// {  SCString message ;   message.Format("sc.ValueFormat  = %d", sc.ValueFormat ) ;    sc.AddMessageToLog(message,0) ; }
		sc.AutoLoop = 0;  
		sc.TransparencyLevel = 90 ;  // 90%
		sc.GraphRegion = 0;  
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 1 ;  // global, puis préciser lesquels sont display avec DisplayNameValueInDataLine
		sc.DisplayStudyInputValues = 0;
		sc.HideStudy = 0 ;  

		
		// DZInput.Name = "Dead Zone" ;
		// DZInput.SetFloat(1);	
		TraceVectorielONOFF.Name = "Trace Vectoriel ON/OFF";
		TraceVectorielONOFF.SetYesNo(1);
		InversionRougeVert.Name = "Vert = PH | Rouge = PB";
		InversionRougeVert.SetYesNo(1);

	
		// paramètres des tracés
		for (int SubgraphIndex = 1; SubgraphIndex <= 12; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre
			sc.Subgraph[SubgraphIndex].DrawZeros = false;  
			sc.Subgraph[SubgraphIndex].DisplayNameValueInDataLine = false ;
		}
		
		PlusHautJour.Name = "PH se"; 
		PlusBasJour.Name = "PB se"; 
		Decile1.Name = "Decile 1";
		Decile2.Name = "Decile 2";
		Decile3.Name = "Decile 3";
		Decile4.Name = "Decile 4";
		Decile5.Name = "Decile 5";
		Decile6.Name = "Decile 6";
		Decile7.Name = "Decile 7";
		Decile8.Name = "Decile 8";
		Decile9.Name = "Decile 9";
		PositionDansRange.Name = "Position Dans Range";
		
		
		
		PlusHautJour.PrimaryColor = RGB(250,0,0);
		PlusBasJour.PrimaryColor = RGB(0,200,0);
		int Lum = 210 ;
		Decile1.PrimaryColor = RGB(Lum,Lum,Lum);
		Decile2.PrimaryColor = RGB(Lum,Lum,Lum);
		Decile3.PrimaryColor = RGB(Lum,Lum,Lum);
		Decile4.PrimaryColor = RGB(Lum,Lum,Lum);
		Decile5.PrimaryColor = RGB(Lum,Lum,Lum);
		Decile6.PrimaryColor = RGB(Lum,Lum,Lum);
		Decile7.PrimaryColor = RGB(Lum,Lum,Lum);
		Decile8.PrimaryColor = RGB(Lum,Lum,Lum);
		Decile9.PrimaryColor = RGB(Lum,Lum,Lum);
		PositionDansRange.PrimaryColor = RGB(0,0,255);

		PlusHautJour.DrawStyle = DRAWSTYLE_HIDDEN ;    // ne pas mettre en DRAWSTYLE_IGNORE pour que l'interactive scale en mode auto fasse un tracé en full range
		PlusBasJour.DrawStyle = DRAWSTYLE_HIDDEN ;
		PositionDansRange.DrawStyle = DRAWSTYLE_HIDDEN ;
		
		PlusHautJour.LineLabel = LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;
		PlusBasJour.LineLabel = LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;
		PositionDansRange.LineLabel = LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;
		
		Decile5.LineWidth = 3;
		
		PositionDansRange.DisplayNameValueInDataLine = true ;

		return ;	
	}  // fin de Set.Defaults
	

	
	if ( sc.UpdateStartIndex == 0  )  
	{ 	
		sc.ValueFormat = 24;
		
		PlusHautJour[0] = sc.High[0] ;  PlusBasJour[0] = sc.Low[0] ;  
		Decile1[0] = 0 ;  Decile1[0] = 0 ;  Decile1[0] = 0 ;  Decile1[0] = 0 ;  Decile1[0] = 0 ;  Decile1[0] = 0 ;  Decile1[0] = 0 ;  Decile1[0] = 0 ;  Decile1[0] = 0 ;

		FirstIndex = 0 ;
		LastIndex = 0 ;
		
		
		// gestion automatique de DZ et GraphRegion
		SCString ChartSymbol = sc.GetChartSymbol(sc.ChartNumber) ;
		n_ACSIL :: s_BarPeriod BarPeriod ;
		sc.GetBarPeriodParameters(BarPeriod) ;
		// cas du FNQ100
		if (  ChartSymbol.Compare("NQ",2) == 0  and  BarPeriod.IntradayChartBarPeriodType == IBPT_NUM_TRADES_PER_BAR  )      
		{
			if ( BarPeriod.IntradayChartBarPeriodParameter1 == 1 )  			{ DZ = 0.5 ; 	sc.GraphRegion = 0 ;  }  
			else if ( BarPeriod.IntradayChartBarPeriodParameter1 == 16 )  	{ DZ = 1 ; 		/* sc.GraphRegion = 0 ;  */ }  
			else if ( BarPeriod.IntradayChartBarPeriodParameter1 == 200 )  	{ DZ = 2 ; 		sc.GraphRegion = 0 ;  }    
			else if ( BarPeriod.IntradayChartBarPeriodParameter1 == 2000 )	{ DZ = 4 ; 		sc.GraphRegion = 0 ;  }  
			else if ( BarPeriod.IntradayChartBarPeriodParameter1 > 2000 )  	{ DZ = 8 ; 		sc.GraphRegion = 0 ;  }  
			else   																					 	{ DZ = 2 ; 		sc.GraphRegion = 0 ;  }  
		}
		
		if ( BarPeriod.IntradayChartBarPeriodParameter1 <= 16 )
		{ 
			Decile1.DrawStyle = DRAWSTYLE_LINE ;
			Decile2.DrawStyle = DRAWSTYLE_LINE ;
			Decile3.DrawStyle = DRAWSTYLE_LINE ;
			Decile4.DrawStyle = DRAWSTYLE_LINE ;
			Decile5.DrawStyle = DRAWSTYLE_LINE ;
			Decile6.DrawStyle = DRAWSTYLE_LINE ;
			Decile7.DrawStyle = DRAWSTYLE_LINE ;
			Decile8.DrawStyle = DRAWSTYLE_LINE ;
			Decile9.DrawStyle = DRAWSTYLE_LINE ;
		}
		else 
		{
			Decile1.DrawStyle = DRAWSTYLE_IGNORE ;
			Decile2.DrawStyle = DRAWSTYLE_IGNORE ;
			Decile3.DrawStyle = DRAWSTYLE_IGNORE ;
			Decile4.DrawStyle = DRAWSTYLE_IGNORE ;
			Decile5.DrawStyle = DRAWSTYLE_IGNORE ;
			Decile6.DrawStyle = DRAWSTYLE_IGNORE ;
			Decile7.DrawStyle = DRAWSTYLE_IGNORE ;
			Decile8.DrawStyle = DRAWSTYLE_IGNORE ;
			Decile9.DrawStyle = DRAWSTYLE_IGNORE ;
		}
		
	}

	
	
	// Coeur de la study : calcul des SCSubgraphRef
	for (int index = sc.UpdateStartIndex; index < sc.ArraySize; index++)                 // début AutoLoop
	{
		
		if  (    (  sc.BaseDateTimeIn.TimeAt(index) == 64800    and   sc.BaseDateTimeIn.TimeAt(index-1)!= 64800 )  or   index==0  )  // RAZ à la première bougie de 18h00
		{
			PlusHautJour[index] = sc.High[index] ;  PlusBasJour[index] = sc.Low[index] ;  
		}
		else if  (    (  sc.BaseDateTimeIn.TimeAt(index) >= 34200    and   sc.BaseDateTimeIn.TimeAt(index-1) < 34200 )   )  // RAZ à la première bougie de 9h30
		{ 	
			PlusHautJour[index] = sc.High[index] ;  PlusBasJour[index] = sc.Low[index] ;   
		}
		else // tous les autres chandeliers
		{
			if ( sc.High[index] > PlusHautJour[index-1] ) 	{ PlusHautJour[index] = sc.High[index] ; }	 	else	{ PlusHautJour[index] = PlusHautJour[index-1]  ; }
			if ( sc.Low[index] < PlusBasJour[index-1] ) 	{ PlusBasJour[index] = sc.Low[index] ; }		else	{ PlusBasJour[index] = PlusBasJour[index-1] ;  }
		}
		
		// Calcul des déciles et PositionDansRange
		float PBJ = PlusBasJour[index] ;
		float PHJ = PlusHautJour[index] ;
		Decile1[index] = 0.9 * PBJ + 0.1 * PHJ  ;
		Decile2[index] = 0.8 * PBJ + 0.2 * PHJ  ;
		Decile3[index] = 0.7 * PBJ + 0.3 * PHJ  ;
		Decile4[index] = 0.6 * PBJ + 0.4 * PHJ  ;
		Decile5[index] = 0.5 * PBJ + 0.5 * PHJ  ;
		Decile6[index] = 0.4 * PBJ + 0.6 * PHJ  ;
		Decile7[index] = 0.3 * PBJ + 0.7 * PHJ  ;
		Decile8[index] = 0.2 * PBJ + 0.8 * PHJ  ;
		Decile9[index] = 0.1 * PBJ + 0.9 * PHJ  ;
		PositionDansRange[index] = (sc.Close[index]-PBJ)/max((PHJ-PBJ),1) ;
	}
	
	
	// Tracé vectoriel des sc.Subgraph PlusHautJour et PlusBasJour
	if ( /* sc.ChartWindowIsActive  and */  TraceVectorielONOFF.GetYesNo() )
	{	
		// Le recalcul des rectangles n'est réalisé que si FirstIndex ou LastIndex varie
		if ( FirstIndex != sc.IndexOfFirstVisibleBar  or  LastIndex != sc.IndexOfLastVisibleBar )
		{	
			FirstIndex = sc.IndexOfFirstVisibleBar ;
			LastIndex = sc.IndexOfLastVisibleBar ;

			// les useTool doivent être effacés à chaque appel du DLL pour éviter une fuite de mémoire   
			sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   // all non user drawn Chart Drawings on the specified ChartNumber will be deleted. The LineNumber parameter in this case is ignored and should be 0. 		
			
			s_sc* const ptrsc = &sc ;
			COLORREF CouleurHaute ;
			COLORREF CouleurBasse ;
			
			if ( InversionRougeVert.GetYesNo() ) 	{ CouleurHaute = RGB(0,200,0) ;  CouleurBasse = RGB(250,0,0) ;  }		// suivi de tendance
			else  												{ CouleurHaute = RGB(250,0,0) ;  CouleurBasse = RGB(0,200,0) ;  }     	// contre-tendance (AMR)
			
			// Zone transparente rouge au-dessus de PHJ
			{
				int beginIndex = FirstIndex ;
				float PHJ_IndexMoinsUn = PlusHautJour[FirstIndex] ;
				float PHJ_Index ;
				for ( int index = FirstIndex+1 ; index < LastIndex+1 ; index++ )
				{
					PHJ_Index = PlusHautJour[index] ; 
					// on teste si on fait un nouveau plus haut. Si oui, on trace un rectangle entre beginIndex et index-1. Etc jusqu'au prochain nouveau plus haut.
					if ( PHJ_Index != PHJ_IndexMoinsUn)
					{
						
						if ( PHJ_Index != 0 and PHJ_IndexMoinsUn != 0 )
						{
							creationRectangle (beginIndex, index, PHJ_IndexMoinsUn, PHJ_IndexMoinsUn+DZ, CouleurHaute, ptrsc)    ;      
							creationRectangle (index-1, index, PHJ_IndexMoinsUn+DZ, PHJ_Index+DZ, CouleurHaute, ptrsc)    ;      
							beginIndex = index ;
						}
						else // après 16h02 ou premier rectangle du jour
						{
							creationRectangle (beginIndex, index, PHJ_IndexMoinsUn, PHJ_IndexMoinsUn+DZ, CouleurHaute, ptrsc)    ;     
							beginIndex = index-1 ;
						}
						PHJ_IndexMoinsUn = PHJ_Index ;
					}
				}
				//Dernier rectangle
				creationRectangle (beginIndex, LastIndex, PlusHautJour[LastIndex], PlusHautJour[LastIndex]+DZ, CouleurHaute, ptrsc)    ;    
			}
			
			// Zone transparente rouge en-dessous de PBJ
			{
				int beginIndex = FirstIndex ;
				float PBJ_IndexMoinsUn = PlusBasJour[FirstIndex] ;
				float PBJ_Index ;
				for ( int index = FirstIndex+1 ; index < LastIndex+1 ; index++ )
				{
					PBJ_Index = PlusBasJour[index] ; 
					// on teste si on fait un nouveau plus bas. Si oui, on trace un rectangle entre beginIndex et index-1. Etc jusqu'au prochain nouveau plus haut.
					if ( PBJ_Index != PBJ_IndexMoinsUn)
					{
						
						if ( PBJ_Index != 0 and PBJ_IndexMoinsUn != 0 )
						{
							creationRectangle (beginIndex, index, PBJ_IndexMoinsUn, PBJ_IndexMoinsUn-DZ, CouleurBasse, ptrsc)    ;      
							creationRectangle (index-1, index, PBJ_IndexMoinsUn-DZ, PBJ_Index-DZ, CouleurBasse, ptrsc)    ;      
							beginIndex = index ;
						}
						else // après 16h02 ou premier rectangle du jour
						{
							creationRectangle (beginIndex, index, PBJ_IndexMoinsUn, PBJ_IndexMoinsUn-DZ, CouleurBasse, ptrsc)    ;     
							beginIndex = index-1 ;
						}
						PBJ_IndexMoinsUn = PBJ_Index ;
					}
				}
				//Dernier rectangle
				creationRectangle (beginIndex, LastIndex, PlusBasJour[LastIndex], PlusBasJour[LastIndex]-DZ, CouleurBasse, ptrsc)    ;    
			}
		}

		
		// lorsque je suis déconnecté d'IB, pour les backtests :
		if ( sc.ServerConnectionState != SCS_CONNECTED )  
		{	sc.UpdateAlways = 1 ;  }
	}
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool FileExists (const std::string& name)             // https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
 {
    std::ifstream f(name.c_str());
    return f.good();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct TexteBasCentre_UTAMAOA : s_UseTool   // Texte bas centré
{											
	TexteBasCentre_UTAMAOA (SCString texte, int tailleTexte,  COLORREF couleur, int graphRegion, int Line_Number) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
	{
		Clear(); 
		AddMethod = UTAM_ADD_OR_ADJUST ;			
		LineNumber = Line_Number ;
		DrawingType = DRAWING_TEXT;
		Text = texte ;		
		BeginDateTime = 75 ;     // To use a value relative to the left side of the chart for BeginDateTime rather than an absolute Date Time value, specify an integer value from 1 to 150
		UseRelativeVerticalValues = true ;   // If UseRelativeVerticalValues is set to 1 or a nonzero value, then BeginValue is a value relative to the Chart Region itself. It is a percentage. Where 1 = 1%.
		BeginValue = 1 ;   
		FontSize = tailleTexte ; 
		Color = couleur ;
		Region = graphRegion ; 
		FontBold = 1 ;
		TextAlignment = DT_BOTTOM | DT_CENTER;  // options disponibles : DT_TOP, DT_VCENTER, DT_BOTTOM, DT_CENTER, DT_LEFT, DT_RIGHT
	} 
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_VariationParametresTestes(SCStudyInterfaceRef sc) 
{
	
	// Inputs /////////////////////////////////////////////////////////////////////////////////////////////
	SCInputRef NomFichier_Input = sc.Input[0];
	
	SCInputRef PeriodeEMA_Spread_Input_Initial = sc.Input[3];
	SCInputRef PeriodeEMA_Spread_Input_Increment = sc.Input[4];
	SCInputRef PeriodeEMA_Spread_Input_Final = sc.Input[5];
	
	SCInputRef PeriodeEMA_Ecart_Input_Initial = sc.Input[6];
	SCInputRef PeriodeEMA_Ecart_Input_Increment = sc.Input[7];
	SCInputRef PeriodeEMA_Ecart_Input_Final = sc.Input[8];
	
	SCInputRef RSI_Surachat_Input_Initial = sc.Input[12]; 
	SCInputRef RSI_Surachat_Input_Increment = sc.Input[13]; 
	SCInputRef RSI_Surachat_Input_Final = sc.Input[14]; 
	
	SCInputRef Seuil_Ecart_Absolu_Input_Initial = sc.Input[15]; 
	SCInputRef Seuil_Ecart_Absolu_Input_Increment = sc.Input[16]; 
	SCInputRef Seuil_Ecart_Absolu_Input_Final = sc.Input[17]; 
	
	SCInputRef Exposant_Input_Initial = sc.Input[18]; 
	SCInputRef Exposant_Input_Increment = sc.Input[19]; 
	SCInputRef Exposant_Input_Final = sc.Input[20]; 
	
	SCInputRef Diviseur_RSI_Extremum_Input_Initial = sc.Input[24]; 
	SCInputRef Diviseur_RSI_Extremum_Input_Increment = sc.Input[25]; 
	SCInputRef Diviseur_RSI_Extremum_Input_Final = sc.Input[26]; 
	
	SCInputRef ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable_Input_Initial = sc.Input[27]; 
	SCInputRef ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable_Input_Increment = sc.Input[28]; 
	SCInputRef ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable_Input_Final = sc.Input[29]; 
	
	SCInputRef ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable_Input_Initial = sc.Input[30]; 
	SCInputRef ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable_Input_Increment = sc.Input[31]; 
	SCInputRef ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable_Input_Final = sc.Input[32]; 
	
	SCInputRef ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable_Input_Initial = sc.Input[33]; 
	SCInputRef ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable_Input_Increment = sc.Input[34]; 
	SCInputRef ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable_Input_Final = sc.Input[35]; 
	
	SCInputRef ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable_Input_Initial = sc.Input[36]; 
	SCInputRef ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable_Input_Increment = sc.Input[37]; 
	SCInputRef ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable_Input_Final = sc.Input[38]; 
	
	
	// Références /////////////////////////////////////////////////////////////////////////////////////////////
	
	int& StudyID = sc.GetPersistentInt(0);
	int& Clicks_PreviousCycle = sc.GetPersistentInt(1);  
	int& PeriodeEMA_Spread_Compteur = sc.GetPersistentInt(2);  
	int& PeriodeEMA_Ecart_Compteur = sc.GetPersistentInt(3);  
	int& VariationsParametresEnCours = sc.GetPersistentInt(4);    // c'est un booléen qui indique si la présente study est en cours (les paramètres sont en cours de parcours) ou est terminée (tous les paramètres ont déjà été variés)
	int& Variations = sc.GetPersistentInt(5);  
	int& Compteur_Variations = sc.GetPersistentInt(6);  
	
	float& RSI_Surachat_Compteur = sc.GetPersistentFloat(1);  
	float& Seuil_Ecart_Absolu_Compteur = sc.GetPersistentFloat(2);  
	float& Exposant_Compteur = sc.GetPersistentFloat(3);  
	float& Diviseur_RSI_Extremum_Compteur = sc.GetPersistentFloat(4);  
	float& ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable_Compteur = sc.GetPersistentFloat(5);  
	float& ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable_Compteur = sc.GetPersistentFloat(6);  
	float& ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable_Compteur = sc.GetPersistentFloat(7);  
	float& ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable_Compteur = sc.GetPersistentFloat(8);  
	
	
	

	// Bloc sc.SetDefaults /////////////////////////////////////////////////////////////////////////////////////////////
	if (sc.SetDefaults)
	{
		sc.GraphName = "Variation des parametres testes" ;   
		sc.GraphRegion = 0 ;  
		sc.ValueFormat = 0 ;
		sc.AutoLoop = 0 ;  
		sc.DisplayStudyName = 1;
		sc.DisplayStudyInputValues = 1;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 0 ;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		
		// Inputs /////////////////////////////////////////////////////////////////////////////////////////////
		NomFichier_Input.Name = "Nom du fichier de log";
		NomFichier_Input.SetString("Tous les parametres ont ete parcourus");
		
		PeriodeEMA_Spread_Input_Initial.Name = "PeriodeEMA_Spread_Input_Initial";
		PeriodeEMA_Spread_Input_Initial.SetInt(1500);
		PeriodeEMA_Spread_Input_Increment.Name = "PeriodeEMA_Spread_Input_Increment";
		PeriodeEMA_Spread_Input_Increment.SetInt(1500);
		PeriodeEMA_Spread_Input_Final.Name = "PeriodeEMA_Spread_Input_Final";
		PeriodeEMA_Spread_Input_Final.SetInt(6000);
		
		PeriodeEMA_Ecart_Input_Initial.Name = "PeriodeEMA_Ecart_Input_Initial";
		PeriodeEMA_Ecart_Input_Initial.SetInt(1500);
		PeriodeEMA_Ecart_Input_Increment.Name = "PeriodeEMA_Ecart_Input_Increment";
		PeriodeEMA_Ecart_Input_Increment.SetInt(1500);
		PeriodeEMA_Ecart_Input_Final.Name = "PeriodeEMA_Ecart_Input_Final";
		PeriodeEMA_Ecart_Input_Final.SetInt(6000);
		
		RSI_Surachat_Input_Initial.Name = "RSI_Surachat_Input_Initial";
		RSI_Surachat_Input_Initial.SetFloat(40);
		RSI_Surachat_Input_Increment.Name = "RSI_Surachat_Input_Increment";
		RSI_Surachat_Input_Increment.SetFloat(10);
		RSI_Surachat_Input_Final.Name = "RSI_Surachat_Input_Final";
		RSI_Surachat_Input_Final.SetFloat(60);
		
		Seuil_Ecart_Absolu_Input_Initial.Name = "Seuil_Ecart_Absolu_Input_Initial";
		Seuil_Ecart_Absolu_Input_Initial.SetFloat(1.1);
		Seuil_Ecart_Absolu_Input_Increment.Name = "Seuil_Ecart_Absolu_Input_Increment";
		Seuil_Ecart_Absolu_Input_Increment.SetFloat(1);
		Seuil_Ecart_Absolu_Input_Final.Name = "Seuil_Ecart_Absolu_Input_Final";
		Seuil_Ecart_Absolu_Input_Final.SetFloat(1);
		
		Exposant_Input_Initial.Name = "Exposant_Input_Initial";
		Exposant_Input_Initial.SetFloat(2);
		Exposant_Input_Increment.Name = "Exposant_Input_Increment";
		Exposant_Input_Increment.SetFloat(1);
		Exposant_Input_Final.Name = "Exposant_Input_Final";
		Exposant_Input_Final.SetFloat(1);
		
		Diviseur_RSI_Extremum_Input_Initial.Name = "Diviseur_RSI_Extremum_Input_Initial";
		Diviseur_RSI_Extremum_Input_Initial.SetFloat(1.6);
		Diviseur_RSI_Extremum_Input_Increment.Name = "Diviseur_RSI_Extremum_Input_Increment";
		Diviseur_RSI_Extremum_Input_Increment.SetFloat(0.2);
		Diviseur_RSI_Extremum_Input_Final.Name = "Diviseur_RSI_Extremum_Input_Final";
		Diviseur_RSI_Extremum_Input_Final.SetFloat(2.4);
		
		ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable_Input_Initial.Name = "ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable_Input_Initial";
		ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable_Input_Initial.SetFloat(1);
		ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable_Input_Increment.Name = "ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable_Input_Increment";
		ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable_Input_Increment.SetFloat(1);
		ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable_Input_Final.Name = "ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable_Input_Final";
		ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable_Input_Final.SetFloat(1);
		
		ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable_Input_Initial.Name = "ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable_Input_Initial";
		ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable_Input_Initial.SetFloat(1);
		ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable_Input_Increment.Name = "ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable_Input_Increment";
		ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable_Input_Increment.SetFloat(1);
		ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable_Input_Final.Name = "ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable_Input_Final";
		ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable_Input_Final.SetFloat(1);
		
		ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable_Input_Initial.Name = "ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable_Input_Initial";
		ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable_Input_Initial.SetFloat(1);
		ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable_Input_Increment.Name = "ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable_Input_Increment";
		ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable_Input_Increment.SetFloat(1);
		ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable_Input_Final.Name = "ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable_Input_Final";
		ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable_Input_Final.SetFloat(1);
		
		ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable_Input_Initial.Name = "ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable_Input_Initial";
		ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable_Input_Initial.SetFloat(1);
		ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable_Input_Increment.Name = "ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable_Input_Increment";
		ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable_Input_Increment.SetFloat(1);
		ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable_Input_Final.Name = "ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable_Input_Final";
		ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable_Input_Final.SetFloat(1);

	
		return ;	
	} // fin sc.Defaults
	
	

	if ( sc.UpdateStartIndex == 0 )
	{
		// Initialisations
		sc.GraphRegion = 0 ;  		
		VariationsParametresEnCours = 1 ;
		Compteur_Variations = 1 ;  

		// Initialisations des compteurs
		PeriodeEMA_Spread_Compteur = PeriodeEMA_Spread_Input_Initial.GetInt() ;
		PeriodeEMA_Ecart_Compteur = PeriodeEMA_Ecart_Input_Initial.GetInt() ;
		RSI_Surachat_Compteur = RSI_Surachat_Input_Initial.GetFloat() ;
		Seuil_Ecart_Absolu_Compteur = Seuil_Ecart_Absolu_Input_Initial.GetFloat() ;
		Exposant_Compteur = Exposant_Input_Initial.GetFloat() ;
		Diviseur_RSI_Extremum_Compteur = Diviseur_RSI_Extremum_Input_Initial.GetFloat() ;
		ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable_Compteur = ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable_Input_Initial.GetFloat() ;
		ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable_Compteur = ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable_Input_Initial.GetFloat() ;
		ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable_Compteur = ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable_Input_Initial.GetFloat() ;
		ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable_Compteur = ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable_Input_Initial.GetFloat() ;
		
		// Récupération de la StudyID de la study principale
		StudyID = sc.GetStudyIDByName(sc.ChartNumber, "SUIVI TENDANCE spread future-indice", 0) ; 
		if  (StudyID == 0 )
		{ SCString message ;  message.Format("!! ERREUR !! Le chart #%d ne contient pas la study ''SUIVI TENDANCE spread future-indice''", sc.ChartNumber) ; sc.AddMessageToLog(message,1) ; }
		
		// Les incréments doivent être strictement positifs
		if ( PeriodeEMA_Spread_Input_Increment.GetInt() <=0  or  PeriodeEMA_Ecart_Input_Increment.GetInt() <=0  or RSI_Surachat_Input_Increment.GetFloat()  <=0  or Seuil_Ecart_Absolu_Input_Increment.GetFloat() <=0  or Exposant_Input_Increment.GetFloat() <=0  
			or Diviseur_RSI_Extremum_Input_Increment.GetFloat() <=0  or ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable_Input_Increment.GetFloat()  <=0  or ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable_Input_Increment.GetFloat()  <=0  or ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable_Input_Increment.GetFloat()  <=0  or ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable_Input_Increment.GetFloat() <=0  )
		{ 
			VariationsParametresEnCours = 0 ;
			SCString message ;  message.Format(" !! ERREUR !! Les incréments doivent être strictement positifs") ;  sc.AddMessageToLog(message, 1) ; 
		}
		
		// Si la study s'appelle "Tous les parametres ont ete parcourus", ça veut dire qu'il faut changer de nom avant de la relancer
		SCString Texte = NomFichier_Input.GetString() ;
		if ( Texte.Compare("Tous les parametres ont ete parcourus",0) == 0 )      // returns 0 if they are the same
		{
			VariationsParametresEnCours = 0 ;
			SCString message ;  message.Format(" !! WARNING !! Modifier le nom du fichier de log avant de relancer la study") ;  sc.AddMessageToLog(message, 1) ;
			return ;
		}
		
		// Génération du nom du fichier de log
		SCString TemporarySCString = NomFichier_Input.GetString() ;  
		SCString NameLogFile ;  NameLogFile.Format("C:\\SierraChart\\Data\\Optimisations\\%s", TemporarySCString.GetChars() ) ;  NameLogFile.AppendFormat(".csv") ;
		
		// Transfert du nom de fichier de log à la study principale
		sc.SetChartStudyInputString(sc.ChartNumber, StudyID, 15, NameLogFile );	// int SetChartStudyInputString(int ChartNumber, int StudyID, int InputIndex, const SCString& StringValue);

		// Création des noms de colonne s'il s'agit de la première ouverture du fichier
		std::string string_NomFichierLog = NameLogFile.GetChars(); // conversion SCString en string
		if ( ! FileExists(string_NomFichierLog) )  // le fichier n'existe pas
		{ 
			// Déclaration du flux d'écriture
			std::ofstream myStream ;
			myStream.open(string_NomFichierLog, std::ofstream::out | std::ofstream::app) ;       
			if ( myStream.is_open() )
			{	 
				// en-têtes de colonnes du fichier .csv
				// myStream << 						";" << "Positions longues et courtes"<< ";"			<< ";" 							<< ";" 				<< ";"			<< ";" << "Positions longues"<< ";"								<< ";" 							<< ";" 				<< ";"			<< ";" << "Positions courtes"<< ";"								<< ";" 							<< ";" 				<< ";"			<< ";" << "PeriodesEMA" << ";" 		<< ";" << "Seuils" << ";"						<< ";"						<< ";"				<<"\n" ; 
				myStream << "Compteur" << ";" << "Nb Positions" << ";" << "Gain Moyen (pt)" << ";" << "Gain Net (pts)" << ";" << "PF" << ";" << "IC" << ";" 	<< "Nb Positions" << ";" << "Gain Moyen (pt)" << ";" << "Gain Net (pts)" << ";" << "PF" << ";" << "IC" << ";" << "Nb Positions" << ";" << "Gain Moyen (pt)" << ";" << "Gain Net (pts)" << ";" << "PF" << ";" << "IC" << ";" << "Spread" << ";" << "Ecart" << ";" << "RSI" << ";" << "Ecart Abs" << ";" << "Exposant" << ";"  << "Diviseur RSI Extr" << ";" << "ValeurMaxBonus Entree Cadran Favorable" << ";" << "ValeurMinBonus Entree Cadran Defavorable" << ";" << "ValeurMaxBonus Sortie Cadran Favorable" << ";" << "ValeurMinBonus Sortie Cadran Defavorable" << ";" << "Glissement" << ";" << "\n" ; 
			}  
			else { SCString message ;  message.Format("Ouverture du fichier a la creation : erreur") ;  sc.AddMessageToLog(message, 1) ;  }
		}
		else
		{
			SCString message ;  message.Format(" !! ERREUR !! Le nom de fichier existe deja. La variation des parametres est stopppee.") ;  sc.AddMessageToLog(message, 1) ;
			VariationsParametresEnCours = 0 ;
		}
		
		// Calcul du nombre de variations que la study devra réaliser
		int Variations_PeriodeEMA_Spread = max(1, floor( (PeriodeEMA_Spread_Input_Final.GetInt()-PeriodeEMA_Spread_Input_Initial.GetInt())/PeriodeEMA_Spread_Input_Increment.GetInt() +1 ) ) ;
		int Variations_PeriodeEMA_Ecart = max(1, floor( (PeriodeEMA_Ecart_Input_Final.GetInt()-PeriodeEMA_Ecart_Input_Initial.GetInt())/PeriodeEMA_Ecart_Input_Increment.GetInt() +1 ) ) ;
		int Variations_RSI_Surachat = max(1, floor( (RSI_Surachat_Input_Final.GetFloat()-RSI_Surachat_Input_Initial.GetFloat())/RSI_Surachat_Input_Increment.GetFloat() +1 ) ) ;
		int Variations_Seuil_Ecart_Absolu = max(1, floor( (Seuil_Ecart_Absolu_Input_Final.GetFloat()-Seuil_Ecart_Absolu_Input_Initial.GetFloat())/Seuil_Ecart_Absolu_Input_Increment.GetFloat() +1 ) ) ;
		int Variations_Exposant = max(1, floor( (Exposant_Input_Final.GetFloat()-Exposant_Input_Initial.GetFloat())/Exposant_Input_Increment.GetFloat() +1 ) ) ;
		int Variations_Diviseur_RSI_Extremum = max(1, floor( (Diviseur_RSI_Extremum_Input_Final.GetFloat()-Diviseur_RSI_Extremum_Input_Initial.GetFloat())/Diviseur_RSI_Extremum_Input_Increment.GetFloat() +1 ) ) ;
		int Variations_ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable = max(1, floor( (ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable_Input_Final.GetFloat()-ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable_Input_Initial.GetFloat())/ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable_Input_Increment.GetFloat() +1 ) ) ;
		int Variations_ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable = max(1, floor( (ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable_Input_Final.GetFloat()-ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable_Input_Initial.GetFloat())/ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable_Input_Increment.GetFloat() +1 ) ) ;
		int Variations_ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable = max(1, floor( (ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable_Input_Final.GetFloat()-ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable_Input_Initial.GetFloat())/ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable_Input_Increment.GetFloat() +1 ) ) ;
		int Variations_ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable = max(1, floor( (ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable_Input_Final.GetFloat()-ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable_Input_Initial.GetFloat())/ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable_Input_Increment.GetFloat() +1 ) ) ;
		Variations = Variations_PeriodeEMA_Spread * Variations_PeriodeEMA_Ecart * Variations_RSI_Surachat * Variations_Seuil_Ecart_Absolu * Variations_Exposant * Variations_Diviseur_RSI_Extremum * Variations_ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable * Variations_ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable * Variations_ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable * Variations_ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable ;
		SCString message ; 
		message.Format("Variations_PeriodeEMA_Spread : %d", Variations_PeriodeEMA_Spread) ;  sc.AddMessageToLog(message, 0) ;
		message.Format("Variations_PeriodeEMA_Ecart : %d", Variations_PeriodeEMA_Ecart) ;  sc.AddMessageToLog(message, 0) ;
		message.Format("Variations_RSI_Surachat : %d", Variations_RSI_Surachat) ;  sc.AddMessageToLog(message, 0) ;
		message.Format("Variations_Seuil_Ecart_Absolu : %d", Variations_Seuil_Ecart_Absolu) ;  sc.AddMessageToLog(message, 0) ;
		message.Format("Variations_Exposant : %d", Variations_Exposant) ;  sc.AddMessageToLog(message, 0) ;
		message.Format("Variations_Diviseur_RSI_Extremum : %d", Variations_Diviseur_RSI_Extremum) ;  sc.AddMessageToLog(message, 0) ;
		message.Format("Variations_ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable : %d", Variations_ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable) ;  sc.AddMessageToLog(message, 0) ;
		message.Format("Variations_ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable : %d", Variations_ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable) ;  sc.AddMessageToLog(message, 0) ;
		message.Format("Variations_ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable : %d", Variations_ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable) ;  sc.AddMessageToLog(message, 0) ;
		message.Format("Variations_ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable : %d", Variations_ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable) ;  sc.AddMessageToLog(message, 0) ;
		message.Format("Variations TOTAL : %d", Variations) ;  sc.AddMessageToLog(message, 0) ;
		
		
		// Initialisations des inputs de la study principale
		if ( VariationsParametresEnCours )
		{
			sc.SetPersistentIntForChartStudy(sc.ChartNumber, StudyID, 8, Compteur_Variations) ;		// void SetPersistentIntForChartStudy(int ChartNumber, int StudyID, int Key, int Value);
			sc.SetChartStudyInputInt(sc.ChartNumber, StudyID, 1, PeriodeEMA_Spread_Compteur);	
			sc.SetChartStudyInputInt(sc.ChartNumber, StudyID, 2, PeriodeEMA_Ecart_Compteur);	
			sc.SetChartStudyInputFloat(sc.ChartNumber, StudyID, 4, RSI_Surachat_Compteur);	
			sc.SetChartStudyInputFloat(sc.ChartNumber, StudyID, 5, Seuil_Ecart_Absolu_Compteur);	
			sc.SetChartStudyInputFloat(sc.ChartNumber, StudyID, 6, Exposant_Compteur);	
			sc.SetChartStudyInputFloat(sc.ChartNumber, StudyID, 8, Diviseur_RSI_Extremum_Compteur);	
			sc.SetChartStudyInputFloat(sc.ChartNumber, StudyID, 9, ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable_Compteur);	
			sc.SetChartStudyInputFloat(sc.ChartNumber, StudyID, 10, ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable_Compteur);	
			sc.SetChartStudyInputFloat(sc.ChartNumber, StudyID, 11, ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable_Compteur);	
			sc.SetChartStudyInputFloat(sc.ChartNumber, StudyID, 12, ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable_Compteur);	
			sc.SetChartStudyInputInt(sc.ChartNumber, StudyID, 14, 1);		// TriggerRecalculate de la study principale est mis à 1
		}
		
	}  // fin sc.UpdateStartIndex== 0



	
	
	if ( VariationsParametresEnCours == 0 )
	{
		NomFichier_Input.SetString("Tous les parametres ont ete parcourus") ;    // lorsque tous les paramètres ont été parcourus, on modifie le nom du fichier pour ne pas poursuivre l'écriture dans le fichier déjà créé
		
		// Génération du nom du fichier de log et envoi à la study externe
		SCString TemporarySCString = NomFichier_Input.GetString() ;  SCString NameLogFile ;
		NameLogFile.Format("C:\\SierraChart\\Data\\Optimisations\\%s", TemporarySCString.GetChars() ) ;
		NameLogFile.AppendFormat(".csv") ;
		sc.SetChartStudyInputString(sc.ChartNumber, StudyID, 15, NameLogFile );
	}



	// Incréments des compteurs de la study et modifications des inputs de la study principale
	int TriggerRecalculate_ExternalStudy ;   // statut de l'input TriggerRecalculate de la study principale
	sc.GetChartStudyInputInt(sc.ChartNumber, StudyID, 14, TriggerRecalculate_ExternalStudy);	
	if ( VariationsParametresEnCours  and  TriggerRecalculate_ExternalStudy==0 )		// si TriggerRecalculate_ExternalStudy vaut 1 ça veut dire que le jeu de paramètres en place n'a pas encore été testé
	{
		Compteur_Variations ++ ;
		
		// Incréments des compteurs de la study 
		if ( PeriodeEMA_Spread_Compteur + PeriodeEMA_Spread_Input_Increment.GetInt() <= PeriodeEMA_Spread_Input_Final.GetInt() )
		{	PeriodeEMA_Spread_Compteur += PeriodeEMA_Spread_Input_Increment.GetInt() ;  }
		else
		{
			PeriodeEMA_Spread_Compteur = PeriodeEMA_Spread_Input_Initial.GetInt() ;
			if ( PeriodeEMA_Ecart_Compteur + PeriodeEMA_Ecart_Input_Increment.GetInt() <= PeriodeEMA_Ecart_Input_Final.GetInt() )
			{	PeriodeEMA_Ecart_Compteur += PeriodeEMA_Ecart_Input_Increment.GetInt() ;  }
			else
			{
				PeriodeEMA_Ecart_Compteur = PeriodeEMA_Ecart_Input_Initial.GetInt() ;
				if ( RSI_Surachat_Compteur + RSI_Surachat_Input_Increment.GetFloat() <= RSI_Surachat_Input_Final.GetFloat() )
				{	RSI_Surachat_Compteur += RSI_Surachat_Input_Increment.GetFloat() ;  }
				else
				{
					RSI_Surachat_Compteur = RSI_Surachat_Input_Initial.GetFloat() ;
					if ( Seuil_Ecart_Absolu_Compteur + Seuil_Ecart_Absolu_Input_Increment.GetFloat() <= Seuil_Ecart_Absolu_Input_Final.GetFloat() )
					{	Seuil_Ecart_Absolu_Compteur += Seuil_Ecart_Absolu_Input_Increment.GetFloat() ;  }
					else
					{
						Seuil_Ecart_Absolu_Compteur = Seuil_Ecart_Absolu_Input_Initial.GetFloat() ;
						if ( Exposant_Compteur + Exposant_Input_Increment.GetFloat() <= Exposant_Input_Final.GetFloat() )
						{	Exposant_Compteur += Exposant_Input_Increment.GetFloat() ;  }
						else
						{
							Exposant_Compteur = Exposant_Input_Initial.GetFloat() ;
							if ( Diviseur_RSI_Extremum_Compteur + Diviseur_RSI_Extremum_Input_Increment.GetFloat() <= Diviseur_RSI_Extremum_Input_Final.GetFloat() )
							{	Diviseur_RSI_Extremum_Compteur += Diviseur_RSI_Extremum_Input_Increment.GetFloat() ;  }
							else
							{
								Diviseur_RSI_Extremum_Compteur = Diviseur_RSI_Extremum_Input_Initial.GetFloat() ;
								if ( ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable_Compteur + ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable_Input_Increment.GetFloat() <= ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable_Input_Final.GetFloat() )
								{	ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable_Compteur += ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable_Input_Increment.GetFloat() ;  }
								else
								{
									ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable_Compteur = ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable_Input_Initial.GetFloat() ;
									if ( ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable_Compteur + ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable_Input_Increment.GetFloat() <= ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable_Input_Final.GetFloat() )
									{	ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable_Compteur += ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable_Input_Increment.GetFloat() ;  }
									else
									{
										ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable_Compteur = ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable_Input_Initial.GetFloat() ;
										if ( ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable_Compteur + ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable_Input_Increment.GetFloat() <= ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable_Input_Final.GetFloat() )
										{	ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable_Compteur += ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable_Input_Increment.GetFloat() ;  }
										else
										{
											ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable_Compteur = ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable_Input_Initial.GetFloat() ;
											if ( ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable_Compteur + ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable_Input_Increment.GetFloat() <= ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable_Input_Final.GetFloat() )
											{	ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable_Compteur += ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable_Input_Increment.GetFloat() ;  }
											else
											{
												ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable_Compteur = ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable_Input_Initial.GetFloat() ;
												VariationsParametresEnCours = 0 ;
												SCString message ;  message.Format(" !! WARNING !! Variation des parametres terminee") ;  sc.AddMessageToLog(message, 1) ;
												SCString AlertPathAndFileName = "C:\\SierraChart\\AlertSounds\\blong2.wav" ;  sc.PlaySound(AlertPathAndFileName,1) ;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		
		// Modifications des inputs de la study principale
		if ( VariationsParametresEnCours ) // test nécessaire pour éviter de faire calculer deux fois le dernier jeu d'inputs 
		{
			sc.SetPersistentIntForChartStudy(sc.ChartNumber, StudyID, 8, Compteur_Variations) ;		// void SetPersistentIntForChartStudy(int ChartNumber, int StudyID, int Key, int Value);
			sc.SetChartStudyInputInt(sc.ChartNumber, StudyID, 1, PeriodeEMA_Spread_Compteur);	
			sc.SetChartStudyInputInt(sc.ChartNumber, StudyID, 2, PeriodeEMA_Ecart_Compteur);	
			sc.SetChartStudyInputFloat(sc.ChartNumber, StudyID, 4, RSI_Surachat_Compteur);	
			sc.SetChartStudyInputFloat(sc.ChartNumber, StudyID, 5, Seuil_Ecart_Absolu_Compteur);	
			sc.SetChartStudyInputFloat(sc.ChartNumber, StudyID, 6, Exposant_Compteur);	
			sc.SetChartStudyInputFloat(sc.ChartNumber, StudyID, 8, Diviseur_RSI_Extremum_Compteur);	
			sc.SetChartStudyInputFloat(sc.ChartNumber, StudyID, 9, ValeurMaxBonus_PositionDansRange_Entree_CadranFavorable_Compteur);	
			sc.SetChartStudyInputFloat(sc.ChartNumber, StudyID, 10, ValeurMinBonus_PositionDansRange_Entree_CadranDefavorable_Compteur);	
			sc.SetChartStudyInputFloat(sc.ChartNumber, StudyID, 11, ValeurMaxBonus_PositionDansRange_Sortie_CadranFavorable_Compteur);	
			sc.SetChartStudyInputFloat(sc.ChartNumber, StudyID, 12, ValeurMinBonus_PositionDansRange_Sortie_CadranDefavorable_Compteur);	
			sc.SetChartStudyInputInt(sc.ChartNumber, StudyID, 14, 1);		// TriggerRecalculate de la study principale est remis à 1
		}
	}



	// Mesure de DureeCycle et affichage sur le graphique
	{ 
		// Calcul de DureeCycle avec clock() :
		int ClicksDuPresentCycle = clock() ;
		int DureeCycle = ClicksDuPresentCycle - Clicks_PreviousCycle ;  
		Clicks_PreviousCycle = ClicksDuPresentCycle ;
		
		// Création d'instance de TexteBasCentre_UTAMAOA et appel de sc.UseTool
		SCString Texte ;
		Texte.Format("DureeCycle : %04d ms | Variations : %d/%d", DureeCycle/10*10, Compteur_Variations, Variations) ;  
		TexteBasCentre_UTAMAOA objetTexte(Texte, 8, RGB(0,127,255), sc.GraphRegion, 1000001);   // 1000001 est le Line_Number spécifique pur DrawTempsDeCalculs
		sc.UseTool(objetTexte); 
	}



	// pour les backtests :
	{	sc.UpdateAlways = 1 ;  }
	
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_TestEcritureFichierCSV(SCStudyInterfaceRef sc) 
{
	
	// Inputs /////////////////////////////////////////////////////////////////////////////////////////////
	SCInputRef NomFichier = sc.Input[0];

	// Références /////////////////////////////////////////////////////////////////////////////////////////////
	SCString& NameLogFile = sc.GetPersistentSCString(1);
	

	// Bloc sc.SetDefaults /////////////////////////////////////////////////////////////////////////////////////////////
	if (sc.SetDefaults)
	{
		sc.GraphName = "scsf_TestEcritureFichierCSV" ;   
		sc.GraphRegion = 0 ;  
		sc.ValueFormat = 0 ;
		sc.AutoLoop = 0 ;  
		sc.DisplayStudyName = 1;
		sc.DisplayStudyInputValues = 1;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 0 ;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		
		// Inputs /////////////////////////////////////////////////////////////////////////////////////////////
		NomFichier.Name = "Nom du fichier de log";
		NomFichier.SetString("n.d.");
		
	
		return ;	
	} // fin sc.Defaults
	
	

	if ( sc.UpdateStartIndex == 0 )
	{
		// Initialisations
		sc.GraphRegion = 0 ;  		
		
		// Génération du nom du fichier de log
		SCString TemporarySCString = NomFichier.GetString() ;
		NameLogFile.Format("C:\\SierraChart\\Data\\Optimisations\\%s", TemporarySCString.GetChars() ) ;
		NameLogFile.AppendFormat(".csv") ;
		

		// Création des noms de colonne s'il s'agit de la première ouverture du fichier
		std::string string_NomFichierLog = NameLogFile.GetChars(); // conversion SCString en string
		// if ( ! FileExists(string_NomFichierLog) )  // le fichier n'existe pas
		{ 
			// Déclaration du flux d'écriture
			std::ofstream myStream ;
			myStream.open(string_NomFichierLog, std::ofstream::out | std::ofstream::app) ;       
			if ( myStream.is_open() )
			{	 
				for ( int i = 1 ; i < 120000 ; i++ )
				{
					// en-têtes de colonnes du fichier .csv
					myStream << "Positions longues et courtes"<< ";"			<< ";" 							<< ";" 				<< ";"			<< ";" << "Positions longues"<< ";"								<< ";" 							<< ";" 				<< ";"			<< ";" << "Positions courtes"<< ";"								<< ";" 							<< ";" 				<< ";"			<< ";" << "PeriodesEMA" << ";" 		<< ";" << "Seuils" <<"\n" ; 
					myStream << "Nb Positions" << ";" << "Gain Moyen (pt)" << ";" << "Gain Net (pts)" << ";" << "PF" << ";" << "IC" << ";" 	<< "Nb Positions" << ";" << "Gain Moyen (pt)" << ";" << "Gain Net (pts)" << ";" << "PF" << ";" << "IC" << ";" << "Nb Positions" << ";" << "Gain Moyen (pt)" << ";" << "Gain Net (pts)" << ";" << "PF" << ";" << "IC" << ";" << "Spread" << ";" << "Ecart" << ";" << "RSI" << ";" << "Ecart Abs" << ";" << "Exposant" << ";"  << "Diviseur RSI Extr" << ";" << "VMaxB Entr Fav" << ";" << "VMinB Entr Defav" << ";" << "VMaxB Sort Fav" << ";" << "VMinB Sort Defav" << ";" << "Glisse" << "\n" ; 
				}
			}  
			else { SCString message ;  message.Format("Ouverture du fichier a la creation : erreur") ;  sc.AddMessageToLog(message, 1) ;  }
		}

		
	}  // fin sc.UpdateStartIndex== 0





}


