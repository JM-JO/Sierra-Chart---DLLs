/* 
v1.0 (2019/2/19) : l'indicateur "Sigmax512x256x128x64x32x16" retourne les contributions à Sigma de x16,x32,x64,x128,x256, x512 à chaque cloture de bougie pour un NiveauCible qui serait le close de la bougie. Il est identique à la study "Sigmax512...x16 v1.1 (2000t)" de PRT, et de plus il s'adapte automatiquement au X-ticks.
v1.1 (2019/3/3) : améliorations du code : manual looping.
v1.2 (2019/3/4) : le SpreadIndex est récupéré automatiquement sur le graphe 1-tick.
v1.3 (2019/3/4) : le TPNu est récupéré automatiquement sur le graphe 200-ticks.
v2.0 (2019/3/4) : création Sigmax8x4x2x1x05x025 avec les mêmes formules que Sigmax8...x025 v1.5 de PRT. 
v2.1 (2019/3/5) : TPNu et SpreadIndex sont pris du même graphe que le présent indice. La v2.1 est une version de référence (calculs exhaustifs / lent), avant les bidouilles pour accélérer.
v2.2 (2019/3/6) : accélération du code : le MouvementVerticalSommeNormeUp (resp. Down) est calculé moins fréquemment s'il est faible, et moins fréquemment si TPNu est élevé. 
						Période = max (1, ceil ( ( -MouvementVerticalSommeNormeUp*5 +1 ) *TPNu ) ). Ainsi pour Mvt = -0.4, Periode = 3*TPNu. Pour Mvt >= 0.2, la période est de 1.  Résultat : on descend de 1269ms à 236ms.
v2.3 (2019/3/6) : améliorations diverses : background, lignes horizontales, popups.
v2.4 (2019/3/6) : accélération du code avec les fonctions FastHighest et FastLowest (itération utilisée = sqrt(TPTicks*X) où X est le X-uple). On descend de 262 à 113ms.
v3.0 (2019/3/6) : création de Sigmax512x256x128x64x32x16x8x4x2x1x05x025 sur le 16-ticks en récupérant les valeurs de Sigmax512x256x128x64x32x16 sur le 2000-ticks.
v3.1 (2019/3/14) : gestion du nb de barres insuffisantes (si un indicateur retourne 0.001 alors une study appelante saura que cet indicateur n'est pas utilisable).
v3.2 (2019/3/14) : gestion du nb de barres insuffisantes (plutôt que de retourner 0.01, on utilise le SCSubgraphRef "Background" qui contient déjà un booléen). Background==1 ssi données insuffisantes.
v3.3 (2019/3/22) : on peut choisir entre une récupération automatique du TPNu ou le mettre manuellement. Compatible v1941.
v3.4 (2019/3/22) : on trace à nouveau les MouvementVerticalUp et MouvementVerticalDown. On peut les mettre invisible/visible en switchant les commentaires. L'input EnvergureMoyenne4000Ticks de la study scsf_Sigmax512x256x128x64x32x16 sur le 2000 ticks est récupéré dans la study Sigma du 16 ticks.
v3.5 (2019/12/9) : simplification du code. Suppression de TPTicksInput. Augmentation de la fréquence de calcul pour Sigma<20%.
v3.6 (2019/12/9) : introduction de NewSigma. Augmentation de l'échantillonnage de calcul de Sigma.
v3.7 (2020/1/7) : pour scsf_Sigma_x512x256x128x64x32x16x8x4x2x1x05x025, choix d'une récupération auto ou manuelle de EnvergureMoyenne4000Ticks avec EnvergureMoyenne4000Ticks_ChoixMode via la study "TR 4000-ticks moyen (seance precedente, via chart 2000-ticks)".
v3.8 (2020/1/10) : la variable EnvergureMoyenne4000Ticks (qui sera appelée par AMR) est reportée dans un SCFloatArrayRef de scsf_Sigma_x512x256x128x64x32x16x8x4x2x1x05x025.
v3.9 (2020/1/13) : pour scsf_Sigma_x512x256x128x64x32x16, choix d'une récupération auto ou manuelle de EnvergureMoyenne4000Ticks avec EnvergureMoyenne4000Ticks_ChoixMode via la study "TR 4000-ticks moyen (seance precedente, via chart 2000-ticks)".
____(2020/3/2) : modification des lignes horizonatles correspondent à NewRatioSigma=180% pour le BMoy correspondant (suite à modification de NewRatioSigma).
v4.0 (2020/3/16) : la valeur de EnvergureMoyenne4000Ticks à IndexOfLastVisibleBar est affichée sur le graphe.    !!! v4.0 fait bugger AMR (les zones grisées des multiples de TP sont rectangulaires au lieu d'être courbes)
v4.1 (2020/3/16) : suppression de l'affichage de EnvergureMoyenne4000Ticks via TexteBasCentre_UTAMAOA. Pour contourner ce bug, EnvergureMoyenne4000Ticks est affiché par la study AMR.
v4.2 (2020/3/19) : Toggle Manuel/Auto pour EnvergureMoyenne4000Ticks_ChoixMode de Sigma via raccourci clavier "M".
v4.3 (2020/3/20) : suppression de scsf_Sigma_x8x4x2x1x05x025, et de tout ce qui a trait à SpreadIndex pour le calcul de Sigma.
v4.4 (2020/3/20) : l'EnvergureMoyenne4000Ticks a 3 modes de fonctionnement pour Sigma : Manuel, MoyenneSeancePrecedente, VIX/1.35.
v4.5 (2020/3/20) : l'EnvergureMoyenne4000Ticks a 3 modes de fonctionnement pour scsf_Sigma_x512x256x128x64x32x16 : Manuel, MoyenneSeancePrecedente, VIX/1.35.
v4.6 (2020/3/21) : suppression de l'input Bouton_NewSigma (appelé par F4, qui gérait la bascule entre Sigma et NewSigma) => on ne garde que NewSigma.
v4.7 (2020/6/3) : l'EnvergureMoyenne4000Ticks a un 4è mode de fonctionnement pour Sigma : f(SpreadIndex).
v4.8 (2020/6/3) : l'EnvergureMoyenne4000Ticks a un 4è mode de fonctionnement pour scsf_Sigma_x512x256x128x64x32x16 : f(SpreadIndex).
v4.9 (2020/6/3) : la study scsf_Sigma_x512x256x128x64x32x16 récupère le mode utilisé par la study Sigma.
v5.0 (2020/06/04) : coder les variables des différents modes de Sigma sous forme d'Input (Range Manuel, Diviseur VIX).
v5.1 (2020/09/16) : ajout de "[32] TR 4000-ticks moyen (seance glissante de 24h, via chart 2000-ticks)" comme proxy de EnvergureMoyenne4000t.
v5.2 (2020/09/16) : ajout d'une synchro des modes entre Sigma et Sigma_x512x256x128x64x32x16 (l'ajout est fait dans le bloc "if Startupdateindex == 0" de Sigma).



Améliorations à apporter : 
	- 
	
	



SOMMAIRE DES STRUCTURES ET FONCTIONS :
LigneHorizontale
TexteSpecial
DrawLineAndSpecialText
DrawLine
Highest
Lowest
FastHighest
FastLowest
TexteBasCentre_UTAMAOA
scsf_Sigma_x512x256x128x64x32x16
scsf_Sigma_x512x256x128x64x32x16x8x4x2x1x05x025


 */

#include "sierrachart.h"

