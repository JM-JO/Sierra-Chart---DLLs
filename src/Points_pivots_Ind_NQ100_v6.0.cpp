/*
v1.0 (2019/01/29) : création de "Points_pivots_v1.0" en copiant le code source de "scsf_PivotPointsDaily" dans "Studies8.cpp".
v1.1 (01/29) : élagage du code (suppression des méthodes de calcul alternatives, suppression des R/S supérieurs à 4)  => le code est très mauvais (tous les PP sont entièrement recalculés à chaque index).
v2.0 (01/31): création de ma propre méthode de récupération des H/L/C de la veille en appelant le graphe journalier. Calcul des PP et des mid jusqu'à R4/S4.
v2.1 (2019/02/03) : ajout des quarts journaliers. 
v2.2 (2019/02/03) : ajout des niveaux Full/Mid/Quart hebdos et mensuels.
v2.3 (2019/02/03) : ajout des niveaux Eighth hebdos et mensuels. Correction de la formule des R4/S4.
v2.4 (2019/02/03) : ajout des niveaux SiXteenths mensuels.
v2.5 (2019/02/04) : ajout de IntradayBarIndex==0 sur le graphe source (ie intraday) pour accélérer le code.
v2.7 (2019/02/06) : ajout d'un détecteur d'OpenGap dans la fonction "Pivots J (Full/Mid/Quart) ". 
v2.7 (2019/02/25) : ajout du PH absolu.
v2.8 (2019/2/28) : looping manuel.
v3.0 (2019/2/28) : projection des points pivots J de l'indice sur le future.
v3.1 (2019/3/1) : projection des points pivots H&M de l'indice sur le future. Le Name est aligné sur la gauche du graphe.
v3.2 (2019/3/20) : le SpreadMoyen n'est plus récupéré sur un chart externe, mais via la Study OverlayINQ100 du même chart.
v3.3 (2019/3/20) : rajout d'ExtraArray ( sc.Subgraph[SubgraphIndex].Arrays[1] ) lors des projections sur l'indice. Ces ExtraArray contiennent des NiveauIndice sans correction.
v3.3.1 (2019/3/27) : correction du bug de l'OpenGap non affiché pour les niveaux daily projetés.
v3.4.0 (2019/6/14) : les pivots journaliers (Full/Mid) sont précalculés à 16h02 (au lieu de J+1 à 9h30m15s) ; les pivots hebdos (Full/Mid/Quart) le vendredi à 16h02 ; les pivots mensuels (Full/Mid/Quart) à 16h02 en cochant "Calcul du pivot forcé".
v3.4.1 (2019/6/14) :  la variable HoraireDebutSeance (9h30m24s) est remplacée par la variable HoraireDebutTrace (9h30m25s), dans le calcul des pivots projetés.
v3.4.2 (2019/6/14) : modification des noms des sc.GraphName des PP projetés sur future.
v3.5.0 (2019/7/6) : le tracé de tous les niveaux indices projetés est togglé ON/OFF via un bouton. Le statut est géré par la study "Prix typique". Les fonctions du DLL study Points_pivots_INQ100 consultent ce statut pour gérer la variable sc.HideStudy.
v3.7.0 (2019/7/12) : ajout de la study scsf_CorrectionBugOpenUT1J. Si Open==0 on redownloade les 2 dernières bougies du graphe UT 1J.
v3.7.1 (2019/7/15) : pour éviter de submerger le log, scsf_CorrectionBugOpenUT1J ne tente le download que toutes les 10s et à partir de 9h30m25s.
v3.8.0 (2019/9/28) : les niveaux indice projetés sont tracés à partir de 9h00 (le SpreadFutureIndice correspond à sa valeur à la cloture de la veille) pour naviguer sur les niveaux avant 9h30m25s.
v3.8.1 (2019/10/22) : résolution du bug de navigation de niveaux avant 9h30m15s (l'index du chart externe (J/H/M) est différent avant/après 9h30m15s).
v4.0.0 (2019/10/26) : tracé des PP Sixteenths mensuels en vectoriel sur le graphe indice. Le niveau retenu est celui de index = sc.LastVisibleBar.
v4.0.1 (2019/10/26) : idem avec les autres PP sur le graphe indice.
v4.0.2 (2019/10/27) : on refait l'épaisseur des lignes sur le graphe indice.
v4.1.0 (2019/10/28) : tracé des PP projetés sur le future en vectoriel. Le niveau retenu est celui de index = sc.LastVisibleBar.
v4.1.1 (2019/10/30) : choix du tracé de l'affichage des quarts H (avec un input). Les niveaux qui valent <500 ne sont pas tracés.
v4.2.0 (2019/11/8) : tracé des PP 1h et 4h sur l'indice. Tracé des niveaux vectoriels projetés sur le future.
v4.3 (2019/11/15) : "BooleanHideStudy" devient "NiveauxIndicesAffiches_Statut" (vaut 0 (OFF), 1 (All) ou 2 (Lean)). S'assurer que les PP1h et PP4h ne sont pas faux lorsqu'ils sont calculés en live : si la nvelle bougie n'existe pas encore, ne pas calculer les PP (car le close de l'heure qui s'est terminée pourrait être faux).
v4.4 (2019/12/02) : correction d'un "faux bug" de flux : suite à la journée du ve 29/11/2019 (Thanksgiving), à cause de la séance qui se termine à 13h (et le flux qui stoppe à 14h15), lors du calcul des PP non projetés le lundi 2/12 (pour le pivot J, H et M) on entre FirstIndex avec le correcteur de bug. On remplace tous les 13h02/16h02 par 13h04/16h04 pour que les graphes sur l'UT 4min soient correctement tracés.
v4.5 (2019/12/03) : création de la fonction PeriodeChartEnSecondes, et de la variable persistante PeriodeChartEnSecondes qui stocke cette valeur.
v4.6 (2020/03/10) : résolution du bug de tracé de l'Open Gap mal géré à cause du bug de la bougie journalière mal construite => créer un ExtraArray OpenDaily dans la study scsf_MesPointsPivotsJournaliers. Cet Array vaut la valeur de l'Open pdt toute la séance. Réutiliser cette valeur de l'OpenDaily pour calculer la valeur du DemiGap.
v4.8 (2020/06/25) : on remplace GetTimeWithoutMilliseconds par GetTimeInSecondsWithoutMilliseconds.
v4.9 (2020/6/28) : accélération de SC par ajout de la condition : " if ( sc.ChartWindowIsActive  or  NbMonitors == 2 ) ". Et on ne trace que les niveaux visibles sur l'écran.
v5.0 (2020/7/7) : introduction des OpenGap hebdo et mensuel sur les graphes indice => fonctionne.
v5.1 (2020/7/8) : résolution d'un bug de Sierra : au premier calcul des studies, la valeur de BottomCoordinate est fausse (elle est identique à TopCoordinate) => comme BottomCoordinate est faux après le premier calcul, les niveaux ne sont jamais tracés au premier calcul (ils sont considérés comme en dehors de l'écran).
v5.2 (2020/7/9) : résolution d'un bug de tracé des niveaux hebdo lié à vendredi = jour férié. Ajout de ShiftFirstIndex pour les niveaux hebdos.
v5.3 (2020/7/9) : modif de l'ordre des SCSubgraph hebdo et mensuel pour que PH/PB soient affichés au-dessus de OpenGap.
v5.4 (2020/7/9) : introduction des OpenGap/DemiGap hebdo et mensuel projetés sur future. 
v5.5 (2020/7/31) : ajout d'un input pour l'affichages des Mid4 et Full4 (J/H/M).
v5.6 (2020/8/5) : ajout d'un input pour l'affichages des PH et PB de la veille (J/H/M), ainsi que Close Veille. !!! v5.6 est buggé : les niveaux monthly projetés ne s'affichent pas !!!
v5.7.1 (2020/8/5) : scsf_MesPointsPivotsMensuelsProjetes est construit à partir de v5.4 (non buggé) : on fait un copier-coller de scsf_MesPointsPivotsMensuelsProjetes issu de v5.7.
v5.8 (2020/09/05) : ajout de l'input AffichageDesMid4Full4ONOFF pour l'affichages des eighths H et M (RE13/16 et SE13/16).  Débuggages liés à l'utilisation de GetHour() pour les niveaux projetés.
v5.9 (2020/09/30) : modif mineure sur les blocs d'affichage des niveaux non projetés.
v6.0 (2020/10/8) : ajout des PH et PB en SCFloatArrayRef dans PP 1h projetés, afin de les faire récupérer par NavigNiveaux pour calculer le Range.

	
Améliorations possibles : 
	- créer une fonction unique pour corriger les bugs de flux (commune à tous les indicateurs) : BugDeFlux (char UT [J/H/M], int Year, int Month, int Day, &HighCorrige, &LowCorrige, &CloseCorrige) => pas pertinent, se contenter d'utiliser une correction intrinsèque à scsf_MesPointsPivotsJournaliers pour les data fausses. Et utiliser des corrections spécifiques pour les différentes UT (typiquement lorsqu'il y a des ponts ou des demi-séances)
	- scsf_MesPointsPivotsJournaliersProjetes : supprimer les quarts (ils sont calculés mais non affichés).
	


SOMMAIRE DES STRUCTURES ET FONCTIONS :
CalculatePPs
CalculateEighths
CalculateSiXteenths
Highest
ArrondiSiDepasse
NiveauxIndicesAffiches_Statut
MonTexteString
MaLigneJusquAIndex
creationLigneEtTexte
PeriodeChartEnSecondes
NumberOfMonitors
WindowsVersionNumber
scsf_MesPointsPivotsUneHeure
scsf_MesPointsPivotsQuatreHeures
scsf_MesPointsPivotsJournaliers
scsf_CorrectionBugOpenUT1J
scsf_MesPointsPivotsHebdo
scsf_MesPointsPivotsMensuels
scsf_MesEighthsHebdo
scsf_MesEighthsMensuels
scsf_MesSiXteenthsMensuels
scsf_MesPointsPivotsUneHeureProjetes
scsf_MesPointsPivotsQuatreHeuresProjetes
scsf_MesPointsPivotsJournaliersProjetes
scsf_MesPointsPivotsHebdoProjetes
scsf_MesPointsPivotsMensuelsProjetes
scsf_MesEighthsMensuelsProjetes
*/

#include "sierrachart.h"  // When including in your source code file additional header files, they need to be placed above the #include "sierrachart.h" line. 

SCDLLName("Points pivots de l'indice NQ100")



// Calcul des niveaux Full/Mid/Quart ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CalculatePPs (float PriorHigh, float PriorLow, float PriorClose, float& PP, float& R1, float& R2, float& R3, float& R4, float& S1, float& S2, float& S3, float& S4, float& mR1, float& mR2, float& mR3, float& mR4, float& mS1, float& mS2, float& mS3, float& mS4, 
							float& RQ1, float& RQ2, float& RQ3, float& RQ4, float& RQ5, float& RQ6, float& RQ7, float& RQ8, float& SQ1, float& SQ2, float& SQ3, float& SQ4, float& SQ5, float& SQ6, float& SQ7, float& SQ8)
	{
		PP = (PriorHigh + PriorLow + PriorClose) / 3;
		R1 = (PP * 2) - PriorLow;
		R2 = PP + (PriorHigh - PriorLow);
		R3 = 2 * PP + (PriorHigh - 2*PriorLow); 
		R4 = R3 + PriorHigh - PriorLow;
		//R4bis = 3 * PP + (PriorHigh - 3 * PriorLow);
		S1 = (2 * PP) - PriorHigh;
		S2 = PP - (PriorHigh - PriorLow);
		S3 = 2 * PP - (2 * PriorHigh - PriorLow);
		S4 = S3 - PriorHigh + PriorLow;
		//S4bis = 3 * PP - (3 * PriorHigh - PriorLow);
		mR1 = (PP + R1) / 2;
		mR2 = (R1 + R2) / 2;
		mR3 = (R2 + R3) / 2;
		mR4 = (R3 + R4) / 2;
		mS1 = (PP + S1) / 2;
		mS2 = (S1 + S2) / 2;
		mS3 = (S2 + S3) / 2;
		mS4 = (S3 + S4) / 2;
		
		// - calcul des R/S quarts :
		RQ1 = (PP+mR1)/2;
		RQ2 = (mR1+R1)/2;
		RQ3 = (R1+mR2)/2;
		RQ4 = (mR2+R2)/2;
		RQ5 = (R2+mR3)/2;
		RQ6 = (mR3+R3)/2;
		RQ7 = (R3+mR4)/2;
		RQ8 = (mR4+R4)/2;
		SQ1 = (PP+mS1)/2;
		SQ2 = (mS1+S1)/2;
		SQ3 = (S1+mS2)/2;
		SQ4 = (mS2+S2)/2;
		SQ5 = (S2+mS3)/2;
		SQ6 = (mS3+S3)/2;
		SQ7 = (S3+mS4)/2;
		SQ8 = (mS4+S4)/2;
	}
	
// Calcul des niveaux Eighth ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CalculateEighths ( 	float PriorHigh, float PriorLow, float PriorClose, 
								float& RE1, float& RE2, float& RE3, float& RE4, float& RE5, float& RE6, float& RE7, float& RE8, float& RE9, float& RE10, float& RE11, float& RE12, float& RE13, float& RE14, float& RE15, float& RE16, 
								float& SE1, float& SE2, float& SE3, float& SE4, float& SE5, float& SE6, float& SE7, float& SE8, float& SE9, float& SE10, float& SE11, float& SE12, float& SE13, float& SE14, float& SE15, float& SE16)
	{
		float PP, R1, R2, R3, R4, S1, S2, S3, S4, mR1, mR2, mR3, mR4, mS1, mS2, mS3, mS4, RQ1, RQ2, RQ3, RQ4, RQ5, RQ6, RQ7, RQ8, SQ1, SQ2, SQ3, SQ4, SQ5, SQ6, SQ7, SQ8;
		
		PP = (PriorHigh + PriorLow + PriorClose) / 3;
		R1 = (PP * 2) - PriorLow;
		R2 = PP + (PriorHigh - PriorLow);
		R3 = 2 * PP + (PriorHigh - 2*PriorLow); 
		R4 = R3 + PriorHigh - PriorLow;
		S1 = (2 * PP) - PriorHigh;
		S2 = PP - (PriorHigh - PriorLow);
		S3 = 2 * PP - (2 * PriorHigh - PriorLow);
		S4 = S3 - PriorHigh + PriorLow;
		mR1 = (PP + R1) / 2;
		mR2 = (R1 + R2) / 2;
		mR3 = (R2 + R3) / 2;
		mR4 = (R3 + R4) / 2;
		mS1 = (PP + S1) / 2;
		mS2 = (S1 + S2) / 2;
		mS3 = (S2 + S3) / 2;
		mS4 = (S3 + S4) / 2;
		
		// - calcul des R/S quarts :
		RQ1 = (PP+mR1)/2;;
		RQ2 = (mR1+R1)/2;;
		RQ3 = (R1+mR2)/2;;
		RQ4 = (mR2+R2)/2;;
		RQ5 = (R2+mR3)/2;;
		RQ6 = (mR3+R3)/2;;
		RQ7 = (R3+mR4)/2;;
		RQ8 = (mR4+R4)/2;;
		SQ1 = (PP+mS1)/2;;
		SQ2 = (mS1+S1)/2;;
		SQ3 = (S1+mS2)/2;;
		SQ4 = (mS2+S2)/2;;
		SQ5 = (S2+mS3)/2;;
		SQ6 = (mS3+S3)/2;;
		SQ7 = (S3+mS4)/2;;
		SQ8 = (mS4+S4)/2;;
		
		// - calcul des résistances eighth :
		RE1  = (PP+RQ1)/2;
		RE2  = (RQ1+mR1)/2;
		RE3  = (mR1+RQ2)/2;
		RE4  = (RQ2+R1)/2;
		RE5  = (R1+RQ3)/2;
		RE6  = (RQ3+mR2)/2;
		RE7  = (mR2+RQ4)/2;
		RE8  = (RQ4+R2)/2;
		RE9  = (R2+RQ5)/2;
		RE10 = (RQ5+mR3)/2;
		RE11 = (mR3+RQ6)/2;
		RE12 = (RQ6+R3)/2;
		RE13 = (R3+RQ7)/2;
		RE14 = (RQ7+mR4)/2;
		RE15 = (mR4+RQ8)/2;
		RE16 = (RQ8+R4)/2;
		
		// - calcul des supports eighth :
		SE1  = (PP+SQ1)/2;
		SE2  = (SQ1+mS1)/2;
		SE3  = (mS1+SQ2)/2;
		SE4  = (SQ2+S1)/2;
		SE5  = (S1+SQ3)/2;
		SE6  = (SQ3+mS2)/2;
		SE7  = (mS2+SQ4)/2;
		SE8  = (SQ4+S2)/2;
		SE9  = (S2+SQ5)/2;
		SE10 = (SQ5+mS3)/2;
		SE11 = (mS3+SQ6)/2;
		SE12 = (SQ6+S3)/2;
		SE13 = (S3+SQ7)/2;
		SE14 = (SQ7+mS4)/2;
		SE15 = (mS4+SQ8)/2;
		SE16 = (SQ8+S4)/2;
			
	}

// Calcul des niveaux SiXteenths  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CalculateSiXteenths ( 	float PriorHigh, float PriorLow, float PriorClose, 
										float& RX1, float& RX2, float& RX3, float& RX4, float& RX5, float& RX6, float& RX7, float& RX8, float& RX9, float& RX10, float& RX11, float& RX12, float& RX13, float& RX14, float& RX15, float& RX16, 
										float& RX17, float& RX18, float& RX19, float& RX20, float& RX21, float& RX22, float& RX23, float& RX24, float& RX25, float& RX26, float& RX27, float& RX28, float& RX29, float& RX30, float& RX31, float& RX32, 
										float& SX1, float& SX2, float& SX3, float& SX4, float& SX5, float& SX6, float& SX7, float& SX8, float& SX9, float& SX10, float& SX11, float& SX12, float& SX13, float& SX14, float& SX15, float& SX16, 
										float& SX17, float& SX18, float& SX19, float& SX20, float& SX21, float& SX22, float& SX23, float& SX24, float& SX25, float& SX26, float& SX27, float& SX28, float& SX29, float& SX30, float& SX31, float& SX32 ) 
	{
		float PP, R1, R2, R3, R4, S1, S2, S3, S4, mR1, mR2, mR3, mR4, mS1, mS2, mS3, mS4, RQ1, RQ2, RQ3, RQ4, RQ5, RQ6, RQ7, RQ8, SQ1, SQ2, SQ3, SQ4, SQ5, SQ6, SQ7, SQ8;
		float RE1, RE2, RE3, RE4, RE5, RE6, RE7, RE8, RE9, RE10, RE11, RE12, RE13, RE14, RE15, RE16 ;
		float SE1, SE2, SE3, SE4, SE5, SE6, SE7, SE8, SE9, SE10, SE11, SE12, SE13, SE14, SE15, SE16 ;
		
		PP = (PriorHigh + PriorLow + PriorClose) / 3;
		R1 = (PP * 2) - PriorLow;
		R2 = PP + (PriorHigh - PriorLow);
		R3 = 2 * PP + (PriorHigh - 2*PriorLow); 
		R4 = R3 + PriorHigh - PriorLow;
		S1 = (2 * PP) - PriorHigh;
		S2 = PP - (PriorHigh - PriorLow);
		S3 = 2 * PP - (2 * PriorHigh - PriorLow);
		S4 = S3 - PriorHigh + PriorLow;
		mR1 = (PP + R1) / 2;
		mR2 = (R1 + R2) / 2;
		mR3 = (R2 + R3) / 2;
		mR4 = (R3 + R4) / 2;
		mS1 = (PP + S1) / 2;
		mS2 = (S1 + S2) / 2;
		mS3 = (S2 + S3) / 2;
		mS4 = (S3 + S4) / 2;
		
		// - calcul des R/S quarts :
		RQ1 = (PP+mR1)/2;
		RQ2 = (mR1+R1)/2;
		RQ3 = (R1+mR2)/2;
		RQ4 = (mR2+R2)/2;
		RQ5 = (R2+mR3)/2;
		RQ6 = (mR3+R3)/2;
		RQ7 = (R3+mR4)/2;
		RQ8 = (mR4+R4)/2;
		SQ1 = (PP+mS1)/2;
		SQ2 = (mS1+S1)/2;
		SQ3 = (S1+mS2)/2;
		SQ4 = (mS2+S2)/2;
		SQ5 = (S2+mS3)/2;
		SQ6 = (mS3+S3)/2;
		SQ7 = (S3+mS4)/2;
		SQ8 = (mS4+S4)/2;
		
		// - calcul des résistances eighth :
		RE1  = (PP+RQ1)/2;
		RE2  = (RQ1+mR1)/2;
		RE3  = (mR1+RQ2)/2;
		RE4  = (RQ2+R1)/2;
		RE5  = (R1+RQ3)/2;
		RE6  = (RQ3+mR2)/2;
		RE7  = (mR2+RQ4)/2;
		RE8  = (RQ4+R2)/2;
		RE9  = (R2+RQ5)/2;
		RE10 = (RQ5+mR3)/2;
		RE11 = (mR3+RQ6)/2;
		RE12 = (RQ6+R3)/2;
		RE13 = (R3+RQ7)/2;
		RE14 = (RQ7+mR4)/2;
		RE15 = (mR4+RQ8)/2;
		RE16 = (RQ8+R4)/2;
		
		// - calcul des supports eighth :
		SE1  = (PP+SQ1)/2;
		SE2  = (SQ1+mS1)/2;
		SE3  = (mS1+SQ2)/2;
		SE4  = (SQ2+S1)/2;
		SE5  = (S1+SQ3)/2;
		SE6  = (SQ3+mS2)/2;
		SE7  = (mS2+SQ4)/2;
		SE8  = (SQ4+S2)/2;
		SE9  = (S2+SQ5)/2;
		SE10 = (SQ5+mS3)/2;
		SE11 = (mS3+SQ6)/2;
		SE12 = (SQ6+S3)/2;
		SE13 = (S3+SQ7)/2;
		SE14 = (SQ7+mS4)/2;
		SE15 = (mS4+SQ8)/2;
		SE16 = (SQ8+S4)/2;
		
		
		// - calcul des résistances siXteenth :
		RX1  = (PP+RE1)/2;
		RX2  = (RE1+RQ1)/2;
		RX3  = (RQ1+RE2)/2;
		RX4  = (RE2+mR1)/2;
		RX5  = (mR1+RE3)/2;
		RX6  = (RE3+RQ2)/2;
		RX7  = (RQ2+RE4)/2;
		RX8  = (RE4+R1)/2;
		RX9  = (R1+RE5)/2;
		RX10 = (RE5+RQ3)/2;
		RX11 = (RQ3+RE6)/2;
		RX12 = (RE6+mR2)/2;
		RX13 = (mR2+RE7)/2;
		RX14 = (RE7+RQ4)/2;
		RX15 = (RQ4+RE8)/2;
		RX16 = (RE8+R2)/2;
		RX17 = (R2+RE9)/2;
		RX18 = (RE9+RQ5)/2;
		RX19 = (RQ5+RE10)/2;
		RX20 = (RE10+mR3)/2;
		RX21 = (mR3+RE11)/2;
		RX22 = (RE11+RQ6)/2;
		RX23 = (RQ6+RE12)/2;
		RX24 = (RE12+R3)/2;
		RX25 = (R3+RE13)/2;
		RX26 = (RE13+RQ7)/2;
		RX27 = (RQ7+RE14)/2;
		RX28 = (RE14+mR4)/2;
		RX29 = (mR4+RE15)/2;
		RX30 = (RE15+RQ8)/2;
		RX31 = (RQ8+RE16)/2; 
		RX32 = (RE16+R4)/2;
		
		// - calcul des supports siXteenth :
		SX1  = (PP+SE1)/2;
		SX2  = (SE1+SQ1)/2;
		SX3  = (SQ1+SE2)/2;
		SX4  = (SE2+mS1)/2;
		SX5  = (mS1+SE3)/2;
		SX6  = (SE3+SQ2)/2;
		SX7  = (SQ2+SE4)/2;
		SX8  = (SE4+S1)/2;
		SX9  = (S1+SE5)/2;
		SX10 = (SE5+SQ3)/2;
		SX11 = (SQ3+SE6)/2;
		SX12 = (SE6+mS2)/2;
		SX13 = (mS2+SE7)/2;
		SX14 = (SE7+SQ4)/2;
		SX15 = (SQ4+SE8)/2;
		SX16 = (SE8+S2)/2;
		SX17 = (S2+SE9)/2;
		SX18 = (SE9+SQ5)/2;
		SX19 = (SQ5+SE10)/2;
		SX20 = (SE10+mS3)/2;
		SX21 = (mS3+SE11)/2;
		SX22 = (SE11+SQ6)/2;
		SX23 = (SQ6+SE12)/2;
		SX24 = (SE12+S3)/2;
		SX25 = (S3+SE13)/2;
		SX26 = (SE13+SQ7)/2;
		SX27 = (SQ7+SE14)/2;
		SX28 = (SE14+mS4)/2;
		SX29 = (mS4+SE15)/2;
		SX30 = (SE15+SQ8)/2;
		SX31 = (SQ8+SE16)/2;
		SX32 = (SE16+S4)/2;
	}
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float Highest (SCGraphData ExternalChartData, int ExternalChartIndex)  
{
	// mettre erreur dans Log si NbChandeliers <1 // note : en l'état si NbChandeliers <1, Highest retourne -FLT_MAX
	float highest = -FLT_MAX  ;
	for (int Index = ExternalChartIndex - 1 ; Index > -1 ; Index--)          // l'instruction " ExternalChartIndex - 1 " fait que le Highest sera calculé sans tenir compte de la bougie active ( qui correspond à ExternalChartIndex )
	{
		if ( ExternalChartData[SC_HIGH][Index] > highest )
			highest = ExternalChartData[SC_HIGH][Index] ;
	}
	return highest ;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ArrondiSiDepasse(SCSubgraphRef NiveauX, float arrondi, int index)
{
	if ( NiveauX[index-1] == 0 )
	{ NiveauX[index] = round ( NiveauX.Arrays[0][index] / arrondi) * arrondi ; return ; }

	if  ( NiveauX.Arrays[0][index] > NiveauX[index-1] + arrondi )
	{ NiveauX[index] = NiveauX[index-1] + arrondi ; }
	else if ( NiveauX.Arrays[0][index] < NiveauX[index-1] - arrondi )
	{ NiveauX[index] = NiveauX[index-1] - arrondi ; }
	else
	{ NiveauX[index] = NiveauX[index-1] ; }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int NiveauxIndicesAffiches_Statut(s_sc* ptrsc)    // vaut 0 (OFF), 1 (All), 2 (Custom), 3 (niveaux tradés), ou 4 (niveaux backtesté)
{
	int NiveauxIndicesAffiches_Statut = 0 ;
	int StudyIDPrixTypique = ptrsc->GetStudyIDByName(ptrsc->ChartNumber, "Prix typique", 0);
	if  (StudyIDPrixTypique == 0 )  
	{	SCString message ;  message.Format("!! ERREUR !! Le graphique ne contient pas la study ''Prix typique''. Necessaire pour determiner sc.HideStudy.") ; ptrsc->AddMessageToLog(message,1) ; }
	else
	{	ptrsc->GetChartStudyInputInt(ptrsc->ChartNumber, StudyIDPrixTypique, 5, NiveauxIndicesAffiches_Statut) ;  }    // int GetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int& r_IntegerValue);
	return NiveauxIndicesAffiches_Statut ;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
struct MonTexteString : s_UseTool // MonTexteString crée du texte avec un string
{											
	MonTexteString(SCString Texte, float niveau, int epaisseur, COLORREF rgb, s_sc* ptrsc) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
	{
		Clear(); 
		AddMethod = UTAM_ADD_ALWAYS;			
		DrawingType = DRAWING_TEXT;
		//BeginDateTime = 0;  // 0 = position à gauche ; 150 pour la position à droite
		//TextAlignment = DT_VCENTER | DT_LEFT ;
		BeginDateTime = 0;  // 0 = position à gauche ; 150 pour la position à droite
		TextAlignment = DT_VCENTER | DT_LEFT ;
		//FontBold = 1;
		// Color = RGB(220, 220, 220); //backgroundbackground gris du graphique
		Color = RGB(255, 255, 255); //backgroundbackground gris du graphique
		BeginValue = niveau;
		FontSize = epaisseur+6;
		FontBackColor = rgb;
		Text = Texte;			
		Region = ptrsc->GraphRegion ; 
	} 
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct MaLigneJusquAIndex : s_UseTool  // MaLigneJusquALastIndex crée une ligne entre index=beginIndex et index=endIndex
	{
		MaLigneJusquAIndex(int beginIndex, int endIndex, float niveau, int epaisseur, COLORREF rgb, s_sc* ptrsc) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
		{
			Clear(); 
			DrawingType = DRAWING_LINE;
			LineStyle = LINESTYLE_SOLID;
			AddMethod = UTAM_ADD_ALWAYS;	
			BeginIndex = beginIndex ;
			EndIndex = endIndex ;
			BeginValue = niveau ;
			EndValue = niveau ;
			LineWidth = epaisseur;
			Color = rgb; 
			Region = ptrsc->GraphRegion ; 
			DrawUnderneathMainGraph = true ;
		} 
	};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	float TopCoordinate ;		// la déclaration de ces deux variables ici permet d'éviter de les repasser en paramètre de toutes les fonctions creationLigneJusquAIndexEtTexte
	float BottomCoordinate ;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void creationLigneEtTexte (SCString Texte, int beginIndex, int endIndex, float niveau, int monEpaisseur, COLORREF maCouleur, s_sc* ptrsc)
	{
		if ( niveau > BottomCoordinate  and  niveau < TopCoordinate )
		{
			if ( monEpaisseur > 0  and  niveau > 500 )
			{
				//Création d'instance de MaLigneJusquAIndex et appel de sc.UseTool
				MaLigneJusquAIndex objetMaLigne(beginIndex, endIndex, niveau, monEpaisseur, maCouleur, ptrsc);
				ptrsc->UseTool(objetMaLigne); // c'est sc.UseTool qui est appelé
				//Création d'instances de MonTexte et appel de sc.UseTool
				MonTexteString objetMonTexteString(Texte, niveau, monEpaisseur, maCouleur, ptrsc);
				ptrsc->UseTool(objetMonTexteString); 
			}
		}
	}
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int PeriodeChartEnSecondes (s_sc* ptrsc)
{
	n_ACSIL :: s_BarPeriod BarPeriod ;
	ptrsc->GetBarPeriodParameters(BarPeriod) ;
	
	if ( BarPeriod.IntradayChartBarPeriodType == IBPT_DAYS_MINS_SECS  )      // cas du INQ100 : UT==20s ou UT==4min
	{
		return BarPeriod.IntradayChartBarPeriodParameter1 ;
	}
	else
	{
		{	SCString message ;  message.Format("!! ERREUR !! Le graphique n'est pas en unite de temps (BarPeriod.IntradayChartBarPeriodType != IBPT_DAYS_MINS_SECS)") ; ptrsc->AddMessageToLog(message,1) ; }
		return 0 ;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int NumberOfMonitors()		// Number of display monitors on desktop
{
	return GetSystemMetrics(80) ;		// https://docs.microsoft.com/fr-fr/windows/win32/api/winuser/nf-winuser-getsystemmetrics
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* int WindowsVersionNumber()		// retourne la version de Windows (multipliée par 10 pour ne pas avoir un float)		// WindowsVersionNumber vaut 61 pour mon PC de bureau et 62 pour mon VPS
{
	DWORD dwVersion = 0; 
	DWORD dwMajorVersion = 0;
	DWORD dwMinorVersion = 0; 
	DWORD dwBuild = 0;

	dwVersion = GetVersion();
 
	// Get the Windows version.
	dwMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
	dwMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));
	
	return dwMajorVersion*10+dwMinorVersion ;
}
 */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_MesPointsPivotsMensuels(SCStudyInterfaceRef sc)		// [1]
{
	SCSubgraphRef OpenGap = sc.Subgraph[0];   
	SCSubgraphRef PHAbsolu = sc.Subgraph[1];		// appelé par Navig_niveaux pour GenerationFichier_csv
	SCSubgraphRef PHPrior = sc.Subgraph[2];    		// appelé par Navig_niveaux pour GenerationFichier_csv
	SCSubgraphRef PP = sc.Subgraph[3];					// appelé par Navig_niveaux pour GenerationFichier_csv
	SCSubgraphRef R1 = sc.Subgraph[4];					// ...
	SCSubgraphRef R2 = sc.Subgraph[5];
	SCSubgraphRef R3 = sc.Subgraph[6];
	SCSubgraphRef R4 = sc.Subgraph[7];
	SCSubgraphRef S1 = sc.Subgraph[8];
	SCSubgraphRef S2 = sc.Subgraph[9];
	SCSubgraphRef S3 = sc.Subgraph[10];
	SCSubgraphRef S4 = sc.Subgraph[11];
	SCSubgraphRef mR1 = sc.Subgraph[12];
	SCSubgraphRef mR2 = sc.Subgraph[13];
	SCSubgraphRef mR3 = sc.Subgraph[14];
	SCSubgraphRef mR4 = sc.Subgraph[15];
	SCSubgraphRef mS1 = sc.Subgraph[16];
	SCSubgraphRef mS2 = sc.Subgraph[17];
	SCSubgraphRef mS3 = sc.Subgraph[18];
	SCSubgraphRef mS4 = sc.Subgraph[19];
	SCSubgraphRef RQ1 = sc.Subgraph[20];
	SCSubgraphRef RQ2 = sc.Subgraph[21];
	SCSubgraphRef RQ3 = sc.Subgraph[22];
	SCSubgraphRef RQ4 = sc.Subgraph[23];
	SCSubgraphRef RQ5 = sc.Subgraph[24];
	SCSubgraphRef RQ6 = sc.Subgraph[25];
	SCSubgraphRef RQ7 = sc.Subgraph[26];
	SCSubgraphRef RQ8 = sc.Subgraph[27];
	SCSubgraphRef SQ1 = sc.Subgraph[28];
	SCSubgraphRef SQ2 = sc.Subgraph[29];
	SCSubgraphRef SQ3 = sc.Subgraph[30];
	SCSubgraphRef SQ4 = sc.Subgraph[31];
	SCSubgraphRef SQ5 = sc.Subgraph[32];
	SCSubgraphRef SQ6 = sc.Subgraph[33];
	SCSubgraphRef SQ7 = sc.Subgraph[34];
	SCSubgraphRef SQ8 = sc.Subgraph[35];
	SCSubgraphRef SettlePrior = sc.Subgraph[36];
	SCSubgraphRef DemiGap = sc.Subgraph[37];   
	SCSubgraphRef PBPrior = sc.Subgraph[38];			// appelé par Navig_niveaux pour GenerationFichier_csv
	const int NUMBER_OF_STUDY_SUBGRAPHS = 39; 


	SCFloatArrayRef OpenMensuel = sc.Subgraph[0].Arrays[0] ; 		// appelé par scsf_MesPointsPivotsHebdosProjetes
	SCFloatArrayRef FirstIndex = sc.Subgraph[0].Arrays[1] ;  			// Premier index de la semaine

	
	SCInputRef ExternalChartNumber = sc.Input[0];
	SCInputRef CalculForce = sc.Input[1];
	SCInputRef TraceVectorielONOFF = sc.Input[2];								// modifié par Prix_typique 		
	SCInputRef AffichageDesQuartsONOFF = sc.Input[3];						// modifié par Prix_typique 		
	SCInputRef AffichageDesMid4Full4ONOFF = sc.Input[4];					// modifié par Prix_typique 		
	SCInputRef AffichageDesPHPBBougiePrecedente = sc.Input[5];		// modifié par Prix_typique 		
	
	
	int& NbMonitors = sc.GetPersistentIntFast(0);  
	int& FirstVisibleBar = sc.GetPersistentIntFast(1);  
	int& LastVisibleBar = sc.GetPersistentIntFast(2);  
	int& ShiftFirstIndex = sc.GetPersistentIntFast(3);  

	float& BottomCoordinatePrevious = sc.GetPersistentFloatFast(0);  
	float& TopCoordinatePrevious = sc.GetPersistentFloatFast(1);  


	if (sc.SetDefaults)
	{
		sc.AutoLoop = 0;  
		sc.GraphName = "[1] Pivots Ind M (Full/Mid/Quart)";      // appelé par Navig_niveaux pour GenerationFichier_csv		// modifié par PrixTypique
		sc.ScaleRangeType = SCALE_SAMEASREGION;
		sc.ValueFormat = VALUEFORMAT_INHERITED;
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 0 ;
		sc.DisplayStudyInputValues = 0;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		
		sc.HideStudy = 1 ;


		// ExternalChartNumber.Name = "Monthly Chart Number";
		ExternalChartNumber.SetChartNumber(1);
		CalculForce.Name = "Calcul force des pivots M";  // calcul forcé des PP à 16h04 (utile le dernier jour du mois pour calculer les PP du mois suivant)
		CalculForce.SetYesNo(0);
		TraceVectorielONOFF.Name = "Trace Vectoriel ON/OFF";
		TraceVectorielONOFF.SetYesNo(1);
		AffichageDesQuartsONOFF.Name = "Affichage des Quarts";		
		AffichageDesQuartsONOFF.SetYesNo(1);
		AffichageDesMid4Full4ONOFF.Name = "Affichage des Mid4 et Full4";	
		AffichageDesMid4Full4ONOFF.SetYesNo(1);
		AffichageDesPHPBBougiePrecedente.Name = "Affichage des PH et PB du mois precedent";	
		AffichageDesPHPBBougiePrecedente.SetYesNo(1);
		
		// DisplayOrder des inputs
		TraceVectorielONOFF.DisplayOrder = 1 ;

		
		

		for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].DrawStyle = DRAWSTYLE_DASH;
			sc.Subgraph[SubgraphIndex].PrimaryColor = RGB(0,0,0);
			sc.Subgraph[SubgraphIndex].LineLabel = LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_LEFT_EDGE | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_CENTER | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
			sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre
			sc.Subgraph[SubgraphIndex].DrawZeros = false;
			sc.Subgraph[SubgraphIndex].LineStyle = LINESTYLE_SOLID ;
		}

		SettlePrior.DrawStyle = DRAWSTYLE_HIDDEN;
		
		PHPrior.PrimaryColor = RGB(255, 0, 0);
		PBPrior.PrimaryColor = RGB(0,168,0);
		SettlePrior.PrimaryColor = RGB(255,0,255);
		PHAbsolu.PrimaryColor = RGB(255, 0, 0);

		OpenGap.PrimaryColor = RGB(210,0,210);
		OpenGap.LineLabel =                                 LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_LEFT_EDGE  | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_CENTER | LL_VALUE_ALIGN_VALUES_SCALE | LL_NAME_ALIGN_ABOVE; 

		DemiGap.PrimaryColor = RGB(210,0,210);


		OpenGap.Name = "                         OpenGap M";		
		PHAbsolu.Name = "                                   / PH Absolu";
		PHPrior.Name = "                      / PH MP";
		PP.Name = "                        PP M";
		R1.Name = "                        R1 M";
		R2.Name = "                        R2 M";
		R3.Name = "                        R3 M";
		R4.Name = "                        R4 M";
		S1.Name = "                        S1 M";
		S2.Name = "                        S2 M";
		S3.Name = "                        S3 M";
		S4.Name = "                        S4 M";
		mR1.Name = "                        mR1 M";
		mR2.Name = "                        mR2 M";
		mR3.Name = "                        mR3 M";
		mR4.Name = "                        mR4 M";
		mS1.Name = "                        mS1 M";
		mS2.Name = "                        mS2 M";
		mS3.Name = "                        mS3 M";
		mS4.Name = "                        mS4 M";
		RQ1.Name = "                        RQ1 M";
		RQ2.Name = "                        RQ2 M";
		RQ3.Name = "                        RQ3 M";
		RQ4.Name = "                        RQ4 M";
		RQ5.Name = "                        RQ5 M";
		RQ6.Name = "                        RQ6 M";
		RQ7.Name = "                        RQ7 M";
		RQ8.Name = "                        RQ8 M";
		SQ1.Name = "                        SQ1 M";
		SQ2.Name = "                        SQ2 M";
		SQ3.Name = "                        SQ3 M";
		SQ4.Name = "                        SQ4 M";
		SQ5.Name = "                        SQ5 M";
		SQ6.Name = "                        SQ6 M";
		SQ7.Name = "                        SQ7 M";
		SQ8.Name = "                        SQ8 M";
		SettlePrior.Name = "Settle MP";
		DemiGap.Name = "    DemiGap M";		// 4 espaces de plus que la version J
		PBPrior.Name = "                      / PB MP";
		
		
		
		PHPrior.LineWidth = 4;
		PBPrior.LineWidth = 4;
		SettlePrior.LineWidth = 0;
		PP.LineWidth = 5;
		R1.LineWidth = 5;
		R2.LineWidth = 5;
		R3.LineWidth = 5;
		R4.LineWidth = 5;
		S1.LineWidth = 5;
		S2.LineWidth = 5;
		S3.LineWidth = 5;
		S4.LineWidth = 5;
		mR1.LineWidth = 4;
		mR2.LineWidth = 4;
		mR3.LineWidth = 4;
		mR4.LineWidth = 4;
		mS1.LineWidth = 4;
		mS2.LineWidth = 4;
		mS3.LineWidth = 4;
		mS4.LineWidth = 4;
		RQ1.LineWidth = 3;
		RQ2.LineWidth = 3;
		RQ3.LineWidth = 3;
		RQ4.LineWidth = 3;
		RQ5.LineWidth = 3;
		RQ6.LineWidth = 3;
		RQ7.LineWidth = 3;
		RQ8.LineWidth = 3;
		SQ1.LineWidth = 3;
		SQ2.LineWidth = 3;
		SQ3.LineWidth = 3;
		SQ4.LineWidth = 3;
		SQ5.LineWidth = 3;
		SQ6.LineWidth = 3;
		SQ7.LineWidth = 3;
		SQ8.LineWidth = 3;
		PHAbsolu.LineWidth = 4;
		OpenGap.LineWidth = 8;
		DemiGap.LineWidth = 3;
		
		return;
	}  // fin du bloc de " if (sc.SetDefaults) "
	
	if ( sc.UpdateStartIndex == 0 )
	{
		sc.GraphRegion = 0 ;
		NbMonitors = NumberOfMonitors() ;
		FirstVisibleBar = 0 ;
		LastVisibleBar = 0 ;
		
		// ShiftFirstIndex
		int PeriodeChart = PeriodeChartEnSecondes (&sc) ;
		if ( PeriodeChart == 20 )  ShiftFirstIndex = 34 ;
		else if ( PeriodeChart == 240 )  ShiftFirstIndex = 3 ;

		// Résolution de l'écran => Décalages des affichages des noms
		int LargeurEnPixels = sc.StudyRegionRightCoordinate - sc.StudyRegionLeftCoordinate ;		// The top left pixel coordinate of the chart is at 0, 0. These coordinates increase moving towards the bottom and right.
		if ( LargeurEnPixels > 1900 )		// 2400 est le nb de pixels dans la config standard sur écran QHD, 1735 (session PC bureau ouvert sur PC portable) sur config Full HD		
		{ 	 // Config QHD
			OpenGap.Name = "                         OpenGap M";		
			PHAbsolu.Name = "                                   / PH Absolu";
			PHPrior.Name = "                      / PH MP";
			PP.Name = "                        PP M";
			R1.Name = "                        R1 M";
			R2.Name = "                        R2 M";
			R3.Name = "                        R3 M";
			R4.Name = "                        R4 M";
			S1.Name = "                        S1 M";
			S2.Name = "                        S2 M";
			S3.Name = "                        S3 M";
			S4.Name = "                        S4 M";
			mR1.Name = "                        mR1 M";
			mR2.Name = "                        mR2 M";
			mR3.Name = "                        mR3 M";
			mR4.Name = "                        mR4 M";
			mS1.Name = "                        mS1 M";
			mS2.Name = "                        mS2 M";
			mS3.Name = "                        mS3 M";
			mS4.Name = "                        mS4 M";
			RQ1.Name = "                        RQ1 M";
			RQ2.Name = "                        RQ2 M";
			RQ3.Name = "                        RQ3 M";
			RQ4.Name = "                        RQ4 M";
			RQ5.Name = "                        RQ5 M";
			RQ6.Name = "                        RQ6 M";
			RQ7.Name = "                        RQ7 M";
			RQ8.Name = "                        RQ8 M";
			SQ1.Name = "                        SQ1 M";
			SQ2.Name = "                        SQ2 M";
			SQ3.Name = "                        SQ3 M";
			SQ4.Name = "                        SQ4 M";
			SQ5.Name = "                        SQ5 M";
			SQ6.Name = "                        SQ6 M";
			SQ7.Name = "                        SQ7 M";
			SQ8.Name = "                        SQ8 M";
			SettlePrior.Name = "Settle MP";
			DemiGap.Name = "    DemiGap M";		// 4 espaces de plus que la version J
			PBPrior.Name = "                      / PB MP";
		}		
		else 
		{ 	 // Config Full HD 
			OpenGap.Name = "               OpenGap M";		
			PHAbsolu.Name = "                                   / PH Absolu";
			PHPrior.Name = "                      / PH MP";
			PP.Name = "                        PP M";
			R1.Name = "                        R1 M";
			R2.Name = "                        R2 M";
			R3.Name = "                        R3 M";
			R4.Name = "                        R4 M";
			S1.Name = "                        S1 M";
			S2.Name = "                        S2 M";
			S3.Name = "                        S3 M";
			S4.Name = "                        S4 M";
			mR1.Name = "                        mR1 M";
			mR2.Name = "                        mR2 M";
			mR3.Name = "                        mR3 M";
			mR4.Name = "                        mR4 M";
			mS1.Name = "                        mS1 M";
			mS2.Name = "                        mS2 M";
			mS3.Name = "                        mS3 M";
			mS4.Name = "                        mS4 M";
			RQ1.Name = "                        RQ1 M";
			RQ2.Name = "                        RQ2 M";
			RQ3.Name = "                        RQ3 M";
			RQ4.Name = "                        RQ4 M";
			RQ5.Name = "                        RQ5 M";
			RQ6.Name = "                        RQ6 M";
			RQ7.Name = "                        RQ7 M";
			RQ8.Name = "                        RQ8 M";
			SQ1.Name = "                        SQ1 M";
			SQ2.Name = "                        SQ2 M";
			SQ3.Name = "                        SQ3 M";
			SQ4.Name = "                        SQ4 M";
			SQ5.Name = "                        SQ5 M";
			SQ6.Name = "                        SQ6 M";
			SQ7.Name = "                        SQ7 M";
			SQ8.Name = "                        SQ8 M";
			SettlePrior.Name = "Settle MP";
			DemiGap.Name = "    DemiGap M";		// 4 espaces de plus que la version J
			PBPrior.Name = "                      / PB MP";
		}		

		// Affichage des quarts, des Mid4Full4 et des PH/PB : par défaut on affiche tout, puis on met  "LineWidth = 0" à ce qui ne doit pas être affiché
		{
			RQ1.LineWidth = 3;
			RQ2.LineWidth = 3;
			RQ3.LineWidth = 3;
			RQ4.LineWidth = 3;
			RQ5.LineWidth = 3;
			RQ6.LineWidth = 3;
			RQ7.LineWidth = 3;
			RQ8.LineWidth = 3;
			SQ1.LineWidth = 3;
			SQ2.LineWidth = 3;
			SQ3.LineWidth = 3;
			SQ4.LineWidth = 3;
			SQ5.LineWidth = 3;
			SQ6.LineWidth = 3;
			SQ7.LineWidth = 3;
			SQ8.LineWidth = 3;
			mR4.LineWidth = 4;
			mS4.LineWidth = 4;
			R4.LineWidth = 5;
			S4.LineWidth = 5;
			PHPrior.LineWidth = 4;
			PBPrior.LineWidth = 4;
			SettlePrior.LineWidth = 0;
			PHAbsolu.LineWidth = 4;
			OpenGap.LineWidth = 6;
			DemiGap.LineWidth = 2;
		}
		
		// Quarts non affichés
		if ( AffichageDesQuartsONOFF.GetYesNo() == 0 )
		{
			RQ1.LineWidth = 0;
			RQ2.LineWidth = 0;
			RQ3.LineWidth = 0;
			RQ4.LineWidth = 0;
			RQ5.LineWidth = 0;
			RQ6.LineWidth = 0;
			RQ7.LineWidth = 0;
			RQ8.LineWidth = 0;
			SQ1.LineWidth = 0;
			SQ2.LineWidth = 0;
			SQ3.LineWidth = 0;
			SQ4.LineWidth = 0;
			SQ5.LineWidth = 0;
			SQ6.LineWidth = 0;
			SQ7.LineWidth = 0;
			SQ8.LineWidth = 0;
		}
		
		// Mid4/Full4 non affichés
		if ( AffichageDesMid4Full4ONOFF.GetYesNo() == 0 )
		{
			RQ7.LineWidth = 0;
			RQ8.LineWidth = 0;
			SQ7.LineWidth = 0;
			SQ8.LineWidth = 0;
			mR4.LineWidth = 0;
			mS4.LineWidth = 0;
			R4.LineWidth = 0;
			S4.LineWidth = 0;
		}
		
		// PH/PB non affichés
		if ( AffichageDesPHPBBougiePrecedente.GetYesNo() == 0 )
		{
			PHPrior.LineWidth = 0;
			PBPrior.LineWidth = 0;
			SettlePrior.LineWidth = 0;
			PHAbsolu.LineWidth = 0;
			OpenGap.LineWidth = 5;
			DemiGap.LineWidth = 0;
		}

	}	// fin de " if ( sc.UpdateStartIndex == 0 ) "

	
	for (int index = sc.UpdateStartIndex; index < sc.ArraySize; index++)                 // début AutoLoop
	{
	
		int OpenTimeValue = sc.BaseDateTimeIn[index].GetTimeInSecondsWithoutMilliseconds()  ; //GetTimeInSecondsWithoutMilliseconds retourne une time_value
		
		if (  CalculForce.GetYesNo()  and  OpenTimeValue ==  57840  )   // CALCUL FORCE DES PP du mois suivant, réalisé à 16h04 
		{
			FirstIndex[index] = index ;
			
			SCGraphData ExternalChartData;
			sc.GetChartBaseData(ExternalChartNumber.GetChartNumber(), ExternalChartData);
			int ExternalChartIndex = sc.GetContainingIndexForDateTimeIndex(ExternalChartNumber.GetChartNumber(), index); // on récupère l'index du chandelier du mois actif

			float fPHPrior = 0, fPBPrior = 0 , fSettlePrior = 0, fPP = 0 ;
			float fR1 = 0, fR2 = 0, fR3 = 0, fR4 = 0, fS1 = 0, fS2 = 0, fS3 = 0, fS4 = 0 ;
			float fmR1 = 0, fmR2 = 0, fmR3 = 0, fmR4 = 0, fmS1 = 0, fmS2 = 0,  fmS3 = 0,   fmS4 = 0 ; 
			float fRQ1 = 0, fRQ2 = 0, fRQ3 = 0, fRQ4 = 0, fRQ5 = 0, fRQ6 = 0, fRQ7 = 0, fRQ8 = 0 ;
			float fSQ1 = 0, fSQ2 = 0, fSQ3 = 0, fSQ4 = 0, fSQ5 = 0, fSQ6 = 0, fSQ7 = 0, fSQ8 = 0 ;

			// on récupère HLC
			fPHPrior = ExternalChartData[SC_HIGH][ExternalChartIndex];
			fPBPrior = ExternalChartData[SC_LOW][ExternalChartIndex];
			fSettlePrior = ExternalChartData[SC_LAST][ExternalChartIndex];
			
			// correction des bugs de flux
			SCDateTimeArray ExternalChartDateTime;
			sc.GetChartDateTimeArray(ExternalChartNumber.GetChartNumber(), ExternalChartDateTime);
			int Year, Month, Day;
			ExternalChartDateTime[ExternalChartIndex].GetDateYMD(Year, Month, Day);
		/* 	if (Year==2019 and Month==1 and Day==31)  // premier jour de la semaine pour lequel HLC est faux
			{
				//fPHPrior = nd;
				fPBPrior = nd;
				//fSettlePrior = nd;
			} */
			
			//on calcule les PP 
			CalculatePPs (fPHPrior, fPBPrior, fSettlePrior, fPP, fR1, fR2, fR3, fR4, fS1, fS2, fS3, fS4, fmR1, fmR2, fmR3, fmR4, fmS1, fmS2, fmS3, fmS4, fRQ1, fRQ2, fRQ3, fRQ4, fRQ5, fRQ6, fRQ7, fRQ8, fSQ1, fSQ2, fSQ3, fSQ4, fSQ5, fSQ6, fSQ7, fSQ8);
			
			// les valeurs sont entrées dans les sc.Subgraph[]
			PHPrior[index] = fPHPrior;
			PBPrior[index] = fPBPrior;
			SettlePrior[index] = fSettlePrior;
			PP[index] = fPP;
			R1[index] = fR1;
			R2[index] = fR2;
			R3[index] = fR3;
			R4[index] = fR4;
			S1[index] = fS1;
			S2[index] = fS2;
			S3[index] = fS3;
			S4[index] = fS4;
			mR1[index] = fmR1;
			mR2[index] = fmR2;
			mR3[index] = fmR3;
			mR4[index] = fmR4;
			mS1[index] = fmS1;
			mS2[index] = fmS2;
			mS3[index] = fmS3;
			mS4[index] = fmS4;
			RQ1[index] = fRQ1;
			RQ2[index] = fRQ2;
			RQ3[index] = fRQ3;
			RQ4[index] = fRQ4;
			RQ5[index] = fRQ5;
			RQ6[index] = fRQ6;
			RQ7[index] = fRQ7;
			RQ8[index] = fRQ8;
			SQ1[index] = fSQ1;
			SQ2[index] = fSQ2;
			SQ3[index] = fSQ3;
			SQ4[index] = fSQ4;
			SQ5[index] = fSQ5;
			SQ6[index] = fSQ6;
			SQ7[index] = fSQ7;
			SQ8[index] = fSQ8;
			PHAbsolu[index] = Highest (ExternalChartData, ExternalChartIndex) ;    // PHAbsolu est calculé sur le plus haut absolu des mois précédents (ceci exclut le mois en cours)
			OpenGap[index] = 0;
			DemiGap[index] = 0;
			OpenMensuel[index] = 0;
		}

		else if (    sc.BaseDateTimeIn[index].GetMonth()  !=   sc.BaseDateTimeIn[index-1].GetMonth()     or   index==0   )   // ie on change de mois   ou   on est sur le premier index
		{
			FirstIndex[index] = index - ShiftFirstIndex ;
			
			SCGraphData ExternalChartData;
			sc.GetChartBaseData(ExternalChartNumber.GetChartNumber(), ExternalChartData);
			int ExternalChartIndex = sc.GetContainingIndexForDateTimeIndex(ExternalChartNumber.GetChartNumber(), index); // on récupère l'index du nouveau chandelier

			float fPHPrior = 0, fPBPrior = 0 , fSettlePrior = 0, fPP = 0 ;
			float fR1 = 0, fR2 = 0, fR3 = 0, fR4 = 0, fS1 = 0, fS2 = 0, fS3 = 0, fS4 = 0 ;
			float fmR1 = 0, fmR2 = 0, fmR3 = 0, fmR4 = 0, fmS1 = 0, fmS2 = 0,  fmS3 = 0,   fmS4 = 0 ; 
			float fRQ1 = 0, fRQ2 = 0, fRQ3 = 0, fRQ4 = 0, fRQ5 = 0, fRQ6 = 0, fRQ7 = 0, fRQ8 = 0 ;
			float fSQ1 = 0, fSQ2 = 0, fSQ3 = 0, fSQ4 = 0, fSQ5 = 0, fSQ6 = 0, fSQ7 = 0, fSQ8 = 0 ;
			
			// on récupère HLC
			fPHPrior = ExternalChartData[SC_HIGH][ExternalChartIndex-1];
			fPBPrior = ExternalChartData[SC_LOW][ExternalChartIndex-1];
			fSettlePrior = ExternalChartData[SC_LAST][ExternalChartIndex-1];
			
			// correction des bugs de flux
			SCDateTimeArray ExternalChartDateTime;
			sc.GetChartDateTimeArray(ExternalChartNumber.GetChartNumber(), ExternalChartDateTime);
			int Year, Month, Day;
			ExternalChartDateTime[ExternalChartIndex-1].GetDateYMD(Year, Month, Day);
			if (Year==2019 and Month==11 and Day==25)  // premier jour de la semaine pour lequel HLC est faux   // ve 29/11/2019 est cloturé à 13h
			{
				FirstIndex[index] = index - ShiftFirstIndex ;
				//fPHPrior = nd;
				// fPBPrior = nd;
				//fSettlePrior = nd;
			}
			
			//on calcule les PP 
			CalculatePPs (fPHPrior, fPBPrior, fSettlePrior, fPP, fR1, fR2, fR3, fR4, fS1, fS2, fS3, fS4, fmR1, fmR2, fmR3, fmR4, fmS1, fmS2, fmS3, fmS4, fRQ1, fRQ2, fRQ3, fRQ4, fRQ5, fRQ6, fRQ7, fRQ8, fSQ1, fSQ2, fSQ3, fSQ4, fSQ5, fSQ6, fSQ7, fSQ8);
			
			// les valeurs sont entrées dans les sc.Subgraph[]
			PHPrior[index] = fPHPrior;
			PBPrior[index] = fPBPrior;
			SettlePrior[index] = fSettlePrior;
			PP[index] = fPP;
			R1[index] = fR1;
			R2[index] = fR2;
			R3[index] = fR3;
			R4[index] = fR4;
			S1[index] = fS1;
			S2[index] = fS2;
			S3[index] = fS3;
			S4[index] = fS4;
			mR1[index] = fmR1;
			mR2[index] = fmR2;
			mR3[index] = fmR3;
			mR4[index] = fmR4;
			mS1[index] = fmS1;
			mS2[index] = fmS2;
			mS3[index] = fmS3;
			mS4[index] = fmS4;
			RQ1[index] = fRQ1;
			RQ2[index] = fRQ2;
			RQ3[index] = fRQ3;
			RQ4[index] = fRQ4;
			RQ5[index] = fRQ5;
			RQ6[index] = fRQ6;
			RQ7[index] = fRQ7;
			RQ8[index] = fRQ8;
			SQ1[index] = fSQ1;
			SQ2[index] = fSQ2;
			SQ3[index] = fSQ3;
			SQ4[index] = fSQ4;
			SQ5[index] = fSQ5;
			SQ6[index] = fSQ6;
			SQ7[index] = fSQ7;
			SQ8[index] = fSQ8;
			PHAbsolu[index] = Highest (ExternalChartData, ExternalChartIndex) ;    // PHAbsolu est calculé sur le plus haut absolu des mois précédents (ceci exclut le mois en cours)
			OpenMensuel[index] = sc.Open[index];
			
			// Recherche d'un opengap dans la première bougie du jour
			if (   sc.Open[index] > fPHPrior   )      // gap haussier  
			{
				OpenGap[index] = fPHPrior;
				DemiGap[index] = (sc.Open[index] + fPHPrior)/2;
			}
			else if (   ( sc.Open[index] < fPBPrior )    )     //    gap baissier
			{
				OpenGap[index] = fPBPrior;
				DemiGap[index] = (sc.Open[index] + fPBPrior)/2;
			}
			else    // pas de gap
			{
				OpenGap[index] = 0;
				DemiGap[index] = 0;
			}
		}
	 
		else // on recopie les valeurs de index-1
		{
			FirstIndex[index] = FirstIndex[index-1] ;
			PHPrior[index] = PHPrior[index-1] ;
			PBPrior[index] = PBPrior[index-1] ;
			SettlePrior[index] = SettlePrior[index-1] ;
			PP[index] = PP[index-1];
			R1[index] = R1[index-1];
			R2[index] = R2[index-1];
			R3[index] = R3[index-1];
			R4[index] = R4[index-1];
			S1[index] = S1[index-1];
			S2[index] = S2[index-1];
			S3[index] = S3[index-1];
			S4[index] = S4[index-1];
			mR1[index] = mR1[index-1];
			mR2[index] = mR2[index-1];
			mR3[index] = mR3[index-1];
			mR4[index] = mR4[index-1];
			mS1[index] = mS1[index-1];
			mS2[index] = mS2[index-1];
			mS3[index] = mS3[index-1];
			mS4[index] = mS4[index-1];
			RQ1[index] = RQ1[index-1];
			RQ2[index] = RQ2[index-1];
			RQ3[index] = RQ3[index-1];
			RQ4[index] = RQ4[index-1];
			RQ5[index] = RQ5[index-1];
			RQ6[index] = RQ6[index-1];
			RQ7[index] = RQ7[index-1];
			RQ8[index] = RQ8[index-1];
			SQ1[index] = SQ1[index-1];
			SQ2[index] = SQ2[index-1];
			SQ3[index] = SQ3[index-1];
			SQ4[index] = SQ4[index-1];
			SQ5[index] = SQ5[index-1];
			SQ6[index] = SQ6[index-1];
			SQ7[index] = SQ7[index-1];
			SQ8[index] = SQ8[index-1];
			PHAbsolu[index] = PHAbsolu[index-1];
			OpenGap[index] = OpenGap[index-1];
			DemiGap[index] = DemiGap[index-1];
			OpenMensuel[index] = OpenMensuel[index-1];
		}
	}
	

	// Bloc d'affichage       
	if ( sc.ChartWindowIsActive  or  NbMonitors == 2  or  sc.IsFullRecalculation )
	{
		BottomCoordinate = sc.YPixelCoordinateToGraphValue(sc.StudyRegionBottomCoordinate) ;		// la variable est déclarée en dehors de la fct scsf
		TopCoordinate = sc.YPixelCoordinateToGraphValue(sc.StudyRegionTopCoordinate) ;				// la variable est déclarée en dehors de la fct scsf
		if (  TraceVectorielONOFF.GetYesNo()  and  ( FirstVisibleBar != sc.IndexOfFirstVisibleBar  or  LastVisibleBar != sc.IndexOfLastVisibleBar  or  sc.IsFullRecalculation  or  fabs(BottomCoordinatePrevious-BottomCoordinate) > 0.02  or  fabs(TopCoordinatePrevious-TopCoordinate) > 0.02 )  )          	// Le recalcul des lignes n'est réalisé que si FirstIndex ou LastVisibleBar ou BottomCoordinate varie (on fait l'hypothèse que TopCoordinate et BottomCoordinate varient de manière synchronisée)		// Note : Sierra a un bug interne pour le premier calcul de BottomCoordinate (il vaut la même valeur que TopCoordinate)    // Il faut rajouter sc.IsFullRecalculation car lors d'une Full ReCalculation les useTool non UDD sont effacés
		{	
			FirstVisibleBar = sc.IndexOfFirstVisibleBar ;
			LastVisibleBar = sc.IndexOfLastVisibleBar ;
			BottomCoordinatePrevious = BottomCoordinate ;
			TopCoordinatePrevious = TopCoordinate ;
			
			// les useTool doivent être effacés à chaque appel du DLL pour éviter une fuite de mémoire   
			sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   // all non user drawn Chart Drawings on the specified ChartNumber will be deleted. The LineNumber parameter in this case is ignored and should be 0. 		
			
			s_sc* const ptrsc = &sc;
			int const firstIndex = FirstIndex[LastVisibleBar] ;  // index de la première bougie du mois : début du tracé de la ligne  // FirstIndex est un array qui contient le firstIndex
			
			for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)
			{
				float const Niveau = sc.Subgraph[SubgraphIndex][LastVisibleBar] ;
				SCString const Texte = sc.Subgraph[SubgraphIndex].Name ;
				int const epaisseur = sc.Subgraph[SubgraphIndex].LineWidth ;
				COLORREF const couleur = sc.Subgraph[SubgraphIndex].PrimaryColor;
				creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, Niveau, epaisseur, couleur, ptrsc);
			}

			// recalcul immédiat
			sc.UpdateAlways = 1 ;
		}
	}
	if ( TraceVectorielONOFF.GetYesNo() == 0 )  sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   

}
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_MesEighthsMensuels(SCStudyInterfaceRef sc)		// [2]
{
	SCSubgraphRef RE1 = sc.Subgraph[0];    // appelé par Navig_niveaux pour GenerationFichier_csv
	SCSubgraphRef RE2 = sc.Subgraph[1];	   
	SCSubgraphRef RE3 = sc.Subgraph[2];
	SCSubgraphRef RE4 = sc.Subgraph[3];
	SCSubgraphRef RE5 = sc.Subgraph[4];
	SCSubgraphRef RE6 = sc.Subgraph[5];
	SCSubgraphRef RE7 = sc.Subgraph[6];
	SCSubgraphRef RE8 = sc.Subgraph[7];
	SCSubgraphRef RE9 = sc.Subgraph[8];
	SCSubgraphRef RE10 = sc.Subgraph[9];
	SCSubgraphRef RE11 = sc.Subgraph[10];
	SCSubgraphRef RE12 = sc.Subgraph[11];
	SCSubgraphRef RE13 = sc.Subgraph[12];
	SCSubgraphRef RE14 = sc.Subgraph[13];
	SCSubgraphRef RE15 = sc.Subgraph[14];
	SCSubgraphRef RE16 = sc.Subgraph[15];
	SCSubgraphRef SE1 = sc.Subgraph[16];
	SCSubgraphRef SE2 = sc.Subgraph[17];
	SCSubgraphRef SE3 = sc.Subgraph[18];
	SCSubgraphRef SE4 = sc.Subgraph[19];
	SCSubgraphRef SE5 = sc.Subgraph[20];
	SCSubgraphRef SE6 = sc.Subgraph[21];
	SCSubgraphRef SE7 = sc.Subgraph[22];
	SCSubgraphRef SE8 = sc.Subgraph[23];
	SCSubgraphRef SE9 = sc.Subgraph[24];
	SCSubgraphRef SE10 = sc.Subgraph[25];
	SCSubgraphRef SE11 = sc.Subgraph[26];
	SCSubgraphRef SE12 = sc.Subgraph[27];
	SCSubgraphRef SE13 = sc.Subgraph[28];
	SCSubgraphRef SE14 = sc.Subgraph[29];
	SCSubgraphRef SE15 = sc.Subgraph[30];      
	SCSubgraphRef SE16 = sc.Subgraph[31];      
	const int NUMBER_OF_STUDY_SUBGRAPHS = 32;
	
	SCFloatArrayRef FirstIndex = sc.Subgraph[0].Arrays[0] ;  // Premier index du mois 

	SCInputRef ExternalChartNumber = sc.Input[0];
	SCInputRef CalculForce = sc.Input[1];
	SCInputRef TraceVectorielONOFF = sc.Input[2];								// modifié par Prix_typique
	SCInputRef AffichageDesMid4Full4ONOFF = sc.Input[4];					// modifié par Prix_typique 		
	

	int& NbMonitors = sc.GetPersistentIntFast(0);  
	int& FirstVisibleBar = sc.GetPersistentIntFast(1);  
	int& LastVisibleBar = sc.GetPersistentIntFast(2);  
	int& ShiftFirstIndex = sc.GetPersistentIntFast(3);  

	float& BottomCoordinatePrevious = sc.GetPersistentFloatFast(0);  
	float& TopCoordinatePrevious = sc.GetPersistentFloatFast(1);  	

	
	if (sc.SetDefaults)
	{
		sc.AutoLoop = 0;  
		sc.GraphName = "[2] Pivots Ind M (Eighth)";    // appelé par Navig_niveaux pour GenerationFichier_csv		// appelé par Prix_typique du chart #6
		sc.ScaleRangeType = SCALE_SAMEASREGION;
		sc.ValueFormat = VALUEFORMAT_INHERITED;
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 0 ;
		sc.DisplayStudyInputValues = 0;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		
		sc.HideStudy = 1 ;

		
		ExternalChartNumber.Name = "Monthly Chart Number";
		ExternalChartNumber.SetChartNumber(1);
		CalculForce.Name = "Calcul force des pivots M";  // calcul forcé des PP à 16h04 (utile le dernier jour du mois pour calculer les PP du mois suivant)
		CalculForce.SetYesNo(0);
		TraceVectorielONOFF.Name = "Trace Vectoriel ON/OFF";
		TraceVectorielONOFF.SetYesNo(1);

		// DisplayOrder des inputs
		TraceVectorielONOFF.DisplayOrder = 1 ;
		
		

		for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].DrawStyle = DRAWSTYLE_DASH;
			sc.Subgraph[SubgraphIndex].PrimaryColor = RGB(0,0,0);
			sc.Subgraph[SubgraphIndex].LineLabel = LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_LEFT_EDGE | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_CENTER | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
			sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre
			sc.Subgraph[SubgraphIndex].DrawZeros = false;
			sc.Subgraph[SubgraphIndex].LineStyle = LINESTYLE_SOLID ;
			sc.Subgraph[SubgraphIndex].LineWidth = 2 ;
		}

	
		RE1.Name = "                        RE1 M";
		RE2.Name = "                        RE2 M";
		RE3.Name = "                        RE3 M";
		RE4.Name = "                        RE4 M";
		RE5.Name = "                        RE5 M";
		RE6.Name = "                        RE6 M";
		RE7.Name = "                        RE7 M";
		RE8.Name = "                        RE8 M";
		RE9.Name = "                        RE9 M";
		RE10.Name = "                        RE10 M";
		RE11.Name = "                        RE11 M";
		RE12.Name = "                        RE12 M";
		RE13.Name = "                        RE13 M";
		RE14.Name = "                        RE14 M";
		RE14.Name = "                        RE14 M";
		RE15.Name = "                        RE15 M";
		RE16.Name = "                        RE16 M";
		SE1.Name = "                        SE1 M";
		SE2.Name = "                        SE2 M";
		SE3.Name = "                        SE3 M";
		SE4.Name = "                        SE4 M";
		SE5.Name = "                        SE5 M";
		SE6.Name = "                        SE6 M";
		SE7.Name = "                        SE7 M";
		SE8.Name = "                        SE8 M";
		SE9.Name = "                        SE9 M";
		SE10.Name = "                        SE10 M";
		SE11.Name = "                        SE11 M";
		SE12.Name = "                        SE12 M";
		SE13.Name = "                        SE13 M";
		SE14.Name = "                        SE14 M";
		SE14.Name = "                        SE14 M";
		SE15.Name = "                        SE15 M";
		SE16.Name = "                        SE16 M";
		
		return;
	}  // fin du bloc de " if (sc.SetDefaults) "
	
	
	if ( sc.UpdateStartIndex == 0 )
	{
		sc.GraphRegion = 0 ;
		NbMonitors = NumberOfMonitors() ;
		FirstVisibleBar = 0 ;
		LastVisibleBar = 0 ;

		int PeriodeChart = PeriodeChartEnSecondes (&sc) ;
		if ( PeriodeChart == 20 )  ShiftFirstIndex = 34 ;
		else if ( PeriodeChart == 240 )  ShiftFirstIndex = 3 ;
		
		// Affichage des eights liés à Mid4Full4 : par défaut on affiche tout, puis on met  "LineWidth = 0" à ce qui ne doit pas être affiché
		{
			RE13.LineWidth = 2;
			RE14.LineWidth = 2;
			RE15.LineWidth = 2;
			RE16.LineWidth = 2;
			SE13.LineWidth = 2;
			SE14.LineWidth = 2;
			SE15.LineWidth = 2;
			SE16.LineWidth = 2;
		}		
		
		// Mid4/Full4 non affichés
		if ( AffichageDesMid4Full4ONOFF.GetYesNo() == 0 )
		{
			RE13.LineWidth = 0;
			RE14.LineWidth = 0;
			RE15.LineWidth = 0;
			RE16.LineWidth = 0;
			SE13.LineWidth = 0;
			SE14.LineWidth = 0;
			SE15.LineWidth = 0;
			SE16.LineWidth = 0;
		}		
		
	}
	
	
	for (int index = sc.UpdateStartIndex; index < sc.ArraySize; index++)                 // début looping manuel
	{
		
		int OpenTimeValue = sc.BaseDateTimeIn[index].GetTimeInSecondsWithoutMilliseconds()  ; //GetTimeInSecondsWithoutMilliseconds retourne une time_value
		
		if (  CalculForce.GetYesNo()  and  OpenTimeValue ==  57840  )   // CALCUL DES PP du mois suivant, réalisé à 16h04
		{
			FirstIndex[index] = index ;
			
			SCGraphData ExternalChartData;
			sc.GetChartBaseData(ExternalChartNumber.GetChartNumber(), ExternalChartData);
			int ExternalChartIndex = sc.GetContainingIndexForDateTimeIndex(ExternalChartNumber.GetChartNumber(), index); // on récupère l'index du chandelier du mois actif
			
			float fPHPrior = 0, fPBPrior = 0 , fSettlePrior = 0 ;
			float fRE1, fRE2, fRE3, fRE4, fRE5, fRE6, fRE7, fRE8, fRE9, fRE10, fRE11, fRE12, fRE13, fRE14, fRE15, fRE16 ;
			float fSE1, fSE2, fSE3, fSE4, fSE5, fSE6, fSE7, fSE8, fSE9, fSE10, fSE11, fSE12, fSE13, fSE14, fSE15, fSE16 ;
				
			// on récupère HLC
			fPHPrior = ExternalChartData[SC_HIGH][ExternalChartIndex];
			fPBPrior = ExternalChartData[SC_LOW][ExternalChartIndex];
			fSettlePrior = ExternalChartData[SC_LAST][ExternalChartIndex];
			
			// correction des bugs de flux
			SCDateTimeArray ExternalChartDateTime;
			sc.GetChartDateTimeArray(ExternalChartNumber.GetChartNumber(), ExternalChartDateTime);
			int Year, Month, Day;
			ExternalChartDateTime[ExternalChartIndex-1].GetDateYMD(Year, Month, Day);
		/* 	if (Year==2019 and Month==1 and Day==31)  // premier jour de la semaine pour lequel HLC est faux
			{
				//fPHPrior = nd;
				//fPBPrior = nd;
				//fSettlePrior = nd;
			} */

			//on calcule les PP 
			CalculateEighths (	fPHPrior, fPBPrior, fSettlePrior, 
										fRE1, fRE2, fRE3, fRE4, fRE5, fRE6, fRE7, fRE8, fRE9, fRE10, fRE11, fRE12, fRE13, fRE14, fRE15, fRE16, 
										fSE1, fSE2, fSE3, fSE4, fSE5, fSE6, fSE7, fSE8, fSE9, fSE10, fSE11, fSE12, fSE13, fSE14, fSE15, fSE16 ) ;
			
			// les valeurs sont entrées dans les sc.Subgraph[]
			RE1[index] =  fRE1;
			RE2[index] = fRE2;
			RE3[index] = fRE3;
			RE4[index] = fRE4;
			RE5[index] = fRE5;
			RE6[index] = fRE6;
			RE7[index] = fRE7;
			RE8[index] = fRE8;
			RE9[index] = fRE9;
			RE10[index] = fRE10;
			RE11[index] = fRE11;
			RE12[index] = fRE12;
			RE13[index] = fRE13;
			RE14[index] = fRE14;
			RE14[index] = fRE14;
			RE15[index] = fRE15;
			RE16[index] = fRE16;
			SE1[index] = fSE1;
			SE2[index] = fSE2;
			SE3[index] = fSE3;
			SE4[index] = fSE4;
			SE5[index] = fSE5;
			SE6[index] = fSE6;
			SE7[index] = fSE7;
			SE8[index] = fSE8;
			SE9[index] = fSE9;
			SE10[index] = fSE10;
			SE11[index] = fSE11;
			SE12[index] = fSE12;
			SE13[index] = fSE13;
			SE14[index] = fSE14;
			SE14[index] = fSE14;
			SE15[index] = fSE15;
			SE16[index] = fSE16;
			
		}
		
		else if (    sc.BaseDateTimeIn[index].GetMonth()  !=   sc.BaseDateTimeIn[index-1].GetMonth()     or   index==0     )   // ie on change de mois 
		{
			FirstIndex[index] = index - 3 ;
			
			SCGraphData ExternalChartData;
			sc.GetChartBaseData(ExternalChartNumber.GetChartNumber(), ExternalChartData);
			int ExternalChartIndex = sc.GetContainingIndexForDateTimeIndex(ExternalChartNumber.GetChartNumber(), index); // on récupère l'index du nouveau chandelier

			float fPHPrior = 0, fPBPrior = 0 , fSettlePrior = 0 ;
			float fRE1, fRE2, fRE3, fRE4, fRE5, fRE6, fRE7, fRE8, fRE9, fRE10, fRE11, fRE12, fRE13, fRE14, fRE15, fRE16 ;
			float fSE1, fSE2, fSE3, fSE4, fSE5, fSE6, fSE7, fSE8, fSE9, fSE10, fSE11, fSE12, fSE13, fSE14, fSE15, fSE16 ;
				
			// on récupère HLC
			fPHPrior = ExternalChartData[SC_HIGH][ExternalChartIndex-1];
			fPBPrior = ExternalChartData[SC_LOW][ExternalChartIndex-1];
			fSettlePrior = ExternalChartData[SC_LAST][ExternalChartIndex-1];
			
			// correction des bugs de flux
			SCDateTimeArray ExternalChartDateTime;
			sc.GetChartDateTimeArray(ExternalChartNumber.GetChartNumber(), ExternalChartDateTime);
			int Year, Month, Day;
			ExternalChartDateTime[ExternalChartIndex-1].GetDateYMD(Year, Month, Day);
			if (Year==2019 and Month==11 and Day==25)  // premier jour de la semaine pour lequel HLC est faux   // ve 29/11/2019 est cloturé à 13h
			{
				FirstIndex[index] = index - ShiftFirstIndex ;
				//fPHPrior = nd;
				// fPBPrior = nd;
				//fSettlePrior = nd;
			}
			
			//on calcule les PP 
			CalculateEighths (	fPHPrior, fPBPrior, fSettlePrior, 
										fRE1, fRE2, fRE3, fRE4, fRE5, fRE6, fRE7, fRE8, fRE9, fRE10, fRE11, fRE12, fRE13, fRE14, fRE15, fRE16, 
										fSE1, fSE2, fSE3, fSE4, fSE5, fSE6, fSE7, fSE8, fSE9, fSE10, fSE11, fSE12, fSE13, fSE14, fSE15, fSE16 ) ;
			
			// les valeurs sont entrées dans les sc.Subgraph[]
			RE1[index] =  fRE1;
			RE2[index] = fRE2;
			RE3[index] = fRE3;
			RE4[index] = fRE4;
			RE5[index] = fRE5;
			RE6[index] = fRE6;
			RE7[index] = fRE7;
			RE8[index] = fRE8;
			RE9[index] = fRE9;
			RE10[index] = fRE10;
			RE11[index] = fRE11;
			RE12[index] = fRE12;
			RE13[index] = fRE13;
			RE14[index] = fRE14;
			RE14[index] = fRE14;
			RE15[index] = fRE15;
			RE16[index] = fRE16;
			SE1[index] = fSE1;
			SE2[index] = fSE2;
			SE3[index] = fSE3;
			SE4[index] = fSE4;
			SE5[index] = fSE5;
			SE6[index] = fSE6;
			SE7[index] = fSE7;
			SE8[index] = fSE8;
			SE9[index] = fSE9;
			SE10[index] = fSE10;
			SE11[index] = fSE11;
			SE12[index] = fSE12;
			SE13[index] = fSE13;
			SE14[index] = fSE14;
			SE14[index] = fSE14;
			SE15[index] = fSE15;
			SE16[index] = fSE16;
			
		}
		 
		else // on n'est pas dans le premier chandelier du jour
		{
			FirstIndex[index] = FirstIndex[index-1] ;
			RE1[index] =  RE1[index-1];
			RE2[index] = RE2[index-1];
			RE3[index] = RE3[index-1];
			RE4[index] = RE4[index-1];
			RE5[index] = RE5[index-1];
			RE6[index] = RE6[index-1];
			RE7[index] = RE7[index-1];
			RE8[index] = RE8[index-1];
			RE9[index] = RE9[index-1];
			RE10[index] = RE10[index-1];
			RE11[index] = RE11[index-1];
			RE12[index] = RE12[index-1];
			RE13[index] = RE13[index-1];
			RE14[index] = RE14[index-1];
			RE14[index] = RE14[index-1];
			RE15[index] = RE15[index-1];
			RE16[index] = RE16[index-1];
			SE1[index] = SE1[index-1];
			SE2[index] = SE2[index-1];
			SE3[index] = SE3[index-1];
			SE4[index] = SE4[index-1];
			SE5[index] = SE5[index-1];
			SE6[index] = SE6[index-1];
			SE7[index] = SE7[index-1];
			SE8[index] = SE8[index-1];
			SE9[index] = SE9[index-1];
			SE10[index] = SE10[index-1];
			SE11[index] = SE11[index-1];
			SE12[index] = SE12[index-1];
			SE13[index] = SE13[index-1];
			SE14[index] = SE14[index-1];
			SE14[index] = SE14[index-1];
			SE15[index] = SE15[index-1];
			SE16[index] = SE16[index-1];
		}
	}
	

	// Bloc d'affichage       
	if ( sc.ChartWindowIsActive  or  NbMonitors == 2  or  sc.IsFullRecalculation )
	{
		BottomCoordinate = sc.YPixelCoordinateToGraphValue(sc.StudyRegionBottomCoordinate) ;		// la variable est déclarée en dehors de la fct scsf
		TopCoordinate = sc.YPixelCoordinateToGraphValue(sc.StudyRegionTopCoordinate) ;				// la variable est déclarée en dehors de la fct scsf
		if (  TraceVectorielONOFF.GetYesNo()  and  ( FirstVisibleBar != sc.IndexOfFirstVisibleBar  or  LastVisibleBar != sc.IndexOfLastVisibleBar  or  sc.IsFullRecalculation  or  fabs(BottomCoordinatePrevious-BottomCoordinate) > 0.02  or  fabs(TopCoordinatePrevious-TopCoordinate) > 0.02 )  )          	// Le recalcul des lignes n'est réalisé que si FirstIndex ou LastVisibleBar ou BottomCoordinate varie (on fait l'hypothèse que TopCoordinate et BottomCoordinate varient de manière synchronisée)		// Note : Sierra a un bug interne pour le premier calcul de BottomCoordinate (il vaut la même valeur que TopCoordinate)    // Il faut rajouter sc.IsFullRecalculation car lors d'une Full ReCalculation les useTool non UDD sont effacés
		{	
			FirstVisibleBar = sc.IndexOfFirstVisibleBar ;
			LastVisibleBar = sc.IndexOfLastVisibleBar ;
			BottomCoordinatePrevious = BottomCoordinate ;
			TopCoordinatePrevious = TopCoordinate ;

			// les useTool doivent être effacés à chaque appel du DLL pour éviter une fuite de mémoire
			sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   // all non user drawn Chart Drawings on the specified ChartNumber will be deleted. The LineNumber parameter in this case is ignored and should be 0. 		
			
			s_sc* const ptrsc = &sc;
			int const firstIndex = FirstIndex[LastVisibleBar] ;  // index de la première bougie du mois : début du tracé de la ligne  // FirstIndex est un array qui contient le firstIndex
			
			for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)
			{
				float const Niveau = sc.Subgraph[SubgraphIndex][LastVisibleBar] ;
				SCString const Texte = sc.Subgraph[SubgraphIndex].Name ;
				int const epaisseur = sc.Subgraph[SubgraphIndex].LineWidth ;
				COLORREF const couleur = sc.Subgraph[SubgraphIndex].PrimaryColor;
				creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, Niveau, epaisseur, couleur, ptrsc);
			}
			
			// recalcul immédiat
			sc.UpdateAlways = 1 ;
		}
	}
	if ( TraceVectorielONOFF.GetYesNo() == 0 )  sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_MesSiXteenthsMensuels(SCStudyInterfaceRef sc)		// [3]
{
	SCSubgraphRef RX1 = sc.Subgraph[0]; 
	SCSubgraphRef RX2 = sc.Subgraph[1]; 
	SCSubgraphRef RX3 = sc.Subgraph[2];
	SCSubgraphRef RX4 = sc.Subgraph[3];
	SCSubgraphRef RX5 = sc.Subgraph[4];
	SCSubgraphRef RX6 = sc.Subgraph[5];
	SCSubgraphRef RX7 = sc.Subgraph[6];
	SCSubgraphRef RX8 = sc.Subgraph[7];
	SCSubgraphRef RX9 = sc.Subgraph[8];
	SCSubgraphRef RX10 = sc.Subgraph[9];
	SCSubgraphRef RX11 = sc.Subgraph[10];
	SCSubgraphRef RX12 = sc.Subgraph[11];
	SCSubgraphRef RX13 = sc.Subgraph[12];
	SCSubgraphRef RX14 = sc.Subgraph[13];
	SCSubgraphRef RX15 = sc.Subgraph[14];
	SCSubgraphRef RX16 = sc.Subgraph[15];
	SCSubgraphRef RX17 = sc.Subgraph[16];
	SCSubgraphRef RX18 = sc.Subgraph[17];
	SCSubgraphRef RX19 = sc.Subgraph[18];
	SCSubgraphRef RX20 = sc.Subgraph[19];
	SCSubgraphRef RX21 = sc.Subgraph[20];
	SCSubgraphRef RX22 = sc.Subgraph[21];
	SCSubgraphRef RX23 = sc.Subgraph[22];
	SCSubgraphRef RX24 = sc.Subgraph[23];
	SCSubgraphRef RX25 = sc.Subgraph[24];
	SCSubgraphRef RX26 = sc.Subgraph[25];
	SCSubgraphRef RX27 = sc.Subgraph[26];
	SCSubgraphRef RX28 = sc.Subgraph[27];
	SCSubgraphRef RX29 = sc.Subgraph[28];
	SCSubgraphRef RX30 = sc.Subgraph[29];
	//SCSubgraphRef RX31 = sc.Subgraph[30]; // fait partie des 4 SubGraphes non affichés (limite interne de SC = 60 SubGraphes)     Note : pour l'afficher, il suffit de lui donner un numéro <59
	//SCSubgraphRef RX32 = sc.Subgraph[31]; // fait partie des 4 SubGraphes non affichés (limite interne de SC = 60 SubGraphes) 	
	SCSubgraphRef SX1 = sc.Subgraph[32];
	SCSubgraphRef SX2 = sc.Subgraph[33];	
	SCSubgraphRef SX3 = sc.Subgraph[34];
	SCSubgraphRef SX4 = sc.Subgraph[35];
	SCSubgraphRef SX5 = sc.Subgraph[36];
	SCSubgraphRef SX6 = sc.Subgraph[37];
	SCSubgraphRef SX7 = sc.Subgraph[38];
	SCSubgraphRef SX8 = sc.Subgraph[39];
	SCSubgraphRef SX9 = sc.Subgraph[40];
	SCSubgraphRef SX10 = sc.Subgraph[41];
	SCSubgraphRef SX11 = sc.Subgraph[42];
	SCSubgraphRef SX12 = sc.Subgraph[43];
	SCSubgraphRef SX13 = sc.Subgraph[44];
	SCSubgraphRef SX14 = sc.Subgraph[45];
	SCSubgraphRef SX15 = sc.Subgraph[46];
	SCSubgraphRef SX16 = sc.Subgraph[47];
	SCSubgraphRef SX17 = sc.Subgraph[48];
	SCSubgraphRef SX18 = sc.Subgraph[49];
	SCSubgraphRef SX19 = sc.Subgraph[50];
	SCSubgraphRef SX20 = sc.Subgraph[51];
	SCSubgraphRef SX21 = sc.Subgraph[52];
	SCSubgraphRef SX22 = sc.Subgraph[53];
	SCSubgraphRef SX23 = sc.Subgraph[54];
	SCSubgraphRef SX24 = sc.Subgraph[55];
	SCSubgraphRef SX25 = sc.Subgraph[56];
	SCSubgraphRef SX26 = sc.Subgraph[57];
	SCSubgraphRef SX27 = sc.Subgraph[58];
	SCSubgraphRef SX28 = sc.Subgraph[59]; 
	SCSubgraphRef SX29 = sc.Subgraph[30]; 
	SCSubgraphRef SX30 = sc.Subgraph[31]; 
	//SCSubgraphRef SX31 = sc.Subgraph[62]; // fait partie des 4 SubGraphes non affichés (limite interne de SC = 60 SubGraphes)
	//SCSubgraphRef SX32 = sc.Subgraph[63]; // fait partie des 4 SubGraphes non affichés (limite interne de SC = 60 SubGraphes)
	const int NUMBER_OF_STUDY_SUBGRAPHS = 60;  //limite max = 60
	
	SCFloatArrayRef FirstIndex = sc.Subgraph[0].Arrays[0] ;  // Premier index du mois 

	SCInputRef ExternalChartNumber = sc.Input[0];
	SCInputRef TraceVectorielONOFF = sc.Input[2];
	
	
	int& NbMonitors = sc.GetPersistentIntFast(0);  
	int& FirstVisibleBar = sc.GetPersistentIntFast(1);  
	int& LastVisibleBar = sc.GetPersistentIntFast(2);  
	int& ShiftFirstIndex = sc.GetPersistentIntFast(3);  

	float& BottomCoordinatePrevious = sc.GetPersistentFloatFast(0);  
	float& TopCoordinatePrevious = sc.GetPersistentFloatFast(1);  

	
	if (sc.SetDefaults)
	{
		sc.AutoLoop = 0;  
		sc.GraphName = "[3] Pivots Ind M (SiXteenth)";
		sc.ScaleRangeType = SCALE_SAMEASREGION;
		sc.ValueFormat = VALUEFORMAT_INHERITED;
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 0 ;
		sc.DisplayStudyInputValues = 0;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		sc.HideStudy = 1 ;
		
		ExternalChartNumber.Name = "Monthly Chart Number";
		ExternalChartNumber.SetChartNumber(1);
		TraceVectorielONOFF.Name = "Trace Vectoriel ON/OFF";
		TraceVectorielONOFF.SetYesNo(0);

		// DisplayOrder des inputs
		TraceVectorielONOFF.DisplayOrder = 1 ;		

		for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].DrawStyle = DRAWSTYLE_DASH;
			sc.Subgraph[SubgraphIndex].PrimaryColor = RGB(0,0,0);
			// sc.Subgraph[SubgraphIndex].LineLabel = LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_LEFT_EDGE | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_CENTER | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
			sc.Subgraph[SubgraphIndex].LineLabel = LL_DISPLAY_NAME | LL_NAME_ALIGN_LEFT_EDGE | LL_NAME_REVERSE_COLORS ;  // affiche le nom de l'indicateur retourné, mais pas sa valeur
			sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre
			sc.Subgraph[SubgraphIndex].DrawZeros = false;
			sc.Subgraph[SubgraphIndex].LineStyle = LINESTYLE_SOLID ;
			sc.Subgraph[SubgraphIndex].LineWidth = 1 ;
		}

	
		RX1.Name = "                        RX1 M";
		RX2.Name = "                        RX2 M";
		RX3.Name = "                        RX3 M";
		RX4.Name = "                        RX4 M";
		RX5.Name = "                        RX5 M";
		RX6.Name = "                        RX6 M";
		RX7.Name = "                        RX7 M";
		RX8.Name = "                        RX8 M";
		RX9.Name = "                        RX9 M";
		RX10.Name = "                        RX10 M";
		RX11.Name = "                        RX11 M";
		RX12.Name = "                        RX12 M";
		RX13.Name = "                        RX13 M";
		RX14.Name = "                        RX14 M";
		RX14.Name = "                        RX14 M";
		RX15.Name = "                        RX15 M";
		RX16.Name = "                        RX16 M";
		RX17.Name = "                        RX17 M";
		RX18.Name = "                        RX18 M";
		RX19.Name = "                        RX19 M";
		RX20.Name = "                        RX20 M";
		RX21.Name = "                        RX21 M";
		RX22.Name = "                        RX22 M";
		RX23.Name = "                        RX23 M";
		RX24.Name = "                        RX24 M";
		RX25.Name = "                        RX25 M";
		RX26.Name = "                        RX26 M";
		RX27.Name = "                        RX27 M";
		RX28.Name = "                        RX28 M";
		RX29.Name = "                        RX29 M";
		RX30.Name = "                        RX30 M";
		//RX31.Name = "                        RX31 M";
		//RX32.Name = "                        RX32 M";
		SX1.Name = "                        SX1 M";
		SX2.Name = "                        SX2 M";
		SX3.Name = "                        SX3 M";
		SX4.Name = "                        SX4 M";
		SX5.Name = "                        SX5 M";
		SX6.Name = "                        SX6 M";
		SX7.Name = "                        SX7 M";
		SX8.Name = "                        SX8 M";
		SX9.Name = "                        SX9 M";
		SX10.Name = "                        SX10 M";
		SX11.Name = "                        SX11 M";
		SX12.Name = "                        SX12 M";
		SX13.Name = "                        SX13 M";
		SX14.Name = "                        SX14 M";
		SX14.Name = "                        SX14 M";
		SX15.Name = "                        SX15 M";
		SX16.Name = "                        SX16 M";
		SX17.Name = "                        SX17 M";
		SX18.Name = "                        SX18 M";
		SX19.Name = "                        SX19 M";
		SX20.Name = "                        SX20 M";
		SX21.Name = "                        SX21 M";
		SX22.Name = "                        SX22 M";
		SX23.Name = "                        SX23 M";
		SX24.Name = "                        SX24 M";
		SX25.Name = "                        SX25 M";
		SX26.Name = "                        SX26 M";
		SX27.Name = "                        SX27 M";
		SX28.Name = "                        SX28 M";
		SX29.Name = "                        SX29 M";
		SX30.Name = "                        SX30 M";
		//SX31.Name = "                        SX31 M";
		//SX32.Name = "                        SX32 M";
		
		return;
	}  // fin du bloc de " if (sc.SetDefaults) "
	
	
	if ( sc.UpdateStartIndex == 0 )
	{
		sc.GraphRegion = 0 ;
		NbMonitors = NumberOfMonitors() ;

		FirstVisibleBar = 0 ;
		LastVisibleBar = 0 ;
		int PeriodeChart = PeriodeChartEnSecondes (&sc) ;
		if ( PeriodeChart == 20 )  ShiftFirstIndex = 34 ;
		else if ( PeriodeChart == 240 )  ShiftFirstIndex = 3 ;
	}	
	
	
	for (int index = sc.UpdateStartIndex; index < sc.ArraySize; index++)                 // début AutoLoop pour le calcul des sc.Subgraph
	{
		if (    sc.BaseDateTimeIn[index].GetMonth()  !=   sc.BaseDateTimeIn[index-1].GetMonth()      or   index==0    )   // ie on change de mois 
		{
			sc.GraphRegion = 0 ;
			
			FirstIndex[index] = index ;
			
			SCGraphData ExternalChartData;
			sc.GetChartBaseData(ExternalChartNumber.GetChartNumber(), ExternalChartData);
			int ExternalChartIndex = sc.GetContainingIndexForDateTimeIndex(ExternalChartNumber.GetChartNumber(), index); // on récupère l'index du nouveau chandelier

			float fPHPrior = 0, fPBPrior = 0 , fSettlePrior = 0 ;
			float fRX1, fRX2, fRX3, fRX4, fRX5, fRX6, fRX7, fRX8, fRX9, fRX10, fRX11, fRX12, fRX13, fRX14, fRX15, fRX16 ;
			float fRX17, fRX18, fRX19, fRX20, fRX21, fRX22, fRX23, fRX24, fRX25, fRX26, fRX27, fRX28, fRX29, fRX30, fRX31, fRX32 ;
			float fSX1, fSX2, fSX3, fSX4, fSX5, fSX6, fSX7, fSX8, fSX9, fSX10, fSX11, fSX12, fSX13, fSX14, fSX15, fSX16 ;
			float fSX17, fSX18, fSX19, fSX20, fSX21, fSX22, fSX23, fSX24, fSX25, fSX26, fSX27, fSX28, fSX29, fSX30, fSX31, fSX32 ;

			// on récupère HLC
			fPHPrior = ExternalChartData[SC_HIGH][ExternalChartIndex-1];
			fPBPrior = ExternalChartData[SC_LOW][ExternalChartIndex-1];
			fSettlePrior = ExternalChartData[SC_LAST][ExternalChartIndex-1];
			
			// correction des bugs de flux
			SCDateTimeArray ExternalChartDateTime;
			sc.GetChartDateTimeArray(ExternalChartNumber.GetChartNumber(), ExternalChartDateTime);
			int Year, Month, Day;
			ExternalChartDateTime[ExternalChartIndex-1].GetDateYMD(Year, Month, Day);
			if (Year==2019 and Month==11 and Day==25)  // premier jour de la semaine pour lequel HLC est faux   // ve 29/11/2019 est cloturé à 13h
			{
				FirstIndex[index] = index - ShiftFirstIndex ;
				//fPHPrior = nd;
				// fPBPrior = nd;
				//fSettlePrior = nd;
			}

			//on calcule les PP (il n'y a pas de limite au nombre de variables qu'on peut passer => ne pas supprimer RX31/RX32/SX31/SX32
			CalculateSiXteenths (	fPHPrior, fPBPrior, fSettlePrior, 
											fRX1, fRX2, fRX3, fRX4, fRX5, fRX6, fRX7, fRX8, fRX9, fRX10, fRX11, fRX12, fRX13, fRX14, fRX15, fRX16, fRX17, fRX18, fRX19, fRX20, fRX21, fRX22, fRX23, fRX24, fRX25, fRX26, fRX27, fRX28, fRX29, fRX30, fRX31, fRX32,
											fSX1, fSX2, fSX3, fSX4, fSX5, fSX6, fSX7, fSX8, fSX9, fSX10, fSX11, fSX12, fSX13, fSX14, fSX15, fSX16, fSX17, fSX18, fSX19, fSX20, fSX21, fSX22, fSX23, fSX24, fSX25, fSX26, fSX27, fSX28, fSX29, fSX30, fSX31, fSX32 );
																			
			// les valeurs sont entrées dans les sc.Subgraph[]
			RX1[index] = fRX1;
			RX2[index] = fRX2;
			RX3[index] = fRX3;
			RX4[index] = fRX4;
			RX5[index] = fRX5;
			RX6[index] = fRX6;
			RX7[index] = fRX7;
			RX8[index] = fRX8;
			RX9[index] = fRX9;
			RX10[index] = fRX10;
			RX11[index] = fRX11;
			RX12[index] = fRX12;
			RX13[index] = fRX13;
			RX14[index] = fRX14;
			RX14[index] = fRX14;
			RX15[index] = fRX15;
			RX16[index] = fRX16;
			RX17[index] = fRX17;
			RX18[index] = fRX18;
			RX19[index] = fRX19;
			RX20[index] = fRX20;
			RX21[index] = fRX21;
			RX22[index] = fRX22;
			RX23[index] = fRX23;
			RX24[index] = fRX24;
			RX25[index] = fRX25;
			RX26[index] = fRX26;
			RX27[index] = fRX27;
			RX28[index] = fRX28;
			RX29[index] = fRX29;
			RX30[index] = fRX30;
			//RX31[index] = fRX31;
			//RX32[index] = fRX32;
			SX1[index] = fSX1;
			SX2[index] = fSX2;
			SX3[index] = fSX3;
			SX4[index] = fSX4;
			SX5[index] = fSX5;
			SX6[index] = fSX6;
			SX7[index] = fSX7;
			SX8[index] = fSX8;
			SX9[index] = fSX9;
			SX10[index] = fSX10;
			SX11[index] = fSX11;
			SX12[index] = fSX12;
			SX13[index] = fSX13;
			SX14[index] = fSX14;
			SX14[index] = fSX14;
			SX15[index] = fSX15;
			SX16[index] = fSX16;
			SX17[index] = fSX17;
			SX18[index] = fSX18;
			SX19[index] = fSX19;
			SX20[index] = fSX20;
			SX21[index] = fSX21;
			SX22[index] = fSX22;
			SX23[index] = fSX23;
			SX24[index] = fSX24;
			SX25[index] = fSX25;
			SX26[index] = fSX26;
			SX27[index] = fSX27;
			SX28[index] = fSX28;
			SX29[index] = fSX29;
			SX30[index] = fSX30;
			//SX31[index] = fSX31;
			//SX32[index] = fSX32;		
		}
		
		else // on n'est pas dans le premier chandelier du mois
		{
			FirstIndex[index] = FirstIndex[index-1] ;
			RX1[index] = RX1[index-1];
			RX2[index] = RX2[index-1];
			RX3[index] = RX3[index-1];
			RX4[index] = RX4[index-1];
			RX5[index] = RX5[index-1];
			RX6[index] = RX6[index-1];
			RX7[index] = RX7[index-1];
			RX8[index] = RX8[index-1];
			RX9[index] = RX9[index-1];
			RX10[index] = RX10[index-1];
			RX11[index] = RX11[index-1];
			RX12[index] = RX12[index-1];
			RX13[index] = RX13[index-1];
			RX14[index] = RX14[index-1];
			RX14[index] = RX14[index-1];
			RX15[index] = RX15[index-1];
			RX16[index] = RX16[index-1];
			RX17[index] = RX17[index-1];
			RX18[index] = RX18[index-1];
			RX19[index] = RX19[index-1];
			RX20[index] = RX20[index-1];
			RX21[index] = RX21[index-1];
			RX22[index] = RX22[index-1];
			RX23[index] = RX23[index-1];
			RX24[index] = RX24[index-1];
			RX25[index] = RX25[index-1];
			RX26[index] = RX26[index-1];
			RX27[index] = RX27[index-1];
			RX28[index] = RX28[index-1];
			RX29[index] = RX29[index-1];
			RX30[index] = RX30[index-1];
			//RX31[index] = RX31[index-1];
			//RX32[index] = RX32[index-1];
			SX1[index] = SX1[index-1];
			SX2[index] = SX2[index-1];
			SX3[index] = SX3[index-1];
			SX4[index] = SX4[index-1];
			SX5[index] = SX5[index-1];
			SX6[index] = SX6[index-1];
			SX7[index] = SX7[index-1];
			SX8[index] = SX8[index-1];
			SX9[index] = SX9[index-1];
			SX10[index] = SX10[index-1];
			SX11[index] = SX11[index-1];
			SX12[index] = SX12[index-1];
			SX13[index] = SX13[index-1];
			SX14[index] = SX14[index-1];
			SX14[index] = SX14[index-1];
			SX15[index] = SX15[index-1];
			SX16[index] = SX16[index-1];
			SX17[index] = SX17[index-1];
			SX18[index] = SX18[index-1];
			SX19[index] = SX19[index-1];
			SX20[index] = SX20[index-1];
			SX21[index] = SX21[index-1];
			SX22[index] = SX22[index-1];
			SX23[index] = SX23[index-1];
			SX24[index] = SX24[index-1];
			SX25[index] = SX25[index-1];
			SX26[index] = SX26[index-1];
			SX27[index] = SX27[index-1];
			SX28[index] = SX28[index-1];
			SX29[index] = SX29[index-1];
			SX30[index] = SX30[index-1];
			//SX31[index] = SX31[index-1];
			//SX32[index] = SX32[index-1];	
		}
	}
	
	
	// Bloc d'affichage       
	if ( sc.ChartWindowIsActive  or  NbMonitors == 2  or  sc.IsFullRecalculation )
	{
		BottomCoordinate = sc.YPixelCoordinateToGraphValue(sc.StudyRegionBottomCoordinate) ;		// la variable est déclarée en dehors de la fct scsf
		TopCoordinate = sc.YPixelCoordinateToGraphValue(sc.StudyRegionTopCoordinate) ;				// la variable est déclarée en dehors de la fct scsf
		if (  TraceVectorielONOFF.GetYesNo()  and  ( FirstVisibleBar != sc.IndexOfFirstVisibleBar  or  LastVisibleBar != sc.IndexOfLastVisibleBar  or  sc.IsFullRecalculation  or  fabs(BottomCoordinatePrevious-BottomCoordinate) > 0.02  or  fabs(TopCoordinatePrevious-TopCoordinate) > 0.02 )  )          	// Le recalcul des lignes n'est réalisé que si FirstIndex ou LastVisibleBar ou BottomCoordinate varie (on fait l'hypothèse que TopCoordinate et BottomCoordinate varient de manière synchronisée)		// Note : Sierra a un bug interne pour le premier calcul de BottomCoordinate (il vaut la même valeur que TopCoordinate)    // Il faut rajouter sc.IsFullRecalculation car lors d'une Full ReCalculation les useTool non UDD sont effacés
		{	
			FirstVisibleBar = sc.IndexOfFirstVisibleBar ;
			LastVisibleBar = sc.IndexOfLastVisibleBar ;
			BottomCoordinatePrevious = BottomCoordinate ;
			TopCoordinatePrevious = TopCoordinate ;

			// les useTool doivent être effacés à chaque appel du DLL pour éviter une fuite de mémoire
			sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   // all non user drawn Chart Drawings on the specified ChartNumber will be deleted. The LineNumber parameter in this case is ignored and should be 0. 		
			
			s_sc* const ptrsc = &sc;
			int const firstIndex = FirstIndex[LastVisibleBar] ;  // index de la première bougie du mois : début du tracé de la ligne  // FirstIndex est un array qui contient le firstIndex
			
			for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)
			{
				float const Niveau = sc.Subgraph[SubgraphIndex][LastVisibleBar] ;
				SCString const Texte = sc.Subgraph[SubgraphIndex].Name ;
				int const epaisseur = sc.Subgraph[SubgraphIndex].LineWidth ;
				COLORREF const couleur = sc.Subgraph[SubgraphIndex].PrimaryColor;
				creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, Niveau, epaisseur, couleur, ptrsc);
			}
			
			// recalcul immédiat
			sc.UpdateAlways = 1 ;
		}
	}
	if ( TraceVectorielONOFF.GetYesNo() == 0 )  sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_MesPointsPivotsHebdo(SCStudyInterfaceRef sc)		// [4]
{
	SCSubgraphRef OpenGap = sc.Subgraph[0];   
	SCSubgraphRef PHPrior = sc.Subgraph[1];      // appelé par Navig_niveaux pour GenerationFichier_csv
	SCSubgraphRef PBPrior = sc.Subgraph[2];
	SCSubgraphRef PP = sc.Subgraph[3];   
	SCSubgraphRef R1 = sc.Subgraph[4];   
	SCSubgraphRef R2 = sc.Subgraph[5];   
	SCSubgraphRef R3 = sc.Subgraph[6];
	SCSubgraphRef R4 = sc.Subgraph[7];
	SCSubgraphRef S1 = sc.Subgraph[8];
	SCSubgraphRef S2 = sc.Subgraph[9];
	SCSubgraphRef S3 = sc.Subgraph[10];
	SCSubgraphRef S4 = sc.Subgraph[11];
	SCSubgraphRef mR1 = sc.Subgraph[12];
	SCSubgraphRef mR2 = sc.Subgraph[13];
	SCSubgraphRef mR3 = sc.Subgraph[14];
	SCSubgraphRef mR4 = sc.Subgraph[15];
	SCSubgraphRef mS1 = sc.Subgraph[16];
	SCSubgraphRef mS2 = sc.Subgraph[17];
	SCSubgraphRef mS3 = sc.Subgraph[18];
	SCSubgraphRef mS4 = sc.Subgraph[19];
	SCSubgraphRef RQ1 = sc.Subgraph[20];
	SCSubgraphRef RQ2 = sc.Subgraph[21];
	SCSubgraphRef RQ3 = sc.Subgraph[22];
	SCSubgraphRef RQ4 = sc.Subgraph[23];
	SCSubgraphRef RQ5 = sc.Subgraph[24];
	SCSubgraphRef RQ6 = sc.Subgraph[25];
	SCSubgraphRef RQ7 = sc.Subgraph[26];
	SCSubgraphRef RQ8 = sc.Subgraph[27];
	SCSubgraphRef SQ1 = sc.Subgraph[28];
	SCSubgraphRef SQ2 = sc.Subgraph[29];
	SCSubgraphRef SQ3 = sc.Subgraph[30];
	SCSubgraphRef SQ4 = sc.Subgraph[31];
	SCSubgraphRef SQ5 = sc.Subgraph[32];
	SCSubgraphRef SQ6 = sc.Subgraph[33];
	SCSubgraphRef SQ7 = sc.Subgraph[34];   
	SCSubgraphRef SQ8 = sc.Subgraph[35];   
	SCSubgraphRef SettlePrior = sc.Subgraph[36];
	SCSubgraphRef DemiGap = sc.Subgraph[37];   
	const int NUMBER_OF_STUDY_SUBGRAPHS = 38;  //37+1

	
	SCFloatArrayRef OpenHebdo = sc.Subgraph[0].Arrays[0] ; 		// appelé par scsf_MesPointsPivotsHebdosProjetes
	SCFloatArrayRef FirstIndex = sc.Subgraph[0].Arrays[1] ;  			// array contenant le firstIndex = Premier index de la semaine


	SCInputRef ExternalChartNumber = sc.Input[0];
	SCInputRef CalculForce = sc.Input[1];
	SCInputRef TraceVectorielONOFF = sc.Input[2];								// modifié par Prix_typique 		
	SCInputRef AffichageDesQuartsONOFF = sc.Input[3];		// modifié par Prix_typique 		
	SCInputRef AffichageDesMid4Full4ONOFF = sc.Input[4];		// modifié par Prix_typique 		
	SCInputRef AffichageDesPHPBBougiePrecedente = sc.Input[5];		// modifié par Prix_typique 	
	
	
	int& NbMonitors = sc.GetPersistentIntFast(0);  
	int& FirstVisibleBar = sc.GetPersistentIntFast(1);  
	int& LastVisibleBar = sc.GetPersistentIntFast(2);  
	int& ShiftFirstIndex = sc.GetPersistentIntFast(3);  

	float& BottomCoordinatePrevious = sc.GetPersistentFloatFast(0);  
	float& TopCoordinatePrevious = sc.GetPersistentFloatFast(1);  
	

	if (sc.SetDefaults)
	{
		sc.AutoLoop = 0;  
		sc.GraphName = "[4] Pivots Ind H (Full/Mid/Quart)";       // appelé par Navig_niveaux pour GenerationFichier_csv		// appelé par Prix_typique du chart #6
		sc.ScaleRangeType = SCALE_SAMEASREGION;
		sc.ValueFormat = VALUEFORMAT_INHERITED;
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 0 ;
		sc.DisplayStudyInputValues = 0;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		
		sc.HideStudy = 1 ;

		// ExternalChartNumber.Name = "Weeky Chart Number";
		ExternalChartNumber.SetChartNumber(5);
		CalculForce.Name = "Calcul force des pivots H a 13h04";  // calcul forcé des PP à 13h04 (utile lorsque le dernier jour de la semaine n'est pas un vendredi)
		CalculForce.SetYesNo(0);
		TraceVectorielONOFF.Name = "Trace Vectoriel ON/OFF";
		TraceVectorielONOFF.SetYesNo(1);
		AffichageDesQuartsONOFF.Name = "Affichage des Quarts";		
		AffichageDesQuartsONOFF.SetYesNo(1);
		AffichageDesMid4Full4ONOFF.Name = "Affichage des Mid4 et Full4";		
		AffichageDesMid4Full4ONOFF.SetYesNo(1);
		AffichageDesPHPBBougiePrecedente.Name = "Affichage des PH et PB du mois precedent";	
		AffichageDesPHPBBougiePrecedente.SetYesNo(1);		

		// DisplayOrder des inputs
		TraceVectorielONOFF.DisplayOrder = 1 ;		

		for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].DrawStyle = DRAWSTYLE_DASH;
			sc.Subgraph[SubgraphIndex].PrimaryColor = RGB(154,154,154);
			sc.Subgraph[SubgraphIndex].LineLabel = LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_LEFT_EDGE | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_CENTER | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
			sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre
			sc.Subgraph[SubgraphIndex].DrawZeros = false;
			sc.Subgraph[SubgraphIndex].LineStyle = LINESTYLE_SOLID ;
		}

		SettlePrior.DrawStyle = DRAWSTYLE_HIDDEN;
		
		PHPrior.PrimaryColor = RGB(255, 0, 0);
		PBPrior.PrimaryColor = RGB(0,168,0);
		SettlePrior.PrimaryColor = RGB(255,0,255);
		
		OpenGap.PrimaryColor = RGB(210,0,210);
		OpenGap.LineLabel =                                 LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_LEFT_EDGE  | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_CENTER | LL_VALUE_ALIGN_VALUES_SCALE | LL_NAME_ALIGN_ABOVE; 

		DemiGap.PrimaryColor = RGB(210,0,210);
		
		
		PHPrior.LineWidth = 4;
		PBPrior.LineWidth = 4;
		SettlePrior.LineWidth = 0;
		PP.LineWidth = 4;
		R1.LineWidth = 4;
		R2.LineWidth = 4;
		R3.LineWidth = 4;
		R4.LineWidth = 4;
		S1.LineWidth = 4;
		S2.LineWidth = 4;
		S3.LineWidth = 4;
		S4.LineWidth = 4;
		mR1.LineWidth = 3;
		mR2.LineWidth = 3;
		mR3.LineWidth = 3;
		mR4.LineWidth = 3;
		mS1.LineWidth = 3;
		mS2.LineWidth = 3;
		mS3.LineWidth = 3;
		mS4.LineWidth = 3;
		RQ1.LineWidth = 2;
		RQ2.LineWidth = 2;
		RQ3.LineWidth = 2;
		RQ4.LineWidth = 2;
		RQ5.LineWidth = 2;
		RQ6.LineWidth = 2;
		RQ7.LineWidth = 2;
		RQ8.LineWidth = 2;
		SQ1.LineWidth = 2;
		SQ2.LineWidth = 2;
		SQ3.LineWidth = 2;
		SQ4.LineWidth = 2;
		SQ5.LineWidth = 2;
		SQ6.LineWidth = 2;
		SQ7.LineWidth = 2;
		SQ8.LineWidth = 2;
		OpenGap.LineWidth = 8;
		DemiGap.LineWidth = 3;
		
		return;
	}  // fin du bloc de " if (sc.SetDefaults) "
	
	
	if ( sc.UpdateStartIndex == 0 )
	{
		sc.GraphRegion = 0 ;
		NbMonitors = NumberOfMonitors() ;
		FirstVisibleBar = 0 ;
		LastVisibleBar = 0 ;
		int PeriodeChart = PeriodeChartEnSecondes (&sc) ;
		if ( PeriodeChart == 20 )  ShiftFirstIndex = 34 ;
		else if ( PeriodeChart == 240 )  ShiftFirstIndex = 3 ;
		
		// Affichage des quarts, des Mid4Full4 et des PHPB : par défaut on affiche tout, puis on met  "LineWidth = 0" à ce qui ne doit pas être affiché
		{
			RQ1.LineWidth = 2;
			RQ2.LineWidth = 2;
			RQ3.LineWidth = 2;
			RQ4.LineWidth = 2;
			RQ5.LineWidth = 2;
			RQ6.LineWidth = 2;
			RQ7.LineWidth = 2;
			RQ8.LineWidth = 2;
			SQ1.LineWidth = 2;
			SQ2.LineWidth = 2;
			SQ3.LineWidth = 2;
			SQ4.LineWidth = 2;
			SQ5.LineWidth = 2;
			SQ6.LineWidth = 2;
			SQ7.LineWidth = 2;
			SQ8.LineWidth = 2;
			mR4.LineWidth = 3;
			mS4.LineWidth = 3;
			R4.LineWidth = 4;
			S4.LineWidth = 4;
			PHPrior.LineWidth = 4;
			PBPrior.LineWidth = 4;
			SettlePrior.LineWidth = 0;
			OpenGap.LineWidth = 6;
			DemiGap.LineWidth = 2;			
		}
		
		// Quarts non affichés
		if ( AffichageDesQuartsONOFF.GetYesNo() == 0 )
		{
			RQ1.LineWidth = 0;
			RQ2.LineWidth = 0;
			RQ3.LineWidth = 0;
			RQ4.LineWidth = 0;
			RQ5.LineWidth = 0;
			RQ6.LineWidth = 0;
			RQ7.LineWidth = 0;
			RQ8.LineWidth = 0;
			SQ1.LineWidth = 0;
			SQ2.LineWidth = 0;
			SQ3.LineWidth = 0;
			SQ4.LineWidth = 0;
			SQ5.LineWidth = 0;
			SQ6.LineWidth = 0;
			SQ7.LineWidth = 0;
			SQ8.LineWidth = 0;
		}
		
		// Mid4/Full4 non affichés
		if ( AffichageDesMid4Full4ONOFF.GetYesNo() == 0 )
		{
			RQ7.LineWidth = 0;
			RQ8.LineWidth = 0;
			SQ7.LineWidth = 0;
			SQ8.LineWidth = 0;
			mR4.LineWidth = 0;
			mS4.LineWidth = 0;
			R4.LineWidth = 0;
			S4.LineWidth = 0;
		}
		
		// PH/PB non affichés
		if ( AffichageDesPHPBBougiePrecedente.GetYesNo() == 0 )
		{
			PHPrior.LineWidth = 0;
			PBPrior.LineWidth = 0;
			SettlePrior.LineWidth = 0;
			OpenGap.LineWidth = 5;
			DemiGap.LineWidth = 0;
		}		
		
		{ 	 // Config Full HD 
			PHPrior.Name = "           / PH HP";
			PBPrior.Name = "           / PB HP";
			//SettlePrior.Name = "Settle HP";
			PP.Name = "                PP H";
			R1.Name = "                R1 H";
			R2.Name = "                R2 H";
			R3.Name = "                R3 H";
			R4.Name = "                R4 H";
			S1.Name = "                S1 H";
			S2.Name = "                S2 H";
			S3.Name = "                S3 H";
			S4.Name = "                S4 H";
			mR1.Name = "            mR1 H";
			mR2.Name = "            mR2 H";
			mR3.Name = "            mR3 H";
			mR4.Name = "            mR4 H";
			mS1.Name = "            mS1 H";
			mS2.Name = "            mS2 H";
			mS3.Name = "            mS3 H";
			mS4.Name = "            mS4 H";
			RQ1.Name = "            RQ1 H";
			RQ2.Name = "            RQ2 H";
			RQ3.Name = "            RQ3 H";
			RQ4.Name = "            RQ4 H";
			RQ5.Name = "            RQ5 H";
			RQ6.Name = "            RQ6 H";
			RQ7.Name = "            RQ7 H";
			RQ8.Name = "            RQ8 H";
			SQ1.Name = "            SQ1 H";
			SQ2.Name = "            SQ2 H";
			SQ3.Name = "            SQ3 H";
			SQ4.Name = "            SQ4 H";
			SQ5.Name = "            SQ5 H";
			SQ6.Name = "            SQ6 H";
			SQ7.Name = "            SQ7 H";
			SQ8.Name = "            SQ8 H";
			OpenGap.Name = "           OpenGap H";		
			DemiGap.Name = "  DemiGap H";		// 2 espaces de plus que la version J
		}		

	}

	
	
	for (int index = sc.UpdateStartIndex; index < sc.ArraySize; index++)                 // début AutoLoop
	{
	
		int OpenTimeValue = sc.BaseDateTimeIn[index].GetTimeInSecondsWithoutMilliseconds()  ; //GetTimeInSecondsWithoutMilliseconds retourne une time_value
		
		if (  (  sc.BaseDateTimeIn[index].GetDayOfWeek() == FRIDAY  and  OpenTimeValue ==  57840 )  or  ( CalculForce.GetYesNo()  and  OpenTimeValue ==  47040 )  )  // CALCUL DES PP de la semaine suivante, réalisé le vendredi à 16h04     // CALCUL DES PP pour la semaine suivante, réalisé à 13h04
		{
			FirstIndex[index] = index ;
			
			SCGraphData ExternalChartData;
			sc.GetChartBaseData(ExternalChartNumber.GetChartNumber(), ExternalChartData);
			int ExternalChartIndex = sc.GetContainingIndexForDateTimeIndex(ExternalChartNumber.GetChartNumber(), index); // on récupère l'index du nouveau chandelier sur le Weekly chart

			float fPHPrior = 0, fPBPrior = 0 , fSettlePrior = 0, fPP = 0 ;
			float fR1 = 0, fR2 = 0, fR3 = 0, fR4 = 0, fS1 = 0, fS2 = 0, fS3 = 0, fS4 = 0 ;
			float fmR1 = 0, fmR2 = 0, fmR3 = 0, fmR4 = 0, fmS1 = 0, fmS2 = 0,  fmS3 = 0,   fmS4 = 0 ; 
			float fRQ1 = 0, fRQ2 = 0, fRQ3 = 0, fRQ4 = 0, fRQ5 = 0, fRQ6 = 0, fRQ7 = 0, fRQ8 = 0 ;
			float fSQ1 = 0, fSQ2 = 0, fSQ3 = 0, fSQ4 = 0, fSQ5 = 0, fSQ6 = 0, fSQ7 = 0, fSQ8 = 0 ;

			// on récupère HLC
			fPHPrior = ExternalChartData[SC_HIGH][ExternalChartIndex];
			fPBPrior = ExternalChartData[SC_LOW][ExternalChartIndex];
			fSettlePrior = ExternalChartData[SC_LAST][ExternalChartIndex];
			
			// correction des bugs de flux
			SCDateTimeArray ExternalChartDateTime;
			sc.GetChartDateTimeArray(ExternalChartNumber.GetChartNumber(), ExternalChartDateTime);
			int Year, Month, Day;
			ExternalChartDateTime[ExternalChartIndex].GetDateYMD(Year, Month, Day);
		/* 	if (Year==2019 and Month==1 and Day==31)  // premier jour de la semaine pour lequel HLC est faux
			{
				//fPHPrior = nd;
				fPBPrior = nd;
				//fSettlePrior = nd;
			} */

			//on calcule les PP 
			CalculatePPs (fPHPrior, fPBPrior, fSettlePrior, fPP, fR1, fR2, fR3, fR4, fS1, fS2, fS3, fS4, fmR1, fmR2, fmR3, fmR4, fmS1, fmS2, fmS3, fmS4, fRQ1, fRQ2, fRQ3, fRQ4, fRQ5, fRQ6, fRQ7, fRQ8, fSQ1, fSQ2, fSQ3, fSQ4, fSQ5, fSQ6, fSQ7, fSQ8);
			
			// les valeurs sont entrées dans les sc.Subgraph[]
			PHPrior[index] = fPHPrior;
			PBPrior[index] = fPBPrior;
			SettlePrior[index] = fSettlePrior;
			PP[index] = fPP;
			R1[index] = fR1;
			R2[index] = fR2;
			R3[index] = fR3;
			R4[index] = fR4;
			S1[index] = fS1;
			S2[index] = fS2;
			S3[index] = fS3;
			S4[index] = fS4;
			mR1[index] = fmR1;
			mR2[index] = fmR2;
			mR3[index] = fmR3;
			mR4[index] = fmR4;
			mS1[index] = fmS1;
			mS2[index] = fmS2;
			mS3[index] = fmS3;
			mS4[index] = fmS4;
			RQ1[index] = fRQ1;
			RQ2[index] = fRQ2;
			RQ3[index] = fRQ3;
			RQ4[index] = fRQ4;
			RQ5[index] = fRQ5;
			RQ6[index] = fRQ6;
			RQ7[index] = fRQ7;
			RQ8[index] = fRQ8;
			SQ1[index] = fSQ1;
			SQ2[index] = fSQ2;
			SQ3[index] = fSQ3;
			SQ4[index] = fSQ4;
			SQ5[index] = fSQ5;
			SQ6[index] = fSQ6;
			SQ7[index] = fSQ7;
			SQ8[index] = fSQ8;
			OpenGap[index] = 0;
			DemiGap[index] = 0;
			OpenHebdo[index] = 0;
		}
	
		else if (    sc.BaseDateTimeIn[index].GetDayOfWeek()  <  sc.BaseDateTimeIn[index-1].GetDayOfWeek()    or   index==0    )  // RECALCUL DES PP à chaque changement de semaine
		{
			// FirstIndex[index] = index ;
			FirstIndex[index] = index - ShiftFirstIndex ;
			
			SCGraphData ExternalChartData;
			sc.GetChartBaseData(ExternalChartNumber.GetChartNumber(), ExternalChartData);
			int ExternalChartIndex = sc.GetContainingIndexForDateTimeIndex(ExternalChartNumber.GetChartNumber(), index); // on récupère l'index du nouveau chandelier sur le Weekly chart

			float fPHPrior = 0, fPBPrior = 0 , fSettlePrior = 0, fPP = 0 ;
			float fR1 = 0, fR2 = 0, fR3 = 0, fR4 = 0, fS1 = 0, fS2 = 0, fS3 = 0, fS4 = 0 ;
			float fmR1 = 0, fmR2 = 0, fmR3 = 0, fmR4 = 0, fmS1 = 0, fmS2 = 0,  fmS3 = 0,   fmS4 = 0 ; 
			float fRQ1 = 0, fRQ2 = 0, fRQ3 = 0, fRQ4 = 0, fRQ5 = 0, fRQ6 = 0, fRQ7 = 0, fRQ8 = 0 ;
			float fSQ1 = 0, fSQ2 = 0, fSQ3 = 0, fSQ4 = 0, fSQ5 = 0, fSQ6 = 0, fSQ7 = 0, fSQ8 = 0 ;
			
			// on récupère HLC
			fPHPrior = ExternalChartData[SC_HIGH][ExternalChartIndex-1];
			fPBPrior = ExternalChartData[SC_LOW][ExternalChartIndex-1];
			fSettlePrior = ExternalChartData[SC_LAST][ExternalChartIndex-1];
			
			// correction des bugs de flux
			SCDateTimeArray ExternalChartDateTime;
			sc.GetChartDateTimeArray(ExternalChartNumber.GetChartNumber(), ExternalChartDateTime);
			int Year, Month, Day;
			ExternalChartDateTime[ExternalChartIndex-1].GetDateYMD(Year, Month, Day);
			if (Year==2019 and Month==11 and Day==25)  // premier jour de la semaine pour lequel HLC est faux   // ve 29/11/2019 est cloturé à 13h
			{
				FirstIndex[index] = index - ShiftFirstIndex ;
				//fPHPrior = nd;
				// fPBPrior = nd;
				//fSettlePrior = nd;
			}
			
			//on calcule les PP 
			CalculatePPs (fPHPrior, fPBPrior, fSettlePrior, fPP, fR1, fR2, fR3, fR4, fS1, fS2, fS3, fS4, fmR1, fmR2, fmR3, fmR4, fmS1, fmS2, fmS3, fmS4, fRQ1, fRQ2, fRQ3, fRQ4, fRQ5, fRQ6, fRQ7, fRQ8, fSQ1, fSQ2, fSQ3, fSQ4, fSQ5, fSQ6, fSQ7, fSQ8);
			
			// les valeurs sont entrées dans les sc.Subgraph[]
			PHPrior[index] = fPHPrior;
			PBPrior[index] = fPBPrior;
			SettlePrior[index] = fSettlePrior;
			PP[index] = fPP;
			R1[index] = fR1;
			R2[index] = fR2;
			R3[index] = fR3;
			R4[index] = fR4;
			S1[index] = fS1;
			S2[index] = fS2;
			S3[index] = fS3;
			S4[index] = fS4;
			mR1[index] = fmR1;
			mR2[index] = fmR2;
			mR3[index] = fmR3;
			mR4[index] = fmR4;
			mS1[index] = fmS1;
			mS2[index] = fmS2;
			mS3[index] = fmS3;
			mS4[index] = fmS4;
			RQ1[index] = fRQ1;
			RQ2[index] = fRQ2;
			RQ3[index] = fRQ3;
			RQ4[index] = fRQ4;
			RQ5[index] = fRQ5;
			RQ6[index] = fRQ6;
			RQ7[index] = fRQ7;
			RQ8[index] = fRQ8;
			SQ1[index] = fSQ1;
			SQ2[index] = fSQ2;
			SQ3[index] = fSQ3;
			SQ4[index] = fSQ4;
			SQ5[index] = fSQ5;
			SQ6[index] = fSQ6;
			SQ7[index] = fSQ7;
			SQ8[index] = fSQ8;
			OpenHebdo[index] = sc.Open[index];
			
			// Recherche d'un opengap dans la première bougie du jour
			if (   sc.Open[index] > fPHPrior   )      // gap haussier  
			{
				OpenGap[index] = fPHPrior;
				DemiGap[index] = (sc.Open[index] + fPHPrior)/2;
			}
			else if (   ( sc.Open[index] < fPBPrior )    )     //    gap baissier
			{
				OpenGap[index] = fPBPrior;
				DemiGap[index] = (sc.Open[index] + fPBPrior)/2;
			}
			else    // pas de gap
			{
				OpenGap[index] = 0;
				DemiGap[index] = 0;
			}
			
		}
		
		else // on n'est pas dans le premier chandelier de la nouvelle semaine ou le vendredi à 16h04
		{
			FirstIndex[index] = FirstIndex[index-1] ;
			PHPrior[index] = PHPrior[index-1] ;
			PBPrior[index] = PBPrior[index-1] ;
			SettlePrior[index] = SettlePrior[index-1] ;
			PP[index] = PP[index-1];
			R1[index] = R1[index-1];
			R2[index] = R2[index-1];
			R3[index] = R3[index-1];
			R4[index] = R4[index-1];
			S1[index] = S1[index-1];
			S2[index] = S2[index-1];
			S3[index] = S3[index-1];
			S4[index] = S4[index-1];
			mR1[index] = mR1[index-1];
			mR2[index] = mR2[index-1];
			mR3[index] = mR3[index-1];
			mR4[index] = mR4[index-1];
			mS1[index] = mS1[index-1];
			mS2[index] = mS2[index-1];
			mS3[index] = mS3[index-1];
			mS4[index] = mS4[index-1];
			RQ1[index] = RQ1[index-1];
			RQ2[index] = RQ2[index-1];
			RQ3[index] = RQ3[index-1];
			RQ4[index] = RQ4[index-1];
			RQ5[index] = RQ5[index-1];
			RQ6[index] = RQ6[index-1];
			RQ7[index] = RQ7[index-1];
			RQ8[index] = RQ8[index-1];
			SQ1[index] = SQ1[index-1];
			SQ2[index] = SQ2[index-1];
			SQ3[index] = SQ3[index-1];
			SQ4[index] = SQ4[index-1];
			SQ5[index] = SQ5[index-1];
			SQ6[index] = SQ6[index-1];
			SQ7[index] = SQ7[index-1];
			SQ8[index] = SQ8[index-1];
			OpenGap[index] = OpenGap[index-1];
			DemiGap[index] = DemiGap[index-1];
			OpenHebdo[index] = OpenHebdo[index-1];
		}
		
	}
	
	
	// Bloc d'affichage       
	if ( sc.ChartWindowIsActive  or  NbMonitors == 2  or  sc.IsFullRecalculation )
	{
		BottomCoordinate = sc.YPixelCoordinateToGraphValue(sc.StudyRegionBottomCoordinate) ;		// la variable est déclarée en dehors de la fct scsf
		TopCoordinate = sc.YPixelCoordinateToGraphValue(sc.StudyRegionTopCoordinate) ;				// la variable est déclarée en dehors de la fct scsf
		if (  TraceVectorielONOFF.GetYesNo()  and  ( FirstVisibleBar != sc.IndexOfFirstVisibleBar  or  LastVisibleBar != sc.IndexOfLastVisibleBar  or  sc.IsFullRecalculation  or  fabs(BottomCoordinatePrevious-BottomCoordinate) > 0.02  or  fabs(TopCoordinatePrevious-TopCoordinate) > 0.02 )  )          	// Le recalcul des lignes n'est réalisé que si FirstIndex ou LastVisibleBar ou BottomCoordinate varie (on fait l'hypothèse que TopCoordinate et BottomCoordinate varient de manière synchronisée)		// Note : Sierra a un bug interne pour le premier calcul de BottomCoordinate (il vaut la même valeur que TopCoordinate)    // Il faut rajouter sc.IsFullRecalculation car lors d'une Full ReCalculation les useTool non UDD sont effacés
		{	
			FirstVisibleBar = sc.IndexOfFirstVisibleBar ;
			LastVisibleBar = sc.IndexOfLastVisibleBar ;
			BottomCoordinatePrevious = BottomCoordinate ;
			TopCoordinatePrevious = TopCoordinate ;
			
			// les useTool doivent être effacés à chaque appel du DLL pour éviter une fuite de mémoire
			sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   // all non user drawn Chart Drawings on the specified ChartNumber will be deleted. The LineNumber parameter in this case is ignored and should be 0. 		
			
			s_sc* const ptrsc = &sc;
			int const firstIndex = FirstIndex[LastVisibleBar] ;  // index de la première bougie du tracé de la ligne  // FirstIndex est un array qui contient le firstIndex
			
			for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)
			{
				float const Niveau = sc.Subgraph[SubgraphIndex][LastVisibleBar] ;
				SCString const Texte = sc.Subgraph[SubgraphIndex].Name ;
				int const epaisseur = sc.Subgraph[SubgraphIndex].LineWidth ;
				COLORREF const couleur = sc.Subgraph[SubgraphIndex].PrimaryColor;
				creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, Niveau, epaisseur, couleur, ptrsc);
			}
			
			// recalcul immédiat
			sc.UpdateAlways = 1 ;				
		}
	}
	if ( TraceVectorielONOFF.GetYesNo() == 0 )  sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_MesEighthsHebdo(SCStudyInterfaceRef sc)		// [5]
{
	SCSubgraphRef RE1 = sc.Subgraph[0];  
	SCSubgraphRef RE2 = sc.Subgraph[1];	
	SCSubgraphRef RE3 = sc.Subgraph[2];
	SCSubgraphRef RE4 = sc.Subgraph[3];
	SCSubgraphRef RE5 = sc.Subgraph[4];
	SCSubgraphRef RE6 = sc.Subgraph[5];
	SCSubgraphRef RE7 = sc.Subgraph[6];
	SCSubgraphRef RE8 = sc.Subgraph[7];
	SCSubgraphRef RE9 = sc.Subgraph[8];
	SCSubgraphRef RE10 = sc.Subgraph[9];
	SCSubgraphRef RE11 = sc.Subgraph[10];
	SCSubgraphRef RE12 = sc.Subgraph[11];
	SCSubgraphRef RE13 = sc.Subgraph[12];
	SCSubgraphRef RE14 = sc.Subgraph[13];
	SCSubgraphRef RE15 = sc.Subgraph[14];
	SCSubgraphRef RE16 = sc.Subgraph[15];
	SCSubgraphRef SE1 = sc.Subgraph[16];
	SCSubgraphRef SE2 = sc.Subgraph[17];
	SCSubgraphRef SE3 = sc.Subgraph[18];
	SCSubgraphRef SE4 = sc.Subgraph[19];
	SCSubgraphRef SE5 = sc.Subgraph[20];
	SCSubgraphRef SE6 = sc.Subgraph[21];
	SCSubgraphRef SE7 = sc.Subgraph[22];
	SCSubgraphRef SE8 = sc.Subgraph[23];
	SCSubgraphRef SE9 = sc.Subgraph[24];
	SCSubgraphRef SE10 = sc.Subgraph[25];
	SCSubgraphRef SE11 = sc.Subgraph[26];
	SCSubgraphRef SE12 = sc.Subgraph[27];
	SCSubgraphRef SE13 = sc.Subgraph[28];
	SCSubgraphRef SE14 = sc.Subgraph[29];
	SCSubgraphRef SE15 = sc.Subgraph[30];
	SCSubgraphRef SE16 = sc.Subgraph[31];
	const int NUMBER_OF_STUDY_SUBGRAPHS = 32; 
	
	SCFloatArrayRef FirstIndex = sc.Subgraph[0].Arrays[0] ;  // Premier index du tracé 


	SCInputRef ExternalChartNumber = sc.Input[0];
	SCInputRef TraceVectorielONOFF = sc.Input[2];		
	SCInputRef AffichageDesMid4Full4ONOFF = sc.Input[4];					// modifié par Prix_typique 	
	
	
	int& NbMonitors = sc.GetPersistentIntFast(0);  
	int& FirstVisibleBar = sc.GetPersistentIntFast(1);  
	int& LastVisibleBar = sc.GetPersistentIntFast(2);  
	int& ShiftFirstIndex = sc.GetPersistentIntFast(3);  

	float& BottomCoordinatePrevious = sc.GetPersistentFloatFast(0);  
	float& TopCoordinatePrevious = sc.GetPersistentFloatFast(1);  

	
	
	if (sc.SetDefaults)
	{
		sc.AutoLoop = 0;  
		sc.GraphName = "[5] Pivots Ind H (Eighth)";   	
		sc.ScaleRangeType = SCALE_SAMEASREGION;
		sc.ValueFormat = VALUEFORMAT_INHERITED;
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 0 ;
		sc.DisplayStudyInputValues = 0;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;

		sc.HideStudy = 1 ;
		
		ExternalChartNumber.Name = "Weekly Chart Number";
		ExternalChartNumber.SetChartNumber(5);
		TraceVectorielONOFF.Name = "Trace Vectoriel ON/OFF";
		TraceVectorielONOFF.SetYesNo(0);
		AffichageDesMid4Full4ONOFF.Name = "Affichage des Mid4 et Full4";	
		AffichageDesMid4Full4ONOFF.SetYesNo(1);		

		// DisplayOrder des inputs
		TraceVectorielONOFF.DisplayOrder = 1 ;

		for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].DrawStyle = DRAWSTYLE_DASH;
			sc.Subgraph[SubgraphIndex].PrimaryColor = RGB(154,154,154);
			// sc.Subgraph[SubgraphIndex].LineLabel = LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_LEFT_EDGE | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_CENTER | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
			sc.Subgraph[SubgraphIndex].LineLabel = LL_DISPLAY_NAME | LL_NAME_ALIGN_LEFT_EDGE | LL_NAME_REVERSE_COLORS ;  // affiche le nom de l'indicateur retourné, mais pas sa valeur
			sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre
			sc.Subgraph[SubgraphIndex].DrawZeros = false;
			sc.Subgraph[SubgraphIndex].LineStyle = LINESTYLE_SOLID ;
			sc.Subgraph[SubgraphIndex].LineWidth = 1 ;
		}

	
		RE1.Name = "            RE1 H";
		RE2.Name = "            RE2 H";
		RE3.Name = "            RE3 H";
		RE4.Name = "            RE4 H";
		RE5.Name = "            RE5 H";
		RE6.Name = "            RE6 H";
		RE7.Name = "            RE7 H";
		RE8.Name = "            RE8 H";
		RE9.Name = "            RE9 H";
		RE10.Name = "          RE10 H";
		RE11.Name = "          RE11 H";
		RE12.Name = "          RE12 H";
		RE13.Name = "          RE13 H";
		RE14.Name = "          RE14 H";
		RE14.Name = "          RE14 H";
		RE15.Name = "          RE15 H";
		RE16.Name = "          RE16 H";
		SE1.Name = "            SE1 H";
		SE2.Name = "            SE2 H";
		SE3.Name = "            SE3 H";
		SE4.Name = "            SE4 H";
		SE5.Name = "            SE5 H";
		SE6.Name = "            SE6 H";
		SE7.Name = "            SE7 H";
		SE8.Name = "            SE8 H";
		SE9.Name = "            SE9 H";
		SE10.Name = "          SE10 H";
		SE11.Name = "          SE11 H";
		SE12.Name = "          SE12 H";
		SE13.Name = "          SE13 H";
		SE14.Name = "          SE14 H";
		SE14.Name = "          SE14 H";
		SE15.Name = "          SE15 H";
		SE16.Name = "          SE16 H";
		
		return;
	}  // fin du bloc de " if (sc.SetDefaults) "
	
	
	if ( sc.UpdateStartIndex == 0 )
	{
		sc.GraphRegion = 0 ;
		NbMonitors = NumberOfMonitors() ;
		FirstVisibleBar = 0 ;
		LastVisibleBar = 0 ;

		int PeriodeChart = PeriodeChartEnSecondes (&sc) ;
		if ( PeriodeChart == 20 )  ShiftFirstIndex = 34 ;
		else if ( PeriodeChart == 240 )  ShiftFirstIndex = 3 ;
		
		// Affichage des eights liés à Mid4Full4 : par défaut on affiche tout, puis on met  "LineWidth = 0" à ce qui ne doit pas être affiché
		{
			RE13.LineWidth = 1;
			RE14.LineWidth = 1;
			RE15.LineWidth = 1;
			RE16.LineWidth = 1;
			SE13.LineWidth = 1;
			SE14.LineWidth = 1;
			SE15.LineWidth = 1;
			SE16.LineWidth = 1;
		}		

		// Mid4/Full4 non affichés
		if ( AffichageDesMid4Full4ONOFF.GetYesNo() == 0 )
		{
			RE13.LineWidth = 0;
			RE14.LineWidth = 0;
			RE15.LineWidth = 0;
			RE16.LineWidth = 0;
			SE13.LineWidth = 0;
			SE14.LineWidth = 0;
			SE15.LineWidth = 0;
			SE16.LineWidth = 0;
		}	
		
	}

	
	
	for (int index = sc.UpdateStartIndex; index < sc.ArraySize; index++)                 // début AutoLoop
	{
	
		int OpenTimeValue = sc.BaseDateTimeIn[index].GetTimeInSecondsWithoutMilliseconds()  ; //GetTimeInSecondsWithoutMilliseconds retourne une time_value
		
		if ( sc.BaseDateTimeIn[index].GetDayOfWeek() == FRIDAY  and  OpenTimeValue ==  57840 )   // CALCUL DES PP de la semaine suivante, réalisé le vendredi à 16h04
		{
			FirstIndex[index] = index ;
			
			SCGraphData ExternalChartData;
			sc.GetChartBaseData(ExternalChartNumber.GetChartNumber(), ExternalChartData);
			int ExternalChartIndex = sc.GetContainingIndexForDateTimeIndex(ExternalChartNumber.GetChartNumber(), index); // on récupère l'index du nouveau chandelier sur le Weekly chart

			float fPHPrior = 0, fPBPrior = 0 , fSettlePrior = 0 ;
			float fRE1, fRE2, fRE3, fRE4, fRE5, fRE6, fRE7, fRE8, fRE9, fRE10, fRE11, fRE12, fRE13, fRE14, fRE15, fRE16 ;
			float fSE1, fSE2, fSE3, fSE4, fSE5, fSE6, fSE7, fSE8, fSE9, fSE10, fSE11, fSE12, fSE13, fSE14, fSE15, fSE16 ;
			
			// on récupère HLC
			fPHPrior = ExternalChartData[SC_HIGH][ExternalChartIndex];
			fPBPrior = ExternalChartData[SC_LOW][ExternalChartIndex];
			fSettlePrior = ExternalChartData[SC_LAST][ExternalChartIndex];
			
			// correction des bugs de flux
			SCDateTimeArray ExternalChartDateTime;
			sc.GetChartDateTimeArray(ExternalChartNumber.GetChartNumber(), ExternalChartDateTime);
			int Year, Month, Day;
			ExternalChartDateTime[ExternalChartIndex].GetDateYMD(Year, Month, Day);
		/* 	if (Year==2019 and Month==1 and Day==31)  // premier jour de la semaine pour lequel HLC est faux
			{
				//fPHPrior = nd;
				//fPBPrior = nd;
				//fSettlePrior = nd;
			} */
			
			//on calcule les PP 
			CalculateEighths (	fPHPrior, fPBPrior, fSettlePrior, 
										fRE1, fRE2, fRE3, fRE4, fRE5, fRE6, fRE7, fRE8, fRE9, fRE10, fRE11, fRE12, fRE13, fRE14, fRE15, fRE16, 
										fSE1, fSE2, fSE3, fSE4, fSE5, fSE6, fSE7, fSE8, fSE9, fSE10, fSE11, fSE12, fSE13, fSE14, fSE15, fSE16 ) ;
			
			// les valeurs sont entrées dans les sc.Subgraph[]
			RE1[index] =  fRE1;
			RE2[index] = fRE2;
			RE3[index] = fRE3;
			RE4[index] = fRE4;
			RE5[index] = fRE5;
			RE6[index] = fRE6;
			RE7[index] = fRE7;
			RE8[index] = fRE8;
			RE9[index] = fRE9;
			RE10[index] = fRE10;
			RE11[index] = fRE11;
			RE12[index] = fRE12;
			RE13[index] = fRE13;
			RE14[index] = fRE14;
			RE14[index] = fRE14;
			RE15[index] = fRE15;
			RE16[index] = fRE16;
			SE1[index] = fSE1;
			SE2[index] = fSE2;
			SE3[index] = fSE3;
			SE4[index] = fSE4;
			SE5[index] = fSE5;
			SE6[index] = fSE6;
			SE7[index] = fSE7;
			SE8[index] = fSE8;
			SE9[index] = fSE9;
			SE10[index] = fSE10;
			SE11[index] = fSE11;
			SE12[index] = fSE12;
			SE13[index] = fSE13;
			SE14[index] = fSE14;
			SE14[index] = fSE14;
			SE15[index] = fSE15;
			SE16[index] = fSE16;
			
		}
	
	
		else if (    sc.BaseDateTimeIn[index].GetDayOfWeek()  <  sc.BaseDateTimeIn[index-1].GetDayOfWeek()      or   index==0      )   // ie on change de semaine 
		{
			// FirstIndex[index] = index ;
			FirstIndex[index] = index - ShiftFirstIndex ;
			
			SCGraphData ExternalChartData;
			sc.GetChartBaseData(ExternalChartNumber.GetChartNumber(), ExternalChartData);
			int ExternalChartIndex = sc.GetContainingIndexForDateTimeIndex(ExternalChartNumber.GetChartNumber(), index); // on récupère l'index du nouveau chandelier sur le Weekly chart

			float fPHPrior = 0, fPBPrior = 0 , fSettlePrior = 0 ;
			float fRE1, fRE2, fRE3, fRE4, fRE5, fRE6, fRE7, fRE8, fRE9, fRE10, fRE11, fRE12, fRE13, fRE14, fRE15, fRE16 ;
			float fSE1, fSE2, fSE3, fSE4, fSE5, fSE6, fSE7, fSE8, fSE9, fSE10, fSE11, fSE12, fSE13, fSE14, fSE15, fSE16 ;
			
			// on récupère HLC
			fPHPrior = ExternalChartData[SC_HIGH][ExternalChartIndex-1];
			fPBPrior = ExternalChartData[SC_LOW][ExternalChartIndex-1];
			fSettlePrior = ExternalChartData[SC_LAST][ExternalChartIndex-1];
			
			// correction des bugs de flux
			SCDateTimeArray ExternalChartDateTime;
			sc.GetChartDateTimeArray(ExternalChartNumber.GetChartNumber(), ExternalChartDateTime);
			int Year, Month, Day;
			ExternalChartDateTime[ExternalChartIndex-1].GetDateYMD(Year, Month, Day);
			if (Year==2019 and Month==11 and Day==25)  // premier jour de la semaine pour lequel HLC est faux   // ve 29/11/2019 est cloturé à 13h
			{
				FirstIndex[index] = index - ShiftFirstIndex ;
				//fPHPrior = nd;
				// fPBPrior = nd;
				//fSettlePrior = nd;
			}
			
			//on calcule les PP 
			CalculateEighths (	fPHPrior, fPBPrior, fSettlePrior, 
										fRE1, fRE2, fRE3, fRE4, fRE5, fRE6, fRE7, fRE8, fRE9, fRE10, fRE11, fRE12, fRE13, fRE14, fRE15, fRE16, 
										fSE1, fSE2, fSE3, fSE4, fSE5, fSE6, fSE7, fSE8, fSE9, fSE10, fSE11, fSE12, fSE13, fSE14, fSE15, fSE16 ) ;
			
			// les valeurs sont entrées dans les sc.Subgraph[]
			RE1[index] = fRE1;
			RE2[index] = fRE2;
			RE3[index] = fRE3;
			RE4[index] = fRE4;
			RE5[index] = fRE5;
			RE6[index] = fRE6;
			RE7[index] = fRE7;
			RE8[index] = fRE8;
			RE9[index] = fRE9;
			RE10[index] = fRE10;
			RE11[index] = fRE11;
			RE12[index] = fRE12;
			RE13[index] = fRE13;
			RE14[index] = fRE14;
			RE14[index] = fRE14;
			RE15[index] = fRE15;
			RE16[index] = fRE16;
			SE1[index] = fSE1;
			SE2[index] = fSE2;
			SE3[index] = fSE3;
			SE4[index] = fSE4;
			SE5[index] = fSE5;
			SE6[index] = fSE6;
			SE7[index] = fSE7;
			SE8[index] = fSE8;
			SE9[index] = fSE9;
			SE10[index] = fSE10;
			SE11[index] = fSE11;
			SE12[index] = fSE12;
			SE13[index] = fSE13;
			SE14[index] = fSE14;
			SE14[index] = fSE14;
			SE15[index] = fSE15;
			SE16[index] = fSE16;
			
		}
		 
		else // on n'est pas dans le premier chandelier du jour
		{
			FirstIndex[index] = FirstIndex[index-1] ;
			RE1[index] =  RE1[index-1];
			RE2[index] = RE2[index-1];
			RE3[index] = RE3[index-1];
			RE4[index] = RE4[index-1];
			RE5[index] = RE5[index-1];
			RE6[index] = RE6[index-1];
			RE7[index] = RE7[index-1];
			RE8[index] = RE8[index-1];
			RE9[index] = RE9[index-1];
			RE10[index] = RE10[index-1];
			RE11[index] = RE11[index-1];
			RE12[index] = RE12[index-1];
			RE13[index] = RE13[index-1];
			RE14[index] = RE14[index-1];
			RE14[index] = RE14[index-1];
			RE15[index] = RE15[index-1];
			RE16[index] = RE16[index-1];
			SE1[index] = SE1[index-1];
			SE2[index] = SE2[index-1];
			SE3[index] = SE3[index-1];
			SE4[index] = SE4[index-1];
			SE5[index] = SE5[index-1];
			SE6[index] = SE6[index-1];
			SE7[index] = SE7[index-1];
			SE8[index] = SE8[index-1];
			SE9[index] = SE9[index-1];
			SE10[index] = SE10[index-1];
			SE11[index] = SE11[index-1];
			SE12[index] = SE12[index-1];
			SE13[index] = SE13[index-1];
			SE14[index] = SE14[index-1];
			SE14[index] = SE14[index-1];
			SE15[index] = SE15[index-1];
			SE16[index] = SE16[index-1];
		}
	}
	
	
	// Bloc d'affichage       
	if ( sc.ChartWindowIsActive  or  NbMonitors == 2  or  sc.IsFullRecalculation )
	{
		BottomCoordinate = sc.YPixelCoordinateToGraphValue(sc.StudyRegionBottomCoordinate) ;		// la variable est déclarée en dehors de la fct scsf
		TopCoordinate = sc.YPixelCoordinateToGraphValue(sc.StudyRegionTopCoordinate) ;				// la variable est déclarée en dehors de la fct scsf
		if (  TraceVectorielONOFF.GetYesNo()  and  ( FirstVisibleBar != sc.IndexOfFirstVisibleBar  or  LastVisibleBar != sc.IndexOfLastVisibleBar  or  sc.IsFullRecalculation  or  fabs(BottomCoordinatePrevious-BottomCoordinate) > 0.02  or  fabs(TopCoordinatePrevious-TopCoordinate) > 0.02 )  )          	// Le recalcul des lignes n'est réalisé que si FirstIndex ou LastVisibleBar ou BottomCoordinate varie (on fait l'hypothèse que TopCoordinate et BottomCoordinate varient de manière synchronisée)		// Note : Sierra a un bug interne pour le premier calcul de BottomCoordinate (il vaut la même valeur que TopCoordinate)    // Il faut rajouter sc.IsFullRecalculation car lors d'une Full ReCalculation les useTool non UDD sont effacés
		{	
			FirstVisibleBar = sc.IndexOfFirstVisibleBar ;
			LastVisibleBar = sc.IndexOfLastVisibleBar ;
			BottomCoordinatePrevious = BottomCoordinate ;
			TopCoordinatePrevious = TopCoordinate ;

			// les useTool doivent être effacés à chaque appel du DLL pour éviter une fuite de mémoire
			sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   // all non user drawn Chart Drawings on the specified ChartNumber will be deleted. The LineNumber parameter in this case is ignored and should be 0. 		
			
			s_sc* const ptrsc = &sc;
			int const firstIndex = FirstIndex[LastVisibleBar] ;  // index de la première bougie du mois : début du tracé de la ligne  // FirstIndex est un array qui contient le firstIndex
			
			for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)
			{
				float const Niveau = sc.Subgraph[SubgraphIndex][LastVisibleBar] ;
				SCString const Texte = sc.Subgraph[SubgraphIndex].Name ;
				int const epaisseur = sc.Subgraph[SubgraphIndex].LineWidth ;
				COLORREF const couleur = sc.Subgraph[SubgraphIndex].PrimaryColor;
				creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, Niveau, epaisseur, couleur, ptrsc);
			}
			
			// recalcul immédiat
			sc.UpdateAlways = 1 ;
		}
	}		
	if ( TraceVectorielONOFF.GetYesNo() == 0 )  sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_MesPointsPivotsJournaliers(SCStudyInterfaceRef sc)		// [6]
{
	SCSubgraphRef OpenGap = sc.Subgraph[0];																															// appelé par Amplitude_Mvt            
	SCSubgraphRef PHPrior = sc.Subgraph[1];    							// appelé par Navig_niveaux pour GenerationFichier_csv		// appelé par Amplitude_Mvt            
	SCSubgraphRef PBPrior = sc.Subgraph[2];    							// appelé par Navig_niveaux pour GenerationFichier_csv		// appelé par Amplitude_Mvt            
	SCSubgraphRef PP = sc.Subgraph[3];										// appelé par Navig_niveaux pour GenerationFichier_csv
	SCSubgraphRef R1 = sc.Subgraph[4];										// appelé par Navig_niveaux pour GenerationFichier_csv
	SCSubgraphRef R2 = sc.Subgraph[5];										// ...
	SCSubgraphRef R3 = sc.Subgraph[6];
	SCSubgraphRef R4 = sc.Subgraph[7];
	SCSubgraphRef S1 = sc.Subgraph[8];
	SCSubgraphRef S2 = sc.Subgraph[9];
	SCSubgraphRef S3 = sc.Subgraph[10];
	SCSubgraphRef S4 = sc.Subgraph[11];
	SCSubgraphRef mR1 = sc.Subgraph[12];
	SCSubgraphRef mR2 = sc.Subgraph[13];
	SCSubgraphRef mR3 = sc.Subgraph[14];
	SCSubgraphRef mR4 = sc.Subgraph[15];
	SCSubgraphRef mS1 = sc.Subgraph[16];
	SCSubgraphRef mS2 = sc.Subgraph[17];
	SCSubgraphRef mS3 = sc.Subgraph[18];									// ...
	SCSubgraphRef mS4 = sc.Subgraph[19];									// appelé par Navig_niveaux pour GenerationFichier_csv
	SCSubgraphRef RQ1 = sc.Subgraph[20];
	SCSubgraphRef RQ2 = sc.Subgraph[21];
	SCSubgraphRef RQ3 = sc.Subgraph[22];
	SCSubgraphRef RQ4 = sc.Subgraph[23];
	SCSubgraphRef RQ5 = sc.Subgraph[24];
	SCSubgraphRef RQ6 = sc.Subgraph[25];
	SCSubgraphRef RQ7 = sc.Subgraph[26];
	SCSubgraphRef RQ8 = sc.Subgraph[27];
	SCSubgraphRef SQ1 = sc.Subgraph[28];
	SCSubgraphRef SQ2 = sc.Subgraph[29];
	SCSubgraphRef SQ3 = sc.Subgraph[30];
	SCSubgraphRef SQ4 = sc.Subgraph[31];
	SCSubgraphRef SQ5 = sc.Subgraph[32];
	SCSubgraphRef SQ6 = sc.Subgraph[33];
	SCSubgraphRef SQ7 = sc.Subgraph[34];
	SCSubgraphRef SQ8 = sc.Subgraph[35];
	SCSubgraphRef SettlePrior = sc.Subgraph[36];						// appelé par Navig_niveaux pour GenerationFichier_csv
	SCSubgraphRef DemiGap = sc.Subgraph[37];						// appelé par Navig_niveaux pour GenerationFichier_csv
	const int NUMBER_OF_STUDY_SUBGRAPHS = 38;  


	SCFloatArrayRef OpenDaily = sc.Subgraph[0].Arrays[0] ; 		// appelé par scsf_MesPointsPivotsJournaliersProjetes
	SCFloatArrayRef FirstIndex = sc.Subgraph[0].Arrays[1] ;  		// Premier index du jour
	

	SCInputRef ExternalChartNumber = sc.Input[0];
	SCInputRef CalculForce = sc.Input[1];
	SCInputRef TraceVectorielONOFF = sc.Input[2];								// modifié par Prix_typique 		
	SCInputRef AffichageDesMid4Full4ONOFF = sc.Input[4];					// modifié par Prix_typique 		
	SCInputRef AffichageDesPHPBBougiePrecedente = sc.Input[5];		// modifié par Prix_typique 		
	
	
	int& NbMonitors = sc.GetPersistentIntFast(0);  
	int& FirstVisibleBar = sc.GetPersistentIntFast(1);  
	int& LastVisibleBar = sc.GetPersistentIntFast(2);  

	float& BottomCoordinatePrevious = sc.GetPersistentFloatFast(0);  
	float& TopCoordinatePrevious = sc.GetPersistentFloatFast(1);  


	

	if (sc.SetDefaults)
	{
		sc.AutoLoop = 0;  
		sc.GraphName = "[6] Pivots Ind J (Full/Mid/Quart)";       // appelé par Amplitude_Mvt      // appelé par Navig_niveaux pour GenerationFichier_csv	// appelé par scsf_MesPointsPivotsJournaliersProjetes
		sc.ScaleRangeType = SCALE_SAMEASREGION;
		sc.ValueFormat = 2;
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 0 ;
		sc.DisplayStudyInputValues = 0;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		
		sc.HideStudy = 1 ;

		// ExternalChartNumber.Name = "Daily Chart Number";
		ExternalChartNumber.SetChartNumber(2);
		CalculForce.Name = "Calcul force des pivots J a 13h04";  // calcul forcé des PP à 13h04 (utile lorsque la séance finit à 13h)
		CalculForce.SetYesNo(0);
		TraceVectorielONOFF.Name = "Trace Vectoriel ON/OFF";
		{
			if ( PeriodeChartEnSecondes(&sc) == 240 )  TraceVectorielONOFF.SetYesNo(0) ;   // Les PPJ ne sont pas tracés sur le graphe en UT 4min.
			else TraceVectorielONOFF.SetYesNo(1) ;
		}
		AffichageDesMid4Full4ONOFF.Name = "Affichage des Mid4 et Full4";	
		AffichageDesMid4Full4ONOFF.SetYesNo(1);
		AffichageDesPHPBBougiePrecedente.Name = "Affichage des PH et PB du mois precedent";	
		AffichageDesPHPBBougiePrecedente.SetYesNo(1);		
		
		// DisplayOrder des inputs
		TraceVectorielONOFF.DisplayOrder = 1 ;
	
		for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].DrawStyle = DRAWSTYLE_DASH;
			sc.Subgraph[SubgraphIndex].LineStyle = LINESTYLE_SOLID ;
			sc.Subgraph[SubgraphIndex].PrimaryColor = RGB(195,195,0) ;
			sc.Subgraph[SubgraphIndex].LineLabel = LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_LEFT_EDGE  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
			sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre
			sc.Subgraph[SubgraphIndex].DrawZeros = false ;
		}		
		
		// Niveaux quarts : on affiche leur nom mais pas leur valeur
		for (int SubgraphIndex = 20; SubgraphIndex < 36; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].LineLabel = LL_DISPLAY_NAME | LL_NAME_ALIGN_LEFT_EDGE  | LL_NAME_REVERSE_COLORS ;  // affiche le nom de l'indicateur retourné et sa valeur
		}

		
		PHPrior.PrimaryColor = RGB(255,0,0);
		
		PBPrior.PrimaryColor = RGB(0,168,0);
		
		SettlePrior.PrimaryColor = RGB(210,0,210);
		SettlePrior.DrawStyle = DRAWSTYLE_HIDDEN;
		
		OpenGap.PrimaryColor = RGB(210,0,210);
		OpenGap.LineLabel =                                 LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_LEFT_EDGE  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS  | LL_VALUE_ALIGN_VALUES_SCALE | LL_NAME_ALIGN_ABOVE; 

		DemiGap.PrimaryColor = RGB(210,0,210);
		
		
		PHPrior.Name = "PH JP";
		PBPrior.Name = "PB JP";
		SettlePrior.Name = "Cloture JP";
		PP.Name = "PP J";
		R1.Name = "R1 J";
		R2.Name = "R2 J";
		R3.Name = "R3 J";
		R4.Name = "R4 J";
		S1.Name = "S1 J";
		S2.Name = "S2 J";
		S3.Name = "S3 J";
		S4.Name = "S4 J";
		mR1.Name = "mR1 J";
		mR2.Name = "mR2 J";
		mR3.Name = "mR3 J";
		mR4.Name = "mR4 J";
		mS1.Name = "mS1 J";
		mS2.Name = "mS2 J";
		mS3.Name = "mS3 J";
		mS4.Name = "mS4 J";
		RQ1.Name = "RQ1 J";
		RQ2.Name = "RQ2 J";
		RQ3.Name = "RQ3 J";
		RQ4.Name = "RQ4 J";
		RQ5.Name = "RQ5 J";
		RQ6.Name = "RQ6 J";
		RQ7.Name = "RQ7 J";
		RQ8.Name = "RQ8 J";
		SQ1.Name = "SQ1 J";
		SQ2.Name = "SQ2 J";
		SQ3.Name = "SQ3 J";
		SQ4.Name = "SQ4 J";
		SQ5.Name = "SQ5 J";
		SQ6.Name = "SQ6 J";
		SQ7.Name = "SQ7 J";
		SQ8.Name = "SQ8 J";
		OpenGap.Name = "         OpenGap J";
		DemiGap.Name = "DemiGap J";
		
		
		PHPrior.LineWidth = 4;
		PBPrior.LineWidth = 4;
		SettlePrior.LineWidth = 2;
		PP.LineWidth = 3;
		R1.LineWidth = 3;
		R2.LineWidth = 3;
		R3.LineWidth = 3;
		R4.LineWidth = 3;
		S1.LineWidth = 3;
		S2.LineWidth = 3;
		S3.LineWidth = 3;
		S4.LineWidth = 3;
		mR1.LineWidth = 2;
		mR2.LineWidth = 2;
		mR3.LineWidth = 2;
		mR4.LineWidth = 2;
		mS1.LineWidth = 2;
		mS2.LineWidth = 2;
		mS3.LineWidth = 2;
		mS4.LineWidth = 2;
		RQ1.LineWidth = 1;
		RQ2.LineWidth = 1;
		RQ3.LineWidth = 1;
		RQ4.LineWidth = 1;
		RQ5.LineWidth = 1;
		RQ6.LineWidth = 1;
		RQ7.LineWidth = 1;
		RQ8.LineWidth = 1;
		SQ1.LineWidth = 1;
		SQ2.LineWidth = 1;
		SQ3.LineWidth = 1;
		SQ4.LineWidth = 1;
		SQ5.LineWidth = 1;
		SQ6.LineWidth = 1;
		SQ7.LineWidth = 1;
		SQ8.LineWidth = 1;
		OpenGap.LineWidth = 8;
		DemiGap.LineWidth = 3;
		
		return;
	}  // fin du bloc de " if (sc.SetDefaults) "
	
	
	
	if ( sc.UpdateStartIndex == 0 )
	{
		sc.GraphRegion = 0 ;
		NbMonitors = NumberOfMonitors() ;
		FirstVisibleBar = 0 ;
		LastVisibleBar = 0 ;

		// Affichage des Mid4Full4 et des PH/PB : par défaut on affiche tout, puis on met  "LineWidth = 0" à ce qui ne doit pas être affiché
		{
			RQ7.LineWidth = 1;
			RQ8.LineWidth = 1;
			SQ7.LineWidth = 1;
			SQ8.LineWidth = 1;
			mR4.LineWidth = 2;
			mS4.LineWidth = 2;
			R4.LineWidth = 3;
			S4.LineWidth = 3;
			PHPrior.LineWidth = 4;
			PBPrior.LineWidth = 4;
			SettlePrior.LineWidth = 2;
			OpenGap.LineWidth = 6;
			DemiGap.LineWidth = 2;
		}
		
		// Mid4/Full4 affichés
		if ( AffichageDesMid4Full4ONOFF.GetYesNo() == 0 )
		{
			RQ7.LineWidth = 0;
			RQ8.LineWidth = 0;
			SQ7.LineWidth = 0;
			SQ8.LineWidth = 0;
			mR4.LineWidth = 0;
			mS4.LineWidth = 0;
			R4.LineWidth = 0;
			S4.LineWidth = 0;
		}

		// PH/PB non affichés
		if ( AffichageDesPHPBBougiePrecedente.GetYesNo() == 0 )
		{
			PHPrior.LineWidth = 0;
			PBPrior.LineWidth = 0;
			SettlePrior.LineWidth = 0;
			OpenGap.LineWidth = 5;
			DemiGap.LineWidth = 0;
		}		
	}
	
	
	
	for (int index = sc.UpdateStartIndex ; index < sc.ArraySize ; index++)                 // début AutoLoop
	{
		int OpenTimeValue = sc.BaseDateTimeIn[index].GetTimeInSecondsWithoutMilliseconds()  ; //GetTimeInSecondsWithoutMilliseconds retourne une time_value
		
		if (  ( OpenTimeValue ==  57840 )  or  ( CalculForce.GetYesNo()  and  OpenTimeValue ==  47040  )  )    // CALCUL DES PP du lendemain, réalisé à 16h02, avec 57840 = 16*3600 + 4*60     // CALCUL DES PP du lendemain, réalisé à 13h04, avec 47040 = 13*3600 + 4*60
		{
			FirstIndex[index] = index ;		// on prend FirstIndex égal à l'index de 16h02
			
			//  "GetChartBaseData" copie intégralement les données source du chart en UT 1J
			SCGraphData ExternalChartData;
			sc.GetChartBaseData(ExternalChartNumber.GetChartNumber(), ExternalChartData);
			int ExternalChartIndex = sc.GetContainingIndexForDateTimeIndex(ExternalChartNumber.GetChartNumber(), index); // on récupère l'index du nouveau chandelier sur le daily chart

			float fPHPrior = 0, fPBPrior = 0 , fSettlePrior = 0, fPP = 0 ;
			float fR1 = 0, fR2 = 0, fR3 = 0, fR4 = 0, fS1 = 0, fS2 = 0, fS3 = 0, fS4 = 0 ;
			float fmR1 = 0, fmR2 = 0, fmR3 = 0, fmR4 = 0, fmS1 = 0, fmS2 = 0,  fmS3 = 0,   fmS4 = 0 ; 
			float fRQ1 = 0, fRQ2 = 0, fRQ3 = 0, fRQ4 = 0, fRQ5 = 0, fRQ6 = 0, fRQ7 = 0, fRQ8 = 0 ;
			float fSQ1 = 0, fSQ2 = 0, fSQ3 = 0, fSQ4 = 0, fSQ5 = 0, fSQ6 = 0, fSQ7 = 0, fSQ8 = 0 ;
			
			// on récupère HLC
			fPHPrior = ExternalChartData[SC_HIGH][ExternalChartIndex];
			fPBPrior = ExternalChartData[SC_LOW][ExternalChartIndex];
			fSettlePrior = ExternalChartData[SC_LAST][ExternalChartIndex];
			
			// correction des bugs de flux
			{
				SCDateTimeArray ExternalChartDateTime;		
				sc.GetChartDateTimeArray(ExternalChartNumber.GetChartNumber(), ExternalChartDateTime);
				int Year, Month, Day;
				ExternalChartDateTime[ExternalChartIndex].GetDateYMD(Year, Month, Day);
				if (Year==2020 and Month==5 and Day==29)  // jour pour lequel le high est faux
				{
					// le high du jour, fourni par BarChart, est faux  => il faut éditer les data daily et mettre la bonne valeur (9573.55 remplace xxx)
					if ( fabs(fPHPrior-9573.55) >0.01 )
					{ SCString message ;  message.Format("!! ERREUR dans data du graphe UT daily du INQ100 !! Le High reel du 29 mai 2020 vaut : 9573.55 pts (vs %.2f pts qui est faux)", fPHPrior) ; sc.AddMessageToLog(message,1) ; }
				}
				if (Year==2020 and Month==6 and Day==25)  // jour pour lequel le high est faux
				{
					// le high du jour, fourni par BarChart, est faux  => il faut éditer les data daily et mettre la bonne valeur (10110.29 remplace xxx)
					if ( fabs(fPHPrior-10110.29) >0.01 )
					{ SCString message ;  message.Format("!! ERREUR dans data du graphe UT daily du INQ100 !! Le High reel du 25 juin 2020 vaut : 10110.29 pts (vs %.2f pts qui est faux)", fPHPrior) ; sc.AddMessageToLog(message,1) ; }
				}
			}

			//on calcule les PP 
			CalculatePPs (fPHPrior, fPBPrior, fSettlePrior, fPP, fR1, fR2, fR3, fR4, fS1, fS2, fS3, fS4, fmR1, fmR2, fmR3, fmR4, fmS1, fmS2, fmS3, fmS4, fRQ1, fRQ2, fRQ3, fRQ4, fRQ5, fRQ6, fRQ7, fRQ8, fSQ1, fSQ2, fSQ3, fSQ4, fSQ5, fSQ6, fSQ7, fSQ8);
			
			// les valeurs sont entrées dans les sc.Subgraph[]
			PHPrior[index] = fPHPrior;
			PBPrior[index] = fPBPrior;
			SettlePrior[index] = fSettlePrior;
			PP[index] = fPP;
			R1[index] = fR1;
			R2[index] = fR2;
			R3[index] = fR3;
			R4[index] = fR4;
			S1[index] = fS1;
			S2[index] = fS2;
			S3[index] = fS3;
			S4[index] = fS4;
			mR1[index] = fmR1;
			mR2[index] = fmR2;
			mR3[index] = fmR3;
			mR4[index] = fmR4;
			mS1[index] = fmS1;
			mS2[index] = fmS2;
			mS3[index] = fmS3;
			mS4[index] = fmS4;
			RQ1[index] = fRQ1;
			RQ2[index] = fRQ2;
			RQ3[index] = fRQ3;
			RQ4[index] = fRQ4;
			RQ5[index] = fRQ5;
			RQ6[index] = fRQ6;
			RQ7[index] = fRQ7;
			RQ8[index] = fRQ8;
			SQ1[index] = fSQ1;
			SQ2[index] = fSQ2;
			SQ3[index] = fSQ3;
			SQ4[index] = fSQ4;
			SQ5[index] = fSQ5;
			SQ6[index] = fSQ6;
			SQ7[index] = fSQ7;
			SQ8[index] = fSQ8;
			OpenGap[index] = 0;
			DemiGap[index] = 0;
			OpenDaily[index] = 0;
		}
		

		else if ( sc.BaseDateTimeIn[index].GetDate() != sc.BaseDateTimeIn[index-1].GetDate()  or  index==0 )    // RECALCUL DES PP à l'ouverture de la séance (et calcul de l'open gap)
		{
			FirstIndex[index] = FirstIndex[index-1] ;   // on prend FirstIndex égal à l'index de 16h02 à J-1
			
			SCGraphData ExternalChartData;
			sc.GetChartBaseData(ExternalChartNumber.GetChartNumber(), ExternalChartData);
			int ExternalChartIndex = sc.GetContainingIndexForDateTimeIndex(ExternalChartNumber.GetChartNumber(), index); // on récupère l'index du nouveau chandelier sur le daily chart

			float fPHPrior = 0, fPBPrior = 0 , fSettlePrior = 0, fPP = 0 ;
			float fR1 = 0, fR2 = 0, fR3 = 0, fR4 = 0, fS1 = 0, fS2 = 0, fS3 = 0, fS4 = 0 ;
			float fmR1 = 0, fmR2 = 0, fmR3 = 0, fmR4 = 0, fmS1 = 0, fmS2 = 0,  fmS3 = 0,   fmS4 = 0 ; 
			float fRQ1 = 0, fRQ2 = 0, fRQ3 = 0, fRQ4 = 0, fRQ5 = 0, fRQ6 = 0, fRQ7 = 0, fRQ8 = 0 ;
			float fSQ1 = 0, fSQ2 = 0, fSQ3 = 0, fSQ4 = 0, fSQ5 = 0, fSQ6 = 0, fSQ7 = 0, fSQ8 = 0 ;
			
			// on récupère HLC de la veille
			fPHPrior = ExternalChartData[SC_HIGH][ExternalChartIndex-1];
			fPBPrior = ExternalChartData[SC_LOW][ExternalChartIndex-1];
			fSettlePrior = ExternalChartData[SC_LAST][ExternalChartIndex-1];
			
			// correction des bugs de flux
			SCDateTimeArray ExternalChartDateTime;
			sc.GetChartDateTimeArray(ExternalChartNumber.GetChartNumber(), ExternalChartDateTime);
			int Year, Month, Day;
			ExternalChartDateTime[ExternalChartIndex-1].GetDateYMD(Year, Month, Day);
			if (Year==2019 and Month==11 and Day==29)  // vendredi de Thanksgiving : cloture à 13h  =>  correction de FirstIndex qui n'a pas été déterminé à 16h04
			{
				FirstIndex[index] = index - 3 ;
				//fPHPrior = nd;
				// fPBPrior = nd;
				//fSettlePrior = nd;
			}
			
			//on calcule les PP 
			CalculatePPs (fPHPrior, fPBPrior, fSettlePrior, fPP, fR1, fR2, fR3, fR4, fS1, fS2, fS3, fS4, fmR1, fmR2, fmR3, fmR4, fmS1, fmS2, fmS3, fmS4, fRQ1, fRQ2, fRQ3, fRQ4, fRQ5, fRQ6, fRQ7, fRQ8, fSQ1, fSQ2, fSQ3, fSQ4, fSQ5, fSQ6, fSQ7, fSQ8);
			
			// les valeurs sont entrées dans les sc.Subgraph[]
			PHPrior[index] = fPHPrior;
			PBPrior[index] = fPBPrior;
			SettlePrior[index] = fSettlePrior;
			PP[index] = fPP;
			R1[index] = fR1;
			R2[index] = fR2;
			R3[index] = fR3;
			R4[index] = fR4;
			S1[index] = fS1;
			S2[index] = fS2;
			S3[index] = fS3;
			S4[index] = fS4;
			mR1[index] = fmR1;
			mR2[index] = fmR2;
			mR3[index] = fmR3;
			mR4[index] = fmR4;
			mS1[index] = fmS1;
			mS2[index] = fmS2;
			mS3[index] = fmS3;
			mS4[index] = fmS4;
			RQ1[index] = fRQ1;
			RQ2[index] = fRQ2;
			RQ3[index] = fRQ3;
			RQ4[index] = fRQ4;
			RQ5[index] = fRQ5;
			RQ6[index] = fRQ6;
			RQ7[index] = fRQ7;
			RQ8[index] = fRQ8;
			SQ1[index] = fSQ1;
			SQ2[index] = fSQ2;
			SQ3[index] = fSQ3;
			SQ4[index] = fSQ4;
			SQ5[index] = fSQ5;
			SQ6[index] = fSQ6;
			SQ7[index] = fSQ7;
			SQ8[index] = fSQ8;
			OpenDaily[index] = sc.Open[index];
			
			// Recherche d'un opengap dans la première bougie du jour
			if (   sc.Open[index] > fPHPrior   )      // gap haussier  
			{
				OpenGap[index] = fPHPrior;
				DemiGap[index] = (sc.Open[index] + fPHPrior)/2;
			}
			else if (   ( sc.Open[index] < fPBPrior )    )     //    gap baissier
			{
				OpenGap[index] = fPBPrior;
				DemiGap[index] = (sc.Open[index] + fPBPrior)/2;
			}
			else    // pas de gap
			{
				OpenGap[index] = 0;
				DemiGap[index] = 0;
			}
		}
	 
		else // on recopie les valeurs à index-1
		{
			FirstIndex[index] = FirstIndex[index-1] ;
			PHPrior[index] = PHPrior[index-1] ;
			PBPrior[index] = PBPrior[index-1] ;
			SettlePrior[index] = SettlePrior[index-1] ;
			PP[index] = PP[index-1];
			R1[index] = R1[index-1];
			R2[index] = R2[index-1];
			R3[index] = R3[index-1];
			R4[index] = R4[index-1];
			S1[index] = S1[index-1];
			S2[index] = S2[index-1];
			S3[index] = S3[index-1];
			S4[index] = S4[index-1];
			mR1[index] = mR1[index-1];
			mR2[index] = mR2[index-1];
			mR3[index] = mR3[index-1];
			mR4[index] = mR4[index-1];
			mS1[index] = mS1[index-1];
			mS2[index] = mS2[index-1];
			mS3[index] = mS3[index-1];
			mS4[index] = mS4[index-1];
			RQ1[index] = RQ1[index-1];
			RQ2[index] = RQ2[index-1];
			RQ3[index] = RQ3[index-1];
			RQ4[index] = RQ4[index-1];
			RQ5[index] = RQ5[index-1];
			RQ6[index] = RQ6[index-1];
			RQ7[index] = RQ7[index-1];
			RQ8[index] = RQ8[index-1];
			SQ1[index] = SQ1[index-1];
			SQ2[index] = SQ2[index-1];
			SQ3[index] = SQ3[index-1];
			SQ4[index] = SQ4[index-1];
			SQ5[index] = SQ5[index-1];
			SQ6[index] = SQ6[index-1];
			SQ7[index] = SQ7[index-1];
			SQ8[index] = SQ8[index-1];
			OpenGap[index] = OpenGap[index-1];
			DemiGap[index] = DemiGap[index-1];
			OpenDaily[index] = OpenDaily[index-1];
		}

	} // fin de AutoLoop
	
	
	// Bloc d'affichage       
	if ( sc.ChartWindowIsActive  or  NbMonitors == 2  or  sc.IsFullRecalculation )
	{
		BottomCoordinate = sc.YPixelCoordinateToGraphValue(sc.StudyRegionBottomCoordinate) ;		// la variable est déclarée en dehors de la fct scsf
		TopCoordinate = sc.YPixelCoordinateToGraphValue(sc.StudyRegionTopCoordinate) ;				// la variable est déclarée en dehors de la fct scsf
		if (  TraceVectorielONOFF.GetYesNo()  and  ( FirstVisibleBar != sc.IndexOfFirstVisibleBar  or  LastVisibleBar != sc.IndexOfLastVisibleBar  or  sc.IsFullRecalculation  or  fabs(BottomCoordinatePrevious-BottomCoordinate) > 0.02  or  fabs(TopCoordinatePrevious-TopCoordinate) > 0.02 )  )          	// Le recalcul des lignes n'est réalisé que si FirstIndex ou LastVisibleBar ou BottomCoordinate varie (on fait l'hypothèse que TopCoordinate et BottomCoordinate varient de manière synchronisée)		// Note : Sierra a un bug interne pour le premier calcul de BottomCoordinate (il vaut la même valeur que TopCoordinate)    // Il faut rajouter sc.IsFullRecalculation car lors d'une Full ReCalculation les useTool non UDD sont effacés
		{	
			FirstVisibleBar = sc.IndexOfFirstVisibleBar ;
			LastVisibleBar = sc.IndexOfLastVisibleBar ;
			BottomCoordinatePrevious = BottomCoordinate ;
			TopCoordinatePrevious = TopCoordinate ;

			// les useTool doivent être effacés à chaque appel du DLL pour éviter une fuite de mémoire
			sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   // all non user drawn Chart Drawings on the specified ChartNumber will be deleted. The LineNumber parameter in this case is ignored and should be 0. 		
			
			s_sc* const ptrsc = &sc;
			int const firstIndex = FirstIndex[LastVisibleBar] ;  // index de la première bougie du jour : début du tracé de la ligne  // FirstIndex est un array qui contient le firstIndex
			
			for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)
			{
				float const Niveau = sc.Subgraph[SubgraphIndex][LastVisibleBar] ;
				SCString const Texte = sc.Subgraph[SubgraphIndex].Name ;
				int const epaisseur = sc.Subgraph[SubgraphIndex].LineWidth ;
				COLORREF const couleur = sc.Subgraph[SubgraphIndex].PrimaryColor;
				creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, Niveau, epaisseur, couleur, ptrsc);
			}
			
			// recalcul immédiat
			sc.UpdateAlways = 1 ;
		}		
	}
	if ( TraceVectorielONOFF.GetYesNo() == 0 )  sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_MesPointsPivotsQuatreHeures(SCStudyInterfaceRef sc)		// [7]
{
	SCSubgraphRef PP = sc.Subgraph[0];       // appelé par Navig_Niveaux
	SCSubgraphRef R1 = sc.Subgraph[1];       // appelé par Navig_Niveaux
	SCSubgraphRef R2 = sc.Subgraph[2];       // appelé par Navig_Niveaux
	SCSubgraphRef R3 = sc.Subgraph[3];       // appelé par Navig_Niveaux
	SCSubgraphRef R4 = sc.Subgraph[4];       // appelé par Navig_Niveaux
	SCSubgraphRef S1 = sc.Subgraph[5];       // appelé par Navig_Niveaux
	SCSubgraphRef S2 = sc.Subgraph[6];       // appelé par Navig_Niveaux
	SCSubgraphRef S3 = sc.Subgraph[7];       // appelé par Navig_Niveaux
	SCSubgraphRef S4 = sc.Subgraph[8];       // appelé par Navig_Niveaux
	const int NUMBER_OF_STUDY_SUBGRAPHS = 9;  
	
	SCFloatArrayRef FirstIndex = sc.Subgraph[0].Arrays[0] ;  

	SCInputRef ExternalChartNumber = sc.Input[0];
	SCInputRef TraceVectorielONOFF = sc.Input[2];		// modifié par Prix_typique du chart #6
	

	int& NbMonitors = sc.GetPersistentIntFast(0);  
	int& FirstVisibleBar = sc.GetPersistentIntFast(1);  
	int& LastVisibleBar = sc.GetPersistentIntFast(2);  

	float& BottomCoordinatePrevious = sc.GetPersistentFloatFast(0);  
	float& TopCoordinatePrevious = sc.GetPersistentFloatFast(1);  
	

	
	if (sc.SetDefaults)
	{
		sc.AutoLoop = 0;  
		sc.GraphName = "[7] Pivots Ind 4h (Full)";            // appelé par Navig_Niveaux		// appelé par Prix_typique du chart #6
		sc.ScaleRangeType = SCALE_SAMEASREGION;
		sc.GraphRegion = 0;
		sc.ValueFormat = VALUEFORMAT_INHERITED;
		sc.DisplayStudyName = 0;
		sc.DisplayStudyInputValues = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue = 0 ;
		sc.HideStudy = 1 ;

		ExternalChartNumber.Name = "240 Min Chart Number";
		ExternalChartNumber.SetChartNumber(17);
		TraceVectorielONOFF.Name = "Trace Vectoriel ON/OFF";
		TraceVectorielONOFF.SetYesNo(1);

		// DisplayOrder des inputs
		TraceVectorielONOFF.DisplayOrder = 1 ;

		for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].DrawStyle = DRAWSTYLE_DASH;
			sc.Subgraph[SubgraphIndex].PrimaryColor = RGB(255,128,0);
			sc.Subgraph[SubgraphIndex].LineLabel = LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_CENTER | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
			sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre
			sc.Subgraph[SubgraphIndex].DrawZeros = false;
			sc.Subgraph[SubgraphIndex].LineStyle = LINESTYLE_SOLID ;
			sc.Subgraph[SubgraphIndex].LineWidth = 2;
		}
	
		PP.Name = " PP 4h" ;
		R1.Name = " R1 4h" ;
		R2.Name = " R2 4h" ;
		R3.Name = " R3 4h" ;
		R4.Name = " R4 4h" ;
		S1.Name = " S1 4h" ;
		S2.Name = " S2 4h" ;
		S3.Name = " S3 4h" ;
		S4.Name = " S4 4h" ;
		
		return;
	}  // fin du bloc de " if (sc.SetDefaults) "
	

	if ( sc.UpdateStartIndex == 0 )
	{
		sc.GraphRegion = 0 ;
		NbMonitors = NumberOfMonitors() ;
		FirstVisibleBar = 0 ;
		LastVisibleBar = 0 ;
	}

	
	for (int index = sc.UpdateStartIndex; index < sc.ArraySize; index++)                 // début AutoLoop 
	{
		int heure = sc.BaseDateTimeIn[index].GetHour() ;			// GetHour est une fonction buggée (arrondi à l'heure pleine après XXh59m59s500ms), mais son usage ne pose pas de problème sur les indices (il n'y a pas de bougie datée après XXh59m59s500ms).
		
		if (  heure != sc.BaseDateTimeIn[index-1].GetHour()  )
		{	
			if ( heure == 13 )   // chandelier de 13h00            // les PP4h du INQ100 ne sont calculés qu'une seule fois par jour : à 13h00
			{
				FirstIndex[index] = index ;
				
				// déclaration des PP
				float fPHPrior = 0, fPBPrior = 0 , fSettlePrior = 0, fPP = 0 ;
				float fR1 = 0, fR2 = 0, fR3 = 0, fR4 = 0, fS1 = 0, fS2 = 0, fS3 = 0, fS4 = 0 ;
				float fmR1 = 0, fmR2 = 0, fmR3 = 0, fmR4 = 0, fmS1 = 0, fmS2 = 0,  fmS3 = 0,   fmS4 = 0 ; 
				float fRQ1 = 0, fRQ2 = 0, fRQ3 = 0, fRQ4 = 0, fRQ5 = 0, fRQ6 = 0, fRQ7 = 0, fRQ8 = 0 ;
				float fSQ1 = 0, fSQ2 = 0, fSQ3 = 0, fSQ4 = 0, fSQ5 = 0, fSQ6 = 0, fSQ7 = 0, fSQ8 = 0 ;
				
				//  "GetChartBaseData" copie intégralement les données source 
				SCGraphData ExternalChartData;
				sc.GetChartBaseData(ExternalChartNumber.GetChartNumber(), ExternalChartData);
				SCDateTimeArray  ExternalChartTimeArray;
				sc.GetChartDateTimeArray(ExternalChartNumber.GetChartNumber(), ExternalChartTimeArray);
				
				// Récupère un index dans le ExternalChart. Il y a deux possibilités pour cet index : 	- cas1 : il s'agit de la bougie de l'heure précédente
				//																															- cas 2 : il s'agit de la nouvelle bougie
				int ExternalChartIndex = sc.GetContainingIndexForDateTimeIndex(ExternalChartNumber.GetChartNumber(), index); 
				
				// On détermine si ExternalChartIndex et index font partie de la même heure
				if ( ExternalChartTimeArray[ExternalChartIndex].GetHour()  ==  sc.BaseDateTimeIn[index].GetHour() )             // cas 2 :  ExternalChartIndex et index font partie de la même heure
				{
					fPHPrior = ExternalChartData[SC_HIGH][ExternalChartIndex-1];                         
					fPBPrior = ExternalChartData[SC_LOW][ExternalChartIndex-1];
					fSettlePrior = ExternalChartData[SC_LAST][ExternalChartIndex-1];
				}
				else      // cas 1 : ExternalChartIndex correspond à la bougie de l'heure précédente
				{
					fPHPrior = ExternalChartData[SC_HIGH][ExternalChartIndex];                         
					fPBPrior = ExternalChartData[SC_LOW][ExternalChartIndex];
					fSettlePrior = ExternalChartData[SC_LAST][ExternalChartIndex];
				}

				//on calcule les PP 
				CalculatePPs (fPHPrior, fPBPrior, fSettlePrior, fPP, fR1, fR2, fR3, fR4, fS1, fS2, fS3, fS4, fmR1, fmR2, fmR3, fmR4, fmS1, fmS2, fmS3, fmS4, fRQ1, fRQ2, fRQ3, fRQ4, fRQ5, fRQ6, fRQ7, fRQ8, fSQ1, fSQ2, fSQ3, fSQ4, fSQ5, fSQ6, fSQ7, fSQ8);
				
				// les valeurs sont entrées dans les sc.Subgraph[]
				PP[index] = fPP;
				R1[index] = fR1;
				R2[index] = fR2;
				R3[index] = fR3;
				R4[index] = fR4;
				S1[index] = fS1;
				S2[index] = fS2;
				S3[index] = fS3;
				S4[index] = fS4;
			}
			else if ( heure == 16  or  heure == 9 ) //  heure == 16 correspond à la fin de la séance      // pour heure == 9 on refait une RAZ au cas où la veille cloture à 13h
			{
				FirstIndex[index] = 0 ;
				PP[index] = 0;
				R1[index] = 0;
				R2[index] = 0;
				R3[index] = 0;
				R4[index] = 0;
				S1[index] = 0;
				S2[index] = 0;
				S3[index] = 0;
				S4[index] = 0;
			}
			else  // autre changement d'heure
			{
				FirstIndex[index] = FirstIndex[index-1] ;
				PP[index] = PP[index-1];
				R1[index] = R1[index-1];
				R2[index] = R2[index-1];
				R3[index] = R3[index-1];
				R4[index] = R4[index-1];
				S1[index] = S1[index-1];
				S2[index] = S2[index-1];
				S3[index] = S3[index-1];
				S4[index] = S4[index-1];
			}
		}
		else 
		{
			FirstIndex[index] = FirstIndex[index-1] ;
			PP[index] = PP[index-1];
			R1[index] = R1[index-1];
			R2[index] = R2[index-1];
			R3[index] = R3[index-1];
			R4[index] = R4[index-1];
			S1[index] = S1[index-1];
			S2[index] = S2[index-1];
			S3[index] = S3[index-1];
			S4[index] = S4[index-1];
		}
	}

	

	// Bloc d'affichage       
	if ( sc.ChartWindowIsActive  or  NbMonitors == 2  or  sc.IsFullRecalculation )
	{
		BottomCoordinate = sc.YPixelCoordinateToGraphValue(sc.StudyRegionBottomCoordinate) ;		// la variable est déclarée en dehors de la fct scsf
		TopCoordinate = sc.YPixelCoordinateToGraphValue(sc.StudyRegionTopCoordinate) ;				// la variable est déclarée en dehors de la fct scsf
		if (  TraceVectorielONOFF.GetYesNo()  and  ( FirstVisibleBar != sc.IndexOfFirstVisibleBar  or  LastVisibleBar != sc.IndexOfLastVisibleBar  or  sc.IsFullRecalculation  or  fabs(BottomCoordinatePrevious-BottomCoordinate) > 0.02  or  fabs(TopCoordinatePrevious-TopCoordinate) > 0.02 )  )          	// Le recalcul des lignes n'est réalisé que si FirstIndex ou LastVisibleBar ou BottomCoordinate varie (on fait l'hypothèse que TopCoordinate et BottomCoordinate varient de manière synchronisée)		// Note : Sierra a un bug interne pour le premier calcul de BottomCoordinate (il vaut la même valeur que TopCoordinate)    // Il faut rajouter sc.IsFullRecalculation car lors d'une Full ReCalculation les useTool non UDD sont effacés
		{	
			FirstVisibleBar = sc.IndexOfFirstVisibleBar ;
			LastVisibleBar = sc.IndexOfLastVisibleBar ;
			BottomCoordinatePrevious = BottomCoordinate ;
			TopCoordinatePrevious = TopCoordinate ;
			
			// les useTool doivent être effacés à chaque appel du bloc
			sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   // all non user drawn Chart Drawings on the specified ChartNumber will be deleted. The LineNumber parameter in this case is ignored and should be 0. 		
			
			s_sc* const ptrsc = &sc;
			int const firstIndex = FirstIndex[LastVisibleBar] ;
			int const epaisseur = 2 ;
			COLORREF const couleur = RGB(255,128,0) ;
			
			for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)
			{
				float const Niveau = sc.Subgraph[SubgraphIndex][LastVisibleBar] ;
				SCString const Texte = sc.Subgraph[SubgraphIndex].Name ;
				creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, Niveau, epaisseur, couleur, ptrsc);    // void creationLigneEtTexte (SCString Texte, int beginIndex, int endIndex, float niveau, int monEpaisseur, COLORREF maCouleur, s_sc* ptrsc)
			}
			
			// recalcul immédiat
			sc.UpdateAlways = 1 ;
		}
	}
	if ( TraceVectorielONOFF.GetYesNo() == 0 )  sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_MesPointsPivotsUneHeure(SCStudyInterfaceRef sc)		// [8]
{
	SCSubgraphRef PP = sc.Subgraph[0];            // appelé par Navig_Niveaux
	SCSubgraphRef R1 = sc.Subgraph[1];            // appelé par Navig_Niveaux
	SCSubgraphRef R2 = sc.Subgraph[2];            // appelé par Navig_Niveaux
	SCSubgraphRef R3 = sc.Subgraph[3];            // appelé par Navig_Niveaux
	SCSubgraphRef R4 = sc.Subgraph[4];            // appelé par Navig_Niveaux
	SCSubgraphRef S1 = sc.Subgraph[5];            // appelé par Navig_Niveaux
	SCSubgraphRef S2 = sc.Subgraph[6];            // appelé par Navig_Niveaux
	SCSubgraphRef S3 = sc.Subgraph[7];            // appelé par Navig_Niveaux
	SCSubgraphRef S4 = sc.Subgraph[8];            // appelé par Navig_Niveaux
	const int NUMBER_OF_STUDY_SUBGRAPHS = 9;  
	
	SCFloatArrayRef FirstIndex = sc.Subgraph[0].Arrays[0] ;  
	
	SCInputRef ExternalChartNumber = sc.Input[0];
	SCInputRef TraceVectorielONOFF = sc.Input[2];  // modifié par Prix_typique 

	
	int& NbMonitors = sc.GetPersistentIntFast(0);  
	int& FirstVisibleBar = sc.GetPersistentIntFast(1);  
	int& LastVisibleBar = sc.GetPersistentIntFast(2);  
	
	float& BottomCoordinatePrevious = sc.GetPersistentFloatFast(0);  
	float& TopCoordinatePrevious = sc.GetPersistentFloatFast(1);  
	

	if (sc.SetDefaults)
	{
		sc.AutoLoop = 0;  
		sc.GraphName = "[8] Pivots Ind 1h (Full)";                       // appelé par Navig_Niveaux       // appelé par Prix_typique du chart #6
		sc.ScaleRangeType = SCALE_SAMEASREGION;
		sc.GraphRegion = 0;
		sc.ValueFormat = VALUEFORMAT_INHERITED;
		sc.DisplayStudyName = 0;
		sc.DisplayStudyInputValues = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue = 0 ;
		
		sc.HideStudy = 1 ;
		
		ExternalChartNumber.Name = "60 Min Chart Number";
		ExternalChartNumber.SetChartNumber(16);
		TraceVectorielONOFF.Name = "Trace Vectoriel ON/OFF";
		TraceVectorielONOFF.SetYesNo(1);

		// DisplayOrder des inputs
		TraceVectorielONOFF.DisplayOrder = 1 ;

		for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].DrawStyle = DRAWSTYLE_DASH;
			sc.Subgraph[SubgraphIndex].PrimaryColor = RGB(182,108,255);  
			sc.Subgraph[SubgraphIndex].LineLabel = LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_FAR_RIGHT | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_CENTER | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
			sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre
			sc.Subgraph[SubgraphIndex].DrawZeros = false;
			sc.Subgraph[SubgraphIndex].LineStyle = LINESTYLE_SOLID ;
			sc.Subgraph[SubgraphIndex].LineWidth = 2;
		}
	
		PP.Name = "PP 1h";
		R1.Name = "R1 1h";
		R2.Name = "R2 1h";
		R3.Name = "R3 1h";
		R4.Name = "R4 1h";
		S1.Name = "S1 1h";
		S2.Name = "S2 1h";
		S3.Name = "S3 1h";
		S4.Name = "S4 1h";
		
		return;
	}  // fin du bloc de " if (sc.SetDefaults) "
	

	if ( sc.UpdateStartIndex == 0 )
	{
		sc.GraphRegion = 0 ;
		NbMonitors = NumberOfMonitors() ;
		FirstVisibleBar = 0 ;
		LastVisibleBar = 0 ;
	}
	
	
	for (int index = sc.UpdateStartIndex; index < sc.ArraySize; index++)                 // début AutoLoop
	{
		int heure = sc.BaseDateTimeIn[index].GetHour() ;
		
		if (  heure != sc.BaseDateTimeIn[index-1].GetHour()  )   // on change d'heure
		{	
			if ( heure == 16  or  heure == 9 )    // fin de séance        // pour heure == 9 on refait une RAZ au cas où la veille on cloture à 13h
			{
				FirstIndex[index] = 0 ;
				PP[index] = 0;
				R1[index] = 0;
				R2[index] = 0;
				R3[index] = 0;
				R4[index] = 0;
				S1[index] = 0;
				S2[index] = 0;
				S3[index] = 0;
				S4[index] = 0;
			}
			else            // les PP1h du INQ100 sont calculés à chaque changement d'heure (à partir de 10h donc)
			{
				FirstIndex[index] = index ;
				
				// déclaration des PP
				float fPHPrior = 0, fPBPrior = 0 , fSettlePrior = 0, fPP = 0 ;
				float fR1 = 0, fR2 = 0, fR3 = 0, fR4 = 0, fS1 = 0, fS2 = 0, fS3 = 0, fS4 = 0 ;
				float fmR1 = 0, fmR2 = 0, fmR3 = 0, fmR4 = 0, fmS1 = 0, fmS2 = 0,  fmS3 = 0,   fmS4 = 0 ; 
				float fRQ1 = 0, fRQ2 = 0, fRQ3 = 0, fRQ4 = 0, fRQ5 = 0, fRQ6 = 0, fRQ7 = 0, fRQ8 = 0 ;
				float fSQ1 = 0, fSQ2 = 0, fSQ3 = 0, fSQ4 = 0, fSQ5 = 0, fSQ6 = 0, fSQ7 = 0, fSQ8 = 0 ;
				
				//  "GetChartBaseData" copie intégralement les données source 
				SCGraphData ExternalChartData;
				sc.GetChartBaseData(ExternalChartNumber.GetChartNumber(), ExternalChartData);
				SCDateTimeArray  ExternalChartTimeArray;
				sc.GetChartDateTimeArray(ExternalChartNumber.GetChartNumber(), ExternalChartTimeArray);
				
				// Récupère un index dans le ExternalChart. Il y a deux possibilités pour cet index : 	- cas1 : il s'agit de la bougie de l'heure précédente
				//																															- cas 2 : il s'agit de la nouvelle bougie
				int ExternalChartIndex = sc.GetContainingIndexForDateTimeIndex(ExternalChartNumber.GetChartNumber(), index); 
				
				// On détermine si ExternalChartIndex et index font partie de la même heure
				if ( ExternalChartTimeArray[ExternalChartIndex].GetHour()  ==  sc.BaseDateTimeIn[index].GetHour() )             // cas 2 :  ExternalChartIndex et index font partie de la même heure
				{
					fPHPrior = ExternalChartData[SC_HIGH][ExternalChartIndex-1];                         
					fPBPrior = ExternalChartData[SC_LOW][ExternalChartIndex-1];
					fSettlePrior = ExternalChartData[SC_LAST][ExternalChartIndex-1];
				}
				else      // cas 1 : ExternalChartIndex correspond à la bougie de l'heure précédente
				{
					fPHPrior = ExternalChartData[SC_HIGH][ExternalChartIndex];                         
					fPBPrior = ExternalChartData[SC_LOW][ExternalChartIndex];
					fSettlePrior = ExternalChartData[SC_LAST][ExternalChartIndex];
				}
				
				//on calcule les PP 
				CalculatePPs (fPHPrior, fPBPrior, fSettlePrior, fPP, fR1, fR2, fR3, fR4, fS1, fS2, fS3, fS4, fmR1, fmR2, fmR3, fmR4, fmS1, fmS2, fmS3, fmS4, fRQ1, fRQ2, fRQ3, fRQ4, fRQ5, fRQ6, fRQ7, fRQ8, fSQ1, fSQ2, fSQ3, fSQ4, fSQ5, fSQ6, fSQ7, fSQ8);
				
				// les valeurs sont entrées dans les sc.Subgraph[]
				PP[index] = fPP;
				R1[index] = fR1;
				R2[index] = fR2;
				R3[index] = fR3;
				R4[index] = fR4;
				S1[index] = fS1;
				S2[index] = fS2;
				S3[index] = fS3;
				S4[index] = fS4;
			}
		}
		else 
		{
			FirstIndex[index] = FirstIndex[index-1] ;
			PP[index] = PP[index-1];
			R1[index] = R1[index-1];
			R2[index] = R2[index-1];
			R3[index] = R3[index-1];
			R4[index] = R4[index-1];
			S1[index] = S1[index-1];
			S2[index] = S2[index-1];
			S3[index] = S3[index-1];
			S4[index] = S4[index-1];
		}
	}

	
	// Bloc d'affichage       
	if ( sc.ChartWindowIsActive  or  NbMonitors == 2  or  sc.IsFullRecalculation )
	{
		BottomCoordinate = sc.YPixelCoordinateToGraphValue(sc.StudyRegionBottomCoordinate) ;		// la variable est déclarée en dehors de la fct scsf
		TopCoordinate = sc.YPixelCoordinateToGraphValue(sc.StudyRegionTopCoordinate) ;				// la variable est déclarée en dehors de la fct scsf
		if (  TraceVectorielONOFF.GetYesNo()  and  ( FirstVisibleBar != sc.IndexOfFirstVisibleBar  or  LastVisibleBar != sc.IndexOfLastVisibleBar  or  sc.IsFullRecalculation  or  fabs(BottomCoordinatePrevious-BottomCoordinate) > 0.02  or  fabs(TopCoordinatePrevious-TopCoordinate) > 0.02 )  )          	// Le recalcul des lignes n'est réalisé que si FirstIndex ou LastVisibleBar ou BottomCoordinate varie (on fait l'hypothèse que TopCoordinate et BottomCoordinate varient de manière synchronisée)		// Note : Sierra a un bug interne pour le premier calcul de BottomCoordinate (il vaut la même valeur que TopCoordinate)    // Il faut rajouter sc.IsFullRecalculation car lors d'une Full ReCalculation les useTool non UDD sont effacés
		{	
			FirstVisibleBar = sc.IndexOfFirstVisibleBar ;
			LastVisibleBar = sc.IndexOfLastVisibleBar ;
			BottomCoordinatePrevious = BottomCoordinate ;
			TopCoordinatePrevious = TopCoordinate ;

			// les useTool doivent être effacés à chaque appel du DLL pour éviter une fuite de mémoire   
			sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   // all non user drawn Chart Drawings on the specified ChartNumber will be deleted. The LineNumber parameter in this case is ignored and should be 0. 		
			
			s_sc* const ptrsc = &sc;
			int const firstIndex = FirstIndex[LastVisibleBar] ;
			int const epaisseur = 2 ;
			COLORREF const couleur = RGB(182,108,255);
			
			for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)
			{
				float const Niveau = sc.Subgraph[SubgraphIndex][LastVisibleBar] ;
				SCString const Texte = sc.Subgraph[SubgraphIndex].Name ;
				creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, Niveau, epaisseur, couleur, ptrsc);    // void creationLigneEtTexte (SCString Texte, int beginIndex, int endIndex, float niveau, int monEpaisseur, COLORREF maCouleur, s_sc* ptrsc)
			}
			
			// recalcul immédiat
			sc.UpdateAlways = 1 ;
		}
	}
	if ( TraceVectorielONOFF.GetYesNo() == 0 )  sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_MesPointsPivotsMensuelsProjetes(SCStudyInterfaceRef sc)		// [A]
{
	SCSubgraphRef OpenGap = sc.Subgraph[0];    
	SCSubgraphRef DemiGap = sc.Subgraph[1];   								   // appelé par Navig_Niveaux   // sc.Subgraph[].Arrays[1] appelé par Navig_Niveaux
	SCSubgraphRef PHAbsolu = sc.Subgraph[2];    // appelé par AMR   // appelé par Navig_Niveaux   // sc.Subgraph[].Arrays[1] appelé par Navig_Niveaux
	SCSubgraphRef PP = sc.Subgraph[3];
	SCSubgraphRef R1 = sc.Subgraph[4];
	SCSubgraphRef R2 = sc.Subgraph[5];
	SCSubgraphRef R3 = sc.Subgraph[6];
	SCSubgraphRef R4 = sc.Subgraph[7];
	SCSubgraphRef S1 = sc.Subgraph[8];
	SCSubgraphRef S2 = sc.Subgraph[9];
	SCSubgraphRef S3 = sc.Subgraph[10];
	SCSubgraphRef S4 = sc.Subgraph[11];
	SCSubgraphRef mR1 = sc.Subgraph[12];
	SCSubgraphRef mR2 = sc.Subgraph[13];
	SCSubgraphRef mR3 = sc.Subgraph[14];
	SCSubgraphRef mR4 = sc.Subgraph[15];
	SCSubgraphRef mS1 = sc.Subgraph[16];
	SCSubgraphRef mS2 = sc.Subgraph[17];
	SCSubgraphRef mS3 = sc.Subgraph[18];
	SCSubgraphRef mS4 = sc.Subgraph[19];
	SCSubgraphRef RQ1 = sc.Subgraph[20];
	SCSubgraphRef RQ2 = sc.Subgraph[21];
	SCSubgraphRef RQ3 = sc.Subgraph[22];
	SCSubgraphRef RQ4 = sc.Subgraph[23];
	SCSubgraphRef RQ5 = sc.Subgraph[24];
	SCSubgraphRef RQ6 = sc.Subgraph[25];
	SCSubgraphRef RQ7 = sc.Subgraph[26];
	SCSubgraphRef RQ8 = sc.Subgraph[27];
	SCSubgraphRef SQ1 = sc.Subgraph[28];
	SCSubgraphRef SQ2 = sc.Subgraph[29];
	SCSubgraphRef SQ3 = sc.Subgraph[30];
	SCSubgraphRef SQ4 = sc.Subgraph[31];
	SCSubgraphRef SQ5 = sc.Subgraph[32];
	SCSubgraphRef SQ6 = sc.Subgraph[33];
	SCSubgraphRef SQ7 = sc.Subgraph[34];
	SCSubgraphRef SQ8 = sc.Subgraph[35];    		// appelé par AMR   // appelé par Navig_Niveaux   // sc.Subgraph[].Arrays[1] appelé par Navig_Niveaux
	SCSubgraphRef PHPrior = sc.Subgraph[36];		// appelé par AMR   // appelé par Navig_Niveaux   // sc.Subgraph[].Arrays[1] appelé par Navig_Niveaux
	SCSubgraphRef PBPrior = sc.Subgraph[37];		// appelé par AMR   // appelé par Navig_Niveaux   // sc.Subgraph[].Arrays[1] appelé par Navig_Niveaux
	const int NUMBER_OF_STUDY_SUBGRAPHS = 38;  
	
	SCFloatArrayRef FirstIndex = sc.Subgraph[0].Arrays[2] ;  


	SCInputRef MonthlyChartNumber = sc.Input[0];
	SCInputRef TraceVectorielONOFF = sc.Input[2];
	SCInputRef AffichageDesQuartsONOFF = sc.Input[3];		// appelé par Navig_Niveaux 		// modifié par PrixTypique
	SCInputRef AffichageDesMid4Full4ONOFF = sc.Input[4];		// appelé par Navig_Niveaux 		// modifié par PrixTypique
	SCInputRef AffichageDesPHPBBougiePrecedente  = sc.Input[5];		// appelé par Navig_Niveaux 		// modifié par PrixTypique
	
	int& NbMonitors = sc.GetPersistentIntFast(0);  
	int& FirstVisibleBar = sc.GetPersistentIntFast(1);  
	int& LastVisibleBar = sc.GetPersistentIntFast(2);  
	int& TopCoordinatePrevious = sc.GetPersistentIntFast(3);  
	int& HoraireDebutTrace = sc.GetPersistentIntFast(4) ;  // time value en secondes
	int& HoraireFinSeance = sc.GetPersistentIntFast(5) ;
	int& StudyIDOverlayINQ100 = sc.GetPersistentIntFast(6) ;
	int& StudyID_PivotsM_NonProjetes = sc.GetPersistentIntFast(7) ;
	int& TriggerPopup = sc.GetPersistentIntFast(8) ;


	if (sc.SetDefaults)
	{
		sc.AutoLoop = 0;  
		sc.CalculationPrecedence = LOW_PREC_LEVEL  ;  // le SpreadMoyen (Study Overlay INQ100) doit être calculé précédemment
		sc.GraphName = "[A] Pivots Ind M (Full/Mid/Quart) projetes sur le future" ;    // appelé par AMR   // appelé par Navig_Niveaux   // appellé par PrixTypique
		sc.ScaleRangeType = SCALE_SAMEASREGION;
		sc.ValueFormat = 2;
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 0 ;
		sc.DisplayStudyInputValues = 0;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		
		sc.HideStudy = 1 ;
		

		// MonthlyChartNumber.Name = "Monthly Chart Number";
		MonthlyChartNumber.SetChartNumber(1);
		TraceVectorielONOFF.Name = "Trace Vectoriel ON/OFF";
		TraceVectorielONOFF.SetYesNo(1);
		AffichageDesQuartsONOFF.Name = "Affichage des Quarts";
		AffichageDesQuartsONOFF.SetYesNo(1);
		AffichageDesMid4Full4ONOFF.Name = "Affichage des Mid4 et Full4";	
		AffichageDesMid4Full4ONOFF.SetYesNo(1);
		AffichageDesPHPBBougiePrecedente.Name = "Affichage des PH et PB du mois precedent";	
		AffichageDesPHPBBougiePrecedente.SetYesNo(1);		

		
		
		for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].DrawStyle = DRAWSTYLE_LINE ;
			sc.Subgraph[SubgraphIndex].LineStyle = LINESTYLE_SOLID ;
			sc.Subgraph[SubgraphIndex].PrimaryColor = RGB(0,0,0);
			sc.Subgraph[SubgraphIndex].LineLabel = LL_DISPLAY_NAME | LL_NAME_REVERSE_COLORS |  LL_NAME_ALIGN_LEFT_EDGE ;  // LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_LEFT_EDGE  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
			sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre
			sc.Subgraph[SubgraphIndex].DrawZeros = false ;
		}

		PHAbsolu.PrimaryColor = RGB(255, 0, 0);
		PHPrior.PrimaryColor = RGB(255,0,0);
		PBPrior.PrimaryColor = RGB(0,168,0);
		OpenGap.PrimaryColor = RGB(210,0,210);
		OpenGap.LineLabel =  LL_DISPLAY_NAME | LL_NAME_REVERSE_COLORS |  LL_NAME_ALIGN_LEFT_EDGE ;
		DemiGap.PrimaryColor = RGB(210,0,210);
		
		
		OpenGap.Name = "                         OpenGap M";		
		DemiGap.Name = "    DemiGap M";		// 4 espaces de plus que la version J
		PHAbsolu.Name = "                                   / PH Absolu";
		PP.Name = "                        PP M";
		R1.Name = "                        R1 M";
		R2.Name = "                        R2 M";
		R3.Name = "                        R3 M";
		R4.Name = "                        R4 M";
		S1.Name = "                        S1 M";
		S2.Name = "                        S2 M";
		S3.Name = "                        S3 M";
		S4.Name = "                        S4 M";
		mR1.Name = "                        mR1 M";
		mR2.Name = "                        mR2 M";
		mR3.Name = "                        mR3 M";
		mR4.Name = "                        mR4 M";
		mS1.Name = "                        mS1 M";
		mS2.Name = "                        mS2 M";
		mS3.Name = "                        mS3 M";
		mS4.Name = "                        mS4 M";
		RQ1.Name = "                        RQ1 M";
		RQ2.Name = "                        RQ2 M";
		RQ3.Name = "                        RQ3 M";
		RQ4.Name = "                        RQ4 M";
		RQ5.Name = "                        RQ5 M";
		RQ6.Name = "                        RQ6 M";
		RQ7.Name = "                        RQ7 M";
		RQ8.Name = "                        RQ8 M";
		SQ1.Name = "                        SQ1 M";
		SQ2.Name = "                        SQ2 M";
		SQ3.Name = "                        SQ3 M";
		SQ4.Name = "                        SQ4 M";
		SQ5.Name = "                        SQ5 M";
		SQ6.Name = "                        SQ6 M";
		SQ7.Name = "                        SQ7 M";
		SQ8.Name = "                        SQ8 M";
		PHPrior.Name = "                      / PH MP";
		PBPrior.Name = "                      / PB MP";
		

		OpenGap.LineWidth = 8;
		DemiGap.LineWidth = 3;
		PHAbsolu.LineWidth = 4;
		PP.LineWidth = 5;
		R1.LineWidth = 5;
		R2.LineWidth = 5;
		R3.LineWidth = 5;
		R4.LineWidth = 5;
		S1.LineWidth = 5;
		S2.LineWidth = 5;
		S3.LineWidth = 5;
		S4.LineWidth = 5;
		mR1.LineWidth = 4;
		mR2.LineWidth = 4;
		mR3.LineWidth = 4;
		mR4.LineWidth = 4;
		mS1.LineWidth = 4;
		mS2.LineWidth = 4;
		mS3.LineWidth = 4;
		mS4.LineWidth = 4;
		RQ1.LineWidth = 3;
		RQ2.LineWidth = 3;
		RQ3.LineWidth = 3;
		RQ4.LineWidth = 3;
		RQ5.LineWidth = 3;
		RQ6.LineWidth = 3;
		RQ7.LineWidth = 3;
		RQ8.LineWidth = 3;
		SQ1.LineWidth = 3;
		SQ2.LineWidth = 3;
		SQ3.LineWidth = 3;
		SQ4.LineWidth = 3;
		SQ5.LineWidth = 3;
		SQ6.LineWidth = 3;
		SQ7.LineWidth = 3;
		SQ8.LineWidth = 3;
		PHPrior.LineWidth = 4;
		PBPrior.LineWidth = 4;
		
		return;
	}  // fin du bloc de " if (sc.SetDefaults) "
	
	
	
	if ( sc.UpdateStartIndex == 0 )
	{	
		sc.GraphRegion = 1 ;
		NbMonitors = NumberOfMonitors() ;
		FirstVisibleBar = 0 ;
		LastVisibleBar = 0 ;
		TriggerPopup = 1 ;
		
		//Initialisations des Niveaux et des NiveauxNonArrondis
		for (int SubgraphIndex = 0; SubgraphIndex <= NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex][0] = 0 ;                         // PPArrondi = ArrondiSiDepasse(PPNonArrondi) ;  // Subgraph tracé sur le graphe du future
			sc.Subgraph[SubgraphIndex].Arrays[0][0] = 0 ;          // PPNonArrondi = PPAvantProjection + fSpreadMoyen ;
			sc.Subgraph[SubgraphIndex].Arrays[1][0] = 0 ;          // PPAvantProjection ;   // c'est la valeur du PP sur le graphe de l'indice
		}
		
		HoraireDebutTrace = HMS_TIME(9,0,0);   // SpreadMoyen n'est pas retourné avant 
		HoraireFinSeance = HMS_TIME(16,0,0);
		
		// Récupération de la studyID sur le graphe de l'indice
		StudyIDOverlayINQ100 = sc.GetStudyIDByName(sc.ChartNumber, "Overlay INQ100 sur le future (avec correction du spread)", 0) ;
		if  (StudyIDOverlayINQ100 == 0 )
		{ SCString message ;  message.Format("!! ERREUR !! Le chart #6 ne contient pas la study ''Overlay INQ100 sur le future (avec correction du spread)''") ; sc.AddMessageToLog(message,1) ; }

		// Récupération de la StudyID
		StudyID_PivotsM_NonProjetes = sc.GetStudyIDByName(15, "[1] Pivots Ind M (Full/Mid/Quart)", 0) ; 
		if  (StudyID_PivotsM_NonProjetes == 0 )
		{ SCString message ;  message.Format("!! ERREUR !! Le graphique source #15 ne contient pas la study ''[1] Pivots Ind M (Full/Mid/Quart)''") ; sc.AddMessageToLog(message,1) ; }
	
		// Affichage des quarts, des Mid4Full4, des PHPB : par défaut on affiche tout, puis on met  "LineWidth = 0" à ce qui ne doit pas être affiché
		{
			RQ1.LineWidth = 3;
			RQ2.LineWidth = 3;
			RQ3.LineWidth = 3;
			RQ4.LineWidth = 3;
			RQ5.LineWidth = 3;
			RQ6.LineWidth = 3;
			RQ7.LineWidth = 3;
			RQ8.LineWidth = 3;
			SQ1.LineWidth = 3;
			SQ2.LineWidth = 3;
			SQ3.LineWidth = 3;
			SQ4.LineWidth = 3;
			SQ5.LineWidth = 3;
			SQ6.LineWidth = 3;
			SQ7.LineWidth = 3;
			SQ8.LineWidth = 3;
			mR4.LineWidth = 4;
			mS4.LineWidth = 4;
			R4.LineWidth = 5;
			S4.LineWidth = 5;
			PHPrior.LineWidth = 4;
			PBPrior.LineWidth = 4;
			PHAbsolu.LineWidth = 4;
			OpenGap.LineWidth = 6;
			DemiGap.LineWidth = 2;		
		}
		
		// Quarts non affichés
		if ( AffichageDesQuartsONOFF.GetYesNo() == 0 )
		{
			RQ1.LineWidth = 0;
			RQ2.LineWidth = 0;
			RQ3.LineWidth = 0;
			RQ4.LineWidth = 0;
			RQ5.LineWidth = 0;
			RQ6.LineWidth = 0;
			RQ7.LineWidth = 0;
			RQ8.LineWidth = 0;
			SQ1.LineWidth = 0;
			SQ2.LineWidth = 0;
			SQ3.LineWidth = 0;
			SQ4.LineWidth = 0;
			SQ5.LineWidth = 0;
			SQ6.LineWidth = 0;
			SQ7.LineWidth = 0;
			SQ8.LineWidth = 0;
		}
		
		// Mid4/Full4 non affichés
		if ( AffichageDesMid4Full4ONOFF.GetYesNo() == 0 )
		{
			RQ7.LineWidth = 0;
			RQ8.LineWidth = 0;
			SQ7.LineWidth = 0;
			SQ8.LineWidth = 0;
			mR4.LineWidth = 0;
			mS4.LineWidth = 0;
			R4.LineWidth = 0;
			S4.LineWidth = 0;
		}

		// PH/PB non affichés
		if ( AffichageDesPHPBBougiePrecedente.GetYesNo() == 0 )
		{
			PHPrior.LineWidth = 0;
			PBPrior.LineWidth = 0;
			PHAbsolu.LineWidth = 0; 
			OpenGap.LineWidth = 5;
			DemiGap.LineWidth = 0;
		}	
	}
	
	
	
	// on récupère l'array de SpreadMoyen
	SCFloatArray SpreadMoyenArray ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDOverlayINQ100, 0, 1, SpreadMoyenArray) ;
	
	
	for (int index = sc.UpdateStartIndex; index < sc.ArraySize; index++)                 // début AutoLoop
	{
		int OpenTimeValue = sc.BaseDateTimeIn[index].GetTimeInSecondsWithoutMilliseconds()  ; //GetTimeInSecondsWithoutMilliseconds retourne une time_value
		int PreviousOpenTimeValue = sc.BaseDateTimeIn[index-1].GetTimeInSecondsWithoutMilliseconds()  ; 
		
		if ( OpenTimeValue >= HoraireDebutTrace and OpenTimeValue < HoraireFinSeance )  // n'est actualisé que pendant l'ouverture du marché action
		{
			if  (  OpenTimeValue  !=  PreviousOpenTimeValue )   //  n'est actualisé que s'il y a une nouvelle seconde 
			{ 
				FirstIndex[index] = index ;
				
				// calcul de fSpreadMoyen
				float fSpreadMoyen = SpreadMoyenArray[index] ;
	
				// if (  OpenTimeValue ==  HoraireDebutTrace   )  
				if (  PP[index-1] == 0  and  OpenTimeValue >=  HoraireDebutTrace  )   // teste si on dépasse HoraireDebutTrace et que les PP n'ont pas encore été calculés
				{
					
					// déclaration des PP
					float fPHPrior = 0, fPBPrior = 0 , fSettlePrior = 0, fOpenCurrent = 0, fPP = 0 ;
					float fR1 = 0, fR2 = 0, fR3 = 0, fR4 = 0, fS1 = 0, fS2 = 0, fS3 = 0, fS4 = 0 ;
					float fmR1 = 0, fmR2 = 0, fmR3 = 0, fmR4 = 0, fmS1 = 0, fmS2 = 0,  fmS3 = 0,   fmS4 = 0 ; 
					float fRQ1 = 0, fRQ2 = 0, fRQ3 = 0, fRQ4 = 0, fRQ5 = 0, fRQ6 = 0, fRQ7 = 0, fRQ8 = 0 ;
					float fSQ1 = 0, fSQ2 = 0, fSQ3 = 0, fSQ4 = 0, fSQ5 = 0, fSQ6 = 0, fSQ7 = 0, fSQ8 = 0 ;


					//  "GetChartBaseData" copie intégralement les données source 
					SCGraphData MonthlyChartData;
					sc.GetChartBaseData(MonthlyChartNumber.GetChartNumber(), MonthlyChartData);
					SCDateTimeArray  MonthlyChartTimeArray;
					sc.GetChartDateTimeArray(MonthlyChartNumber.GetChartNumber(), MonthlyChartTimeArray);
					// on récupère les valeurs de OpenMensuel du chart #15     
					SCFloatArray ExternalArray_OpenMensuel ; 
					sc.GetStudyExtraArrayFromChartUsingID(15, StudyID_PivotsM_NonProjetes, 0, 0, ExternalArray_OpenMensuel) ;		// int GetStudyExtraArrayFromChartUsingID(int ChartNumber, int StudyID, int SubgraphIndex, int ExtraArrayIndex, SCFloatArrayRef ExtraArrayRef);

					
					// Récupère un index dans le MonthlyChart. Il y a deux possibilités pour cet index : 	- cas1 : il s'agit de la bougie du mois précédent, car la bougie du nouveau mois n'est pas encore connue (typiquement le 1er du mois avant 9h30m15s)
					//																															- cas 2 : il s'agit de l'index du mois actif (typiquement le 1er du mois après 9h30m15s ou les jours suivants du mois)
					int MonthlyChartIndex = sc.GetContainingIndexForDateTimeIndex(MonthlyChartNumber.GetChartNumber(), index); 
					if ( MonthlyChartIndex == 0  and  TriggerPopup ) 
					{ 
						SCString message ;  message.Format("!! WARNING !! Le chart #1 (Indice Nasdaq UT 1M) ne contient pas assez d'historique ( ExternalChartIndex == 0 )") ;  sc.AddMessageToLog(message, 1) ;
						TriggerPopup = 0 ;
					}
					
					// On détermine si MonthlyChartIndex et index font partie du même mois
					if ( MonthlyChartTimeArray[MonthlyChartIndex].GetMonth()  ==  sc.BaseDateTimeIn[index].GetMonth() )             // cas 2 :  MonthlyChartIndex et index font partie du même mois
					{
						fPHPrior = MonthlyChartData[SC_HIGH][MonthlyChartIndex-1];                         
						fPBPrior = MonthlyChartData[SC_LOW][MonthlyChartIndex-1];
						fSettlePrior = MonthlyChartData[SC_LAST][MonthlyChartIndex-1];

						// on récupère l'Open du mois courant sur le chart #15 en UT20s, car l'Open est souvent faux sur le chart en UT M
						int Chart15_Index = sc.GetFirstIndexForDate(15, sc.BaseDateTimeIn[index].GetDate()) ; 		// sc.GetFirstIndexForDate() function returns the first array index for the specified ChartNumber where the given TradingDayDate first occurs.
						fOpenCurrent = ExternalArray_OpenMensuel[Chart15_Index];
					}
					else      // cas 1 : MonthlyChartIndex correspond à la bougie du mois précédent
					{
						fPHPrior = MonthlyChartData[SC_HIGH][MonthlyChartIndex];                         
						fPBPrior = MonthlyChartData[SC_LOW][MonthlyChartIndex];
						fSettlePrior = MonthlyChartData[SC_LAST][MonthlyChartIndex];
						fOpenCurrent = (fPHPrior+fPBPrior)/2 ;
					}
					
					
					// correction des bugs de flux
				/* 	int Year, Month, Day;
					MonthlyChartTimeArray[MonthlyChartIndex-1].GetDateYMD(Year, Month, Day);
					if (Year==2019 and Month==1 and Day==31)  // jour pour lequel HLC est faux
					{
						fPHPrior = nd;
						fPBPrior = nd;
						fSettlePrior = nd;
					} */

					//on calcule les PP 
					CalculatePPs (fPHPrior, fPBPrior, fSettlePrior, fPP, fR1, fR2, fR3, fR4, fS1, fS2, fS3, fS4, fmR1, fmR2, fmR3, fmR4, fmS1, fmS2, fmS3, fmS4, fRQ1, fRQ2, fRQ3, fRQ4, fRQ5, fRQ6, fRQ7, fRQ8, fSQ1, fSQ2, fSQ3, fSQ4, fSQ5, fSQ6, fSQ7, fSQ8);
					
					// Sauvegarde des niveaux non arrondis
					PHPrior.Arrays[1][index] = fPHPrior ;
					PBPrior.Arrays[1][index] = fPBPrior ;
					PP.Arrays[1][index] = fPP ;
					R1.Arrays[1][index] = fR1 ;
					R2.Arrays[1][index] = fR2 ;
					R3.Arrays[1][index] = fR3 ;
					R4.Arrays[1][index] = fR4 ;
					S1.Arrays[1][index] = fS1 ;
					S2.Arrays[1][index] = fS2 ;
					S3.Arrays[1][index] = fS3 ;
					S4.Arrays[1][index] = fS4 ;
					mR1.Arrays[1][index] = fmR1 ;
					mR2.Arrays[1][index] = fmR2 ;
					mR3.Arrays[1][index] = fmR3 ;
					mR4.Arrays[1][index] = fmR4 ;
					mS1.Arrays[1][index] = fmS1 ;
					mS2.Arrays[1][index] = fmS2 ;
					mS3.Arrays[1][index] = fmS3 ;
					mS4.Arrays[1][index] = fmS4 ;
					RQ1.Arrays[1][index] = fRQ1 ;
					RQ2.Arrays[1][index] = fRQ2 ;
					RQ3.Arrays[1][index] = fRQ3 ;
					RQ4.Arrays[1][index] = fRQ4 ;
					RQ5.Arrays[1][index] = fRQ5 ;
					RQ6.Arrays[1][index] = fRQ6 ;
					RQ7.Arrays[1][index] = fRQ7 ;
					RQ8.Arrays[1][index] = fRQ8 ;
					SQ1.Arrays[1][index] = fSQ1 ;
					SQ2.Arrays[1][index] = fSQ2 ;
					SQ3.Arrays[1][index] = fSQ3 ;
					SQ4.Arrays[1][index] = fSQ4 ;
					SQ5.Arrays[1][index] = fSQ5 ;
					SQ6.Arrays[1][index] = fSQ6 ;
					SQ7.Arrays[1][index] = fSQ7 ;
					SQ8.Arrays[1][index] = fSQ8 ;
					PHAbsolu.Arrays[1][index] = Highest (MonthlyChartData, MonthlyChartIndex) ;

					
					// Recherche d'un opengap dans la première bougie du jour, puis stockage dans Arrays[1]  (valeurs non corrigées)
					if (   fOpenCurrent > fPHPrior   )      // gap haussier  
					{ OpenGap.Arrays[1][index] = fPHPrior ;  DemiGap.Arrays[1][index] = (fOpenCurrent + fPHPrior)/2 ;  }
					else if (  fOpenCurrent < fPBPrior   )     //    gap baissier  
					{ OpenGap.Arrays[1][index] = fPBPrior ;  DemiGap.Arrays[1][index] = (fOpenCurrent + fPBPrior)/2 ;  }
					else    // pas de gap
					{ OpenGap.Arrays[1][index] = 0; DemiGap.Arrays[1][index] = 0; }
					
					
					// on rappelle la valeur de chaque niveau et on l'adjuste du spread, puis on fait l'arrondi
					for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)  
					{ 	sc.Subgraph[SubgraphIndex].Arrays[0][index] = sc.Subgraph[SubgraphIndex].Arrays[1][index] + fSpreadMoyen ;  ArrondiSiDepasse(sc.Subgraph[SubgraphIndex], 0.5, index) ;  } 
				}

				else // on n'est pas dans le premier chandelier du jour, mais on est dans une nouvelle seconde => on rappelle la valeur de chaque niveau et on l'adjuste du spread, puis on fait l'arrondi
				{   
					for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)  {  sc.Subgraph[SubgraphIndex].Arrays[1][index] = sc.Subgraph[SubgraphIndex].Arrays[1][index-1] ;  sc.Subgraph[SubgraphIndex].Arrays[0][index] = sc.Subgraph[SubgraphIndex].Arrays[1][index] + fSpreadMoyen ; ArrondiSiDepasse(sc.Subgraph[SubgraphIndex], 0.5, index) ;  } 	 
					FirstIndex[index] = FirstIndex[index-1] ;
				}
			}
			
			else   // la barre ne débute pas dans une nouvelle seconde => on recopie les valeurs de l'index précédent
			{	
				for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)  {  sc.Subgraph[SubgraphIndex][index] = sc.Subgraph[SubgraphIndex][index-1] ;  sc.Subgraph[SubgraphIndex].Arrays[0][index] = sc.Subgraph[SubgraphIndex].Arrays[0][index-1] ;  sc.Subgraph[SubgraphIndex].Arrays[1][index] = sc.Subgraph[SubgraphIndex].Arrays[1][index-1] ;   }         
				FirstIndex[index] = FirstIndex[index-1] ;
			}  
		}
		
	} // fin de AutoLoop
	
	
	// Bloc d'affichage       
	if ( TraceVectorielONOFF.GetYesNo() )
	{
		if ( sc.ChartWindowIsActive  or  NbMonitors == 2 )
		{
			TopCoordinate = sc.YPixelCoordinateToGraphValue(sc.StudyRegionTopCoordinate) ;
			if (  FirstVisibleBar != sc.IndexOfFirstVisibleBar  or  LastVisibleBar != sc.IndexOfLastVisibleBar  or  sc.IsFullRecalculation  or  fabs(TopCoordinatePrevious-TopCoordinate) > 0.02  )          	// Le recalcul des lignes n'est réalisé que si FirstIndex ou LastVisibleBar ou TopCoordinate varie (on fait l'hypothèse que TopCoordinate et BottomCoordinate varient de manière synchronisée)    // Il faut rajouter sc.IsFullRecalculation car lors d'une Full ReCalculation les useTool non UDD sont effacés
			{	
				FirstVisibleBar = sc.IndexOfFirstVisibleBar ;
				LastVisibleBar = sc.IndexOfLastVisibleBar ;
				BottomCoordinate = sc.YPixelCoordinateToGraphValue(sc.StudyRegionBottomCoordinate) ;
				TopCoordinatePrevious = TopCoordinate ;
				
				// les useTool doivent être effacés à chaque appel du DLL pour éviter une fuite de mémoire   
				sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   // all non user drawn Chart Drawings on the specified ChartNumber will be deleted. The LineNumber parameter in this case is ignored and should be 0. 		
				
				s_sc* const ptrsc = &sc;
				int const firstIndex = FirstIndex[LastVisibleBar] ;

				// Tracé des pivots, PH, PB
				for (int SubgraphIndex = 0; SubgraphIndex <= 19; SubgraphIndex++)
				{
					float const Niveau = sc.Subgraph[SubgraphIndex].Arrays[0][LastVisibleBar] ;    // la ligne vectorielle est tracée sur une ordonnée prise à LastVisibleBar
					SCString const Texte = sc.Subgraph[SubgraphIndex].Name ;
					int const epaisseur = sc.Subgraph[SubgraphIndex].LineWidth ;
					COLORREF const couleur = sc.Subgraph[SubgraphIndex].PrimaryColor;
					creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, Niveau, epaisseur, couleur, ptrsc);    // void creationLigneEtTexte (SCString Texte, int beginIndex, int endIndex, float niveau, int monEpaisseur, COLORREF maCouleur, s_sc* ptrsc)
				}
				for (int SubgraphIndex = 36; SubgraphIndex <= 37; SubgraphIndex++)
				{
					float const Niveau = sc.Subgraph[SubgraphIndex].Arrays[0][LastVisibleBar] ;    // la ligne vectorielle est tracée sur une ordonnée prise à LastVisibleBar
					SCString const Texte = sc.Subgraph[SubgraphIndex].Name ;
					int const epaisseur = sc.Subgraph[SubgraphIndex].LineWidth ;
					COLORREF const couleur = sc.Subgraph[SubgraphIndex].PrimaryColor;
					creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, Niveau, epaisseur, couleur, ptrsc);    // void creationLigneEtTexte (SCString Texte, int beginIndex, int endIndex, float niveau, int monEpaisseur, COLORREF maCouleur, s_sc* ptrsc)
				}
				
				// Tracé des Quarts
				if ( AffichageDesQuartsONOFF.GetYesNo() )
				{
					for (int SubgraphIndex = 20; SubgraphIndex <= 35 ; SubgraphIndex++)  // RQ1 à SQ8
					{
						float const Niveau = sc.Subgraph[SubgraphIndex].Arrays[0][LastVisibleBar] ;    // la ligne vectorielle est tracée sur une ordonnée prise à LastVisibleBar
						SCString const Texte = sc.Subgraph[SubgraphIndex].Name ;
						int const epaisseur = sc.Subgraph[SubgraphIndex].LineWidth ;
						COLORREF const couleur = sc.Subgraph[SubgraphIndex].PrimaryColor;
						creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, Niveau, epaisseur, couleur, ptrsc);    // void creationLigneEtTexte (SCString Texte, int beginIndex, int endIndex, float niveau, int monEpaisseur, COLORREF maCouleur, s_sc* ptrsc)
					}
				}

				
				
				// recalcul immédiat
				sc.UpdateAlways = 1 ;
			}
		}
	}		
	else sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_MesEighthsMensuelsProjetes(SCStudyInterfaceRef sc)		// [B]
{
	SCSubgraphRef RE1 = sc.Subgraph[0];     // appelé par AMR   // appelé par Navig_Niveaux   // sc.Subgraph[].Arrays[1] appelé par Navig_Niveaux
	SCSubgraphRef RE2 = sc.Subgraph[1];	
	SCSubgraphRef RE3 = sc.Subgraph[2];
	SCSubgraphRef RE4 = sc.Subgraph[3];
	SCSubgraphRef RE5 = sc.Subgraph[4];
	SCSubgraphRef RE6 = sc.Subgraph[5];
	SCSubgraphRef RE7 = sc.Subgraph[6];
	SCSubgraphRef RE8 = sc.Subgraph[7];
	SCSubgraphRef RE9 = sc.Subgraph[8];
	SCSubgraphRef RE10 = sc.Subgraph[9];
	SCSubgraphRef RE11 = sc.Subgraph[10];
	SCSubgraphRef RE12 = sc.Subgraph[11];
	SCSubgraphRef RE13 = sc.Subgraph[12];
	SCSubgraphRef RE14 = sc.Subgraph[13];
	SCSubgraphRef RE15 = sc.Subgraph[14];
	SCSubgraphRef RE16 = sc.Subgraph[15];
	SCSubgraphRef SE1 = sc.Subgraph[16];
	SCSubgraphRef SE2 = sc.Subgraph[17];
	SCSubgraphRef SE3 = sc.Subgraph[18];
	SCSubgraphRef SE4 = sc.Subgraph[19];
	SCSubgraphRef SE5 = sc.Subgraph[20];
	SCSubgraphRef SE6 = sc.Subgraph[21];
	SCSubgraphRef SE7 = sc.Subgraph[22];
	SCSubgraphRef SE8 = sc.Subgraph[23];
	SCSubgraphRef SE9 = sc.Subgraph[24];
	SCSubgraphRef SE10 = sc.Subgraph[25];
	SCSubgraphRef SE11 = sc.Subgraph[26];
	SCSubgraphRef SE12 = sc.Subgraph[27];
	SCSubgraphRef SE13 = sc.Subgraph[28];
	SCSubgraphRef SE14 = sc.Subgraph[29];
	SCSubgraphRef SE15 = sc.Subgraph[30];
	SCSubgraphRef SE16 = sc.Subgraph[31];     // appelé par AMR   // appelé par Navig_Niveaux   // sc.Subgraph[].Arrays[1] appelé par Navig_Niveaux
	const int NUMBER_OF_STUDY_SUBGRAPHS = 32;
	
	SCFloatArrayRef FirstIndex = sc.Subgraph[0].Arrays[2] ;  
	
	
	SCInputRef MonthlyChartNumber = sc.Input[0];
	SCInputRef TraceVectorielONOFF = sc.Input[2];		// modifié par Prix Typique		// appelé par NavigViveaux
	SCInputRef AffichageDesMid4Full4ONOFF = sc.Input[4];					// modifié par Prix_typique 		
	
	int& NbMonitors = sc.GetPersistentIntFast(0);  
	int& FirstVisibleBar = sc.GetPersistentIntFast(1);  
	int& LastVisibleBar = sc.GetPersistentIntFast(2);  
	int& TopCoordinatePrevious = sc.GetPersistentIntFast(3);  
	int& HoraireDebutTrace = sc.GetPersistentIntFast(4) ;  // time value en secondes
	int& HoraireFinSeance = sc.GetPersistentIntFast(5) ;
	int& StudyIDOverlayINQ100 = sc.GetPersistentIntFast(6) ;


	if (sc.SetDefaults)
	{
		sc.AutoLoop = 0;  
		sc.CalculationPrecedence = LOW_PREC_LEVEL  ;  // le SpreadMoyen (Study Overlay INQ100) doit être calculé précédemment
		sc.GraphName = "[B] Pivots Ind M (Eighth) projetes sur le future" ;    // appelé par AMR   // appelé par Navig_Niveaux   // appelé par Prix Typique
		sc.ScaleRangeType = SCALE_SAMEASREGION;
		sc.ValueFormat = 2;
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 0 ;
		sc.DisplayStudyInputValues = 0;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		
		sc.HideStudy = 1 ;

		
		// MonthlyChartNumber.Name = "Monthly Chart Number";  // non modifiable par l'utilisateur
		MonthlyChartNumber.SetChartNumber(1);
		TraceVectorielONOFF.Name = "Trace Vectoriel ON/OFF";	
		TraceVectorielONOFF.SetYesNo(1);
		AffichageDesMid4Full4ONOFF.Name = "Affichage des Mid4 et Full4";	
		AffichageDesMid4Full4ONOFF.SetYesNo(1);
		
		
		for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].DrawStyle = DRAWSTYLE_LINE ;
			sc.Subgraph[SubgraphIndex].LineStyle = LINESTYLE_SOLID ;
			sc.Subgraph[SubgraphIndex].PrimaryColor = RGB(0,0,0);
			sc.Subgraph[SubgraphIndex].LineLabel = LL_DISPLAY_NAME | LL_NAME_REVERSE_COLORS |  LL_NAME_ALIGN_LEFT_EDGE ;  // LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_LEFT_EDGE  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
			sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre
			sc.Subgraph[SubgraphIndex].DrawZeros = false ;
			sc.Subgraph[SubgraphIndex].LineWidth = 2 ; 
		}
		
		
		RE1.Name = "                        RE1 M";
		RE2.Name = "                        RE2 M";
		RE3.Name = "                        RE3 M";
		RE4.Name = "                        RE4 M";
		RE5.Name = "                        RE5 M";
		RE6.Name = "                        RE6 M";
		RE7.Name = "                        RE7 M";
		RE8.Name = "                        RE8 M";
		RE9.Name = "                        RE9 M";
		RE10.Name = "                        RE10 M";
		RE11.Name = "                        RE11 M";
		RE12.Name = "                        RE12 M";
		RE13.Name = "                        RE13 M";
		RE14.Name = "                        RE14 M";
		RE14.Name = "                        RE14 M";
		RE15.Name = "                        RE15 M";
		RE16.Name = "                        RE16 M";
		SE1.Name = "                        SE1 M";
		SE2.Name = "                        SE2 M";
		SE3.Name = "                        SE3 M";
		SE4.Name = "                        SE4 M";
		SE5.Name = "                        SE5 M";
		SE6.Name = "                        SE6 M";
		SE7.Name = "                        SE7 M";
		SE8.Name = "                        SE8 M";
		SE9.Name = "                        SE9 M";
		SE10.Name = "                        SE10 M";
		SE11.Name = "                        SE11 M";
		SE12.Name = "                        SE12 M";
		SE13.Name = "                        SE13 M";
		SE14.Name = "                        SE14 M";
		SE14.Name = "                        SE14 M";
		SE15.Name = "                        SE15 M";
		SE16.Name = "                        SE16 M";
		
		
		return;
	}  // fin du bloc de " if (sc.SetDefaults) "
	
	
	
	if ( sc.UpdateStartIndex == 0 )
	{
		sc.GraphRegion = 1 ;
		NbMonitors = NumberOfMonitors() ;
		FirstVisibleBar = 0 ;
		LastVisibleBar = 0 ;
		
		//Initialisations des Niveaux et des NiveauxNonArrondis
		for (int SubgraphIndex = 0; SubgraphIndex <= NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex][0] = 0 ;                         // PP[0] = 0 ;
			sc.Subgraph[SubgraphIndex].Arrays[0][0] = 0 ;          // PPNonArrondi[0] = 0 ;
			sc.Subgraph[SubgraphIndex].Arrays[1][0] = 0 ;          // NiveauIndiceSansCorrection[0] = 0 ;

		}
		
		HoraireDebutTrace = HMS_TIME(9,0,0);  
		HoraireFinSeance = HMS_TIME(16,0,0);
		
		// Récupération de la studyID sur le graphe de l'indice
		StudyIDOverlayINQ100 = sc.GetStudyIDByName(sc.ChartNumber, "Overlay INQ100 sur le future (avec correction du spread)", 0) ;
		if  (StudyIDOverlayINQ100 == 0 )
		{ SCString message ;  message.Format("!! ERREUR !! Le chart #6 ne contient pas la study ''Overlay INQ100 sur le future (avec correction du spread)''") ; sc.AddMessageToLog(message,1) ; }

		// Affichage des eights liés à Mid4Full4 : par défaut on affiche tout, puis on met  "LineWidth = 0" à ce qui ne doit pas être affiché
		{
			RE13.LineWidth = 2;
			RE14.LineWidth = 2;
			RE15.LineWidth = 2;
			RE16.LineWidth = 2;
			SE13.LineWidth = 2;
			SE14.LineWidth = 2;
			SE15.LineWidth = 2;
			SE16.LineWidth = 2;
		}		

		// Mid4/Full4 non affichés
		if ( AffichageDesMid4Full4ONOFF.GetYesNo() == 0 )
		{
			RE13.LineWidth = 0;
			RE14.LineWidth = 0;
			RE15.LineWidth = 0;
			RE16.LineWidth = 0;
			SE13.LineWidth = 0;
			SE14.LineWidth = 0;
			SE15.LineWidth = 0;
			SE16.LineWidth = 0;
		}	
		
	}
	
	
	// on récupère l'array de SpreadMoyen
	SCFloatArray SpreadMoyenArray ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDOverlayINQ100, 0, 1, SpreadMoyenArray) ;


	
	for (int index = sc.UpdateStartIndex; index < sc.ArraySize; index++)                 // début AutoLoop
	{
		int OpenTimeValue = sc.BaseDateTimeIn[index].GetTimeInSecondsWithoutMilliseconds()  ; //GetTimeInSecondsWithoutMilliseconds retourne une time_value
		int PreviousOpenTimeValue = sc.BaseDateTimeIn[index-1].GetTimeInSecondsWithoutMilliseconds()  ; 
		
		if ( OpenTimeValue >= HoraireDebutTrace and OpenTimeValue < HoraireFinSeance )  // n'est actualisé que pendant l'ouverture du marché action
		{
			if  ( OpenTimeValue  !=  PreviousOpenTimeValue )   //  n'est actualisé que s'il y a une nouvelle seconde 
			{ 
				FirstIndex[index] = index ;
				
				// calcul de fSpreadMoyen
				float fSpreadMoyen = SpreadMoyenArray[index] ;
	
				//if (  OpenTimeValue ==  HoraireDebutTrace   )   // les PP sont recalculés tous les jours 
				if (  RE1[index-1] == 0  and  OpenTimeValue >=  HoraireDebutTrace  )   // teste si on dépasse HoraireDebutTrace et que les PP n'ont pas encore été calculés
				{
					// déclaration des PP
					float fPHPrior = 0, fPBPrior = 0 , fSettlePrior = 0 ;
					float fRE1, fRE2, fRE3, fRE4, fRE5, fRE6, fRE7, fRE8, fRE9, fRE10, fRE11, fRE12, fRE13, fRE14, fRE15, fRE16 ;
					float fSE1, fSE2, fSE3, fSE4, fSE5, fSE6, fSE7, fSE8, fSE9, fSE10, fSE11, fSE12, fSE13, fSE14, fSE15, fSE16 ;

					//  "GetChartBaseData" copie intégralement les données source 
					SCGraphData MonthlyChartData;
					sc.GetChartBaseData(MonthlyChartNumber.GetChartNumber(), MonthlyChartData);
					SCDateTimeArray  MonthlyChartTimeArray;
					sc.GetChartDateTimeArray(MonthlyChartNumber.GetChartNumber(), MonthlyChartTimeArray);
					
					// Récupère un index dans le MonthlyChart. Il y a deux possibilités pour cet index : 	- cas1 : il s'agit de la bougie du mois précédent, car la bougie du nouveau mois n'est pas encore connue (typiquement le 1er du mois avant 9h30m15s)
					//																															- cas 2 : il s'agit de l'index du mois actif (typiquement le 1er du mois après 9h30m15s ou les jours suivants du mois)
					int MonthlyChartIndex = sc.GetContainingIndexForDateTimeIndex(MonthlyChartNumber.GetChartNumber(), index); 
					
					// On détermine si MonthlyChartIndex et index font partie du même mois
					if ( MonthlyChartTimeArray[MonthlyChartIndex].GetMonth()  ==  sc.BaseDateTimeIn[index].GetMonth() )             // cas 2 :  MonthlyChartIndex et index font partie du même mois
					{
						fPHPrior = MonthlyChartData[SC_HIGH][MonthlyChartIndex-1];                         
						fPBPrior = MonthlyChartData[SC_LOW][MonthlyChartIndex-1];
						fSettlePrior = MonthlyChartData[SC_LAST][MonthlyChartIndex-1];
					}
					else      // cas 1 : MonthlyChartIndex correspond à la bougie du mois précédent
					{
						fPHPrior = MonthlyChartData[SC_HIGH][MonthlyChartIndex];                         
						fPBPrior = MonthlyChartData[SC_LOW][MonthlyChartIndex];
						fSettlePrior = MonthlyChartData[SC_LAST][MonthlyChartIndex];
					}
					
					
					// correction des bugs de flux
				/* 	int Year, Month, Day;
					MonthlyChartTimeArray[MonthlyChartIndex-1].GetDateYMD(Year, Month, Day);
						if (Year==2019 and Month==1 and Day==31)  // jour pour lequel HLC est faux
					{
						fPHPrior = nd;
						fPBPrior = nd;
						fSettlePrior = nd;
					} */

					//on calcule les PP 
					CalculateEighths (	fPHPrior, fPBPrior, fSettlePrior, 
												fRE1, fRE2, fRE3, fRE4, fRE5, fRE6, fRE7, fRE8, fRE9, fRE10, fRE11, fRE12, fRE13, fRE14, fRE15, fRE16, 
												fSE1, fSE2, fSE3, fSE4, fSE5, fSE6, fSE7, fSE8, fSE9, fSE10, fSE11, fSE12, fSE13, fSE14, fSE15, fSE16 ) ;
										
										
					// Sauvegarde des niveaux non arrondis
					RE1.Arrays[1][index] =  fRE1;
					RE2.Arrays[1][index] = fRE2;
					RE3.Arrays[1][index] = fRE3;
					RE4.Arrays[1][index] = fRE4;
					RE5.Arrays[1][index] = fRE5;
					RE6.Arrays[1][index] = fRE6;
					RE7.Arrays[1][index] = fRE7;
					RE8.Arrays[1][index] = fRE8;
					RE9.Arrays[1][index] = fRE9;
					RE10.Arrays[1][index] = fRE10;
					RE11.Arrays[1][index] = fRE11;
					RE12.Arrays[1][index] = fRE12;
					RE13.Arrays[1][index] = fRE13;
					RE14.Arrays[1][index] = fRE14;
					RE14.Arrays[1][index] = fRE14;
					RE15.Arrays[1][index] = fRE15;
					RE16.Arrays[1][index] = fRE16;
					SE1.Arrays[1][index] = fSE1;
					SE2.Arrays[1][index] = fSE2;
					SE3.Arrays[1][index] = fSE3;
					SE4.Arrays[1][index] = fSE4;
					SE5.Arrays[1][index] = fSE5;
					SE6.Arrays[1][index] = fSE6;
					SE7.Arrays[1][index] = fSE7;
					SE8.Arrays[1][index] = fSE8;
					SE9.Arrays[1][index] = fSE9;
					SE10.Arrays[1][index] = fSE10;
					SE11.Arrays[1][index] = fSE11;
					SE12.Arrays[1][index] = fSE12;
					SE13.Arrays[1][index] = fSE13;
					SE14.Arrays[1][index] = fSE14;
					SE14.Arrays[1][index] = fSE14;
					SE15.Arrays[1][index] = fSE15;
					SE16.Arrays[1][index] = fSE16;
					
					
					// on rappelle la valeur de chaque niveau et on l'adjuste du spread, puis on fait l'arrondi
					for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)  
					{ 	sc.Subgraph[SubgraphIndex].Arrays[0][index] = sc.Subgraph[SubgraphIndex].Arrays[1][index] + fSpreadMoyen ;  ArrondiSiDepasse(sc.Subgraph[SubgraphIndex], 0.5, index) ;  } 
					
				}

				else // on n'est pas dans le premier chandelier du jour, mais on est dans une nouvelle seconde => on rappelle la valeur de chaque niveau et on l'adjuste du spread, puis on fait l'arrondi
				{   
					for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)  {  sc.Subgraph[SubgraphIndex].Arrays[1][index] = sc.Subgraph[SubgraphIndex].Arrays[1][index-1] ;  sc.Subgraph[SubgraphIndex].Arrays[0][index] = sc.Subgraph[SubgraphIndex].Arrays[1][index] + fSpreadMoyen ; ArrondiSiDepasse(sc.Subgraph[SubgraphIndex], 0.5, index) ;  } 	 
					FirstIndex[index] = FirstIndex[index-1] ;
				}

			}
			
			else   // la barre ne débute pas dans une nouvelle seconde => on recopie les valeurs de l'index précédent
			{	
				for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)  {  sc.Subgraph[SubgraphIndex][index] = sc.Subgraph[SubgraphIndex][index-1] ;  sc.Subgraph[SubgraphIndex].Arrays[0][index] = sc.Subgraph[SubgraphIndex].Arrays[0][index-1] ;  sc.Subgraph[SubgraphIndex].Arrays[1][index] = sc.Subgraph[SubgraphIndex].Arrays[1][index-1] ;   }         
				FirstIndex[index] = FirstIndex[index-1] ;
			}  
			
		}

	} // fin de AutoLoop
	


	
	// Bloc d'affichage       
	if ( TraceVectorielONOFF.GetYesNo() )
	{
		if ( sc.ChartWindowIsActive  or  NbMonitors == 2 )
		{
			TopCoordinate = sc.YPixelCoordinateToGraphValue(sc.StudyRegionTopCoordinate) ;
			if (  FirstVisibleBar != sc.IndexOfFirstVisibleBar  or  LastVisibleBar != sc.IndexOfLastVisibleBar  or  sc.IsFullRecalculation  or  fabs(TopCoordinatePrevious-TopCoordinate) > 0.02  )          	// Le recalcul des lignes n'est réalisé que si FirstIndex ou LastVisibleBar ou TopCoordinate varie (on fait l'hypothèse que TopCoordinate et BottomCoordinate varient de manière synchronisée)    // Il faut rajouter sc.IsFullRecalculation car lors d'une Full ReCalculation les useTool non UDD sont effacés
			{	
				FirstVisibleBar = sc.IndexOfFirstVisibleBar ;
				LastVisibleBar = sc.IndexOfLastVisibleBar ;
				BottomCoordinate = sc.YPixelCoordinateToGraphValue(sc.StudyRegionBottomCoordinate) ;
				TopCoordinatePrevious = TopCoordinate ;
				
				// les useTool doivent être effacés à chaque appel du DLL pour éviter une fuite de mémoire   
				sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   // all non user drawn Chart Drawings on the specified ChartNumber will be deleted. The LineNumber parameter in this case is ignored and should be 0. 		
				
				s_sc* const ptrsc = &sc;
				int const firstIndex = FirstIndex[LastVisibleBar] ;

				for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)
				{
					float const Niveau = sc.Subgraph[SubgraphIndex].Arrays[0][LastVisibleBar] ;    // la ligne vectorielle est tracée sur une ordonnée prise à LastVisibleBar
					SCString const Texte = sc.Subgraph[SubgraphIndex].Name ;
					int const epaisseur = sc.Subgraph[SubgraphIndex].LineWidth ;
					COLORREF const couleur = sc.Subgraph[SubgraphIndex].PrimaryColor;
					creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, Niveau, epaisseur, couleur, ptrsc);    // void creationLigneEtTexte (SCString Texte, int beginIndex, int endIndex, float niveau, int monEpaisseur, COLORREF maCouleur, s_sc* ptrsc)
				}

				// recalcul immédiat
				sc.UpdateAlways = 1 ;
			}
		}
	}
	else sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_MesPointsPivotsHebdoProjetes(SCStudyInterfaceRef sc)		// [C]
{
	SCSubgraphRef OpenGap = sc.Subgraph[0];									// appelé par Navig_Niveaux   // sc.Subgraph[].Arrays[1] appelé par Navig_Niveaux
	SCSubgraphRef PHPrior = sc.Subgraph[1];		// appelé par AMR   // appelé par Navig_Niveaux   // sc.Subgraph[].Arrays[1] appelé par Navig_Niveaux
	SCSubgraphRef PBPrior = sc.Subgraph[2];		// appelé par AMR   // appelé par Navig_Niveaux   // sc.Subgraph[].Arrays[1] appelé par Navig_Niveaux
	SCSubgraphRef PP = sc.Subgraph[3];
	SCSubgraphRef R1 = sc.Subgraph[4];
	SCSubgraphRef R2 = sc.Subgraph[5];
	SCSubgraphRef R3 = sc.Subgraph[6];
	SCSubgraphRef R4 = sc.Subgraph[7];
	SCSubgraphRef S1 = sc.Subgraph[8];
	SCSubgraphRef S2 = sc.Subgraph[9];
	SCSubgraphRef S3 = sc.Subgraph[10];
	SCSubgraphRef S4 = sc.Subgraph[11];
	SCSubgraphRef mR1 = sc.Subgraph[12];
	SCSubgraphRef mR2 = sc.Subgraph[13];
	SCSubgraphRef mR3 = sc.Subgraph[14];
	SCSubgraphRef mR4 = sc.Subgraph[15];
	SCSubgraphRef mS1 = sc.Subgraph[16];
	SCSubgraphRef mS2 = sc.Subgraph[17];
	SCSubgraphRef mS3 = sc.Subgraph[18];
	SCSubgraphRef mS4 = sc.Subgraph[19];
	SCSubgraphRef RQ1 = sc.Subgraph[20];
	SCSubgraphRef RQ2 = sc.Subgraph[21];
	SCSubgraphRef RQ3 = sc.Subgraph[22];
	SCSubgraphRef RQ4 = sc.Subgraph[23];
	SCSubgraphRef RQ5 = sc.Subgraph[24];
	SCSubgraphRef RQ6 = sc.Subgraph[25];
	SCSubgraphRef RQ7 = sc.Subgraph[26];
	SCSubgraphRef RQ8 = sc.Subgraph[27];
	SCSubgraphRef SQ1 = sc.Subgraph[28];
	SCSubgraphRef SQ2 = sc.Subgraph[29];
	SCSubgraphRef SQ3 = sc.Subgraph[30];
	SCSubgraphRef SQ4 = sc.Subgraph[31];
	SCSubgraphRef SQ5 = sc.Subgraph[32];
	SCSubgraphRef SQ6 = sc.Subgraph[33];
	SCSubgraphRef SQ7 = sc.Subgraph[34];        // appelé par AMR		// appelé par Navig_Niveaux   // sc.Subgraph[].Arrays[1] appelé par Navig_Niveaux
	SCSubgraphRef SQ8 = sc.Subgraph[35];          // appelé par AMR   // appelé par Navig_Niveaux   // sc.Subgraph[].Arrays[1] appelé par Navig_Niveaux
	SCSubgraphRef DemiGap = sc.Subgraph[36];         							// appelé par Navig_Niveaux   // sc.Subgraph[].Arrays[1] appelé par Navig_Niveaux

	const int NUMBER_OF_STUDY_SUBGRAPHS = 37;  
	
	SCFloatArrayRef FirstIndex = sc.Subgraph[1].Arrays[2] ;  


	SCInputRef WeeklyChartNumber = sc.Input[0];
	SCInputRef TraceVectorielONOFF = sc.Input[2];				// appelé par Navig_Niveaux 		// modifié par PrixTypique
	SCInputRef AffichageDesQuartsONOFF = sc.Input[3];		// appelé par Navig_Niveaux 	// modifié par PrixTypique
	SCInputRef AffichageDesMid4Full4ONOFF = sc.Input[4];		// appelé par Navig_Niveaux 		// modifié par PrixTypique
	SCInputRef AffichageDesPHPBBougiePrecedente  = sc.Input[5];		// appelé par Navig_Niveaux 		// modifié par PrixTypique
	
	
	int& NbMonitors = sc.GetPersistentIntFast(0);  
	int& FirstVisibleBar = sc.GetPersistentIntFast(1);  
	int& LastVisibleBar = sc.GetPersistentIntFast(2);  
	int& TopCoordinatePrevious = sc.GetPersistentIntFast(3);  
	int& HoraireDebutTrace = sc.GetPersistentIntFast(4) ;  // time value en secondes
	int& HoraireFinSeance = sc.GetPersistentIntFast(5) ;
	int& StudyIDOverlayINQ100 = sc.GetPersistentIntFast(6) ;
	int& StudyID_PivotsH_NonProjetes = sc.GetPersistentIntFast(7) ;
	int& TriggerPopup = sc.GetPersistentIntFast(8) ;


	if (sc.SetDefaults)
	{
		sc.AutoLoop = 0;  
		sc.CalculationPrecedence = LOW_PREC_LEVEL  ;  // le SpreadMoyen (Study Overlay INQ100) doit être calculé précédemment
		sc.GraphName = "[C] Pivots Ind H (Full/Mid/Quart) projetes sur le future";        // appelé par AMR   // appelé par Navig_Niveaux  
		sc.ScaleRangeType = SCALE_SAMEASREGION;
		sc.GraphRegion = 1;
		sc.ValueFormat = 2;
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 0 ;
		sc.DisplayStudyInputValues = 0;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		
		sc.HideStudy = 1 ;

		
		// WeeklyChartNumber.Name = "Weekly Chart Number";
		WeeklyChartNumber.SetChartNumber(5);
		TraceVectorielONOFF.Name = "Trace Vectoriel ON/OFF";
		TraceVectorielONOFF.SetYesNo(1);
		AffichageDesQuartsONOFF.Name = "Affichage des Quarts";					// non modifiable par l'utilisateur => modifiable via PrixTypique
		AffichageDesQuartsONOFF.SetYesNo(1);
		AffichageDesMid4Full4ONOFF.Name = "Affichage des Mid4 et Full4";	// non modifiable par l'utilisateur => modifiable via PrixTypique
		AffichageDesMid4Full4ONOFF.SetYesNo(1);
		AffichageDesPHPBBougiePrecedente.Name = "Affichage des PH et PB de la semaine precedente";	
		AffichageDesPHPBBougiePrecedente.SetYesNo(1);
		

		for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].DrawStyle = DRAWSTYLE_LINE ;
			sc.Subgraph[SubgraphIndex].LineStyle = LINESTYLE_SOLID ;
			sc.Subgraph[SubgraphIndex].PrimaryColor = RGB(154,154,154);
			sc.Subgraph[SubgraphIndex].LineLabel = LL_DISPLAY_NAME | LL_NAME_REVERSE_COLORS |  LL_NAME_ALIGN_LEFT_EDGE ;  // LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_LEFT_EDGE  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
			sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre
			sc.Subgraph[SubgraphIndex].DrawZeros = false ;
		}

		PHPrior.PrimaryColor = RGB(255,0,0);
		PBPrior.PrimaryColor = RGB(0,168,0);
		OpenGap.PrimaryColor = RGB(210,0,210);
		OpenGap.LineLabel =  LL_DISPLAY_NAME | LL_NAME_REVERSE_COLORS |  LL_NAME_ALIGN_LEFT_EDGE ;
		DemiGap.PrimaryColor = RGB(210,0,210);


		
		PHPrior.Name = "           / PH HP";
		PBPrior.Name = "           / PB HP";
		PP.Name = "            PP H";
		R1.Name = "            R1 H";
		R2.Name = "            R2 H";
		R3.Name = "            R3 H";
		R4.Name = "            R4 H";
		S1.Name = "            S1 H";
		S2.Name = "            S2 H";
		S3.Name = "            S3 H";
		S4.Name = "            S4 H";
		mR1.Name = "            mR1 H";
		mR2.Name = "            mR2 H";
		mR3.Name = "            mR3 H";
		mR4.Name = "            mR4 H";
		mS1.Name = "            mS1 H";
		mS2.Name = "            mS2 H";
		mS3.Name = "            mS3 H";
		mS4.Name = "            mS4 H";
		RQ1.Name = "            RQ1 H";
		RQ2.Name = "            RQ2 H";
		RQ3.Name = "            RQ3 H";
		RQ4.Name = "            RQ4 H";
		RQ5.Name = "            RQ5 H";
		RQ6.Name = "            RQ6 H";
		RQ7.Name = "            RQ7 H";
		RQ8.Name = "            RQ8 H";
		SQ1.Name = "            SQ1 H";
		SQ2.Name = "            SQ2 H";
		SQ3.Name = "            SQ3 H";
		SQ4.Name = "            SQ4 H";
		SQ5.Name = "            SQ5 H";
		SQ6.Name = "            SQ6 H";
		SQ7.Name = "            SQ7 H";
		SQ8.Name = "            SQ8 H";
		OpenGap.Name = "                OpenGap H";		
		DemiGap.Name = "  DemiGap H";		// 2 espaces de plus que la version J
		
		
		PHPrior.LineWidth = 4;
		PBPrior.LineWidth = 4;
		PP.LineWidth = 4;
		R1.LineWidth = 4;
		R2.LineWidth = 4;
		R3.LineWidth = 4;
		R4.LineWidth = 4;
		S1.LineWidth = 4;
		S2.LineWidth = 4;
		S3.LineWidth = 4;
		S4.LineWidth = 4;
		mR1.LineWidth = 3;
		mR2.LineWidth = 3;
		mR3.LineWidth = 3;
		mR4.LineWidth = 3;
		mS1.LineWidth = 3;
		mS2.LineWidth = 3;
		mS3.LineWidth = 3;
		mS4.LineWidth = 3;
		RQ1.LineWidth = 2;
		RQ2.LineWidth = 2;
		RQ3.LineWidth = 2;
		RQ4.LineWidth = 2;
		RQ5.LineWidth = 2;
		RQ6.LineWidth = 2;
		RQ7.LineWidth = 2;
		RQ8.LineWidth = 2;
		SQ1.LineWidth = 2;
		SQ2.LineWidth = 2;
		SQ3.LineWidth = 2;
		SQ4.LineWidth = 2;
		SQ5.LineWidth = 2;
		SQ6.LineWidth = 2;
		SQ7.LineWidth = 2;
		SQ8.LineWidth = 2;
		OpenGap.LineWidth = 8;
		DemiGap.LineWidth = 3;
		
		return;
	}  // fin du bloc de " if (sc.SetDefaults) "
	
	
	
	if ( sc.UpdateStartIndex == 0 )
	{
		sc.GraphRegion = 1 ;
		NbMonitors = NumberOfMonitors() ;
		FirstVisibleBar = 0 ;
		LastVisibleBar = 0 ;
		TriggerPopup = 1 ;		
		
		//Initialisations des Niveaux et des NiveauxNonArrondis
		for (int SubgraphIndex = 0; SubgraphIndex <= NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex][0] = 0 ;                         // PP[0] = 0 ;
			sc.Subgraph[SubgraphIndex].Arrays[0][0] = 0 ;          // PPNonArrondi[0] = 0 ;
			sc.Subgraph[SubgraphIndex].Arrays[1][0] = 0 ; 
		}
		
		HoraireDebutTrace = HMS_TIME(9,0,0);  
		HoraireFinSeance = HMS_TIME(16,0,0);
		
		// Récupération de la studyID sur le graphe de l'indice
		StudyIDOverlayINQ100 = sc.GetStudyIDByName(sc.ChartNumber, "Overlay INQ100 sur le future (avec correction du spread)", 0) ;
		if  (StudyIDOverlayINQ100 == 0 )
		{ SCString message ;  message.Format("!! ERREUR !! Le chart #6 ne contient pas la study ''Overlay INQ100 sur le future (avec correction du spread)''") ; sc.AddMessageToLog(message,1) ; }

		// Récupération de la StudyID
		StudyID_PivotsH_NonProjetes = sc.GetStudyIDByName(15, "[4] Pivots Ind H (Full/Mid/Quart)", 0) ; 
		if  (StudyID_PivotsH_NonProjetes == 0 )
		{ SCString message ;  message.Format("!! ERREUR !! Le graphique source #15 ne contient pas la study ''[4] Pivots Ind H (Full/Mid/Quart)''") ; sc.AddMessageToLog(message,1) ; }

		// Affichage des quarts, des Mid4Full4, des PHPB : par défaut on affiche tout, puis on met  "LineWidth = 0" à ce qui ne doit pas être affiché
		{
			RQ1.LineWidth = 2;
			RQ2.LineWidth = 2;
			RQ3.LineWidth = 2;
			RQ4.LineWidth = 2;
			RQ5.LineWidth = 2;
			RQ6.LineWidth = 2;
			RQ7.LineWidth = 2;
			RQ8.LineWidth = 2;
			SQ1.LineWidth = 2;
			SQ2.LineWidth = 2;
			SQ3.LineWidth = 2;
			SQ4.LineWidth = 2;
			SQ5.LineWidth = 2;
			SQ6.LineWidth = 2;
			SQ7.LineWidth = 2;
			SQ8.LineWidth = 2;
			mR4.LineWidth = 3;
			mS4.LineWidth = 3;
			R4.LineWidth = 4;
			S4.LineWidth = 4;
			PHPrior.LineWidth = 4;
			PBPrior.LineWidth = 4;
			OpenGap.LineWidth = 6;
			DemiGap.LineWidth = 2;
		}
		
		// Quarts non affichés
		if ( AffichageDesQuartsONOFF.GetYesNo() == 0 )
		{
			RQ1.LineWidth = 0;
			RQ2.LineWidth = 0;
			RQ3.LineWidth = 0;
			RQ4.LineWidth = 0;
			RQ5.LineWidth = 0;
			RQ6.LineWidth = 0;
			RQ7.LineWidth = 0;
			RQ8.LineWidth = 0;
			SQ1.LineWidth = 0;
			SQ2.LineWidth = 0;
			SQ3.LineWidth = 0;
			SQ4.LineWidth = 0;
			SQ5.LineWidth = 0;
			SQ6.LineWidth = 0;
			SQ7.LineWidth = 0;
			SQ8.LineWidth = 0;
		}
		
		// Mid4/Full4 non affichés
		if ( AffichageDesMid4Full4ONOFF.GetYesNo() == 0 )
		{
			RQ7.LineWidth = 0;
			RQ8.LineWidth = 0;
			SQ7.LineWidth = 0;
			SQ8.LineWidth = 0;
			mR4.LineWidth = 0;
			mS4.LineWidth = 0;
			R4.LineWidth = 0;
			S4.LineWidth = 0;
		}

		// PH/PB non affichés
		if ( AffichageDesPHPBBougiePrecedente.GetYesNo() == 0 )
		{
			PHPrior.LineWidth = 0;
			PBPrior.LineWidth = 0;
			OpenGap.LineWidth = 5;
			DemiGap.LineWidth = 0;
		}		
	}
	
	
	
	// on récupère l'array de SpreadMoyen
	SCFloatArray SpreadMoyenArray ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDOverlayINQ100, 0, 1, SpreadMoyenArray) ;
	
	
	for (int index = sc.UpdateStartIndex; index < sc.ArraySize; index++)                 // début AutoLoop
	{
		int OpenTimeValue = sc.BaseDateTimeIn[index].GetTimeInSecondsWithoutMilliseconds()  ; //GetTimeInSecondsWithoutMilliseconds retourne une time_value
		int PreviousOpenTimeValue = sc.BaseDateTimeIn[index-1].GetTimeInSecondsWithoutMilliseconds()  ; 
		
		if ( OpenTimeValue >= HoraireDebutTrace and OpenTimeValue < HoraireFinSeance )  // n'est actualisé que pendant l'ouverture du marché action
		{
			if  ( OpenTimeValue  !=  PreviousOpenTimeValue )   //  n'est actualisé que s'il y a une nouvelle seconde 
			{ 
				// calcul de fSpreadMoyen
				float fSpreadMoyen = SpreadMoyenArray[index] ;
	
				// if (  OpenTimeValue ==  HoraireDebutTrace   )   // les PP sont recalculés tous les jours 
				if (  PP[index-1] == 0  and  OpenTimeValue >=  HoraireDebutTrace  )   // teste si on dépasse HoraireDebutTrace et que les PP n'ont pas encore été calculés
				{
					FirstIndex[index] = index ;
					
					// déclaration des PP
					float fPHPrior = 0, fPBPrior = 0 , fSettlePrior = 0, fOpenCurrent = 0, fPP = 0 ;
					float fR1 = 0, fR2 = 0, fR3 = 0, fR4 = 0, fS1 = 0, fS2 = 0, fS3 = 0, fS4 = 0 ;
					float fmR1 = 0, fmR2 = 0, fmR3 = 0, fmR4 = 0, fmS1 = 0, fmS2 = 0,  fmS3 = 0,   fmS4 = 0 ; 
					float fRQ1 = 0, fRQ2 = 0, fRQ3 = 0, fRQ4 = 0, fRQ5 = 0, fRQ6 = 0, fRQ7 = 0, fRQ8 = 0 ;
					float fSQ1 = 0, fSQ2 = 0, fSQ3 = 0, fSQ4 = 0, fSQ5 = 0, fSQ6 = 0, fSQ7 = 0, fSQ8 = 0 ;


					//  "GetChartBaseData" copie intégralement les données source 
					SCGraphData WeeklyChartData;
					sc.GetChartBaseData(WeeklyChartNumber.GetChartNumber(), WeeklyChartData);
					SCDateTimeArray  WeeklyChartTimeArray;
					sc.GetChartDateTimeArray(WeeklyChartNumber.GetChartNumber(), WeeklyChartTimeArray);
					// on récupère les valeurs de OpenHebdo du chart #15     
					SCFloatArray ExternalArray_OpenHebdo ;
					sc.GetStudyExtraArrayFromChartUsingID(15, StudyID_PivotsH_NonProjetes, 0, 0, ExternalArray_OpenHebdo) ;		// int GetStudyExtraArrayFromChartUsingID(int ChartNumber, int StudyID, int SubgraphIndex, int ExtraArrayIndex, SCFloatArrayRef ExtraArrayRef);
					
					// Récupère un index dans le WeeklyChart. Il y a deux possibilités pour cet index : 	- cas1 : il s'agit de la bougie de la semaine précédente, car la bougie de la nouvelle semaine n'est pas encore connue (typiquement le lundi avant 9h30m15s)
					//																																	- cas 2 : il s'agit de l'index de la semaine active (typiquement le lundi après 9h30m15s ou les jours suivants de la semaine)
					int WeeklyChartIndex = sc.GetContainingIndexForDateTimeIndex(WeeklyChartNumber.GetChartNumber(), index); 
					if ( WeeklyChartIndex == 0  and  TriggerPopup ) 
					{ 
						SCString message ;  message.Format("!! WARNING !! Le chart #5 (Indice Nasdaq UT 1week) ne contient pas assez d'historique ( ExternalChartIndex == 0 )") ;  sc.AddMessageToLog(message, 1) ;
						TriggerPopup = 0 ;
					}
					
					// On détermine si WeeklyChartIndex et index font partie de la même semaine en divisant la date par 7 et en comparant l'entier du float obtenu (la Date est RAZ le samedi : dimanche=1, lundi=2, ... vendredi=5, samedi=0) : 43762/7 = 6251.71... = 6251 (on ne garde que la partie entière)
					if ( WeeklyChartTimeArray[WeeklyChartIndex].GetDate() /7  ==  sc.BaseDateTimeIn[index].GetDate() /7 )             // cas 2 :  WeeklyChartIndex et index font partie de la même semaine        // GetDate retourne un int
					{
						fPHPrior = WeeklyChartData[SC_HIGH][WeeklyChartIndex-1];                         
						fPBPrior = WeeklyChartData[SC_LOW][WeeklyChartIndex-1];
						fSettlePrior = WeeklyChartData[SC_LAST][WeeklyChartIndex-1];
						
						// on récupère l'Open de la semaine courante sur le chart #15 en UT20s, car l'Open est souvent faux sur le chart en UT H
						int Chart15_Index = sc.GetFirstIndexForDate(15, sc.BaseDateTimeIn[index].GetDate()) ; 		// sc.GetFirstIndexForDate() function returns the first array index for the specified ChartNumber where the given TradingDayDate first occurs.
						fOpenCurrent = ExternalArray_OpenHebdo[Chart15_Index];
						
					}
					else      // cas 1 : WeeklyChartIndex correspond à la bougie de la semaine précédente (car la bougie hebdo de la nouvelle semaine n'est pas encore formée)
					{
						fPHPrior = WeeklyChartData[SC_HIGH][WeeklyChartIndex];                         
						fPBPrior = WeeklyChartData[SC_LOW][WeeklyChartIndex];
						fSettlePrior = WeeklyChartData[SC_LAST][WeeklyChartIndex];
						fOpenCurrent = (fPHPrior+fPBPrior)/2 ;
					}
					
					
					// correction des bugs de flux
				/* 	int Year, Month, Day;
					WeeklyChartTimeArray[WeeklyChartIndex-1].GetDateYMD(Year, Month, Day);
					if (Year==2019 and Month==1 and Day==31)  // jour pour lequel HLC est faux
					{
						fPHPrior = nd;
						fPBPrior = nd;
						fSettlePrior = nd;
					} */

					//on calcule les PP 
					CalculatePPs (fPHPrior, fPBPrior, fSettlePrior, fPP, fR1, fR2, fR3, fR4, fS1, fS2, fS3, fS4, fmR1, fmR2, fmR3, fmR4, fmS1, fmS2, fmS3, fmS4, fRQ1, fRQ2, fRQ3, fRQ4, fRQ5, fRQ6, fRQ7, fRQ8, fSQ1, fSQ2, fSQ3, fSQ4, fSQ5, fSQ6, fSQ7, fSQ8);
					
					// Sauvegarde des niveaux non arrondis
					PHPrior.Arrays[1][index] = fPHPrior ;
					PBPrior.Arrays[1][index] = fPBPrior ;
					PP.Arrays[1][index] = fPP ;
					R1.Arrays[1][index] = fR1 ;
					R2.Arrays[1][index] = fR2 ;
					R3.Arrays[1][index] = fR3 ;
					R4.Arrays[1][index] = fR4 ;
					S1.Arrays[1][index] = fS1 ;
					S2.Arrays[1][index] = fS2 ;
					S3.Arrays[1][index] = fS3 ;
					S4.Arrays[1][index] = fS4 ;
					mR1.Arrays[1][index] = fmR1 ;
					mR2.Arrays[1][index] = fmR2 ;
					mR3.Arrays[1][index] = fmR3 ;
					mR4.Arrays[1][index] = fmR4 ;
					mS1.Arrays[1][index] = fmS1 ;
					mS2.Arrays[1][index] = fmS2 ;
					mS3.Arrays[1][index] = fmS3 ;
					mS4.Arrays[1][index] = fmS4 ;
					RQ1.Arrays[1][index] = fRQ1 ;
					RQ2.Arrays[1][index] = fRQ2 ;
					RQ3.Arrays[1][index] = fRQ3 ;
					RQ4.Arrays[1][index] = fRQ4 ;
					RQ5.Arrays[1][index] = fRQ5 ;
					RQ6.Arrays[1][index] = fRQ6 ;
					RQ7.Arrays[1][index] = fRQ7 ;
					RQ8.Arrays[1][index] = fRQ8 ;
					SQ1.Arrays[1][index] = fSQ1 ;
					SQ2.Arrays[1][index] = fSQ2 ;
					SQ3.Arrays[1][index] = fSQ3 ;
					SQ4.Arrays[1][index] = fSQ4 ;
					SQ5.Arrays[1][index] = fSQ5 ;
					SQ6.Arrays[1][index] = fSQ6 ;
					SQ7.Arrays[1][index] = fSQ7 ;
					SQ8.Arrays[1][index] = fSQ8 ;
					
					
					// Recherche d'un opengap dans la première bougie du jour, puis stockage dans Arrays[1]  (valeurs non corrigées)
					if (   fOpenCurrent > fPHPrior   )      // gap haussier  
					{ OpenGap.Arrays[1][index] = fPHPrior ;  DemiGap.Arrays[1][index] = (fOpenCurrent + fPHPrior)/2 ;  }
					else if (  fOpenCurrent < fPBPrior   )     //    gap baissier  
					{ OpenGap.Arrays[1][index] = fPBPrior ;  DemiGap.Arrays[1][index] = (fOpenCurrent + fPBPrior)/2 ;  }
					else    // pas de gap
					{ OpenGap.Arrays[1][index] = 0; DemiGap.Arrays[1][index] = 0; }
					
					
					// on rappelle la valeur de chaque niveau et on l'adjuste du spread, puis on fait l'arrondi
					for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)  
					{ 	sc.Subgraph[SubgraphIndex].Arrays[0][index] = sc.Subgraph[SubgraphIndex].Arrays[1][index] + fSpreadMoyen ;  ArrondiSiDepasse(sc.Subgraph[SubgraphIndex], 0.5, index) ;  } 
				}
				
				else // on n'est pas dans le premier chandelier du jour, mais on est dans une nouvelle seconde => on rappelle la valeur de chaque niveau et on l'adjuste du spread, puis on fait l'arrondi
				{   
					for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)  {  sc.Subgraph[SubgraphIndex].Arrays[1][index] = sc.Subgraph[SubgraphIndex].Arrays[1][index-1] ;  sc.Subgraph[SubgraphIndex].Arrays[0][index] = sc.Subgraph[SubgraphIndex].Arrays[1][index] + fSpreadMoyen ; ArrondiSiDepasse(sc.Subgraph[SubgraphIndex], 0.5, index) ;  } 	 
					FirstIndex[index] = FirstIndex[index-1] ;
				}
				
			}
			
			else   // la barre ne débute pas dans une nouvelle seconde => on recopie les valeurs de l'index précédent
			{	
				for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)  {  sc.Subgraph[SubgraphIndex][index] = sc.Subgraph[SubgraphIndex][index-1] ;  sc.Subgraph[SubgraphIndex].Arrays[0][index] = sc.Subgraph[SubgraphIndex].Arrays[0][index-1] ;  sc.Subgraph[SubgraphIndex].Arrays[1][index] = sc.Subgraph[SubgraphIndex].Arrays[1][index-1] ;   }         
				FirstIndex[index] = FirstIndex[index-1] ;
			}  
			
		}
		
	} // fin de AutoLoop
	
	
	
	// Bloc d'affichage       
	if ( TraceVectorielONOFF.GetYesNo() )
	{
		if ( sc.ChartWindowIsActive  or  NbMonitors == 2 )
		{
			TopCoordinate = sc.YPixelCoordinateToGraphValue(sc.StudyRegionTopCoordinate) ;
			if (  FirstVisibleBar != sc.IndexOfFirstVisibleBar  or  LastVisibleBar != sc.IndexOfLastVisibleBar  or  sc.IsFullRecalculation  or  fabs(TopCoordinatePrevious-TopCoordinate) > 0.02  )          	// Le recalcul des lignes n'est réalisé que si FirstIndex ou LastVisibleBar ou TopCoordinate varie (on fait l'hypothèse que TopCoordinate et BottomCoordinate varient de manière synchronisée)    // Il faut rajouter sc.IsFullRecalculation car lors d'une Full ReCalculation les useTool non UDD sont effacés
			{	
				FirstVisibleBar = sc.IndexOfFirstVisibleBar ;
				LastVisibleBar = sc.IndexOfLastVisibleBar ;
				BottomCoordinate = sc.YPixelCoordinateToGraphValue(sc.StudyRegionBottomCoordinate) ;
				TopCoordinatePrevious = TopCoordinate ;
				
				// les useTool doivent être effacés à chaque appel du DLL pour éviter une fuite de mémoire   
				sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   // all non user drawn Chart Drawings on the specified ChartNumber will be deleted. The LineNumber parameter in this case is ignored and should be 0. 		
				
				s_sc* const ptrsc = &sc;
				int const firstIndex = FirstIndex[LastVisibleBar] ;
				
				// Tracé des Full et Mid
				for (int SubgraphIndex = 0; SubgraphIndex <= 19 ; SubgraphIndex++)
				{
					float const Niveau = sc.Subgraph[SubgraphIndex].Arrays[0][LastVisibleBar] ;    // la ligne vectorielle est tracée sur une ordonnée prise à LastVisibleBar
					SCString const Texte = sc.Subgraph[SubgraphIndex].Name ;
					int const epaisseur = sc.Subgraph[SubgraphIndex].LineWidth ;
					COLORREF const couleur = sc.Subgraph[SubgraphIndex].PrimaryColor;
					creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, Niveau, epaisseur, couleur, ptrsc);    // void creationLigneEtTexte (SCString Texte, int beginIndex, int endIndex, float niveau, int monEpaisseur, COLORREF maCouleur, s_sc* ptrsc)
				}
				
				// DemiGap  ( SubgraphIndex = 36 )
				{
					float const Niveau = sc.Subgraph[36].Arrays[0][LastVisibleBar] ;    // la ligne vectorielle est tracée sur une ordonnée prise à LastVisibleBar
					SCString const Texte = sc.Subgraph[36].Name ;
					int const epaisseur = sc.Subgraph[36].LineWidth ;
					COLORREF const couleur = sc.Subgraph[36].PrimaryColor;
					creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, Niveau, epaisseur, couleur, ptrsc); 
				}
				
				// Tracé des Quart
				if ( AffichageDesQuartsONOFF.GetYesNo() )
				{
					for (int SubgraphIndex = 20; SubgraphIndex <= 35 ; SubgraphIndex++)  // RQ1 à SQ8
					{
						float const Niveau = sc.Subgraph[SubgraphIndex].Arrays[0][LastVisibleBar] ;    // la ligne vectorielle est tracée sur une ordonnée prise à LastVisibleBar
						SCString const Texte = sc.Subgraph[SubgraphIndex].Name ;
						int const epaisseur = sc.Subgraph[SubgraphIndex].LineWidth ;
						COLORREF const couleur = sc.Subgraph[SubgraphIndex].PrimaryColor;
						creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, Niveau, epaisseur, couleur, ptrsc);    // void creationLigneEtTexte (SCString Texte, int beginIndex, int endIndex, float niveau, int monEpaisseur, COLORREF maCouleur, s_sc* ptrsc)
					}
				}
			
				// recalcul immédiat
				sc.UpdateAlways = 1 ;
			}
		}
	}
	else sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_MesPointsPivotsJournaliersProjetes(SCStudyInterfaceRef sc)		// [D]
{
	SCSubgraphRef OpenGap = sc.Subgraph[0];										// appelé par Navig_Niveaux   // sc.Subgraph[].Arrays[1] appelé par Navig_Niveaux
	SCSubgraphRef PHPrior = sc.Subgraph[1];         // appelé par AMR   // appelé par Navig_Niveaux   // sc.Subgraph[].Arrays[1] appelé par Navig_Niveaux
	SCSubgraphRef PBPrior = sc.Subgraph[2];	     // appelé par AMR    // appelé par Navig_Niveaux   // sc.Subgraph[].Arrays[1] appelé par Navig_Niveaux
	SCSubgraphRef PP = sc.Subgraph[3];                // appelé par AMR    // appelé par Navig_Niveaux   // sc.Subgraph[].Arrays[1] appelé par Navig_Niveaux
	SCSubgraphRef R1 = sc.Subgraph[4];                // ...
	SCSubgraphRef R2 = sc.Subgraph[5];
	SCSubgraphRef R3 = sc.Subgraph[6];
	SCSubgraphRef R4 = sc.Subgraph[7];
	SCSubgraphRef S1 = sc.Subgraph[8];
	SCSubgraphRef S2 = sc.Subgraph[9];
	SCSubgraphRef S3 = sc.Subgraph[10];
	SCSubgraphRef S4 = sc.Subgraph[11];
	SCSubgraphRef mR1 = sc.Subgraph[12];
	SCSubgraphRef mR2 = sc.Subgraph[13];
	SCSubgraphRef mR3 = sc.Subgraph[14];
	SCSubgraphRef mR4 = sc.Subgraph[15];
	SCSubgraphRef mS1 = sc.Subgraph[16];
	SCSubgraphRef mS2 = sc.Subgraph[17];
	SCSubgraphRef mS3 = sc.Subgraph[18];
	SCSubgraphRef mS4 = sc.Subgraph[19];
	SCSubgraphRef RQ1 = sc.Subgraph[20];
	SCSubgraphRef RQ2 = sc.Subgraph[21];
	SCSubgraphRef RQ3 = sc.Subgraph[22];
	SCSubgraphRef RQ4 = sc.Subgraph[23];
	SCSubgraphRef RQ5 = sc.Subgraph[24];
	SCSubgraphRef RQ6 = sc.Subgraph[25];
	SCSubgraphRef RQ7 = sc.Subgraph[26];
	SCSubgraphRef RQ8 = sc.Subgraph[27];
	SCSubgraphRef SQ1 = sc.Subgraph[28];
	SCSubgraphRef SQ2 = sc.Subgraph[29];
	SCSubgraphRef SQ3 = sc.Subgraph[30];
	SCSubgraphRef SQ4 = sc.Subgraph[31];
	SCSubgraphRef SQ5 = sc.Subgraph[32];
	SCSubgraphRef SQ6 = sc.Subgraph[33];                      // ...
	SCSubgraphRef SQ7 = sc.Subgraph[34];                     // appelé par AMR    // appelé par Navig_Niveaux   // sc.Subgraph[].Arrays[1] appelé par Navig_Niveaux
	SCSubgraphRef SQ8 = sc.Subgraph[35];                    // appelé par AMR    // appelé par Navig_Niveaux   // sc.Subgraph[].Arrays[1] appelé par Navig_Niveaux
	SCSubgraphRef SettlePrior = sc.Subgraph[36];            // non appelé
	SCSubgraphRef DemiGap = sc.Subgraph[37];             								   // appelé par Navig_Niveaux   // sc.Subgraph[].Arrays[1] appelé par Navig_Niveaux
	const int NUMBER_OF_STUDY_SUBGRAPHS = 38;  
	
	SCFloatArrayRef FirstIndex = sc.Subgraph[0].Arrays[2] ;  

	
	
	SCInputRef DailyChartNumber = sc.Input[0];
	SCInputRef TraceVectorielONOFF = sc.Input[2];					// appelé par Navig_Niveaux 		// modifié par PrixTypique
	SCInputRef AffichageDesMid4Full4ONOFF = sc.Input[4];		// appelé par Navig_Niveaux 		// modifié par PrixTypique
	SCInputRef AffichageDesPHPBBougiePrecedente  = sc.Input[5];		// appelé par Navig_Niveaux 		// modifié par PrixTypique

	
	
	int& NbMonitors = sc.GetPersistentIntFast(0);  
	int& FirstVisibleBar = sc.GetPersistentIntFast(1);  
	int& LastVisibleBar = sc.GetPersistentIntFast(2);  
	int& TopCoordinatePrevious = sc.GetPersistentIntFast(3);  
	int& HoraireDebutTrace = sc.GetPersistentIntFast(4) ;  // time value en secondes
	int& HoraireFinSeance = sc.GetPersistentIntFast(5) ;
	int& StudyIDOverlayINQ100 = sc.GetPersistentIntFast(6) ;
	int& StudyID_PivotsJ_NonProjetes = sc.GetPersistentIntFast(7) ;
	int& TriggerPopup = sc.GetPersistentIntFast(8) ;
	

	if (sc.SetDefaults)
	{
		sc.AutoLoop = 0;  
		sc.CalculationPrecedence = LOW_PREC_LEVEL  ;  // le SpreadMoyen (Study Overlay INQ100) doit être calculé précédemment
		sc.GraphName = "[D] Pivots Ind J (Full/Mid) projetes sur le future";        // appelé par AMR
		sc.ScaleRangeType = SCALE_SAMEASREGION;
		sc.ValueFormat = 2;
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 0 ;
		sc.DisplayStudyInputValues = 0;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;	
		
		sc.HideStudy = 1 ;

		// DailyChartNumber.Name = "Daily Chart Number";
		DailyChartNumber.SetChartNumber(2);
		TraceVectorielONOFF.Name = "Trace Vectoriel ON/OFF";
		TraceVectorielONOFF.SetYesNo(1);
		AffichageDesMid4Full4ONOFF.Name = "Affichage des Mid4 et Full4";	// non modifiable par l'utilisateur => modifiable via PrixTypique
		AffichageDesMid4Full4ONOFF.SetYesNo(1);
		AffichageDesPHPBBougiePrecedente.Name = "Affichage des PH, PB et Close du jour precedent";	
		AffichageDesPHPBBougiePrecedente.SetYesNo(1);
		
		
		for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].DrawStyle = DRAWSTYLE_LINE ;
			sc.Subgraph[SubgraphIndex].LineStyle = LINESTYLE_SOLID ;
			sc.Subgraph[SubgraphIndex].PrimaryColor = RGB(195,195,0) ;
			sc.Subgraph[SubgraphIndex].LineLabel = LL_DISPLAY_NAME | LL_NAME_REVERSE_COLORS |  LL_NAME_ALIGN_LEFT_EDGE ;  // LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_LEFT_EDGE  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
			sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre
			sc.Subgraph[SubgraphIndex].DrawZeros = false ;
		}

		PHPrior.PrimaryColor = RGB(255,0,0);
		
		PBPrior.PrimaryColor = RGB(0,168,0);
		
		SettlePrior.PrimaryColor = RGB(210,0,210);
		SettlePrior.DrawStyle = DRAWSTYLE_HIDDEN;
		
		OpenGap.PrimaryColor = RGB(210,0,210);
		OpenGap.LineLabel =  LL_DISPLAY_NAME | LL_NAME_REVERSE_COLORS |  LL_NAME_ALIGN_LEFT_EDGE ;

		DemiGap.PrimaryColor = RGB(210,0,210);
		
		
		PHPrior.Name = "PH JP";
		PBPrior.Name = "PB JP";
		SettlePrior.Name = "Cloture JP";
		PP.Name = "PP J";
		R1.Name = "R1 J";
		R2.Name = "R2 J";
		R3.Name = "R3 J";
		R4.Name = "R4 J";
		S1.Name = "S1 J";
		S2.Name = "S2 J";
		S3.Name = "S3 J";
		S4.Name = "S4 J";
		mR1.Name = "mR1 J";
		mR2.Name = "mR2 J";
		mR3.Name = "mR3 J";
		mR4.Name = "mR4 J";
		mS1.Name = "mS1 J";
		mS2.Name = "mS2 J";
		mS3.Name = "mS3 J";
		mS4.Name = "mS4 J";
		// RQ1.Name = "RQ1 J";
		// RQ2.Name = "RQ2 J";
		// RQ3.Name = "RQ3 J";
		// RQ4.Name = "RQ4 J";
		// RQ5.Name = "RQ5 J";
		// RQ6.Name = "RQ6 J";
		// RQ7.Name = "RQ7 J";
		// RQ8.Name = "RQ8 J";
		// SQ1.Name = "SQ1 J";
		// SQ2.Name = "SQ2 J";
		// SQ3.Name = "SQ3 J";
		// SQ4.Name = "SQ4 J";
		// SQ5.Name = "SQ5 J";
		// SQ6.Name = "SQ6 J";
		// SQ7.Name = "SQ7 J";
		// SQ8.Name = "SQ8 J";
		OpenGap.Name = "        OpenGap J";
		DemiGap.Name = "DemiGap J";
		
		
		PHPrior.LineWidth = 4;
		PBPrior.LineWidth = 4;
		SettlePrior.LineWidth = 2;
		PP.LineWidth = 3;
		R1.LineWidth = 3;
		R2.LineWidth = 3;
		R3.LineWidth = 3;
		R4.LineWidth = 3;
		S1.LineWidth = 3;
		S2.LineWidth = 3;
		S3.LineWidth = 3;
		S4.LineWidth = 3;
		mR1.LineWidth = 2;
		mR2.LineWidth = 2;
		mR3.LineWidth = 2;
		mR4.LineWidth = 2;
		mS1.LineWidth = 2;
		mS2.LineWidth = 2;
		mS3.LineWidth = 2;
		mS4.LineWidth = 2;
		RQ1.LineWidth = 1;
		RQ2.LineWidth = 1;
		RQ3.LineWidth = 1;
		RQ4.LineWidth = 1;
		RQ5.LineWidth = 1;
		RQ6.LineWidth = 1;
		RQ7.LineWidth = 1;
		RQ8.LineWidth = 1;
		SQ1.LineWidth = 1;
		SQ2.LineWidth = 1;
		SQ3.LineWidth = 1;
		SQ4.LineWidth = 1;
		SQ5.LineWidth = 1;
		SQ6.LineWidth = 1;
		SQ7.LineWidth = 1;
		SQ8.LineWidth = 1;
		OpenGap.LineWidth = 8;
		DemiGap.LineWidth = 3;
		
		return;
	}  // fin du bloc de " if (sc.SetDefaults) "
	
	
	if ( sc.UpdateStartIndex == 0 )
	{
		sc.GraphRegion = 1 ;
		NbMonitors = NumberOfMonitors() ;
		FirstVisibleBar = 0 ;
		LastVisibleBar = 0 ;
		TriggerPopup = 1 ;
	
		//Initialisations des Niveaux et des NiveauxNonArrondis
		for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex][0] = 0 ;                         //  Arrondi ( PP indice + fSpreadMoyen ) : varie de 0.5 en 0.5 pt
			sc.Subgraph[SubgraphIndex].Arrays[0][0] = 0 ;          //  PP indice + fSpreadMoyen
			sc.Subgraph[SubgraphIndex].Arrays[1][0] = 0 ;          //  PP indice (avant projection)
		}
		
		// HoraireDebutTrace = HMS_TIME(9,30,25);   // SpreadMoyen n'est pas retourné avant 9h30m24s
		HoraireDebutTrace = HMS_TIME(9,0,0);   
		HoraireFinSeance = HMS_TIME(16,0,0);
		
		// Récupération de la studyID sur le graphe 16-ticks
		StudyIDOverlayINQ100 = sc.GetStudyIDByName(sc.ChartNumber, "Overlay INQ100 sur le future (avec correction du spread)", 0) ;
		if  (StudyIDOverlayINQ100 == 0 )
		{ SCString message ;  message.Format("!! ERREUR !! Le chart #6 ne contient pas la study ''Overlay INQ100 sur le future (avec correction du spread)''") ; sc.AddMessageToLog(message,1) ; }
	
		// Récupération de la StudyID
		StudyID_PivotsJ_NonProjetes = sc.GetStudyIDByName(15, "[6] Pivots Ind J (Full/Mid/Quart)", 0) ; 
		if  (StudyID_PivotsJ_NonProjetes == 0 )
		{ SCString message ;  message.Format("!! ERREUR !! Le graphique source #15 ne contient pas la study ''[6] Pivots Ind J (Full/Mid/Quart)''") ; sc.AddMessageToLog(message,1) ; }
	
		// Affichage des Mid4Full4, des PHPB : par défaut on affiche tout, puis on met  "LineWidth = 0" à ce qui ne doit pas être affiché
		{
			RQ7.LineWidth = 1;
			RQ8.LineWidth = 1;
			SQ7.LineWidth = 1;
			SQ8.LineWidth = 1;
			mR4.LineWidth = 2;
			mS4.LineWidth = 2;
			R4.LineWidth = 3;
			S4.LineWidth = 3;
			PHPrior.LineWidth = 4;
			PBPrior.LineWidth = 4;
			SettlePrior.LineWidth = 2;
			OpenGap.LineWidth = 6;
			DemiGap.LineWidth = 3;
		}
		
		if ( AffichageDesMid4Full4ONOFF.GetYesNo() == 0)
		{
			RQ7.LineWidth = 0;
			RQ8.LineWidth = 0;
			SQ7.LineWidth = 0;
			SQ8.LineWidth = 0;
			mR4.LineWidth = 0;
			mS4.LineWidth = 0;
			R4.LineWidth = 0;
			S4.LineWidth = 0;
		}
		
		// PH/PB non affichés
		if ( AffichageDesPHPBBougiePrecedente.GetYesNo() == 0 )
		{
			PHPrior.LineWidth = 0;
			PBPrior.LineWidth = 0;
			SettlePrior.LineWidth = 0;
			OpenGap.LineWidth = 0;
			DemiGap.LineWidth = 0;
		}		
	}
	
	
	// on récupère l'array de SpreadMoyen
	SCFloatArray SpreadMoyenArray ;
	sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDOverlayINQ100, 0, 1, SpreadMoyenArray) ;


	for (int index = sc.UpdateStartIndex; index < sc.ArraySize; index++)                 // AutoLoop
	{
		int OpenTimeValue = sc.BaseDateTimeIn[index].GetTimeInSecondsWithoutMilliseconds()  ; //GetTimeInSecondsWithoutMilliseconds retourne une time_value
		int PreviousOpenTimeValue = sc.BaseDateTimeIn[index-1].GetTimeInSecondsWithoutMilliseconds()  ; 
		
		if ( OpenTimeValue >= HoraireDebutTrace  and  OpenTimeValue < HoraireFinSeance )  // n'est actualisé que entre HoraireDebutTrace et HoraireFinSeance
		{
			if  ( OpenTimeValue  !=  PreviousOpenTimeValue )   //  n'est actualisé que s'il y a une nouvelle seconde 
			{ 
				// calcul de fSpreadMoyen
				float fSpreadMoyen = SpreadMoyenArray[index] ;
	
				if (  PP[index-1] == 0  and  OpenTimeValue >=  HoraireDebutTrace  )   // teste si on dépasse HoraireDebutTrace et que les PP n'ont pas encore été calculés  
				{
					FirstIndex[index] = index ;
					
					// déclaration des PP
					float fPHPrior = 0, fPBPrior = 0 , fSettlePrior = 0, fOpenCurrent = 0, fPP = 0 ;
					float fR1 = 0, fR2 = 0, fR3 = 0, fR4 = 0, fS1 = 0, fS2 = 0, fS3 = 0, fS4 = 0 ;
					float fmR1 = 0, fmR2 = 0, fmR3 = 0, fmR4 = 0, fmS1 = 0, fmS2 = 0,  fmS3 = 0,   fmS4 = 0 ; 
					float fRQ1 = 0, fRQ2 = 0, fRQ3 = 0, fRQ4 = 0, fRQ5 = 0, fRQ6 = 0, fRQ7 = 0, fRQ8 = 0 ;
					float fSQ1 = 0, fSQ2 = 0, fSQ3 = 0, fSQ4 = 0, fSQ5 = 0, fSQ6 = 0, fSQ7 = 0, fSQ8 = 0 ;


					//  "GetChartBaseData" copie intégralement les données source 
					SCGraphData DailyChartData;
					sc.GetChartBaseData(DailyChartNumber.GetChartNumber(), DailyChartData);
					// on récupère les valeurs de OpenDaily du chart #15     
					SCFloatArray ExternalArray_OpenDaily ;
					sc.GetStudyExtraArrayFromChartUsingID(15, StudyID_PivotsJ_NonProjetes, 0, 0, ExternalArray_OpenDaily) ;		// int GetStudyExtraArrayFromChartUsingID(int ChartNumber, int StudyID, int SubgraphIndex, int ExtraArrayIndex, SCFloatArrayRef ExtraArrayRef);

					
					// Récupération de DailyChartIndex (index du jour sur le graphe externe en UT jour)
					int DailyChartIndex = sc.GetExactMatchForSCDateTime(DailyChartNumber.GetChartNumber(), sc.BaseDateTimeIn[index].GetDate()) ;   // sc.GetExactMatchForSCDateTime() returns the index into the Base Data arrays of the chart specified by ChartNumber that exactly matches the DateTime. If there is no exact match, this function returns -1.
					if ( DailyChartIndex == 0  and  TriggerPopup ) 
					{ 
						SCString message ;  message.Format("!! WARNING !! Le chart #2 (Indice Nasdaq UT 1J) ne contient pas assez d'historique ( ExternalChartIndex == 0 )") ;  sc.AddMessageToLog(message, 1) ;
						TriggerPopup = 0 ;
					}
					if ( DailyChartIndex != -1 ) // la bougie journalière existe déjà (car il est après "jour J + 9h30m15s" => DailyChartIndex a la même date que index
					{
						// on récupère HLC de la veille sur le chart en UT J
						fPHPrior = DailyChartData[SC_HIGH][DailyChartIndex-1];                         
						fPBPrior = DailyChartData[SC_LOW][DailyChartIndex-1];
						fSettlePrior = DailyChartData[SC_LAST][DailyChartIndex-1];
						
						// on récupère l'Open du jour courant sur le chart #15 en UT20s, car l'Open est souvent faux sur le chart en UT J
						int Chart15_Index = sc.GetFirstIndexForDate(15, sc.BaseDateTimeIn[index].GetDate()) ; 		// sc.GetFirstIndexForDate() function returns the first array index for the specified ChartNumber where the given TradingDayDate first occurs.
						fOpenCurrent = ExternalArray_OpenDaily[Chart15_Index];
					}
					else // la bougie journalière externe en UT jour n'est pas encore disponible (cas du trading en temps réel lorsqu'il est avant 9h30m15s)
					{
						DailyChartIndex = sc.GetContainingIndexForDateTimeIndex(DailyChartNumber.GetChartNumber(), index); // on récupère l'index de la bougie de la veille sur le daily chart
						
						// on récupère HLC de la veille et on fait en sorte que l'Open du jour ne crée pas un gap (à ce stade l'Open n'existe pas encore)
						fPHPrior = DailyChartData[SC_HIGH][DailyChartIndex];                         
						fPBPrior = DailyChartData[SC_LOW][DailyChartIndex];
						fSettlePrior = DailyChartData[SC_LAST][DailyChartIndex];
						fOpenCurrent = (fPHPrior+fPBPrior)/2 ;
					}
					
					//on calcule les PP 
					CalculatePPs (fPHPrior, fPBPrior, fSettlePrior, fPP, fR1, fR2, fR3, fR4, fS1, fS2, fS3, fS4, fmR1, fmR2, fmR3, fmR4, fmS1, fmS2, fmS3, fmS4, fRQ1, fRQ2, fRQ3, fRQ4, fRQ5, fRQ6, fRQ7, fRQ8, fSQ1, fSQ2, fSQ3, fSQ4, fSQ5, fSQ6, fSQ7, fSQ8);
					
					// Sauvegarde des niveaux non arrondis
					PHPrior.Arrays[1][index] = fPHPrior ;
					PBPrior.Arrays[1][index] = fPBPrior ;
					SettlePrior.Arrays[1][index] = fSettlePrior ;
					PP.Arrays[1][index] = fPP ;
					R1.Arrays[1][index] = fR1 ;
					R2.Arrays[1][index] = fR2 ;
					R3.Arrays[1][index] = fR3 ;
					R4.Arrays[1][index] = fR4 ;
					S1.Arrays[1][index] = fS1 ;
					S2.Arrays[1][index] = fS2 ;
					S3.Arrays[1][index] = fS3 ;
					S4.Arrays[1][index] = fS4 ;
					mR1.Arrays[1][index] = fmR1 ;
					mR2.Arrays[1][index] = fmR2 ;
					mR3.Arrays[1][index] = fmR3 ;
					mR4.Arrays[1][index] = fmR4 ;
					mS1.Arrays[1][index] = fmS1 ;
					mS2.Arrays[1][index] = fmS2 ;
					mS3.Arrays[1][index] = fmS3 ;
					mS4.Arrays[1][index] = fmS4 ;
					RQ1.Arrays[1][index] = fRQ1 ;
					RQ2.Arrays[1][index] = fRQ2 ;
					RQ3.Arrays[1][index] = fRQ3 ;
					RQ4.Arrays[1][index] = fRQ4 ;
					RQ5.Arrays[1][index] = fRQ5 ;
					RQ6.Arrays[1][index] = fRQ6 ;
					RQ7.Arrays[1][index] = fRQ7 ;
					RQ8.Arrays[1][index] = fRQ8 ;
					SQ1.Arrays[1][index] = fSQ1 ;
					SQ2.Arrays[1][index] = fSQ2 ;
					SQ3.Arrays[1][index] = fSQ3 ;
					SQ4.Arrays[1][index] = fSQ4 ;
					SQ5.Arrays[1][index] = fSQ5 ;
					SQ6.Arrays[1][index] = fSQ6 ;
					SQ7.Arrays[1][index] = fSQ7 ;
					SQ8.Arrays[1][index] = fSQ8 ;
					
					
					// Recherche d'un opengap dans la première bougie du jour, puis stockage dans Arrays[1]  (valeurs non corrigées)
					if (   fOpenCurrent > fPHPrior   )      // gap haussier  
					{ OpenGap.Arrays[1][index] = fPHPrior ;  DemiGap.Arrays[1][index] = (fOpenCurrent + fPHPrior)/2 ;  }
					else if (  fOpenCurrent < fPBPrior   )     //    gap baissier  
					{ OpenGap.Arrays[1][index] = fPBPrior ;  DemiGap.Arrays[1][index] = (fOpenCurrent + fPBPrior)/2 ;  }
					else    // pas de gap
					{ OpenGap.Arrays[1][index] = 0; DemiGap.Arrays[1][index] = 0; }
					
					
					// on rappelle la valeur de chaque niveau et on l'adjuste du spread, puis on fait l'arrondi
					for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)  
					{ 	
						sc.Subgraph[SubgraphIndex].Arrays[0][index] = sc.Subgraph[SubgraphIndex].Arrays[1][index] + fSpreadMoyen ;
						ArrondiSiDepasse(sc.Subgraph[SubgraphIndex], 0.5, index) ;  
					} 
					
				}

				else // on n'est pas dans le premier chandelier du jour, mais on est dans une nouvelle seconde => on rappelle la valeur de chaque niveau et on l'adjuste du spread, puis on fait l'arrondi
				{
					for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)  {  sc.Subgraph[SubgraphIndex].Arrays[1][index] = sc.Subgraph[SubgraphIndex].Arrays[1][index-1] ;  sc.Subgraph[SubgraphIndex].Arrays[0][index] = sc.Subgraph[SubgraphIndex].Arrays[1][index] + fSpreadMoyen ; ArrondiSiDepasse(sc.Subgraph[SubgraphIndex], 0.5, index) ;  } 	 
					FirstIndex[index] = FirstIndex[index-1] ;
				}
				
			}
			
			else   // la barre ne débute pas dans une nouvelle seconde => on recopie les valeurs de l'index précédent
			{	
				for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)  {  sc.Subgraph[SubgraphIndex][index] = sc.Subgraph[SubgraphIndex][index-1] ;  sc.Subgraph[SubgraphIndex].Arrays[0][index] = sc.Subgraph[SubgraphIndex].Arrays[0][index-1] ;  sc.Subgraph[SubgraphIndex].Arrays[1][index] = sc.Subgraph[SubgraphIndex].Arrays[1][index-1] ;   }         
				FirstIndex[index] = FirstIndex[index-1] ;
			}  
			
		}
		
		/* else  // le marché action est fermé    
		{  	
			for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)  {  sc.Subgraph[SubgraphIndex][index] = 0 ;  sc.Subgraph[SubgraphIndex].Arrays[0][index] = 0 ;   sc.Subgraph[SubgraphIndex].Arrays[1][index] = 0 ;   }    
			FirstIndex[index] = 0 ;
		}  */

	} // fin de AutoLoop
	

	
	// Bloc d'affichage       
	if ( TraceVectorielONOFF.GetYesNo() )
	{
		if ( sc.ChartWindowIsActive  or  NbMonitors == 2 )
		{
			TopCoordinate = sc.YPixelCoordinateToGraphValue(sc.StudyRegionTopCoordinate) ;
			if (  FirstVisibleBar != sc.IndexOfFirstVisibleBar  or  LastVisibleBar != sc.IndexOfLastVisibleBar  or  sc.IsFullRecalculation  or  fabs(TopCoordinatePrevious-TopCoordinate) > 0.02  )          	// Le recalcul des lignes n'est réalisé que si FirstIndex ou LastVisibleBar ou TopCoordinate varie (on fait l'hypothèse que TopCoordinate et BottomCoordinate varient de manière synchronisée)    // Il faut rajouter sc.IsFullRecalculation car lors d'une Full ReCalculation les useTool non UDD sont effacés
			{	
				FirstVisibleBar = sc.IndexOfFirstVisibleBar ;
				LastVisibleBar = sc.IndexOfLastVisibleBar ;
				BottomCoordinate = sc.YPixelCoordinateToGraphValue(sc.StudyRegionBottomCoordinate) ;
				TopCoordinatePrevious = TopCoordinate ;
				
				// les useTool doivent être effacés à chaque appel du DLL pour éviter une fuite de mémoire   
				sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   // all non user drawn Chart Drawings on the specified ChartNumber will be deleted. The LineNumber parameter in this case is ignored and should be 0. 		
				
				s_sc* const ptrsc = &sc;
				int const firstIndex = FirstIndex[LastVisibleBar] ;

				for (int SubgraphIndex = 0; SubgraphIndex <= 19; SubgraphIndex++)
				{
					float const Niveau = sc.Subgraph[SubgraphIndex].Arrays[0][LastVisibleBar] ;    // la ligne vectorielle est tracée sur une ordonnée prise à LastVisibleBar
					SCString const Texte = sc.Subgraph[SubgraphIndex].Name ;
					int const epaisseur = sc.Subgraph[SubgraphIndex].LineWidth ;
					COLORREF const couleur = sc.Subgraph[SubgraphIndex].PrimaryColor;
					creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, Niveau, epaisseur, couleur, ptrsc);    // void creationLigneEtTexte (SCString Texte, int beginIndex, int endIndex, float niveau, int monEpaisseur, COLORREF maCouleur, s_sc* ptrsc)
				}

				for (int SubgraphIndex = 36; SubgraphIndex <= 37; SubgraphIndex++)
				{
					float const Niveau = sc.Subgraph[SubgraphIndex].Arrays[0][LastVisibleBar] ;    // la ligne vectorielle est tracée sur une ordonnée prise à LastVisibleBar
					SCString const Texte = sc.Subgraph[SubgraphIndex].Name ;
					int const epaisseur = sc.Subgraph[SubgraphIndex].LineWidth ;
					COLORREF const couleur = sc.Subgraph[SubgraphIndex].PrimaryColor;
					creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, Niveau, epaisseur, couleur, ptrsc);    // void creationLigneEtTexte (SCString Texte, int beginIndex, int endIndex, float niveau, int monEpaisseur, COLORREF maCouleur, s_sc* ptrsc)
				}
				
				// recalcul immédiat
				sc.UpdateAlways = 1 ;
			}
		}	
	}
	else sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_MesPointsPivotsQuatreHeureProjetes(SCStudyInterfaceRef sc)		// [E]
{
	SCSubgraphRef PP = sc.Subgraph[0];                	// appelé par Navig_Niveaux   // sc.Subgraph[].Arrays[0] appelé par Navig_Niveaux
	SCSubgraphRef R1 = sc.Subgraph[1];                 	// ...
	SCSubgraphRef R2 = sc.Subgraph[2];
	SCSubgraphRef R3 = sc.Subgraph[3];
	SCSubgraphRef R4 = sc.Subgraph[4];
	SCSubgraphRef S1 = sc.Subgraph[5];
	SCSubgraphRef S2 = sc.Subgraph[6];
	SCSubgraphRef S3 = sc.Subgraph[7];
	SCSubgraphRef S4 = sc.Subgraph[8];
	const int NUMBER_OF_STUDY_SUBGRAPHS = 9;  
	
	SCFloatArrayRef FirstIndex = sc.Subgraph[0].Arrays[1] ;  	// Note : sc.Subgraph[0].Arrays[0] est le niveau indice de PP
	SCFloatArrayRef PHPrior = sc.Subgraph[0].Arrays[2] ;  
	SCFloatArrayRef PBPrior = sc.Subgraph[0].Arrays[3] ;  

	
	SCInputRef ExternalChartNumber = sc.Input[0];
	SCInputRef TraceVectorielONOFF = sc.Input[1];		// modifié par Prix_typique 
	
	
	int& NbMonitors = sc.GetPersistentIntFast(0);  
	int& FirstVisibleBar = sc.GetPersistentIntFast(1);  
	int& LastVisibleBar = sc.GetPersistentIntFast(2);  
	int& TopCoordinatePrevious = sc.GetPersistentIntFast(3);  
	int& HoraireDebutTrace = sc.GetPersistentIntFast(4) ;  // time value en secondes
	int& HoraireFinSeance = sc.GetPersistentIntFast(5) ;
	int& StudyIDOverlayINQ100 = sc.GetPersistentIntFast(6) ;
	int& TriggerPopup = sc.GetPersistentIntFast(7) ;
	

	if (sc.SetDefaults)
	{
		sc.AutoLoop = 0;  
		sc.CalculationPrecedence = LOW_PREC_LEVEL  ;  // le SpreadMoyen (Study Overlay INQ100) doit être calculé précédemment
		sc.GraphName = "[E] Pivots Ind 4h projetes sur le future";        // appelé par AMR		// modifié par Prix_typique 
		sc.ScaleRangeType = SCALE_SAMEASREGION;
		sc.ValueFormat = 2;
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 0 ;
		sc.DisplayStudyInputValues = 0;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;	
		sc.HideStudy = 1 ;

		// ExternalChartNumber.Name = "240 Min Chart Number";
		ExternalChartNumber.SetChartNumber(17);
		TraceVectorielONOFF.Name = "Trace Vectoriel ON/OFF";
		TraceVectorielONOFF.SetYesNo(1);
		
		
		for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].DrawStyle = DRAWSTYLE_DASH;
			sc.Subgraph[SubgraphIndex].LineStyle = LINESTYLE_SOLID ;
			sc.Subgraph[SubgraphIndex].PrimaryColor = RGB(182,108,255);  
			sc.Subgraph[SubgraphIndex].LineLabel = LL_DISPLAY_NAME | LL_NAME_REVERSE_COLORS |  LL_NAME_ALIGN_LEFT_EDGE ;  // LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_LEFT_EDGE  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
			sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre
			sc.Subgraph[SubgraphIndex].DrawZeros = false ;
			sc.Subgraph[SubgraphIndex].LineWidth = 2 ;
		}

		
		PP.Name = "PP 4h";
		R1.Name = "R1 4h";
		R2.Name = "R2 4h";
		R3.Name = "R3 4h";
		R4.Name = "R4 4h";
		S1.Name = "S1 4h";
		S2.Name = "S2 4h";
		S3.Name = "S3 4h";
		S4.Name = "S4 4h";
		
		return;
	}  // fin du bloc de " if (sc.SetDefaults) "
	
	
	if ( sc.UpdateStartIndex == 0 )
	{
		sc.GraphRegion = 1 ;
		NbMonitors = NumberOfMonitors() ;
		FirstVisibleBar = 0 ;
		LastVisibleBar = 0 ;
		TriggerPopup = 1 ;
	
		//Initialisations des Niveaux et des NiveauxNonArrondis
		for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex][0] = 0 ;                         //  PP indice + fSpreadMoyen
			sc.Subgraph[SubgraphIndex].Arrays[0][0] = 0 ;          //  PP indice (avant projection)
		}
		
		HoraireDebutTrace = HMS_TIME(13,0,0);   // PP 4h tracé entre 13h et 16h
		HoraireFinSeance = HMS_TIME(16,0,2);
		
		// Récupération de la studyID sur le graphe 16-ticks
		StudyIDOverlayINQ100 = sc.GetStudyIDByName(sc.ChartNumber, "Overlay INQ100 sur le future (avec correction du spread)", 0) ;
		if  (StudyIDOverlayINQ100 == 0 )
		{ SCString message ;  message.Format("!! ERREUR !! Le chart #6 ne contient pas la study ''Overlay INQ100 sur le future (avec correction du spread)''") ; sc.AddMessageToLog(message,1) ; }
	}
	
	
	// on récupère l'array de SpreadMoyen
	SCFloatArray SpreadMoyenArray ;
	sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDOverlayINQ100, 0, 1, SpreadMoyenArray) ;


	for (int index = sc.UpdateStartIndex; index < sc.ArraySize; index++)                 // AutoLoop
	{
		int OpenTimeValue = sc.BaseDateTimeIn[index].GetTimeInSecondsWithoutMilliseconds()  ; //GetTimeInSecondsWithoutMilliseconds retourne une time_value
		int PreviousOpenTimeValue = sc.BaseDateTimeIn[index-1].GetTimeInSecondsWithoutMilliseconds()  ; 
		
		if ( OpenTimeValue >= HoraireDebutTrace  and  OpenTimeValue < HoraireFinSeance )  // n'est actualisé que entre HoraireDebutTrace et HoraireFinSeance
		{
			if  ( OpenTimeValue  !=  PreviousOpenTimeValue )   //  n'est actualisé que s'il y a une nouvelle seconde 
			{ 
				// calcul de fSpreadMoyen
				float fSpreadMoyen = SpreadMoyenArray[index] ;
				// int heure = sc.BaseDateTimeIn[index].GetHour() ;			//  ATTENTION, GetHour retourne l'heure la plus proche après XXh59m59s500ms !!
				int Foo, Hour_Index, Hour_IndexMoinsUn ;
				sc.BaseDateTimeIn[index].GetDateTimeYMDHMS_MS(Foo, Foo, Foo, Hour_Index, Foo, Foo, Foo);
				sc.BaseDateTimeIn[index-1].GetDateTimeYMDHMS_MS(Foo, Foo, Foo, Hour_IndexMoinsUn, Foo, Foo, Foo);

		
				if (  Hour_Index != Hour_IndexMoinsUn  ) 
				{	
					if ( Hour_Index == 16 )    // fin de séance
					{
						FirstIndex[index] = 0 ;
						PHPrior[index] = 0 ;
						PBPrior[index] = 0 ;
						PP[index] = 0;
						R1[index] = 0;
						R2[index] = 0;
						R3[index] = 0;
						R4[index] = 0;
						S1[index] = 0;
						S2[index] = 0;
						S3[index] = 0;
						S4[index] = 0;
					}
					else if ( Hour_Index == 13 )            // les PP1h du INQ100 ne sont calculés qu'à 13h
					{
						// déclaration des PP
						float fPHPrior = 0, fPBPrior = 0 , fSettlePrior = 0, fOpenCurrent = 0, fPP = 0 ;
						float fR1 = 0, fR2 = 0, fR3 = 0, fR4 = 0, fS1 = 0, fS2 = 0, fS3 = 0, fS4 = 0 ;
						float fmR1 = 0, fmR2 = 0, fmR3 = 0, fmR4 = 0, fmS1 = 0, fmS2 = 0,  fmS3 = 0,   fmS4 = 0 ; 
						float fRQ1 = 0, fRQ2 = 0, fRQ3 = 0, fRQ4 = 0, fRQ5 = 0, fRQ6 = 0, fRQ7 = 0, fRQ8 = 0 ;
						float fSQ1 = 0, fSQ2 = 0, fSQ3 = 0, fSQ4 = 0, fSQ5 = 0, fSQ6 = 0, fSQ7 = 0, fSQ8 = 0 ;


						//  "GetChartBaseData" copie intégralement les données source 
						SCGraphData ExternalChartData;
						sc.GetChartBaseData(ExternalChartNumber.GetChartNumber(), ExternalChartData);
						SCDateTimeArray  ExternalChartTimeArray;
						sc.GetChartDateTimeArray(ExternalChartNumber.GetChartNumber(), ExternalChartTimeArray);
						
						// Récupère un index dans le ExternalChart. Il y a deux possibilités pour cet index : 	- cas 1 : il s'agit de la bougie de l'heure précédente
						//																																	- cas 2 : il s'agit de la nouvelle bougie
						int ExternalChartIndex = sc.GetContainingIndexForDateTimeIndex(ExternalChartNumber.GetChartNumber(), index); 
						if ( ExternalChartIndex == 0  and  TriggerPopup ) 
						{ 
							SCString message ;  message.Format("!! WARNING !! Le chart #17 (Indice Nasdaq UT 4h) ne contient pas assez d'historique ( ExternalChartIndex == 0 )") ;  sc.AddMessageToLog(message, 1) ;
							TriggerPopup = 0 ;
						}
						
						// On détermine si ExternalChartIndex et index font partie de la même heure
						int Hour_ExternalIndex ;
						ExternalChartTimeArray[ExternalChartIndex].GetDateTimeYMDHMS_MS(Foo, Foo, Foo, Hour_ExternalIndex, Foo, Foo, Foo);
						if ( Hour_ExternalIndex  ==  Hour_Index )             // cas 2 :  ExternalChartIndex et index font partie de la même heure
						{
							fPHPrior = ExternalChartData[SC_HIGH][ExternalChartIndex-1];                         
							fPBPrior = ExternalChartData[SC_LOW][ExternalChartIndex-1];
							fSettlePrior = ExternalChartData[SC_LAST][ExternalChartIndex-1];
						}
						else      // cas 1 : ExternalChartIndex correspond à la bougie de l'heure précédente
						{
							fPHPrior = ExternalChartData[SC_HIGH][ExternalChartIndex];                         
							fPBPrior = ExternalChartData[SC_LOW][ExternalChartIndex];
							fSettlePrior = ExternalChartData[SC_LAST][ExternalChartIndex];
						}
						
						//on calcule les PP 
						CalculatePPs (fPHPrior, fPBPrior, fSettlePrior, fPP, fR1, fR2, fR3, fR4, fS1, fS2, fS3, fS4, fmR1, fmR2, fmR3, fmR4, fmS1, fmS2, fmS3, fmS4, fRQ1, fRQ2, fRQ3, fRQ4, fRQ5, fRQ6, fRQ7, fRQ8, fSQ1, fSQ2, fSQ3, fSQ4, fSQ5, fSQ6, fSQ7, fSQ8);
						
						// Sauvegarde des niveaux indice
						FirstIndex[index] = index ;
						PHPrior[index] = fPHPrior ;
						PBPrior[index] = fPBPrior ;
						PP.Arrays[0][index] = fPP ;
						R1.Arrays[0][index] = fR1 ;
						R2.Arrays[0][index] = fR2 ;
						R3.Arrays[0][index] = fR3 ;
						R4.Arrays[0][index] = fR4 ;
						S1.Arrays[0][index] = fS1 ;
						S2.Arrays[0][index] = fS2 ;
						S3.Arrays[0][index] = fS3 ;
						S4.Arrays[0][index] = fS4 ;
						
						
						// on rappelle la valeur de chaque niveau indice et on l'adjuste du spread pour obtenir la valeur du niveau projeté
						for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)  
						{ 	
							sc.Subgraph[SubgraphIndex][index] = sc.Subgraph[SubgraphIndex].Arrays[0][index] + fSpreadMoyen ;
						} 
						
					}
					else		// Hour = 14 ou 15
					{
						for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)  {  sc.Subgraph[SubgraphIndex][index] = sc.Subgraph[SubgraphIndex][index-1] ;  sc.Subgraph[SubgraphIndex].Arrays[0][index] = sc.Subgraph[SubgraphIndex].Arrays[0][index-1] ;   }         
						FirstIndex[index] = FirstIndex[index-1] ;
						PHPrior[index] = PHPrior[index-1] ;
						PBPrior[index] = PBPrior[index-1] ;
					}
				}
				else // on n'est pas dans le premier chandelier de l'UT 4h, mais on est dans une nouvelle seconde => on rappelle la valeur de chaque niveau et on l'adjuste du spread
				{
					for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)  {  sc.Subgraph[SubgraphIndex].Arrays[0][index] = sc.Subgraph[SubgraphIndex].Arrays[0][index-1] ;  sc.Subgraph[SubgraphIndex][index] = sc.Subgraph[SubgraphIndex].Arrays[0][index] + fSpreadMoyen ;  } 	 
					FirstIndex[index] = FirstIndex[index-1] ;
					PHPrior[index] = PHPrior[index-1] ;
					PBPrior[index] = PBPrior[index-1] ;
				}
				
			}
			
			else   // la barre ne débute pas dans une nouvelle seconde => on recopie les valeurs de l'index précédent
			{	
				for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)  {  sc.Subgraph[SubgraphIndex][index] = sc.Subgraph[SubgraphIndex][index-1] ;  sc.Subgraph[SubgraphIndex].Arrays[0][index] = sc.Subgraph[SubgraphIndex].Arrays[0][index-1] ;   }         
				FirstIndex[index] = FirstIndex[index-1] ;
				PHPrior[index] = PHPrior[index-1] ;
				PBPrior[index] = PBPrior[index-1] ;
			}  
			
		}
		
		/* else  // le marché action est fermé    
		{  	
			for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)  {  sc.Subgraph[SubgraphIndex][index] = 0 ;  sc.Subgraph[SubgraphIndex].Arrays[0][index] = 0 ;   sc.Subgraph[SubgraphIndex].Arrays[1][index] = 0 ;   }    
			FirstIndex[index] = 0 ;
		}  */

	} // fin de AutoLoop
	
	
	// Bloc d'affichage       
	if ( TraceVectorielONOFF.GetYesNo() )
	{
		if ( sc.ChartWindowIsActive  or  NbMonitors == 2 )
		{
			TopCoordinate = sc.YPixelCoordinateToGraphValue(sc.StudyRegionTopCoordinate) ;
			if (  FirstVisibleBar != sc.IndexOfFirstVisibleBar  or  LastVisibleBar != sc.IndexOfLastVisibleBar  or  sc.IsFullRecalculation  or  fabs(TopCoordinatePrevious-TopCoordinate) > 0.02  )          	// Le recalcul des lignes n'est réalisé que si FirstIndex ou LastVisibleBar ou TopCoordinate varie (on fait l'hypothèse que TopCoordinate et BottomCoordinate varient de manière synchronisée)    // Il faut rajouter sc.IsFullRecalculation car lors d'une Full ReCalculation les useTool non UDD sont effacés
			{	
				FirstVisibleBar = sc.IndexOfFirstVisibleBar ;
				LastVisibleBar = sc.IndexOfLastVisibleBar ;
				BottomCoordinate = sc.YPixelCoordinateToGraphValue(sc.StudyRegionBottomCoordinate) ;
				TopCoordinatePrevious = TopCoordinate ;
						
				// les useTool doivent être effacés à chaque appel du DLL pour éviter une fuite de mémoire   
				sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   // all non user drawn Chart Drawings on the specified ChartNumber will be deleted. The LineNumber parameter in this case is ignored and should be 0. 		
				
				s_sc* const ptrsc = &sc;
				int const firstIndex = FirstIndex[LastVisibleBar] ;
				int const epaisseur = 2 ;
				COLORREF const couleur = RGB(255,128,0) ;

				for (int SubgraphIndex = 0 ; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS ; SubgraphIndex++)
				{
					float const Niveau = sc.Subgraph[SubgraphIndex][LastVisibleBar] ;    // la ligne vectorielle est tracée sur une ordonnée prise à LastVisibleBar
					SCString const Texte = sc.Subgraph[SubgraphIndex].Name ;
					creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, Niveau, epaisseur, couleur, ptrsc);    // void creationLigneEtTexte (SCString Texte, int beginIndex, int endIndex, float niveau, int monEpaisseur, COLORREF maCouleur, s_sc* ptrsc)
				}
				
				// recalcul immédiat
				sc.UpdateAlways = 1 ;
			}
		}
	}
	else sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_MesPointsPivotsUneHeureProjetes(SCStudyInterfaceRef sc)		// [F]
{
	SCSubgraphRef PP = sc.Subgraph[0];                	// appelé par Navig_Niveaux   // sc.Subgraph[].Arrays[0] appelé par Navig_Niveaux
	SCSubgraphRef R1 = sc.Subgraph[1];                 	// ...
	SCSubgraphRef R2 = sc.Subgraph[2];
	SCSubgraphRef R3 = sc.Subgraph[3];
	SCSubgraphRef R4 = sc.Subgraph[4];
	SCSubgraphRef S1 = sc.Subgraph[5];
	SCSubgraphRef S2 = sc.Subgraph[6];
	SCSubgraphRef S3 = sc.Subgraph[7];
	SCSubgraphRef S4 = sc.Subgraph[8];
	const int NUMBER_OF_STUDY_SUBGRAPHS = 9;  
	
	SCFloatArrayRef FirstIndex = sc.Subgraph[0].Arrays[1] ;  	// Note : sc.Subgraph[0].Arrays[0] est le niveau indice de PP
	SCFloatArrayRef PHPrior = sc.Subgraph[0].Arrays[2] ;  
	SCFloatArrayRef PBPrior = sc.Subgraph[0].Arrays[3] ;  

	
	SCInputRef ExternalChartNumber = sc.Input[0];
	SCInputRef TraceVectorielONOFF = sc.Input[1];		// modifié par Prix Typique
	
	
	int& NbMonitors = sc.GetPersistentIntFast(0);  
	int& FirstVisibleBar = sc.GetPersistentIntFast(1);  
	int& LastVisibleBar = sc.GetPersistentIntFast(2);  
	int& TopCoordinatePrevious = sc.GetPersistentIntFast(3);  
	int& HoraireDebutTrace = sc.GetPersistentIntFast(4) ;  // time value en secondes
	int& HoraireFinSeance = sc.GetPersistentIntFast(5) ;
	int& StudyIDOverlayINQ100 = sc.GetPersistentIntFast(6) ;
	int& TriggerPopup = sc.GetPersistentIntFast(7) ;

	

	if (sc.SetDefaults)
	{
		sc.AutoLoop = 0;  
		sc.CalculationPrecedence = LOW_PREC_LEVEL  ;  // le SpreadMoyen (Study Overlay INQ100) doit être calculé précédemment
		sc.GraphName = "[F] Pivots Ind 1h projetes sur le future";        // appelé par AMR		// modifié par Prix_typique 
		sc.ScaleRangeType = SCALE_SAMEASREGION;
		sc.ValueFormat = 2;
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 0 ;
		sc.DisplayStudyInputValues = 0;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;	
		sc.HideStudy = 1 ;

		// ExternalChartNumber.Name = "60 Min Chart Number";
		ExternalChartNumber.SetChartNumber(16);
		TraceVectorielONOFF.Name = "Trace Vectoriel ON/OFF";
		TraceVectorielONOFF.SetYesNo(1);
		
		
		for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].DrawStyle = DRAWSTYLE_DASH;
			sc.Subgraph[SubgraphIndex].LineStyle = LINESTYLE_SOLID ;
			sc.Subgraph[SubgraphIndex].PrimaryColor = RGB(182,108,255);  
			sc.Subgraph[SubgraphIndex].LineLabel = LL_DISPLAY_NAME | LL_NAME_REVERSE_COLORS |  LL_NAME_ALIGN_LEFT_EDGE ;  // LL_DISPLAY_NAME | LL_DISPLAY_VALUE | LL_NAME_ALIGN_LEFT_EDGE  | LL_VALUE_ALIGN_CENTER | LL_NAME_REVERSE_COLORS | LL_VALUE_ALIGN_VALUES_SCALE;  // affiche le nom de l'indicateur retourné et sa valeur
			sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false ;  // évite l'affichage des valeurs retournées dans la fenêtre
			sc.Subgraph[SubgraphIndex].DrawZeros = false ;
			sc.Subgraph[SubgraphIndex].LineWidth = 2 ;
		}

		
		PP.Name = "PP 1h";
		R1.Name = "R1 1h";
		R2.Name = "R2 1h";
		R3.Name = "R3 1h";
		R4.Name = "R4 1h";
		S1.Name = "S1 1h";
		S2.Name = "S2 1h";
		S3.Name = "S3 1h";
		S4.Name = "S4 1h";
		
		return;
	}  // fin du bloc de " if (sc.SetDefaults) "
	
	
	if ( sc.UpdateStartIndex == 0 )
	{
		sc.GraphRegion = 1 ;
		NbMonitors = NumberOfMonitors() ;
		FirstVisibleBar = 0 ;
		LastVisibleBar = 0 ;
		TriggerPopup = 1 ;
	
		//Initialisations des Niveaux et des NiveauxNonArrondis
		for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex][0] = 0 ;                         //  PP indice + fSpreadMoyen
			sc.Subgraph[SubgraphIndex].Arrays[0][0] = 0 ;          //  PP indice (avant projection)
		}
		
		HoraireDebutTrace = HMS_TIME(10,0,0);   // PP 1h tracé entre 10h et 16h
		HoraireFinSeance = HMS_TIME(16,0,0);
		
		// Récupération de la studyID sur le graphe 16-ticks
		StudyIDOverlayINQ100 = sc.GetStudyIDByName(sc.ChartNumber, "Overlay INQ100 sur le future (avec correction du spread)", 0) ;
		if  (StudyIDOverlayINQ100 == 0 )
		{ SCString message ;  message.Format("!! ERREUR !! Le chart #6 ne contient pas la study ''Overlay INQ100 sur le future (avec correction du spread)''") ; sc.AddMessageToLog(message,1) ; }
	}
	
	
	// on récupère l'array de SpreadMoyen
	SCFloatArray SpreadMoyenArray ;
	sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDOverlayINQ100, 0, 1, SpreadMoyenArray) ;


	for (int index = sc.UpdateStartIndex; index < sc.ArraySize; index++)                 // AutoLoop
	{
		int OpenTimeValue = sc.BaseDateTimeIn[index].GetTimeInSecondsWithoutMilliseconds()  ; //GetTimeInSecondsWithoutMilliseconds retourne une time_value
		int PreviousOpenTimeValue = sc.BaseDateTimeIn[index-1].GetTimeInSecondsWithoutMilliseconds()  ; 
		
		if ( OpenTimeValue >= HoraireDebutTrace  and  OpenTimeValue < HoraireFinSeance )  // n'est actualisé que entre HoraireDebutTrace et HoraireFinSeance
		{
			if  ( OpenTimeValue  !=  PreviousOpenTimeValue )   //  n'est actualisé que s'il y a une nouvelle seconde 
			{ 
				// calcul de fSpreadMoyen
				float fSpreadMoyen = SpreadMoyenArray[index] ;
				// int heure = sc.BaseDateTimeIn[index].GetHour() ;		//  ATTENTION, GetHour retourne l'heure la plus proche après XXh59m59s500ms !!
				int Foo, Hour_Index, Hour_IndexMoinsUn ;
				sc.BaseDateTimeIn[index].GetDateTimeYMDHMS_MS(Foo, Foo, Foo, Hour_Index, Foo, Foo, Foo);
				sc.BaseDateTimeIn[index-1].GetDateTimeYMDHMS_MS(Foo, Foo, Foo, Hour_IndexMoinsUn, Foo, Foo, Foo);

		
				if (  Hour_Index != Hour_IndexMoinsUn  ) 
				{	
					if ( Hour_Index == 16 )    // fin de séance
					{
						FirstIndex[index] = 0 ;
						PHPrior[index] = 0 ;
						PBPrior[index] = 0 ;
						PP[index] = 0;
						R1[index] = 0;
						R2[index] = 0;
						R3[index] = 0;
						R4[index] = 0;
						S1[index] = 0;
						S2[index] = 0;
						S3[index] = 0;
						S4[index] = 0;
					}
					else            // les PP1h du INQ100 sont calculés à chaque changement d'heure (à partir de 10h donc)
					{
						// déclaration des PP
						float fPHPrior = 0, fPBPrior = 0 , fSettlePrior = 0, fOpenCurrent = 0, fPP = 0 ;
						float fR1 = 0, fR2 = 0, fR3 = 0, fR4 = 0, fS1 = 0, fS2 = 0, fS3 = 0, fS4 = 0 ;
						float fmR1 = 0, fmR2 = 0, fmR3 = 0, fmR4 = 0, fmS1 = 0, fmS2 = 0,  fmS3 = 0,   fmS4 = 0 ; 
						float fRQ1 = 0, fRQ2 = 0, fRQ3 = 0, fRQ4 = 0, fRQ5 = 0, fRQ6 = 0, fRQ7 = 0, fRQ8 = 0 ;
						float fSQ1 = 0, fSQ2 = 0, fSQ3 = 0, fSQ4 = 0, fSQ5 = 0, fSQ6 = 0, fSQ7 = 0, fSQ8 = 0 ;


						//  "GetChartBaseData" copie intégralement les données source 
						SCGraphData ExternalChartData;
						sc.GetChartBaseData(ExternalChartNumber.GetChartNumber(), ExternalChartData);
						SCDateTimeArray  ExternalChartTimeArray;
						sc.GetChartDateTimeArray(ExternalChartNumber.GetChartNumber(), ExternalChartTimeArray);
						
						// Récupère un index dans le ExternalChart. Il y a deux possibilités pour cet index : 	- cas 1 : il s'agit de la bougie de l'heure précédente
						//																																	- cas 2 : il s'agit de la nouvelle bougie
						int ExternalChartIndex = sc.GetContainingIndexForDateTimeIndex(ExternalChartNumber.GetChartNumber(), index); 
						if ( ExternalChartIndex == 0  and  TriggerPopup ) 
						{ 
							SCString message ;  message.Format("!! WARNING !! Le chart #16 (Indice Nasdaq UT 1h) ne contient pas assez d'historique ( ExternalChartIndex == 0 )") ;  sc.AddMessageToLog(message, 1) ;
							TriggerPopup = 0 ;
						}
						
						// On détermine si ExternalChartIndex et index font partie de la même heure
						int Hour_ExternalIndex ;
						ExternalChartTimeArray[ExternalChartIndex].GetDateTimeYMDHMS_MS(Foo, Foo, Foo, Hour_ExternalIndex, Foo, Foo, Foo);
						if ( Hour_ExternalIndex  ==  Hour_Index )             // cas 2 :  ExternalChartIndex et index font partie de la même heure
						{
							fPHPrior = ExternalChartData[SC_HIGH][ExternalChartIndex-1];                         
							fPBPrior = ExternalChartData[SC_LOW][ExternalChartIndex-1];
							fSettlePrior = ExternalChartData[SC_LAST][ExternalChartIndex-1];
						}
						else      // cas 1 : ExternalChartIndex correspond à la bougie de l'heure précédente
						{
							fPHPrior = ExternalChartData[SC_HIGH][ExternalChartIndex];                         
							fPBPrior = ExternalChartData[SC_LOW][ExternalChartIndex];
							fSettlePrior = ExternalChartData[SC_LAST][ExternalChartIndex];
						}
						
						//on calcule les PP 
						CalculatePPs (fPHPrior, fPBPrior, fSettlePrior, fPP, fR1, fR2, fR3, fR4, fS1, fS2, fS3, fS4, fmR1, fmR2, fmR3, fmR4, fmS1, fmS2, fmS3, fmS4, fRQ1, fRQ2, fRQ3, fRQ4, fRQ5, fRQ6, fRQ7, fRQ8, fSQ1, fSQ2, fSQ3, fSQ4, fSQ5, fSQ6, fSQ7, fSQ8);
						
						// Sauvegarde des niveaux indice
						FirstIndex[index] = index ;
						PHPrior[index] = fPHPrior ;
						PBPrior[index] = fPBPrior ;
						PP.Arrays[0][index] = fPP ;
						R1.Arrays[0][index] = fR1 ;
						R2.Arrays[0][index] = fR2 ;
						R3.Arrays[0][index] = fR3 ;
						R4.Arrays[0][index] = fR4 ;
						S1.Arrays[0][index] = fS1 ;
						S2.Arrays[0][index] = fS2 ;
						S3.Arrays[0][index] = fS3 ;
						S4.Arrays[0][index] = fS4 ;
						
						
						// on rappelle la valeur de chaque niveau indice et on l'adjuste du spread pour obtenir la valeur du niveau projeté
						for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)  
						{ 	
							sc.Subgraph[SubgraphIndex][index] = sc.Subgraph[SubgraphIndex].Arrays[0][index] + fSpreadMoyen ;
						} 
						
					}
				}
				else // on n'est pas dans le premier chandelier de l'heure, mais on est dans une nouvelle seconde => on rappelle la valeur de chaque niveau et on l'adjuste du spread
				{
					for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)  {  sc.Subgraph[SubgraphIndex].Arrays[0][index] = sc.Subgraph[SubgraphIndex].Arrays[0][index-1] ;  sc.Subgraph[SubgraphIndex][index] = sc.Subgraph[SubgraphIndex].Arrays[0][index] + fSpreadMoyen ;  } 	 
					FirstIndex[index] = FirstIndex[index-1] ;
					PHPrior[index] = PHPrior[index-1] ;
					PBPrior[index] = PBPrior[index-1] ;
				}
				
			}
			
			else   // la barre ne débute pas dans une nouvelle seconde => on recopie les valeurs de l'index précédent
			{	
				for (int SubgraphIndex = 0; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS; SubgraphIndex++)  {  sc.Subgraph[SubgraphIndex][index] = sc.Subgraph[SubgraphIndex][index-1] ;  sc.Subgraph[SubgraphIndex].Arrays[0][index] = sc.Subgraph[SubgraphIndex].Arrays[0][index-1] ;   }         
				FirstIndex[index] = FirstIndex[index-1] ;
				PHPrior[index] = PHPrior[index-1] ;
				PBPrior[index] = PBPrior[index-1] ;
			}  
			
		}
		
	} // fin de AutoLoop
	

	// Bloc d'affichage       
	if ( TraceVectorielONOFF.GetYesNo() )
	{
		if ( sc.ChartWindowIsActive  or  NbMonitors == 2 )
		{
			TopCoordinate = sc.YPixelCoordinateToGraphValue(sc.StudyRegionTopCoordinate) ;
			if (  FirstVisibleBar != sc.IndexOfFirstVisibleBar  or  LastVisibleBar != sc.IndexOfLastVisibleBar  or  sc.IsFullRecalculation  or  fabs(TopCoordinatePrevious-TopCoordinate) > 0.02  )          	// Le recalcul des lignes n'est réalisé que si FirstIndex ou LastVisibleBar ou TopCoordinate varie (on fait l'hypothèse que TopCoordinate et BottomCoordinate varient de manière synchronisée)    // Il faut rajouter sc.IsFullRecalculation car lors d'une Full ReCalculation les useTool non UDD sont effacés
			{	
				FirstVisibleBar = sc.IndexOfFirstVisibleBar ;
				LastVisibleBar = sc.IndexOfLastVisibleBar ;
				BottomCoordinate = sc.YPixelCoordinateToGraphValue(sc.StudyRegionBottomCoordinate) ;
				TopCoordinatePrevious = TopCoordinate ;
				
				// les useTool doivent être effacés à chaque appel du DLL pour éviter une fuite de mémoire   
				sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   // all non user drawn Chart Drawings on the specified ChartNumber will be deleted. The LineNumber parameter in this case is ignored and should be 0. 		
				
				s_sc* const ptrsc = &sc;
				int const firstIndex = FirstIndex[LastVisibleBar] ;
				int const epaisseur = 2 ;
				COLORREF const couleur = RGB(182,108,255) ;

				for (int SubgraphIndex = 0 ; SubgraphIndex < NUMBER_OF_STUDY_SUBGRAPHS ; SubgraphIndex++)
				{
					float const Niveau = sc.Subgraph[SubgraphIndex][LastVisibleBar] ;    // la ligne vectorielle est tracée sur une ordonnée prise à LastVisibleBar
					SCString const Texte = sc.Subgraph[SubgraphIndex].Name ;
					creationLigneEtTexte(Texte, firstIndex, LastVisibleBar, Niveau, epaisseur, couleur, ptrsc);    // void creationLigneEtTexte (SCString Texte, int beginIndex, int endIndex, float niveau, int monEpaisseur, COLORREF maCouleur, s_sc* ptrsc)
				}
				
				// recalcul immédiat
				sc.UpdateAlways = 1 ;
			}
		}
	}		
	else sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   

}