SCDLLName("Sigma")


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct LigneHorizontale : s_UseTool  // LigneHorizontale crée un objet qui contient mes paramètres choisis pour une ligne horizontale  (hérite de toutes les données public et protected de s_UseTool)
	{
		LigneHorizontale(float niveau, int epaisseur, COLORREF maCouleur, int Graphregion) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
		{
			Clear(); 
			DrawingType = DRAWING_HORIZONTALLINE;
			LineStyle = LINESTYLE_SOLID;
			AddMethod = UTAM_ADD_ALWAYS;	
			BeginValue = niveau;
			LineWidth = epaisseur;
			Color = maCouleur; 
			Region = Graphregion ;   // 0 par défaut
		} 
	};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct TexteSpecial : s_UseTool // TexteSpecial crée du texte avec le SCString SpecialText  (hérite de toutes les données public et protected de s_UseTool)
{											
	TexteSpecial(float niveau, SCString SpecialText, int epaisseur, COLORREF maCouleur, int Graphregion) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
	{
		Clear(); 
		AddMethod = UTAM_ADD_ALWAYS;			
		DrawingType = /* DRAWING_STATIONARY_TEXT ;  */DRAWING_TEXT;
		BeginDateTime = 75; //150;
		FontBold = 1;
		Color = RGB(220, 220, 220); //background gris du chart
		TextAlignment = DT_VCENTER | DT_CENTER ;   // options disponibles : DT_TOP, DT_VCENTER, DT_BOTTOM, DT_CENTER, DT_LEFT, DT_RIGHT
		BeginValue = niveau;
		FontSize = epaisseur+6;
		FontBackColor = maCouleur;
		Region = Graphregion ;   // 0 par défaut		
		//Formatage du texte
		Text = SpecialText;		
	} 
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawLineAndSpecialText (float monNiveau, int monEpaisseur, SCString SpecialText, COLORREF maCouleur, s_sc* ptrsc, int Graphregion)                // crée un texte indépendant du niveau tracé
{
	//Création d'instance de LigneHorizontale et appel de sc.UseTool
	LigneHorizontale objetLigneHorizontale(monNiveau, monEpaisseur, maCouleur, Graphregion);
	ptrsc->UseTool(objetLigneHorizontale); // c'est sc.UseTool qui est appelé
	//Création d'instances de TexteSpecial et appel de sc.UseTool
	TexteSpecial objetTexteSpecial(monNiveau, SpecialText, monEpaisseur, maCouleur, Graphregion);
	ptrsc->UseTool(objetTexteSpecial); 
}	

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
void DrawLine (float monNiveau, int monEpaisseur, COLORREF maCouleur, s_sc* ptrsc, int Graphregion)
{
	//Création d'instance de LigneHorizontale et appel de sc.UseTool
	LigneHorizontale objetLigneHorizontale(monNiveau, monEpaisseur, maCouleur, Graphregion);
	ptrsc->UseTool(objetLigneHorizontale); // c'est sc.UseTool qui est appelé
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float Highest (int IndexDebut, int NbChandeliersEnArriere, s_sc* ptrsc)  
{
	// mettre erreur dans Log si NbChandeliers <1 // note : en l'état si NbChandeliers <1, Highest retourne -FLT_MAX
	float highest = -FLT_MAX  ;
	for (int Index = IndexDebut ; Index > IndexDebut - NbChandeliersEnArriere ; Index--)    // Highest et Lowest sont construits de la même manière que les fonctions du même nom de PRT
	{
		if ( ptrsc->BaseData[SC_HIGH][Index] > highest)  highest = ptrsc->BaseData[SC_HIGH][Index] ;
	}
	return highest ;
// note : j'ai gagné 11ms (97 vs 86ms) en remplaçant la variable SC_PARAM par la constante SC_HIGH
// note : j'ai gagné 15ms (86 vs 71ms) en remplaçant la variable SC_PARAM par la constante SC_LOW dans la fonction Lowest	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float Lowest (int IndexDebut, int NbChandeliersEnArriere, s_sc* ptrsc)
{
	// mettre erreur dans Log si NbChandeliers <1 // note : en l'état si NbChandeliers <1, Highest retourne FLT_MAX
	float lowest = FLT_MAX  ;
	for (int Index = IndexDebut ; Index > IndexDebut - NbChandeliersEnArriere ; Index--)
	{
		if ( ptrsc->BaseData[SC_LOW][Index] < lowest)  lowest = ptrsc->BaseData[SC_LOW][Index] ;
	}
	return lowest ;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float FastHighest (int IndexDebut, int NbChandeliersEnArriere, float iteration, s_sc* ptrsc)  
{
	// mettre erreur dans Log si NbChandeliers <1 // note : en l'état si NbChandeliers <1, Highest retourne -FLT_MAX
	float iter = max(1.0,iteration) ;
	float highest = -FLT_MAX  ;
	for (float Index = IndexDebut ; Index > IndexDebut - NbChandeliersEnArriere ; Index -= iter)    // Highest et Lowest sont construits de la même manière que les fonctions du même nom de PRT
	{
		if ( ptrsc->BaseData[SC_HIGH][Index] > highest)  highest = ptrsc->BaseData[SC_HIGH][Index] ;
	}
	return highest ;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float FastLowest (int IndexDebut, int NbChandeliersEnArriere, float iteration, s_sc* ptrsc)
{
	// mettre erreur dans Log si NbChandeliers <1 // note : en l'état si NbChandeliers <1, Highest retourne FLT_MAX
	float iter = max(1.0,iteration) ;
	float lowest = FLT_MAX  ;
	for (float Index = IndexDebut ; Index > IndexDebut - NbChandeliersEnArriere ; Index -= iter)
	{
		if ( ptrsc->BaseData[SC_LOW][Index] < lowest)  lowest = ptrsc->BaseData[SC_LOW][Index] ;
	}
	return lowest ;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_Sigma_x512x256x128x64x32x16(SCStudyInterfaceRef sc) 		// Study pour le 2000-ticks
{
	SCSubgraphRef Background = sc.Subgraph[0] ;  												// appelé par scsf_Sigmax512x256x128x64x32x16x8x4x2x1x05x025
	SCSubgraphRef Sigmax512x256x128x64x32x16Up = sc.Subgraph[1];         	// appelé par scsf_Sigmax512x256x128x64x32x16x8x4x2x1x05x025
	SCSubgraphRef Sigmax512x256x128x64x32x16Down = sc.Subgraph[2];     	// appelé par scsf_Sigmax512x256x128x64x32x16x8x4x2x1x05x025

	
	SCInputRef EnvergureMoyenne4000Ticks_ChoixMode = sc.Input[0];				// modifié par la study Sigma du 16 ticks			// choix entre les modes : Manuel, MoyenneSeancePrecedente, VIX, f(SI)
	SCInputRef VariableSpecifiqueDuMode_Input = sc.Input[1];								// modifié par la study Sigma du 16 ticks			// input commun à tous les modes (valeur et noms différents pour chaque mode)
	SCInputRef ChartNumber_TPTicks = sc.Input[2];  	
	SCInputRef EnvergureMoyenne4000Ticks_ModeManuel = sc.Input[3];     		// modifié par la study Sigma du 16 ticks			// valeur float utilisé dans le cas du mode manuel	
	SCInputRef DiviseurVIX_ModeVIX = sc.Input[4];     											// modifié par la study Sigma du 16 ticks			// valeur float utilisé dans le cas du mode manuel	
	SCInputRef ChartInputs_TPTicks = sc.Input[12];  
	
	s_sc* const ptrsc = &sc; 
	
	float& xTicks = sc.GetPersistentFloatFast(1);  
	float& fEnvergureMoyenne4000Ticks = sc.GetPersistentFloatFast(2);  
	
	int& TriggerPopupNbBarresVirginite = sc.GetPersistentIntFast(1);  
	int& StudyID_EnvergureMoyenne4000Ticks_SeancePrecedente = sc.GetPersistentIntFast(2);  
	int& StudyID_VIX = sc.GetPersistentIntFast(3);  
	int& StudyID_SI = sc.GetPersistentIntFast(4);  
	int& StudyID_EnvergureMoyenne4000Ticks_SeanceGlissante = sc.GetPersistentIntFast(5);  
	
	
	if (sc.SetDefaults)
	{
		sc.GraphName = "Sigma x512x256x128x64x32x16" ;   // appelé par scsf_Sigmax512x256x128x64x32x16x8x4x2x1x05x025
		sc.AutoLoop = 0 ;  
		sc.CalculationPrecedence = LOW_PREC_LEVEL ;  // le VIX, le SpreadIndex_Affichage et EnvergureMoyenne4000Ticks doivent être calculés précédemment
		sc.DisplayStudyName = 1;
		sc.GlobalDisplayStudySubgraphsNameAndValue = 0 ;
		sc.DisplayStudyInputValues = 1;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		sc.ValueFormat = 0 ;
		
		
		EnvergureMoyenne4000Ticks_ChoixMode.Name = "EnvergureMoyenne4000Ticks : choix du mode";
		EnvergureMoyenne4000Ticks_ChoixMode.SetCustomInputStrings("Manuel;MoyenneSeancePrecedente;VIX;f(SI);MoyenneSeanceGlissante");   
		EnvergureMoyenne4000Ticks_ChoixMode.SetCustomInputIndex(4);
		// ChartNumber_TPTicks.Name = "ChartNumber du TPNu (UT 200-ticks)";
		ChartNumber_TPTicks.SetChartNumber(11);
		EnvergureMoyenne4000Ticks_ModeManuel.SetFloat(10.0);	
		DiviseurVIX_ModeVIX.SetFloat(1.25) ;
		VariableSpecifiqueDuMode_Input.Name = "Le nom sera specifie dans le bloc '' if ( sc.UpdateStartIndex== 0 ) ''";    
		VariableSpecifiqueDuMode_Input.SetFloat( DiviseurVIX_ModeVIX.GetFloat() );		
		
		
		
		Background.Name = "Subgraph background"; 
		Background.DrawStyle = DRAWSTYLE_BACKGROUND; 
		Background.DisplayNameValueInWindowsFlags = false ;
		

		for (int SubgraphIndex = 1; SubgraphIndex <= 2; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].DrawStyle = DRAWSTYLE_LINE ;
			sc.Subgraph[SubgraphIndex].LineStyle = LINESTYLE_SOLID ;
			sc.Subgraph[SubgraphIndex].LineLabel = LL_DISPLAY_VALUE | LL_VALUE_ALIGN_CENTER | LL_VALUE_ALIGN_VALUES_SCALE;  // n'affiche que la valeur
			//sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre
			sc.Subgraph[SubgraphIndex].DrawZeros = true ;
			sc.Subgraph[SubgraphIndex].LineWidth = 2;
		}		
		
		Sigmax512x256x128x64x32x16Up.Name = "Sigma x512x256x128x64x32x16 Up"; 
		Sigmax512x256x128x64x32x16Up.PrimaryColor = RGB(0,200,0);
		
		Sigmax512x256x128x64x32x16Down.Name = "Sigma x512x256x128x64x32x16 Down"; 
		Sigmax512x256x128x64x32x16Down.PrimaryColor = RGB(200,0,0);		
		
		return ;	
	} // fin sc.Defaults
	
	
	
	
	if ( sc.UpdateStartIndex== 0 )
	{
		sc.GraphRegion = 0 ; 
		TriggerPopupNbBarresVirginite = 1 ;
		
		
		// {  SCString message ;   message.Format("On vient de rentrer dans le bloc ''if ( sc.UpdateStartIndex== 0 )'' de Sigma sur 2000t ") ;    sc.AddMessageToLog(message, 0) ; }
		

		// Gestion de VariableSpecifiqueDuMode_Input, EnvergureMoyenne4000Ticks_ModeManuel, DiviseurVIX_ModeVIX		// Envoi vers Sigmax512x256x128x64x32x16 d'une modif de range manuel ou de DiviseurVIX 
		int Mode = EnvergureMoyenne4000Ticks_ChoixMode.GetIndex() ;
				if ( Mode == 0 )  { VariableSpecifiqueDuMode_Input.Name = "EnvergureMoyenne4000Ticks Manuel (pts)" ;   EnvergureMoyenne4000Ticks_ModeManuel.SetFloat( VariableSpecifiqueDuMode_Input.GetFloat() )  ; }
		else if ( Mode == 1 )  { VariableSpecifiqueDuMode_Input.Name = "Variable non utilisee" ;  VariableSpecifiqueDuMode_Input.SetFloat(0) ;  }
		else if ( Mode == 2 )  { VariableSpecifiqueDuMode_Input.Name = "Diviseur VIX" ;  DiviseurVIX_ModeVIX.SetFloat( VariableSpecifiqueDuMode_Input.GetFloat() ) ;  }
		else if ( Mode == 3 )  { VariableSpecifiqueDuMode_Input.Name = "Variable non utilisee" ;  VariableSpecifiqueDuMode_Input.SetFloat(0) ;  } 
		else if ( Mode == 4 )  { VariableSpecifiqueDuMode_Input.Name = "Variable non utilisee" ;  VariableSpecifiqueDuMode_Input.SetFloat(0) ;  } 
		
		
		// détermination du XTicks // pas besoin d'être nécessairement en 2000-ticks, la study auto-adapte NbBarresVirginite
		n_ACSIL::s_BarPeriod BarPeriod;
		sc.GetBarPeriodParameters(BarPeriod);
		if ( BarPeriod.IntradayChartBarPeriodType == IBPT_NUM_TRADES_PER_BAR )   // on vérifie que le graphe est bien en XTicks
		{	xTicks = BarPeriod.IntradayChartBarPeriodParameter1; 	}  
		else // message d'erreur dans le log
		{  SCString message ;   message.Format("!! ERREUR !! Le chart #%d n'est pas en ticks, impossible de tracer l'indicateur", sc.ChartNumber) ;    sc.AddMessageToLog(message,1) ; }

		// Récupération de la studyID pour le SpreadIndex
		StudyID_SI = sc.GetStudyIDByName(sc.ChartNumber, "SpreadIndexAffichage", 0) ;
		if  (StudyID_SI == 0 )
		{ SCString message ;  message.Format("!! ERREUR !! Le chart ne contient pas de calcul de SpreadIndexAffichage") ; sc.AddMessageToLog(message,1) ; }
		
		// Récupération de la StudyID pour le TPTicks
		int StudyID = sc.GetStudyIDByName(ChartNumber_TPTicks.GetChartNumber(), "Calcul TPNu", 0) ; 
		// on crée ChartStudySubgraphValues pour le TPTicks
		if  (StudyID == 0 )
		{ SCString message ;  message.Format("!! ERREUR !! Le chart source (#%d) ne contient pas de calcul du TPTicks (StudyID==0)", ChartNumber_TPTicks.GetChartNumber()) ; sc.AddMessageToLog(message,1) ; }
		else	{	ChartInputs_TPTicks.SetChartStudySubgraphValues(ChartNumber_TPTicks.GetChartNumber(),StudyID,1);	}			

		// Récupération de la StudyID pour les ChartInputs_EnvergureMoyenne4000Ticks_SeancePrecedente
		StudyID_EnvergureMoyenne4000Ticks_SeancePrecedente = sc.GetStudyIDByName(sc.ChartNumber, "[31] TR 4000-ticks moyen (seance precedente, via chart 2000-ticks)", 0) ; 
		// on crée les ChartStudySubgraphValues
		if  (StudyID_EnvergureMoyenne4000Ticks_SeancePrecedente == 0 )
		{ SCString message ;  message.Format("!! ERREUR !! Le chart #%d ne contient pas de calcul de ''[31] TR 4000-ticks moyen (seance precedente, via chart 2000-ticks)'' ", sc.ChartNumber) ;  sc.AddMessageToLog(message,1) ; }

		// Récupération de la StudyID pour le VIX
		StudyID_VIX = max ( sc.GetStudyIDByName(sc.ChartNumber, "Symbol: $vix", 0), sc.GetStudyIDByName(sc.ChartNumber, "Symbol: $VIX", 0) ) ;
		if ( StudyID_VIX == 0 )   { SCString message ;  message.Format("!! ERREUR !! La study ''Symbol: $VIX'' n'a pas ete trouvee sur le chart #%d", sc.ChartNumber) ;  sc.AddMessageToLog(message, 1) ;  }

		// Récupération de la StudyID pour les ChartInputs_EnvergureMoyenne4000Ticks_SeanceGlissante
		StudyID_EnvergureMoyenne4000Ticks_SeanceGlissante = sc.GetStudyIDByName(sc.ChartNumber, "[32] TR 4000-ticks moyen (1 seance glissante de 24h, via chart 2000-ticks)", 0) ; 
		// on crée les ChartStudySubgraphValues
		if  (StudyID_EnvergureMoyenne4000Ticks_SeanceGlissante == 0 )
		{ SCString message ;  message.Format("!! ERREUR !! Le chart #%d ne contient pas de calcul de ''[32] TR 4000-ticks moyen (1 seance glissante de 24h, via chart 2000-ticks)'' ", sc.ChartNumber) ;  sc.AddMessageToLog(message,1) ; }

		
		// Ajout de lignes horizontales
		DrawLine(120, 2, RGB(0, 0, 255), ptrsc, sc.GraphRegion);
		DrawLine(110, 1, RGB(0, 0, 255), ptrsc, sc.GraphRegion);
		DrawLine(100, 2, RGB(0, 0, 255), ptrsc, sc.GraphRegion);
		DrawLine(90, 1, RGB(0, 0, 255), ptrsc, sc.GraphRegion);
		DrawLine(80, 2, RGB(0, 0, 255), ptrsc, sc.GraphRegion);
		DrawLine(70, 1, RGB(0, 0, 255), ptrsc, sc.GraphRegion);
		DrawLine(60, 2, RGB(0, 0, 255), ptrsc, sc.GraphRegion);
		DrawLine(50, 1, RGB(0, 0, 255), ptrsc, sc.GraphRegion);
		DrawLine(40, 2, RGB(0, 0, 255), ptrsc, sc.GraphRegion);
		DrawLine(30, 1, RGB(0, 0, 255), ptrsc, sc.GraphRegion);
		DrawLine(20, 2, RGB(0, 0, 255), ptrsc, sc.GraphRegion);
		DrawLine(10, 1, RGB(0, 0, 255), ptrsc, sc.GraphRegion);
		DrawLine(0,  3, RGB(0, 0, 255), ptrsc, sc.GraphRegion);
		DrawLine(-10, 1, RGB(0, 0, 255), ptrsc, sc.GraphRegion);
		DrawLine(-20, 2, RGB(0, 0, 255), ptrsc, sc.GraphRegion);
		
		
		// Recalcul du chart #6 en 16 ticks (en général ça a lieu, mais pas toujours)
		sc.RecalculateChart(6) ; 
	}


	
	// on récupère les valeurs du TPTicks dans le Subgraph adéquat
	SCFloatArray ArrayTPTicks;
	sc.GetStudyArrayFromChartUsingID(ChartInputs_TPTicks.GetChartStudySubgraphValues(), ArrayTPTicks);

	// on récupère le VIX
	SCFloatArray VIX_Array;
	if ( EnvergureMoyenne4000Ticks_ChoixMode.GetIndex() == 2 )  sc.GetStudyArrayUsingID(StudyID_VIX, 3, VIX_Array) ;    // 2 = mode VIX

	// on récupère les valeurs du SpreadIndex dans le Subgraph[0] de "SpreadIndexAffichage" sur le présent Chart 
	SCFloatArray ArraySpreadIndex;
	if ( EnvergureMoyenne4000Ticks_ChoixMode.GetIndex() == 3 )  sc.GetStudyArrayUsingID(StudyID_SI, 0, ArraySpreadIndex) ;		// mode = f(SI)

	
	
	for (int index = sc.UpdateStartIndex; index < sc.ArraySize; index++)                 // début AutoLoop
	{
		// on récupère l'index du nouveau chandelier sur le chart d'où provient le calcul (UT 200-tick) du TPTicks, puis sa valeur
		int TPTicksIndex = sc.GetContainingIndexForDateTimeIndex(ChartNumber_TPTicks.GetChartNumber(), index); 
		float TPTicks = ArrayTPTicks[TPTicksIndex] ;		
		
		
		// Modification de fEnvergureMoyenne4000Ticks lors d'un changement de séance    
		if ( EnvergureMoyenne4000Ticks_ChoixMode.GetIndex() == 0 )    // 0 = mode manuel
		{
			fEnvergureMoyenne4000Ticks = EnvergureMoyenne4000Ticks_ModeManuel.GetFloat() ;
		}
		else if ( EnvergureMoyenne4000Ticks_ChoixMode.GetIndex() == 1 )    // 1 = mode MoyenneSeancePrecedente
		{
			if  (  ( sc.BaseDateTimeIn.TimeAt(index) >= 64800  and  sc.BaseDateTimeIn.TimeAt(index-1) < 64800   )   or   index == 0  )    // 64800 est le nb de secondes à 18h   
			{
				// on récupère les valeurs de EnvergureMoyenne4000Ticks du graphe 2000 ticks
				SCFloatArray ExternalArray_EnvergureMoyenne4000Ticks ;
				sc.GetStudyArrayUsingID(StudyID_EnvergureMoyenne4000Ticks_SeancePrecedente, 2, ExternalArray_EnvergureMoyenne4000Ticks) ;

				// on récupère le fEnvergureMoyenne4000Ticks
				fEnvergureMoyenne4000Ticks = ExternalArray_EnvergureMoyenne4000Ticks[index] ;
				if ( fEnvergureMoyenne4000Ticks == 0 )  { fEnvergureMoyenne4000Ticks = 999 ;  }  // la study bugge sévère si fEnvergureMoyenne4000Ticks vaut 0
			}
		}
		else if ( EnvergureMoyenne4000Ticks_ChoixMode.GetIndex() == 2 )    // 2 = mode VIX
		{
			fEnvergureMoyenne4000Ticks = VIX_Array[index] / DiviseurVIX_ModeVIX.GetFloat() ;
		}
		else if ( EnvergureMoyenne4000Ticks_ChoixMode.GetIndex() == 3 )    // 3 = mode f(SI)
		{
			float SpreadIndex = ArraySpreadIndex[index] ;		
			if ( SpreadIndex == 0 ) fEnvergureMoyenne4000Ticks = 999 ;		// la study bugge sévère si fEnvergureMoyenne4000Ticks vaut 0
			else fEnvergureMoyenne4000Ticks = -0.064*SpreadIndex*SpreadIndex+27.01*SpreadIndex-15.91 ; 		// C'est le modèle 2 (idem pour 7), basé sur l'ensemble des ranges 4000t plottés vs les SpreadIndex moyen sur 4000t		// Conférer l'onglet "UT4000t, SI v Range, 2020-1à3" dans le fichier"VIX et Envergure4000TicksSeancePrecedente et SpreadIndex.xlsx"
		}
		else if ( EnvergureMoyenne4000Ticks_ChoixMode.GetIndex() == 4 )    // 4 = mode MoyenneSeanceGlissante
		{
			// on récupère les valeurs de EnvergureMoyenne4000Ticks du graphe 2000 ticks
			SCFloatArray ExternalArray_EnvergureMoyenne4000Ticks ;
			sc.GetStudyArrayUsingID(StudyID_EnvergureMoyenne4000Ticks_SeanceGlissante, 2, ExternalArray_EnvergureMoyenne4000Ticks) ;

			// on récupère le fEnvergureMoyenne4000Ticks
			fEnvergureMoyenne4000Ticks = ExternalArray_EnvergureMoyenne4000Ticks[index] ;
			if ( fEnvergureMoyenne4000Ticks == 0 )  { fEnvergureMoyenne4000Ticks = 999 ;  }  // la study bugge sévère si fEnvergureMoyenne4000Ticks vaut 0
		}
		else  // nd = mode manuel avec Envergure = 10
		{
			fEnvergureMoyenne4000Ticks = 998 ;
		}
		
		
		
		// Calcul de EnvergureMoyenneFull
		float NbBarresVirginite = TPTicks*1000/xTicks ;  // TP = 1pt = 4ticks <=> 4000 ticks = 2 barres de 2000 ticks
		float EnvergureMoyenneFull = fEnvergureMoyenne4000Ticks * sqrt(TPTicks/4) ;
	
		
		if ( TPTicks != 0  and  index > 512*NbBarresVirginite )  // TPTicks==0 car le "Days to load" du graphe 200-ticks est inférieur à celui du 2000 ticks
		{
			Background[index] = 0 ;  
			
			// Calcul des mouvements verticaux HAUSSIERS : //////////////////////////////

			float MouvementVertical512uple = sc.High[index] - Lowest(index, round(512*NbBarresVirginite+1), ptrsc) ;
			float MouvementVertical256uple = sc.High[index] - Lowest(index, round(256*NbBarresVirginite+1), ptrsc) ;
			float MouvementVertical128uple = sc.High[index] - Lowest(index, round(128*NbBarresVirginite+1), ptrsc) ;
			float MouvementVertical64uple = sc.High[index] - Lowest(index, round(64*NbBarresVirginite+1), ptrsc) ;
			float MouvementVertical32uple = sc.High[index] - Lowest(index, round(32*NbBarresVirginite+1), ptrsc) ;
			float MouvementVertical16uple = sc.High[index] - Lowest(index, round(16*NbBarresVirginite+1), ptrsc) ;

			// Calcul des mouvements verticaux normés et leur contribution :

			// - 512-uple :
			float MouvementVertical512upleNorme = MouvementVertical512uple/(EnvergureMoyenneFull*16*sqrt(2)) ;
			float MouvementVertical512upleContribution = (MouvementVertical512upleNorme-1)/(64*sqrt(2)) ;

			// - 256-uple :
			float MouvementVertical256upleNorme = MouvementVertical256uple/(EnvergureMoyenneFull*16) ;
			float MouvementVertical256upleContribution = (MouvementVertical256upleNorme-1)/(64) ;

			// - 128-uple :
			float MouvementVertical128upleNorme = MouvementVertical128uple/(EnvergureMoyenneFull*8*sqrt(2)) ;
			float MouvementVertical128upleContribution = (MouvementVertical128upleNorme-1)/(32*sqrt(2)) ;

			// - 64-uple :
			float MouvementVertical64upleNorme = MouvementVertical64uple/(EnvergureMoyenneFull*8) ;
			float MouvementVertical64upleContribution = (MouvementVertical64upleNorme-1)/32 ;

			// - 32-uple :
			float MouvementVertical32upleNorme = MouvementVertical32uple/(EnvergureMoyenneFull*4*sqrt(2)) ;
			float MouvementVertical32upleContribution = (MouvementVertical32upleNorme-1)/(16*sqrt(2)) ;

			// - 16-uple :
			float MouvementVertical16upleNorme = MouvementVertical16uple/(EnvergureMoyenneFull*4) ;
			float MouvementVertical16upleContribution = (MouvementVertical16upleNorme-1)/16 ;

			// - Sigmax512x256x128x64x32x16 :
			Sigmax512x256x128x64x32x16Up[index] = round((MouvementVertical512upleContribution+MouvementVertical256upleContribution+MouvementVertical128upleContribution+MouvementVertical64upleContribution+MouvementVertical32upleContribution+MouvementVertical16upleContribution)*100) ;


			// Calcul des mouvements verticaux BAISSIERS : ///////////////////////////////////

			MouvementVertical512uple = Highest(index, round(512*NbBarresVirginite+1), ptrsc)  - sc.Low[index] ;
			MouvementVertical256uple = Highest(index, round(256*NbBarresVirginite+1), ptrsc)  - sc.Low[index] ;
			MouvementVertical128uple = Highest(index, round(128*NbBarresVirginite+1), ptrsc)  - sc.Low[index] ;
			MouvementVertical64uple = Highest(index, round(64*NbBarresVirginite+1), ptrsc)  - sc.Low[index] ;
			MouvementVertical32uple = Highest(index, round(32*NbBarresVirginite+1), ptrsc)  - sc.Low[index] ;
			MouvementVertical16uple = Highest(index, round(16*NbBarresVirginite+1), ptrsc)  - sc.Low[index] ;

			// Calcul des mouvements verticaux normés et leur contribution :

			// - 512-uple :
			MouvementVertical512upleNorme = MouvementVertical512uple/(EnvergureMoyenneFull*16*sqrt(2)) ;
			MouvementVertical512upleContribution = (MouvementVertical512upleNorme-1)/(64*sqrt(2)) ;

			// - 256-uple :
			MouvementVertical256upleNorme = MouvementVertical256uple/(EnvergureMoyenneFull*16) ;
			MouvementVertical256upleContribution = (MouvementVertical256upleNorme-1)/(64) ;

			// - 128-uple :
			MouvementVertical128upleNorme = MouvementVertical128uple/(EnvergureMoyenneFull*8*sqrt(2)) ;
			MouvementVertical128upleContribution = (MouvementVertical128upleNorme-1)/(32*sqrt(2)) ;

			// - 64-uple :
			MouvementVertical64upleNorme = MouvementVertical64uple/(EnvergureMoyenneFull*8) ;
			MouvementVertical64upleContribution = (MouvementVertical64upleNorme-1)/32 ;

			// - 32-uple :
			MouvementVertical32upleNorme = MouvementVertical32uple/(EnvergureMoyenneFull*4*sqrt(2)) ;
			MouvementVertical32upleContribution = (MouvementVertical32upleNorme-1)/(16*sqrt(2)) ;

			// - 16-uple :
			MouvementVertical16upleNorme = MouvementVertical16uple/(EnvergureMoyenneFull*4) ;
			MouvementVertical16upleContribution = (MouvementVertical16upleNorme-1)/16 ;

			// - Sigmax512x256x128x64x32x16 :
			Sigmax512x256x128x64x32x16Down[index] = round((MouvementVertical512upleContribution+MouvementVertical256upleContribution+MouvementVertical128upleContribution+MouvementVertical64upleContribution+MouvementVertical32upleContribution+MouvementVertical16upleContribution)*100) ;

		}
		
		else  // Manque de données pour tracer l'indicateur
		{	
			Sigmax512x256x128x64x32x16Up[index] = 0 ;		
			Sigmax512x256x128x64x32x16Down[index] = 0 ;	
			
			Background[index] = 1 ;  // background a lieu
			
			if ( sc.ArraySize <= 512*NbBarresVirginite )  
			{ Background.DataColor[index] = RGB(100,100,100) ; }	
			else // TPTicks==0
			{	Background.DataColor[index] = RGB(200,200,200) ;	}
			
			// popup du log
			if ( sc.ArraySize <= 512*NbBarresVirginite and TriggerPopupNbBarresVirginite == 1 )
			{  TriggerPopupNbBarresVirginite=0 ;  SCString message ;   message.Format("!! ERREUR !! PAS ASSEZ DE BARRES dans le chart #%d, IMPOSSIBLE DE TRACER L'INDICATEUR (sc.ArraySize <= 512*NbBarresVirginite)", sc.ChartNumber) ;    sc.AddMessageToLog(message,1) ; }	
		}
	
	
	} // fin autoloop
	
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_Sigma(SCStudyInterfaceRef sc)     // Study Sigma du 16-ticks
{
	
	SCSubgraphRef Background = sc.Subgraph[0] ;  
	SCSubgraphRef MaxMouvementVerticalUpOuDown = sc.Subgraph[1] ;  								// Sigma
	SCFloatArrayRef MouvementVerticalSommeNormeUp = sc.Subgraph[1].Arrays[0] ;  			// appelé par scsf_AnnulationMiniRebond
	SCFloatArrayRef MouvementVerticalSommeNormeDown = sc.Subgraph[1].Arrays[1] ;  		// appelé par scsf_AnnulationMiniRebond
	SCFloatArrayRef ProchainIndexCalculMouvementUp =  sc.Subgraph[1].Arrays[2] ;
	SCFloatArrayRef ProchainIndexCalculMouvementDown =  sc.Subgraph[1].Arrays[3] ;
	SCFloatArrayRef ArrayEnvergureMoyenne4000Ticks =  sc.Subgraph[1].Arrays[4] ;				// appelé par scsf_AnnulationMiniRebond            // la différence entre ArrayEnvergureMoyenne4000Ticks et fEnvergureMoyenne4000Ticks est que le premier contient toutes les valeurs pour toutes les séances, là où le second (qui est une variable persistante) ne contient que la valeur du dernier index.
	SCSubgraphRef MouvementVerticalUp = sc.Subgraph[2];  
	SCSubgraphRef MouvementVerticalDown = sc.Subgraph[3];  
	
	
	
	SCInputRef EnvergureMoyenne4000Ticks_ChoixMode = sc.Input[0];       					// choix entre les modes : Manuel, MoyenneSeancePrecedente, VIX, f(SI)
	SCInputRef VariableSpecifiqueDuMode_Input = sc.Input[1];								// input commun à tous les modes (valeur et noms différents pour chaque mode)
	SCInputRef ChartNumber_Sigmax512x256x128x64x32x16 = sc.Input[2]; 
	SCInputRef EnvergureMoyenne4000Ticks_ModeManuel = sc.Input[3];			// valeur float utilisé dans le cas du mode manuel	
	SCInputRef DiviseurVIX_ModeVIX = sc.Input[4];													// valeur float utilisé dans le cas du mode VIX	


	SCInputRef ChartInputs_Sigmax512x256x128x64x32x16Background = sc.Input[13];  
	SCInputRef ChartInputs_Sigmax512x256x128x64x32x16Up = sc.Input[14];  
	SCInputRef ChartInputs_Sigmax512x256x128x64x32x16Down = sc.Input[15];  
	SCInputRef ChartInputs_EnvergureMoyenne4000Ticks_SeancePrecedente = sc.Input[16];     
	SCInputRef ChartInputs_EnvergureMoyenne4000Ticks_SeanceGlissante = sc.Input[17];     
	
	
	s_sc* const ptrsc = &sc; 
	
	int& StudyID_SI = sc.GetPersistentIntFast(1);  
	int& StudyID_TPNu = sc.GetPersistentIntFast(2);  
	int& StudyID_VIX = sc.GetPersistentIntFast(3);  
	int& StudyID_Sigma2000Ticks = sc.GetPersistentIntFast(4);  
	
	float& Sigmax512x256x128x64x32x16Background = sc.GetPersistentFloatFast(1);  
	float& Sigmax512x256x128x64x32x16Up = sc.GetPersistentFloatFast(2);  
	float& Sigmax512x256x128x64x32x16Down = sc.GetPersistentFloatFast(3);  
	float& fEnvergureMoyenne4000Ticks = sc.GetPersistentFloatFast(4);  
	
	
	if (sc.SetDefaults)
	{
		sc.GraphName = "Sigma" ;      // appelé par scsf_AnnulationMiniRebond
		sc.GraphRegion = 0 ;  
		sc.ValueFormat = 0 ;
		sc.AutoLoop = 0 ;  
		sc.CalculationPrecedence = LOW_PREC_LEVEL ;  		// le TPNu et le SpreadIndexAffichage doivent être calculés précédemment
		sc.DisplayStudyName = 1;
		sc.DisplayStudyInputValues = 1;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 0 ;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		
		
		EnvergureMoyenne4000Ticks_ChoixMode.Name = "EnvergureMoyenne4000Ticks : choix du mode";
		EnvergureMoyenne4000Ticks_ChoixMode.SetCustomInputStrings("Manuel;MoyenneSeancePrecedente;VIX;f(SI);MoyenneSeanceGlissante");   
		EnvergureMoyenne4000Ticks_ChoixMode.SetCustomInputIndex(4);
		// ChartNumber_Sigmax512x256x128x64x32x16.Name = "ChartNumber du calcul de Sigma x512x256x128x64x32x16";
		ChartNumber_Sigmax512x256x128x64x32x16.SetChartNumber(12);
		EnvergureMoyenne4000Ticks_ModeManuel.SetFloat(19) ;
		DiviseurVIX_ModeVIX.SetFloat(1.25) ;
		VariableSpecifiqueDuMode_Input.Name = "Le nom sera specifie dans le bloc '' if ( sc.UpdateStartIndex== 0 ) ''";    
		VariableSpecifiqueDuMode_Input.SetFloat( DiviseurVIX_ModeVIX.GetFloat() );		
		

		for (int SubgraphIndex = 1; SubgraphIndex <= 3; SubgraphIndex++)
		{
			// sc.Subgraph[SubgraphIndex].LineLabel = LL_DISPLAY_VALUE | LL_VALUE_ALIGN_CENTER | LL_VALUE_ALIGN_VALUES_SCALE;  // n'affiche que la valeur
			sc.Subgraph[SubgraphIndex].LineLabel =  LL_DISPLAY_VALUE | LL_VALUE_ALIGN_VALUES_SCALE ;  
			//sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false ;
			sc.Subgraph[SubgraphIndex].DrawZeros = true ;
		}		
		
		Background.Name = "Background"; 
		Background.DrawStyle = DRAWSTYLE_BACKGROUND; 
		Background.DisplayNameValueInWindowsFlags = false ;
		
		MaxMouvementVerticalUpOuDown.Name = "MaxMouvementVerticalUpOuDown"; 
		MaxMouvementVerticalUpOuDown.DrawStyle = DRAWSTYLE_LINE ;
		MaxMouvementVerticalUpOuDown.LineWidth = 2;
		
		MouvementVerticalUp.Name = "MouvementVerticalUp";
		MouvementVerticalUp.PrimaryColor = RGB(0,200,0);
		MouvementVerticalUp.DrawStyle = DRAWSTYLE_IGNORE ;
		MouvementVerticalUp.LineWidth = 1;
		
		MouvementVerticalDown.Name = "MouvementVerticalDown";
		MouvementVerticalDown.PrimaryColor = RGB(200,0,0);		
		MouvementVerticalDown.DrawStyle = DRAWSTYLE_IGNORE ;
		MouvementVerticalDown.LineWidth = 1;
		
		return ;	
	} // fin sc.Defaults
	
	
	
	// Bouton "EnvergureMoyenne4000Ticks_ChoixMode" appuyé
	if ( sc.MenuEventID == ACS_BUTTON_127  )  		// bouton M	
	{	
		// modif de Mode sur le chart 16 ticks
		int Mode = EnvergureMoyenne4000Ticks_ChoixMode.GetIndex() ;
		if ( Mode == 4 ) Mode = 2 ;  else Mode = 4 ;
		EnvergureMoyenne4000Ticks_ChoixMode.SetCustomInputIndex(Mode) ; 
		
		// synchronisation du chart 2000 ticks : Mode
		sc.SetChartStudyInputInt(ChartNumber_Sigmax512x256x128x64x32x16.GetChartNumber(), StudyID_Sigma2000Ticks, 0, Mode) ;		// int SetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int IntegerValue);
		
		// synchronisation du chart 2000 ticks : range manuel
		float RangeManuel = EnvergureMoyenne4000Ticks_ModeManuel.GetFloat() ;
		sc.SetChartStudyInputFloat(ChartNumber_Sigmax512x256x128x64x32x16.GetChartNumber(), StudyID_Sigma2000Ticks, 3, RangeManuel) ;		// int SetChartStudyInputFloat(int ChartNumber, int StudyID, int InputIndex, double FloatValue);

		// synchronisation du chart 2000 ticks : DiviseurVIX
		float DiviseurVIX = DiviseurVIX_ModeVIX.GetFloat() ;
		sc.SetChartStudyInputFloat(ChartNumber_Sigmax512x256x128x64x32x16.GetChartNumber(), StudyID_Sigma2000Ticks, 4, DiviseurVIX) ;		

		// Gestion de VariableSpecifiqueDuMode_Input
		/* 		if ( Mode == 0 )  { VariableSpecifiqueDuMode_Input.Name = "EnvergureMoyenne4000Ticks Manuel (pts)" ;  VariableSpecifiqueDuMode_Input.SetFloat( RangeManuel ) ; }
		else if ( Mode == 1 )  { VariableSpecifiqueDuMode_Input.Name = "Variable non utilisee" ;  VariableSpecifiqueDuMode_Input.SetFloat(0) ;  }		// Moyenne Séance Précédente
		else if ( Mode == 2 )  { VariableSpecifiqueDuMode_Input.Name = "Diviseur VIX" ;  VariableSpecifiqueDuMode_Input.SetFloat( DiviseurVIX ) ;  }
		else if ( Mode == 3 )  { VariableSpecifiqueDuMode_Input.Name = "Variable non utilisee" ;  VariableSpecifiqueDuMode_Input.SetFloat(0) ;  }		// f(SI)
		else if ( Mode == 4 )  { VariableSpecifiqueDuMode_Input.Name = "Variable non utilisee" ;  VariableSpecifiqueDuMode_Input.SetFloat(0) ;  }		// Moyenne Séance Glissante */
				if ( Mode == 0 )  { VariableSpecifiqueDuMode_Input.Name = "Recalcul en cours" ;  VariableSpecifiqueDuMode_Input.SetFloat( RangeManuel ) ; }
		else if ( Mode == 1 )  { VariableSpecifiqueDuMode_Input.Name = "Recalcul en cours" ;  VariableSpecifiqueDuMode_Input.SetFloat(0) ;  }		// Moyenne Séance Précédente
		else if ( Mode == 2 )  { VariableSpecifiqueDuMode_Input.Name = "Recalcul en cours" ;  VariableSpecifiqueDuMode_Input.SetFloat( DiviseurVIX ) ;  }
		else if ( Mode == 3 )  { VariableSpecifiqueDuMode_Input.Name = "Recalcul en cours" ;  VariableSpecifiqueDuMode_Input.SetFloat(0) ;  }		// f(SI)
		else if ( Mode == 4 )  { VariableSpecifiqueDuMode_Input.Name = "Recalcul en cours" ;  VariableSpecifiqueDuMode_Input.SetFloat(0) ;  }		// Moyenne Séance Glissante
	
	
		
		// synchronisation du chart 2000 ticks : VariableSpecifiqueDuMode_Input
		sc.SetChartStudyInputFloat(ChartNumber_Sigmax512x256x128x64x32x16.GetChartNumber(), StudyID_Sigma2000Ticks, 1, VariableSpecifiqueDuMode_Input.GetFloat() ) ;		
		
		// Recalculate et Return
		sc.RecalculateChart(ChartNumber_Sigmax512x256x128x64x32x16.GetChartNumber()) ;    // le recalculate du 2000 ticks déclenchera automatiquement un recalculate du 16 ticks
		return ;
	}
	
	
	
	
	if ( sc.UpdateStartIndex== 0 )
	{
		sc.GraphRegion = 0 ;
		sc.ValueFormat = 0 ;
		
		// {  SCString message ;   message.Format("On vient de rentrer dans le bloc ''if ( sc.UpdateStartIndex== 0 )'' de Sigma sur 16t ") ;    sc.AddMessageToLog(message, 0) ; }
		
		// Détermination du XTicks // besoin d'être nécessairement en 16-ticks
		n_ACSIL::s_BarPeriod BarPeriod;
		sc.GetBarPeriodParameters(BarPeriod);
		if ( BarPeriod.IntradayChartBarPeriodType != IBPT_NUM_TRADES_PER_BAR   or   BarPeriod.IntradayChartBarPeriodParameter1 != 16  )  
		{  SCString message ;   message.Format("!! Erreur !! Le chart doit etre en 16-ticks pour tracer l'indicateur") ;    sc.AddMessageToLog(message,1) ; }
		
		// Récupération de la studyID pour le SpreadIndex
		StudyID_SI = sc.GetStudyIDByName(sc.ChartNumber, "SpreadIndexAffichage", 0) ;
		if  (StudyID_SI == 0 )
		{ SCString message ;  message.Format("!! ERREUR !! Le chart ne contient pas de calcul de SpreadIndexAffichage") ; sc.AddMessageToLog(message,1) ; }
		
		// Récupération de la StudyID pour le TPNu
		StudyID_TPNu = sc.GetStudyIDByName(sc.ChartNumber, "Calcul TPNu", 0) ; 
		if  (StudyID_TPNu == 0 )
		{ SCString message ;  message.Format("!! ERREUR !! Le chart ne contient pas de calcul du TPNu") ; sc.AddMessageToLog(message,1) ; }
	
		// Récupération de la studyID pour les ChartInputsSigmaxx512x256x128x64x32x16
		StudyID_Sigma2000Ticks = sc.GetStudyIDByName(ChartNumber_Sigmax512x256x128x64x32x16.GetChartNumber(), "Sigma x512x256x128x64x32x16", 0) ;
		// on crée les ChartStudySubgraphValues
		if  (StudyID_Sigma2000Ticks == 0 )
		{ SCString message ;  message.Format("!! ERREUR !! Le chart source #%d ne contient pas de calcul du Sigmax512x256x128x64x32x16", ChartNumber_Sigmax512x256x128x64x32x16.GetChartNumber()) ; sc.AddMessageToLog(message,1) ; }
		else	
		{	ChartInputs_Sigmax512x256x128x64x32x16Background.SetChartStudySubgraphValues(ChartNumber_Sigmax512x256x128x64x32x16.GetChartNumber(),StudyID_Sigma2000Ticks,0);
			ChartInputs_Sigmax512x256x128x64x32x16Up.SetChartStudySubgraphValues(ChartNumber_Sigmax512x256x128x64x32x16.GetChartNumber(),StudyID_Sigma2000Ticks,1);
			ChartInputs_Sigmax512x256x128x64x32x16Down.SetChartStudySubgraphValues(ChartNumber_Sigmax512x256x128x64x32x16.GetChartNumber(),StudyID_Sigma2000Ticks,2);
		}	
	
		// Récupération de la StudyID pour les ChartInputs_EnvergureMoyenne4000Ticks_SeancePrecedente
		int StudyID = sc.GetStudyIDByName(ChartNumber_Sigmax512x256x128x64x32x16.GetChartNumber(), "[31] TR 4000-ticks moyen (seance precedente, via chart 2000-ticks)", 0) ; 
		// on crée les ChartStudySubgraphValues
		if  (StudyID == 0 )
		{ SCString message ;  message.Format("!! ERREUR !! Le chart source #%d ne contient pas de calcul de ''[31] TR 4000-ticks moyen (seance precedente, via chart 2000-ticks)''", ChartNumber_Sigmax512x256x128x64x32x16.GetChartNumber()) ; sc.AddMessageToLog(message,1) ; }
		else	 {	ChartInputs_EnvergureMoyenne4000Ticks_SeancePrecedente.SetChartStudySubgraphValues(ChartNumber_Sigmax512x256x128x64x32x16.GetChartNumber(),StudyID,2) ;  }

		// Récupération de la StudyID pour les ChartInputs_EnvergureMoyenne4000Ticks_SeanceGlissante
		StudyID = sc.GetStudyIDByName(ChartNumber_Sigmax512x256x128x64x32x16.GetChartNumber(), "[32] TR 4000-ticks moyen (1 seance glissante de 24h, via chart 2000-ticks)", 0) ; 
		// on crée les ChartStudySubgraphValues
		if  (StudyID == 0 )
		{ SCString message ;  message.Format("!! ERREUR !! Le chart source #%d ne contient pas de calcul de ''[32] TR 4000-ticks moyen (1 seance glissante de 24h, via chart 2000-ticks)''", ChartNumber_Sigmax512x256x128x64x32x16.GetChartNumber()) ; sc.AddMessageToLog(message,1) ; }
		else	 {	ChartInputs_EnvergureMoyenne4000Ticks_SeanceGlissante.SetChartStudySubgraphValues(ChartNumber_Sigmax512x256x128x64x32x16.GetChartNumber(),StudyID,2) ;  }
		
		// Récupération de la StudyID pour le VIX
		StudyID_VIX = max ( sc.GetStudyIDByName(sc.ChartNumber, "Symbol: $vix", 0), sc.GetStudyIDByName(sc.ChartNumber, "Symbol: $VIX", 0) ) ;
		if ( StudyID_VIX == 0 )   { SCString message ;  message.Format("!! ERREUR !! La study ''Symbol: $VIX'' n'a pas ete trouvee sur le chart #%d", sc.ChartNumber) ;  sc.AddMessageToLog(message, 1) ;  }
		
		
		// On compare les modes de Sigma sur les charts 16 ticks et 2000 ticks (et on synchronise le cas échéant)
		int Mode = EnvergureMoyenne4000Ticks_ChoixMode.GetIndex() ;
		int Mode_Chart2000Ticks = -1 ;  sc.GetChartStudyInputInt(ChartNumber_Sigmax512x256x128x64x32x16.GetChartNumber(), StudyID_Sigma2000Ticks, 0, Mode_Chart2000Ticks) ;
		if ( Mode_Chart2000Ticks != Mode )
		{ 
			SCString message ;  message.Format("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Mode_Chart2000Ticks != Mode") ;  sc.AddMessageToLog(message, 0) ;  
			
			// synchronisation du chart 2000 ticks : Mode
			sc.SetChartStudyInputInt(ChartNumber_Sigmax512x256x128x64x32x16.GetChartNumber(), StudyID_Sigma2000Ticks, 0, Mode) ;		// int SetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int IntegerValue);
			
			// synchronisation du chart 2000 ticks : range manuel
			float RangeManuel = EnvergureMoyenne4000Ticks_ModeManuel.GetFloat() ;
			sc.SetChartStudyInputFloat(ChartNumber_Sigmax512x256x128x64x32x16.GetChartNumber(), StudyID_Sigma2000Ticks, 3, RangeManuel) ;		// int SetChartStudyInputFloat(int ChartNumber, int StudyID, int InputIndex, double FloatValue);

			// synchronisation du chart 2000 ticks : DiviseurVIX
			float DiviseurVIX = DiviseurVIX_ModeVIX.GetFloat() ;
			sc.SetChartStudyInputFloat(ChartNumber_Sigmax512x256x128x64x32x16.GetChartNumber(), StudyID_Sigma2000Ticks, 4, DiviseurVIX) ;		

			// Gestion de VariableSpecifiqueDuMode_Input
			/* 		if ( Mode == 0 )  { VariableSpecifiqueDuMode_Input.Name = "EnvergureMoyenne4000Ticks Manuel (pts)" ;  VariableSpecifiqueDuMode_Input.SetFloat( RangeManuel ) ; }
			else if ( Mode == 1 )  { VariableSpecifiqueDuMode_Input.Name = "Variable non utilisee" ;  VariableSpecifiqueDuMode_Input.SetFloat(0) ;  }		// Moyenne Séance Précédente
			else if ( Mode == 2 )  { VariableSpecifiqueDuMode_Input.Name = "Diviseur VIX" ;  VariableSpecifiqueDuMode_Input.SetFloat( DiviseurVIX ) ;  }
			else if ( Mode == 3 )  { VariableSpecifiqueDuMode_Input.Name = "Variable non utilisee" ;  VariableSpecifiqueDuMode_Input.SetFloat(0) ;  }		// f(SI)
			else if ( Mode == 4 )  { VariableSpecifiqueDuMode_Input.Name = "Variable non utilisee" ;  VariableSpecifiqueDuMode_Input.SetFloat(0) ;  }		// Moyenne Séance Glissante */
					if ( Mode == 0 )  { VariableSpecifiqueDuMode_Input.Name = "Recalcul en cours" ;  VariableSpecifiqueDuMode_Input.SetFloat( RangeManuel ) ; }
			else if ( Mode == 1 )  { VariableSpecifiqueDuMode_Input.Name = "Recalcul en cours" ;  VariableSpecifiqueDuMode_Input.SetFloat(0) ;  }		// Moyenne Séance Précédente
			else if ( Mode == 2 )  { VariableSpecifiqueDuMode_Input.Name = "Recalcul en cours" ;  VariableSpecifiqueDuMode_Input.SetFloat( DiviseurVIX ) ;  }
			else if ( Mode == 3 )  { VariableSpecifiqueDuMode_Input.Name = "Recalcul en cours" ;  VariableSpecifiqueDuMode_Input.SetFloat(0) ;  }		// f(SI)
			else if ( Mode == 4 )  { VariableSpecifiqueDuMode_Input.Name = "Recalcul en cours" ;  VariableSpecifiqueDuMode_Input.SetFloat(0) ;  }		// Moyenne Séance Glissante
			
			// synchronisation du chart 2000 ticks : VariableSpecifiqueDuMode_Input
			sc.SetChartStudyInputFloat(ChartNumber_Sigmax512x256x128x64x32x16.GetChartNumber(), StudyID_Sigma2000Ticks, 1, VariableSpecifiqueDuMode_Input.GetFloat() ) ;		
			
			// Recalculate et Return
			sc.RecalculateChart(ChartNumber_Sigmax512x256x128x64x32x16.GetChartNumber()) ;    // le recalculate du 2000 ticks déclenchera un recalculate du 16 ticks
			return ;
		}
		
		
		
		
		// Gestion de VariableSpecifiqueDuMode_Input, EnvergureMoyenne4000Ticks_ModeManuel, DiviseurVIX_ModeVIX		// Envoi vers Sigmax512x256x128x64x32x16 d'une modif de range manuel ou de DiviseurVIX 
		if ( Mode == 0 )  
		{ 
			VariableSpecifiqueDuMode_Input.Name = "EnvergureMoyenne4000Ticks Manuel (pts)" ;   
			if ( EnvergureMoyenne4000Ticks_ModeManuel.GetFloat() != VariableSpecifiqueDuMode_Input.GetFloat() )
			{	
				EnvergureMoyenne4000Ticks_ModeManuel.SetFloat( VariableSpecifiqueDuMode_Input.GetFloat() )  ;  

				// synchronisation du chart 2000 ticks : range manuel
				float RangeManuel = EnvergureMoyenne4000Ticks_ModeManuel.GetFloat() ;
				sc.SetChartStudyInputFloat(ChartNumber_Sigmax512x256x128x64x32x16.GetChartNumber(), StudyID_Sigma2000Ticks, 3, RangeManuel) ;	
				sc.SetChartStudyInputFloat(ChartNumber_Sigmax512x256x128x64x32x16.GetChartNumber(), StudyID_Sigma2000Ticks, 1, RangeManuel ) ;		
				
				sc.RecalculateChart(ChartNumber_Sigmax512x256x128x64x32x16.GetChartNumber()) ;    // le recalculate du 2000 ticks déclenchera automatiquement un recalculate du 16 ticks
				return ;
			}
		}
		else if ( Mode == 1 )  		// MoyenneSeancePrecedente
		{ 
			VariableSpecifiqueDuMode_Input.Name = "Variable non utilisee" ;  
			VariableSpecifiqueDuMode_Input.SetFloat(0) ;  
		}
		else if ( Mode == 2 )  
		{ 
			VariableSpecifiqueDuMode_Input.Name = "Diviseur VIX" ;  
			if ( DiviseurVIX_ModeVIX.GetFloat() != VariableSpecifiqueDuMode_Input.GetFloat()   )
			{	
				DiviseurVIX_ModeVIX.SetFloat( VariableSpecifiqueDuMode_Input.GetFloat() ) ;   
				
				// synchronisation du chart 2000 ticks : DiviseurVIX
				float DiviseurVIX = DiviseurVIX_ModeVIX.GetFloat() ;
				sc.SetChartStudyInputFloat(ChartNumber_Sigmax512x256x128x64x32x16.GetChartNumber(), StudyID_Sigma2000Ticks, 4, DiviseurVIX) ;		
				sc.SetChartStudyInputFloat(ChartNumber_Sigmax512x256x128x64x32x16.GetChartNumber(), StudyID_Sigma2000Ticks, 1, DiviseurVIX ) ;		
				
				sc.RecalculateChart(ChartNumber_Sigmax512x256x128x64x32x16.GetChartNumber()) ;    // le recalculate du 2000 ticks déclenchera automatiquement un recalculate du 16 ticks
				return ;
			}
		}
		else if ( Mode == 3 )  		// f(SI)
		{ 
			VariableSpecifiqueDuMode_Input.Name = "Variable non utilisee" ;  
			VariableSpecifiqueDuMode_Input.SetFloat(0) ;  
		}
		else if ( Mode == 4 )  		// Moyenne Séance Glissante
		{ 
			VariableSpecifiqueDuMode_Input.Name = "Variable non utilisee" ;  
			VariableSpecifiqueDuMode_Input.SetFloat(0) ;  
		}
		
	
		// Initialisations
		ProchainIndexCalculMouvementUp[0] = 100 ;
		ProchainIndexCalculMouvementUp[0] = 100 ;
		Sigmax512x256x128x64x32x16Up = 0 ;
		Sigmax512x256x128x64x32x16Down = 0 ;
		
		// Zeroing tools
		sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   // all non user drawn Chart Drawings on the specified ChartNumber will be deleted. The LineNumber parameter in this case is ignored and should be 0. 		

		// Ajout de lignes horizontales
		SCString SpecialText ;
		COLORREF Couleur = RGB(178,102,255) ;
		SpecialText.Format("BMoy 100%%") ;
		DrawLineAndSpecialText (401, 1, SpecialText, Couleur, ptrsc, sc.GraphRegion) ;
		SpecialText.Format("BMoy 80%%") ;
		DrawLineAndSpecialText (367, 1, SpecialText, Couleur, ptrsc, sc.GraphRegion) ;
		SpecialText.Format("BMoy 60%%") ;
		DrawLineAndSpecialText (317, 1, SpecialText, Couleur, ptrsc, sc.GraphRegion) ;
		SpecialText.Format("BMoy 50%%") ;
		DrawLineAndSpecialText (282, 1, SpecialText, Couleur, ptrsc, sc.GraphRegion) ;
		SpecialText.Format("BMoy 40%%") ;
		DrawLineAndSpecialText (239, 1, SpecialText, Couleur, ptrsc, sc.GraphRegion) ;
		SpecialText.Format("BMoy 30%%") ;
		DrawLineAndSpecialText (189, 1, SpecialText, Couleur, ptrsc, sc.GraphRegion) ;
		SpecialText.Format("BMoy 20%%") ;
		DrawLineAndSpecialText (132, 1, SpecialText, Couleur, ptrsc, sc.GraphRegion) ;
		SpecialText.Format("BMoy 10%%") ;
		DrawLineAndSpecialText (74, 1, SpecialText, Couleur, ptrsc, sc.GraphRegion) ;		
		SpecialText.Format(" BMoy 5%% ") ;
		DrawLineAndSpecialText (48, 1, SpecialText, Couleur, ptrsc, sc.GraphRegion) ;				
		SpecialText.Format(" BMoy 0%% ") ;
		DrawLineAndSpecialText (26.4, 1, SpecialText, Couleur, ptrsc, sc.GraphRegion) ;				
		SpecialText.Format(" BMoy -5%% ") ;
		DrawLineAndSpecialText (10.3, 1, SpecialText, Couleur, ptrsc, sc.GraphRegion) ;				
	}

	

	
	// on récupère les valeurs du TP non arrondi dans le Subgraph[2] de "Calcul TPNu" sur le présent Chart 
	SCFloatArray ArrayTPTicks;
	sc.GetStudyArrayUsingID(StudyID_TPNu, 2, ArrayTPTicks);
	
	// on récupère les Sigmax512x256x128x64x32x16XXXXX
	SCFloatArray ArraySigmax512x256x128x64x32x16Background;
	sc.GetStudyArrayFromChartUsingID(ChartInputs_Sigmax512x256x128x64x32x16Background.GetChartStudySubgraphValues(), ArraySigmax512x256x128x64x32x16Background);  
	SCFloatArray ArraySigmax512x256x128x64x32x16Up;
	sc.GetStudyArrayFromChartUsingID(ChartInputs_Sigmax512x256x128x64x32x16Up.GetChartStudySubgraphValues(), ArraySigmax512x256x128x64x32x16Up);  
	SCFloatArray ArraySigmax512x256x128x64x32x16Down;
	sc.GetStudyArrayFromChartUsingID(ChartInputs_Sigmax512x256x128x64x32x16Down.GetChartStudySubgraphValues(), ArraySigmax512x256x128x64x32x16Down);  	
	
	// on récupère le VIX
	SCFloatArray VIX_Array;
	if ( EnvergureMoyenne4000Ticks_ChoixMode.GetIndex() == 2 )  sc.GetStudyArrayUsingID(StudyID_VIX, 3, VIX_Array) ;    // 2 = mode VIX
	
	// on récupère les valeurs du SpreadIndex dans le Subgraph[0] de "SpreadIndexAffichage" sur le présent Chart 
	SCFloatArray ArraySpreadIndex;
	if ( EnvergureMoyenne4000Ticks_ChoixMode.GetIndex() == 3 )  sc.GetStudyArrayUsingID(StudyID_SI, 0, ArraySpreadIndex) ;		// 3 = mode f(SI)

	
	
	
	for (int index = sc.UpdateStartIndex; index < sc.ArraySize; index++)                 // début manual looping
	{
		
		Background[index] = 0 ;  // background est réinitialisé (autrement il peut rester à 1 suite à des recalculations)
		
		// Calcul de TPTicks
		float TPTicks = ArrayTPTicks[index] ;
		
		
		// Modification de fEnvergureMoyenne4000Ticks à chaque index
		if ( EnvergureMoyenne4000Ticks_ChoixMode.GetIndex() == 0 )    // 0 = mode manuel
		{
			fEnvergureMoyenne4000Ticks = EnvergureMoyenne4000Ticks_ModeManuel.GetFloat() ;
		}
		else if ( EnvergureMoyenne4000Ticks_ChoixMode.GetIndex() == 1 )    // 1 = mode MoyenneSeancePrecedente
		{
			if  (  ( sc.BaseDateTimeIn.TimeAt(index) >= 64800  and  sc.BaseDateTimeIn.TimeAt(index-1) < 64800   )   or   index == 0  )    // 64800 est le nb de secondes à 18h   
			{
				// on récupère les valeurs de EnvergureMoyenne4000Ticks (MoyenneSeancePrecedente) du graphe 2000 ticks
				SCFloatArray ExternalArray_EnvergureMoyenne4000Ticks ;
				sc.GetStudyArrayFromChartUsingID(ChartInputs_EnvergureMoyenne4000Ticks_SeancePrecedente.GetChartStudySubgraphValues(), ExternalArray_EnvergureMoyenne4000Ticks);  
				
				// on récupère le fEnvergureMoyenne4000Ticks de la séance
				int PremierIndexSeance = sc.GetContainingIndexForDateTimeIndex(ChartNumber_Sigmax512x256x128x64x32x16.GetChartNumber(), index) ; 
				fEnvergureMoyenne4000Ticks = ExternalArray_EnvergureMoyenne4000Ticks[PremierIndexSeance] ;
			}
		}
		else if ( EnvergureMoyenne4000Ticks_ChoixMode.GetIndex() == 2 )    // 2 = mode VIX
		{
			fEnvergureMoyenne4000Ticks = VIX_Array[index] / DiviseurVIX_ModeVIX.GetFloat() ;
		}
		else if ( EnvergureMoyenne4000Ticks_ChoixMode.GetIndex() == 3 )    // 3 = mode f(SI)
		{
			float SpreadIndex = ArraySpreadIndex[index] ;		
			if ( SpreadIndex == 0 ) fEnvergureMoyenne4000Ticks = 0 ;
			else fEnvergureMoyenne4000Ticks = -0.064*SpreadIndex*SpreadIndex+27.01*SpreadIndex-15.91 ;  ;			// C'est le modèle 2 (idem pour 7), basé sur l'ensemble des ranges 4000t plottés vs les SpreadIndex moyen sur 4000t		// Conférer l'onglet "UT4000t, SI vs Range, 2020-1à3" dans le fichier"VIX et Envergure4000TicksSeancePrecedente et SpreadIndex.xlsx"
		}
		else if ( EnvergureMoyenne4000Ticks_ChoixMode.GetIndex() == 4 )    // 4 = mode MoyenneSeanceGlissante
		{
			if  (  ( 1  )   or   index == 0  )    // on checke toutes les XXX barres
			{
				// on récupère les valeurs de EnvergureMoyenne4000Ticks (MoyenneSeanceGlissante) du graphe 2000 ticks
				SCFloatArray ExternalArray_EnvergureMoyenne4000Ticks ;
				sc.GetStudyArrayFromChartUsingID(ChartInputs_EnvergureMoyenne4000Ticks_SeanceGlissante.GetChartStudySubgraphValues(), ExternalArray_EnvergureMoyenne4000Ticks);  
				
				// on récupère le fEnvergureMoyenne4000Ticks de la séance
				int ExternalIndex = sc.GetContainingIndexForDateTimeIndex(ChartNumber_Sigmax512x256x128x64x32x16.GetChartNumber(), index) ; 
				fEnvergureMoyenne4000Ticks = ExternalArray_EnvergureMoyenne4000Ticks[ExternalIndex] ;
			}
		}

		
		
		// Remplissage de ArrayEnvergureMoyenne4000Ticks
		ArrayEnvergureMoyenne4000Ticks[index] = fEnvergureMoyenne4000Ticks ;
		
		// Calcul de EnvergureMoyenneFull
		float EnvergureMoyenneFull = fEnvergureMoyenne4000Ticks * sqrt(TPTicks/4) ;
		float NbBarresVirginite = TPTicks*1000/16 ;	// float NbBarresVirginite = TPTicks*1000/xTicks ;  // TP = 1pt = 4ticks <=> 4000 ticks = 250 barres de 16 ticks
	
		// Récupération de Sigmax512x256x128x64x32x16Background, Sigmax512x256x128x64x32x16Up et Sigmax512x256x128x64x32x16Down (variables persistantes)
		if ( index %60 == 0 )  // cette récupération n'est réalisée que toutes les 60 barres
		{	
			int IndexSigmax512x256x128x64x32x16 = sc.GetContainingIndexForDateTimeIndex(ChartNumber_Sigmax512x256x128x64x32x16.GetChartNumber(), index); 
			Sigmax512x256x128x64x32x16Background = ArraySigmax512x256x128x64x32x16Background[IndexSigmax512x256x128x64x32x16] ;
			Sigmax512x256x128x64x32x16Up = ArraySigmax512x256x128x64x32x16Up[IndexSigmax512x256x128x64x32x16] ;
			Sigmax512x256x128x64x32x16Down = ArraySigmax512x256x128x64x32x16Down[IndexSigmax512x256x128x64x32x16] ;
		}

		
		//  TPTicks!=0 car le "Days to load" du graphe 200-ticks doit être inférieur à celui du 2000 ticks  // Sigmax512x256x128x64x32x16Background == 0 ssi les données sources sont disponibles
		if ( TPTicks != 0  and  index > 8*NbBarresVirginite and Sigmax512x256x128x64x32x16Background == 0  and  fEnvergureMoyenne4000Ticks != 0 ) 
		{
		
			//////////// Calcul des mouvements verticaux HAUSSIERS : //////////////////////////////
			if ( index >= ProchainIndexCalculMouvementUp[index-1] )
			{
				float LocalHighest = Highest(index, round(NbBarresVirginite/8), ptrsc) ;
				float MouvementVerticalOctuple 		= LocalHighest - FastLowest(index, round(8*NbBarresVirginite), sqrt(8*TPTicks), ptrsc) ;
				float MouvementVerticalQuadruple 	= LocalHighest - FastLowest(index, round(4*NbBarresVirginite), sqrt(4*TPTicks), ptrsc) ;
				float MouvementVerticalDouble 			= LocalHighest - FastLowest(index, round(2*NbBarresVirginite), sqrt(2*TPTicks), ptrsc) ;
				float MouvementVerticalFull 				= LocalHighest - FastLowest(index, round(NbBarresVirginite),    sqrt(TPTicks), ptrsc) ;
				float MouvementVerticalMoitie 			= LocalHighest - FastLowest(index, round(NbBarresVirginite/2), sqrt(TPTicks/2), ptrsc) ;
				float MouvementVerticalQuart 			= LocalHighest - FastLowest(index, round(NbBarresVirginite/4), sqrt(TPTicks/4), ptrsc) ;
				/////// Calcul des mouvements verticaux normés et leur contribution :
				// - octuple :
				float MouvementVerticalOctupleNorme 						= MouvementVerticalOctuple/(EnvergureMoyenneFull*2*sqrt(2)) ;
				float MouvementVerticalOctupleNormeContribution 		= (MouvementVerticalOctupleNorme-1)/(8*sqrt(2)) ;
				// - quadruple :
				float MouvementVerticalQuadrupleNorme 					= MouvementVerticalQuadruple/(EnvergureMoyenneFull*2) ;
				float MouvementVerticalQuadrupleNormeContribution 	= (MouvementVerticalQuadrupleNorme-1)/8 ;
				// - double :
				float MouvementVerticalDoubleNorme 							= MouvementVerticalDouble/(EnvergureMoyenneFull*sqrt(2)) ;
				float MouvementVerticalDoubleNormeContribution 		= (MouvementVerticalDoubleNorme-1)/(4*sqrt(2)) ;
				// - full :
				float MouvementVerticalFullNorme 								= MouvementVerticalFull/EnvergureMoyenneFull ;
				float MouvementVerticalFullNormeContribution				= (MouvementVerticalFullNorme-1)/4 ;
				// - moitié :
				float MouvementVerticalMoitieNorme 							= MouvementVerticalMoitie/(EnvergureMoyenneFull/sqrt(2));
				float MouvementVerticalMoitieNormeContribution			= (MouvementVerticalMoitieNorme-1)/2 ;
				// - quart :
				float MouvementVerticalQuartNorme 							= MouvementVerticalQuart/(EnvergureMoyenneFull/2) ;
				float MouvementVerticalQuartNormeContribution			= (MouvementVerticalQuartNorme-1)/4 ;
				// - MouvementVerticalSommeNorme :
				MouvementVerticalSommeNormeUp[index] 					= MouvementVerticalFullNormeContribution + MouvementVerticalMoitieNormeContribution + MouvementVerticalQuartNormeContribution
																					+ MouvementVerticalDoubleNormeContribution + MouvementVerticalQuadrupleNormeContribution + MouvementVerticalOctupleNormeContribution + Sigmax512x256x128x64x32x16Up/100 ; 
				// ProchainIndexCalculMouvement
				ProchainIndexCalculMouvementUp[index] = index + max (1.0, ceil ( -MouvementVerticalSommeNormeUp[index] * 1.0 * TPTicks ) ) ;   // plus il est négatif, et plus TPTicks est élevé, moins souvent il sera calculé    // * 5.0 dans la v3.5
			}
			else
			{ 	MouvementVerticalSommeNormeUp[index] = MouvementVerticalSommeNormeUp[index-1]  ;  ProchainIndexCalculMouvementUp[index] = ProchainIndexCalculMouvementUp[index-1] ; }
			
			

			//////////// Calcul des mouvements verticaux BAISSIERS : ///////////////////////////////////
			if ( index >= ProchainIndexCalculMouvementDown[index-1] )
			{
				float LocalLowest = Lowest(index, round(NbBarresVirginite/8), ptrsc) ;
				float MouvementVerticalOctuple 		= FastHighest(index, round(8*NbBarresVirginite), sqrt(8*TPTicks), ptrsc) - LocalLowest ;
				float MouvementVerticalQuadruple 	= FastHighest(index, round(4*NbBarresVirginite), sqrt(4*TPTicks), ptrsc) - LocalLowest ;
				float MouvementVerticalDouble 		= FastHighest(index, round(2*NbBarresVirginite), sqrt(2*TPTicks), ptrsc) - LocalLowest ;
				float MouvementVerticalFull 			= FastHighest(index, round(NbBarresVirginite), sqrt(TPTicks), ptrsc)       - LocalLowest ;
				float MouvementVerticalMoitie 		= FastHighest(index, round(NbBarresVirginite/2), sqrt(TPTicks/2), ptrsc) - LocalLowest ;
				float MouvementVerticalQuart 			= FastHighest(index, round(NbBarresVirginite/4), sqrt(TPTicks/4), ptrsc) - LocalLowest ;
				/////// Calcul des mouvements verticaux normés et leur contribution :
				// - octuple :
				float MouvementVerticalOctupleNorme 						= MouvementVerticalOctuple/(EnvergureMoyenneFull*2*sqrt(2)) ;
				float MouvementVerticalOctupleNormeContribution 		= (MouvementVerticalOctupleNorme-1)/(8*sqrt(2)) ;
				// - quadruple :
				float MouvementVerticalQuadrupleNorme 					= MouvementVerticalQuadruple/(EnvergureMoyenneFull*2) ;
				float MouvementVerticalQuadrupleNormeContribution 	= (MouvementVerticalQuadrupleNorme-1)/8 ;
				// - double :
				float MouvementVerticalDoubleNorme 							= MouvementVerticalDouble/(EnvergureMoyenneFull*sqrt(2)) ;
				float MouvementVerticalDoubleNormeContribution 		= (MouvementVerticalDoubleNorme-1)/(4*sqrt(2)) ;
				// - full :
				float MouvementVerticalFullNorme 								= MouvementVerticalFull/EnvergureMoyenneFull ;
				float MouvementVerticalFullNormeContribution				= (MouvementVerticalFullNorme-1)/4 ;
				// - moitié :
				float MouvementVerticalMoitieNorme 							= MouvementVerticalMoitie/(EnvergureMoyenneFull/sqrt(2));
				float MouvementVerticalMoitieNormeContribution			= (MouvementVerticalMoitieNorme-1)/2 ;
				// - quart :
				float MouvementVerticalQuartNorme 							= MouvementVerticalQuart/(EnvergureMoyenneFull/2) ;
				float MouvementVerticalQuartNormeContribution			= (MouvementVerticalQuartNorme-1)/4 ;
				// - MouvementVerticalSommeNorme :
				MouvementVerticalSommeNormeDown[index] 				= MouvementVerticalFullNormeContribution + MouvementVerticalMoitieNormeContribution + MouvementVerticalQuartNormeContribution
																					+ MouvementVerticalDoubleNormeContribution + MouvementVerticalQuadrupleNormeContribution + MouvementVerticalOctupleNormeContribution + Sigmax512x256x128x64x32x16Down/100 ; 
				// ProchainIndexCalculMouvement
				ProchainIndexCalculMouvementDown[index] = index + max (1.0, ceil ( -MouvementVerticalSommeNormeDown[index] * 1.0 * TPTicks ) ) ;    // plus il est négatif, et plus TPTicks est élevé, moins souvent il sera calculé    // * 5.0 dans la v3.5
			}
			else
			{ 	MouvementVerticalSommeNormeDown[index] = MouvementVerticalSommeNormeDown[index-1]  ;  ProchainIndexCalculMouvementDown[index] = ProchainIndexCalculMouvementDown[index-1] ; }
			
			//////////// MaxMouvementVerticalUpOuDown //////////
			MaxMouvementVerticalUpOuDown[index] = round ( ( 1.0 + max ( MouvementVerticalSommeNormeUp[index], MouvementVerticalSommeNormeDown[index] ) ) *100 ) ; 
		}
		
		else  // l'indicateur n'a pas pu être calculé
		{	
			Background[index] = 1 ;  // background a lieu
			MouvementVerticalSommeNormeUp[index] = 0 ;
			MouvementVerticalSommeNormeDown[index] = 0 ;
			MaxMouvementVerticalUpOuDown[index] = 0 ;
			

			if ( Sigmax512x256x128x64x32x16Background == 1 )
			{ Background.DataColor[index] = RGB(100,100,100) ;  }	
			else if (  index < 8*NbBarresVirginite  )  
			{ Background.DataColor[index] = RGB(150,150,150) ; }	
			else if ( TPTicks == 0 )
			{	Background.DataColor[index] = RGB(200,200,200) ;	}
			else if ( fEnvergureMoyenne4000Ticks == 0 )
			{	Background.DataColor[index] = RGB(220,220,220) ;	}
			
			ProchainIndexCalculMouvementUp[index] = ProchainIndexCalculMouvementUp[index-1] ;
			ProchainIndexCalculMouvementDown[index] = ProchainIndexCalculMouvementDown[index-1] ;
		}
		
	
		// couleur de la courbe retournée
		if ( MouvementVerticalSommeNormeUp[index] > MouvementVerticalSommeNormeDown[index] )
		{ MaxMouvementVerticalUpOuDown.DataColor[index] = RGB(0,200,0) ;	}	
		else if ( MouvementVerticalSommeNormeUp[index] < MouvementVerticalSommeNormeDown[index] )
		{ MaxMouvementVerticalUpOuDown.DataColor[index] = RGB(200,0,0) ;	}	
		else
		{ 
			MaxMouvementVerticalUpOuDown.DataColor[index] = MaxMouvementVerticalUpOuDown.DataColor[index-1] ; 
			if ( Background[index] == 1 ) { MaxMouvementVerticalUpOuDown.DataColor[index] = RGB(0,0,0) ; }       // indicateur non calculé => courbe en noir
		}

		
		// Tracés de MouvementVerticalUp et MouvementVerticalDown
		MouvementVerticalUp[index] = round ( ( 1.0 + MouvementVerticalSommeNormeUp[index] ) * 100 ) ;
		MouvementVerticalDown[index] = round ( ( 1.0 + MouvementVerticalSommeNormeDown[index] ) * 100) ;
		
	} // fin manual looping
	
}


