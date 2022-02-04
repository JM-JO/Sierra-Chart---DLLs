/*
v0.0 (2019/7/19) : création de la structure StructureNiveau.
v0.1 (2019/7/21) : le tableau de StructureNiveau appelé Niveau est rempli avec les PP1h et les PP 4h.
v0.2 (2019/7/21) : création de operator> pour StructureNiveau.
v0.3 (2019/7/21) : le tableau de Niveau est déclaré via la classe array de la STL. La fonction std::sort associée à ma fonction "InferieurA_PrixSurFuture" permet de trier le tableau Niveau. 
v0.4 (2019/7/22) : ajout des PP J Indice. Définition de Iterateur_PremiereLigneNonNulle.
v0.5 (2019/7/22) : ajout des PP indice : H et M. Ajout des niveaux ronds indice.
v0.6 (2019/7/22) : calcul de NiveauLePlusProcheDeNiveauCibleActif.
v0.7 (2019/7/23) : la liste des inputs est mise à jour.
v0.8 (2019/7/23) : on bascule sc.CalculationPrecedence de VERY_LOW_PREC_LEVEL à LOW_PREC_LEVEL, afin que la study soit calculée avant AMR. On met un message dans le log si elle est calculée avant les PP future, les PP indice projetés, les niveaux ronds projetés.
v0.9 (2019/7/23) : on détermine NiveauxFutureONOFF et NiveauxIndiceONOFF.
v1.0 (2019/7/23) : résolution du problème de la navigation qui cale sur un niveau.
v1.1 (2019/7/24) : génération de fichier .csv contenant les niveaux indice (fonctionne ; reste à résoudre la conversion des '.' en ',' ).
v1.1.1 (2019/7/25) : conversion des des '.' en ',' .
v1.2.0 (2019/7/25) : pour la génération des niveaux indice, on crée un tableau NiveauIndice contenant uniquement les niveaux de l'indice et utilisant les niveaux non projetés. 
v1.3.0 (2019/8/20) : pour la navigation entre niveaux, l'array Niveau est créé avec " index = sc.IndexOfLastVisibleBar" (vs " index = sc.ArraySize-1 " précédemment).
v1.3.1 (2019/9/22) : ajout de l'input NatureNiveau_ApresClic, et du membre NatureNiveau dans la struct StructureNiveau.
v1.4.0 (2019/10/23) : j'ai mis en commentaire les blocs de code qui génèrent les Eighths mensuels et les Quarts hebdos dans le fichier csv. Lorsque je les retraderai, il faudra supprimer les commentaires.
v1.5.0 (2019/10/29) : les valeurs des niveaux indice projetés sur le future ne sont plus prélevées dans l'Array des valeurs arrondies, mais dans l'Array des valeurs exactes.
v1.5.1 (2019/11/7) : ajout d'un bouton pour générer le fichier .csv. Le raccourci est F8.
v1.6.0 (2019/11/7) : ajout des dizaines (+20, +10, floor, -10).
v1.6.1 (2019/11/12) : ajout des niveaux indice :  PP 1h et PP 4h.
v1.6.2 (2019/11/12) : les studyID sont déterminées dans le bloc "if ( sc.UpdateStartIndex == 0 )" ; ajout d'un message d'érreur spécifique pour la Study manquante (à la place d'une erreur générique).
v1.6.3 (2019/11/12) : lorsqu'une study a son input TraceVectoriel = OFF, alors on ne récupère pas sa valeur afin de ne pas naviguer dessus. Ajout des 25/75.
v1.6.4 (2020/02/03) : quand le niveau de départ et d'arrivée sont distants de moins de 0.25pt (sur future) vs 0.125pt précédemment => on considère qu'on s'est enlisé => on saute au niveau suivant.
v1.6.5 (2020/03/17) : suppression de la récupération des niveaux ronds via les SCSubgraphRef.
v1.7.0 (2020/05/09) : ajout des niveaux PHJ/PBJ.
v1.8.0 (2020/06/24) : on remplace GetTimeWithoutMilliseconds par GetTimeInSecondsWithoutMilliseconds ; augmentation de NbNiveaux.
v1.9.0 (2020/07/09) : modif des numéros des PH/PB H et M. Ajout des OpenGap et DemiGap.
v1.9.1 (2020/07/18) : lors d'un changement de StudyAMRActive, on transmet le PrixSurindice_ApresClic aux studies "Padding et boutons ACS" pour le recalcul du range affiché.
v1.9.2 (2020/07/21) : modifications pour être raccord avec Padding v2.5.
v1.9.3 (2020/07/23) : les niveaux suivants sont ajoutés dans StructureNiveau seulement si ils sont affichés : niveaux 50, niveaux moyens, PP 1h, PP 4h.
v1.9.4 (2020/07/31) : les niveaux suivants sont ajoutés dans StructureNiveau seulement si ils sont affichés : Mid4/Full4.
v1.9.5 (2020/08/04) : ajouts d'inputs pour controler les niveaux affichés (même inputs que ceux transmis par PrixTypique).
v1.9.6 (2020/08/05) : les niveaux suivants sont ajoutés dans StructureNiveau seulement si ils sont affichés : PHPB J/H/M précédents et Close JP.
v2.0 (2020/09/28) : introduction de SousNatureNiveau. Utilisé pour les niveaux ronds.
v2.1 (2020/09/30) : en présence d'un Gap J, le niveau PH ou PB est remplacé par le Gap.
v2.2 (2020/10/05) : introduction de RangePrecedent dans StructureNiveau.
v2.3 (2020/10/07) : lorsque External_Input_ACS_BUTTON_109 ou External_Input_ACS_BUTTON_109 (Niv+ / Niv-) est supérieur à 1, on navigue de plus de 1 niveau d'un coup.


Améliorations possibles : 
	- supprimer les IDU (inutiles) et employer des conteneurs "vector". Plus simple et rapide : incrémenter les IDU (" IDU ++ ; ").  Note : pour faire un find and replace de tous les IDU concernés : utiliser la regex " IDU = . ; " dans Notepad.
	- pour être certain que cette study est calculée après les autres studies du chart #6, faire un test dans " if ( sc.UpdateStartIndex == 0 ) " pour s'assurer que la study est calculée en dernier.
	- remettre à plat la navigation entre niveaux : après 1 clic, on change de niveau (on ne s'embourbe plus) grace à l'IDU. Il faudrait que l'itérateur Iterateur_NiveauLePlusProcheDeNiveauCibleActif soit remplacé par un itérateur qui correspond au niveau cible actif (grace à l'IDU). Ca nécessite d'introduire un input " IDU_ApresClic ", et nécessite de conserver Iterateur_NiveauLePlusProcheDeNiveauCibleActif pour le cas où IDU_ApresClic==0 (changement de study active).
	- il manque les PP J/H/M du future, ses VWAPs et ses niveaux ronds. Les IDU entre 18 et 300 sont disponibles pour les niveaux du future.
*/


#include "sierrachart.h"  // When including in your source code file additional header files, they need to be placed above the #include "sierrachart.h" line. 

SCDLLName("Navigation niveaux")

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct StructureNiveau
{
	// variables membres :
	int IdentifiantUnique ;
	SCString Nom ;
	float PrixSurFuture ;
	float PrixSurIndice ;
	int Nature ;                	// Nature = 0 (psy) / 1 (1h) / 2 (4h) / 3 (jour) / 4 (hebdo) / 5 (mensuel) / 6 (PHJ PBJ)
	int SousNature ;			// Full/Mid/Quart.... 10/20/50/100/125/...
	float RangePrecedent ;
	
	// Constructeur : en fait il ne sert pas, car les instances sont générées lors la création d'un tableau
	StructureNiveau (int IdentifiantUnique_Input, const char* Nom_Input, float PrixSurFuture_Input, float PrixSurindice_Input)
	{
		IdentifiantUnique = IdentifiantUnique_Input ;
		Nom.Format(Nom_Input) ;
		PrixSurFuture = PrixSurFuture_Input ;
		PrixSurIndice = PrixSurindice_Input ;
		Nature = -1 ;
		SousNature = -1 ;
		RangePrecedent = -1 ;
	}
	
	// Constructeur par défaut : indispensable car les instances sont générées lors la création d'un tableau
	StructureNiveau()
	{
		IdentifiantUnique = -1 ;
		Nom.Format("vide") ;
		PrixSurFuture = 0 ;
		PrixSurIndice = 0 ;
		Nature = -1 ;
		SousNature = -1 ;
		RangePrecedent = -1 ;
	}
	
	// Opérateur >
	bool operator> (const StructureNiveau& NiveauB)      // le test " NiveauA > NiveauB " sera interprété ainsi : " NiveauA.operator>(NiveauB) "     // cet opérateur est inutile : la bibliothèque <algorithm> nécessite des prédicats déclarés en dehors de la classe
	{
		if ( PrixSurFuture > NiveauB.PrixSurFuture )  { return 1 ; }
		else { return 0 ; }
	}
	
} ;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool InferieurA_PrixSurFuture (StructureNiveau NiveauA, StructureNiveau NiveauB)    // fonction " inférieur à "
{
	if ( NiveauA.PrixSurFuture < NiveauB.PrixSurFuture )  { return 1 ; }
	else { return 0 ; }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool InferieurA_PrixSurIndice (StructureNiveau NiveauA, StructureNiveau NiveauB)    // fonction " inférieur à "
{
	if ( NiveauA.PrixSurIndice < NiveauB.PrixSurIndice )  { return 1 ; }
	else { return 0 ; }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool PrixSurFutureEstNonNul (StructureNiveau Niveau)   // teste si le membre PrixSurFuture est différent de zéro
{
	if ( Niveau.PrixSurFuture > 500 )  { return 1 ; }   // on retient " > 500 " plutôt que " == 0 "  pour filtrer tous les prix proches de 0 (demigap les jours sans demigap, etc...)
	else { return 0 ; }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool PrixSurIndiceEstNonNul (StructureNiveau Niveau)   // teste si le membre PrixSurIndice est différent de zéro
{
	if ( Niveau.PrixSurIndice > 500 )  { return 1 ; }   // on retient " > 500 " plutôt que " == 0 "  pour filtrer tous les prix proches de 0 (demigap les jours sans demigap, etc...)
	else { return 0 ; }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int GetActiveStudyID(s_sc* ptrsc)
{
	int ActiveStudyID = 0 ;
	int StudyIDPrixTypique = ptrsc->GetStudyIDByName(ptrsc->ChartNumber, "Prix typique", 0);
	if  (StudyIDPrixTypique == 0 )  
	{	SCString message ;  message.Format("ID:%d | !! ERREUR !! Le graphique ne contient pas la study ''Prix typique''. Necessaire pour determiner ''CetteInstanceControleLesBoutons''.", ptrsc->StudyGraphInstanceID) ; ptrsc->AddMessageToLog(message,1) ; }
	else
	{	ptrsc->GetChartStudyInputInt(ptrsc->ChartNumber, StudyIDPrixTypique,1, ActiveStudyID) ;  }    // int GetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int& r_IntegerValue);
	return ActiveStudyID ;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

double NiveauFutureDeLaStudyActive (s_sc* ptrsc)
{
	int ActiveStudyID = GetActiveStudyID(ptrsc) ;
	double NiveauFuture;
	ptrsc->GetChartStudyInputFloat(ptrsc->ChartNumber, ActiveStudyID, 7, NiveauFuture) ;
	return NiveauFuture;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int CalculSousNiveau(int PrixSurIndice)		// conversion de PrixSurIndice en int pour permettre l'opérateur modulo (%)
{
	int SousNiveau ;
			if ( PrixSurIndice % 1000 == 0 )	SousNiveau = 1000 ;			// pour les niveaux spéciaux, ajouter un test avant le test " if ( PrixSurIndice % 1000 == 0 ) "
	else	if ( PrixSurIndice % 500 == 0 )	SousNiveau = 500 ;
	else	if ( ( (PrixSurIndice+1) *3) % 1000 < 4 )	SousNiveau = 333 ;
	else	if ( PrixSurIndice % 250 == 0 )	SousNiveau = 250 ;
	else	if ( PrixSurIndice % 125 == 0 )	SousNiveau = 125 ;
	else	if ( PrixSurIndice % 100 == 0 )	SousNiveau = 100 ;
	else	if ( PrixSurIndice % 50 == 0 )		SousNiveau = 50 ;
	else	if ( PrixSurIndice % 25 == 0 )		SousNiveau = 25 ;
	else	if ( PrixSurIndice % 20 == 0 )		SousNiveau = 20 ;
	else	if ( PrixSurIndice % 10 == 0 )		SousNiveau = 10 ;
	else														SousNiveau = -8 ;
	
	return SousNiveau ;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float NiveauCibleActif;  // cette variable doit être définie en dehors de la fonction scsf_NavigationNiveaux et définie avant le prédicat "EstPlusProcheQue"

bool EstPlusProcheQue (StructureNiveau NiveauA, StructureNiveau NiveauB)    // prédicat " A est plus proche que B de NiveauCibleActif "
{
	if ( fabs(NiveauA.PrixSurFuture-NiveauCibleActif) < fabs(NiveauB.PrixSurFuture-NiveauCibleActif)  )  { return 1 ; }
	else { return 0 ; }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_NavigationNiveaux(SCStudyInterfaceRef sc)
{
	// Liste des inputs
	SCInputRef 	RangePrecedent_ApresClic = sc.Input[0]; 			// appelé par AMR
	SCInputRef 	IdentifiantUnique_ApresClic = sc.Input[1]; 			// appelé par AMR
	SCInputRef 	Nom_ApresClic = sc.Input[2];                         		// appelé par AMR
	SCInputRef 	PrixSurFuture_ApresClic = sc.Input[3];           	// appelé par AMR
	SCInputRef 	PrixSurindice_ApresClic = sc.Input[4];            	// appelé par AMR
	SCInputRef 	Nature_ApresClic = sc.Input[5];            				// appelé par AMR
	SCInputRef 	SousNature_ApresClic = sc.Input[6];            		// appelé par AMR
	SCInputRef 	GenerationFichier_csv = sc.Input[7];         
	SCInputRef	ChartInputs_ExternalArray = sc.Input[8]; 	
	SCInputRef	External_Input_ACS_BUTTON_109 = sc.Input[9];   									// numéro appelé par la study "Padding et boutons ACS"        		// "Niv -" 
	SCInputRef	External_Input_ACS_BUTTON_110 = sc.Input[10];   								// numéro appelé par la study "Padding et boutons ACS"        		// "Niv +"
	SCInputRef 	NiveauxIndices_10_Input = sc.Input[11];														// modifié par Prix_Typique
	SCInputRef 	NiveauxIndices_25_Input = sc.Input[12];														// modifié par Prix_Typique
	SCInputRef 	NiveauxIndices_50_Input = sc.Input[13];														// modifié par Prix_Typique
	SCInputRef 	NiveauxIndices_moyens_Input = sc.Input[14];											// modifié par Prix_Typique
	SCInputRef 	NiveauxIndicesAffiches_Input = sc.Input[20]; 												// modifié par Prix_Typique					// vaut 0 (OFF), 1 (ALL), 2 (Custom), 3 (trades), ou 4 (BT)		
	SCInputRef 	NiveauxIndices_PP1h_Input = sc.Input[21];												// modifié par Prix_Typique
	SCInputRef 	NiveauxIndices_PP4h_Input = sc.Input[22];												// modifié par Prix_Typique
	SCInputRef 	NiveauxIndices_PPQuartsH_Input = sc.Input[23];										// modifié par Prix_Typique
	SCInputRef 	NiveauxIndices_PPEighthsM_Input = sc.Input[24];										// modifié par Prix_Typique
	SCInputRef 	NiveauxIndices_PPQuartsM_Input = sc.Input[25];										// modifié par Prix_Typique
	SCInputRef 	NiveauxIndices_Full4Mid4_Input = sc.Input[26];											// modifié par Prix_Typique
	SCInputRef 	NiveauxIndices_PHPB_JHM_Precedent_Input = sc.Input[27];					// modifié par Prix_Typique
	SCInputRef 	NiveauxIndices_PHJPBJ_Input = sc.Input[28];											// modifié par Prix_Typique
	
	
	int& StudyID_PrixTypique = sc.GetPersistentIntFast(0) ;
	int& StudyID_OverlayINQ100 = sc.GetPersistentIntFast(1) ;
	int& StudyID_PivotsFut4h = sc.GetPersistentIntFast(2) ;
	int& StudyID_PivotsFut1h = sc.GetPersistentIntFast(3) ;
	int& StudyID_PivotsIndM = sc.GetPersistentIntFast(11) ;
	int& StudyID_PivotsIndM_Eighth = sc.GetPersistentIntFast(12) ;
	int& StudyID_PivotsIndH = sc.GetPersistentIntFast(13) ;
	int& StudyID_PivotsIndJ = sc.GetPersistentIntFast(14) ;
	int& StudyID_PivotsInd4h = sc.GetPersistentIntFast(15) ;
	int& StudyID_PivotsInd1h = sc.GetPersistentIntFast(16) ;
	int& StudyID_PHJPBJ = sc.GetPersistentIntFast(17) ;
	int& StudyID_NiveauxPsy = sc.GetPersistentIntFast(21) ;

	int const NbNiveaux = 700 ; // nb de lignes du tableau Niveau
	
	
	if (sc.SetDefaults)
	{
		sc.GraphRegion = 2;	
		sc.AutoLoop = 0;  
		sc.CalculationPrecedence = LOW_PREC_LEVEL  ;  //  Attention car les points pivots projetés doivent être calculés avant lui, sinon GetStudyArrayUsingID pour récupérer les PP ne fonctionne pas.    // les PP projetés sont LOW_PREC_LEVEL
		sc.GraphName = "Navigation niveaux" ;               // appelé par AMR et par Padding
		sc.ScaleRangeType = SCALE_SAMEASREGION;
		sc.GraphRegion = 0;
		//sc.ValueFormat = 2;
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 0 ;
		sc.DisplayStudyInputValues = 0;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		
		// Liste des inputs
		RangePrecedent_ApresClic.Name = "Range Precedent (pts)";
		RangePrecedent_ApresClic.SetFloat(-1);
		IdentifiantUnique_ApresClic.Name = "IdentifiantUnique";
		IdentifiantUnique_ApresClic.SetInt(-1);
		Nom_ApresClic.Name = "Nom";
		Nom_ApresClic.SetString("n.d.");
		PrixSurFuture_ApresClic.Name = "PrixSurFuture";
		PrixSurFuture_ApresClic.SetFloat(-1);
		PrixSurindice_ApresClic.Name = "PrixSurIndice";
		PrixSurindice_ApresClic.SetFloat(-1);
		Nature_ApresClic.Name = "NatureNiveau";
		Nature_ApresClic.SetInt(-1);
		SousNature_ApresClic.Name = "SousNatureNiveau";
		SousNature_ApresClic.SetInt(-1);
		GenerationFichier_csv.Name = "Generation Fichier .csv";
		GenerationFichier_csv.SetYesNo(0);
		// External_Input_ACS_BUTTON_109.Name = "External_Input_ACS_BUTTON_109" ;
		External_Input_ACS_BUTTON_109.SetInt(0) ;    //  "Niv -"
		// External_Input_ACS_BUTTON_110.Name = "External_Input_ACS_BUTTON_110" ;
		External_Input_ACS_BUTTON_110.SetInt(0) ;    //  "Niv -"
		NiveauxIndicesAffiches_Input.Name = "NiveauxIndicesAffiches_Input";
		NiveauxIndicesAffiches_Input.SetInt(2);  // 2 == Custom
		NiveauxIndices_10_Input.Name = "Affichage des niveaux 10";
		NiveauxIndices_10_Input.SetYesNo(0);
		NiveauxIndices_25_Input.Name = "Affichage des niveaux 25";
		NiveauxIndices_25_Input.SetYesNo(0);
		NiveauxIndices_50_Input.Name = "Affichage des niveaux 50";
		NiveauxIndices_50_Input.SetYesNo(0);
		NiveauxIndices_moyens_Input.Name = "Affichage des niveaux moyens (125, 333, ...)";
		NiveauxIndices_moyens_Input.SetYesNo(0);
		NiveauxIndices_PP1h_Input.Name = "Affichage des niveaux PP 1h";
		NiveauxIndices_PP1h_Input.SetYesNo(0);
		NiveauxIndices_PP4h_Input.Name = "Affichage des niveaux PP 4h";
		NiveauxIndices_PP4h_Input.SetYesNo(0);
		NiveauxIndices_PPQuartsH_Input.Name = "Affichage des niveaux Quarts H";
		NiveauxIndices_PPQuartsH_Input.SetYesNo(0);
		NiveauxIndices_PPQuartsM_Input.Name = "Affichage des niveaux Quarts M";
		NiveauxIndices_PPQuartsM_Input.SetYesNo(0);
		NiveauxIndices_PPEighthsM_Input.Name = "Affichage des niveaux Eighths M";
		NiveauxIndices_PPEighthsM_Input.SetYesNo(0);
		NiveauxIndices_Full4Mid4_Input.Name = "Affichage des niveaux Full4 / Mid4";
		NiveauxIndices_Full4Mid4_Input.SetYesNo(0);
		NiveauxIndices_PHPB_JHM_Precedent_Input.Name = "Affichage des niveaux PHPB J/H/M et Close JP";
		NiveauxIndices_PHPB_JHM_Precedent_Input.SetYesNo(0);
		NiveauxIndices_PHJPBJ_Input.Name = "Affichage des niveaux PHJ PBJ dynamiques projetes";
		NiveauxIndices_PHJPBJ_Input.SetYesNo(0);
		
		sc.AddMessageToLog("!! WARNING !! Lorsque la study ''Navigation niveaux'' est ajoutee, penser a resynchroniser les niveaux navigues via le bouton 106 (niveaux affiches)", 1) ;
		
		return;
	}  // fin du bloc de " if (sc.SetDefaults) "
	
	
	if ( sc.UpdateStartIndex == 0 )
	{
		// On met un message dans le log si la study est calculée avant les PP future, les PP indice projetés, les niveaux ronds projetés.   => on fait l'approximation que StudyNumber doit être supérieur à 16 pour que ce soit OK    // idéalement il faudrait tester s'il y a des studies avec un StudyNumber plus élevé
		{
			int StudyNumber ;
			
			for ( StudyNumber = 1 ; StudyNumber < 51 ; ++StudyNumber )
			{
				if ( sc.GetStudyIDByIndex(sc.ChartNumber, StudyNumber) == sc.StudyGraphInstanceID )  break ;   // on cherche le StudyNumber de la StudyID
			}	
			
			if ( StudyNumber < 16 )
			{	SCString message ;   message.Format("!! WARNING !! StudyNumber = %d (trop faible). Cette study doit avoir un numero eleve pour etre calculee apres les study qu'elle appelle", StudyNumber) ;    sc.AddMessageToLog(message,1) ;  }
		}
		
		// Set the ACS Control Bar buttons hover text
		sc.SetCustomStudyControlBarButtonHoverText(109, "Deplacement vers le niveau inferieur");
		sc.SetCustomStudyControlBarButtonHoverText(110, "Deplacement vers le niveau superieur");
		
		// Set the ACS Control Bar buttons text
		sc.SetCustomStudyControlBarButtonText(109, "Niv -") ; 
		sc.SetCustomStudyControlBarButtonText(110, "Niv +") ; 
		
		
		
		// Récupération de la StudyID_PrixTypique sur le graphe 16-ticks
		StudyID_PrixTypique = sc.GetStudyIDByName(sc.ChartNumber, "Prix typique", 0) ;
		if  (StudyID_PrixTypique == 0 )
		{ SCString message ;  message.Format("!! ERREUR !! La study ''Prix typique'' n'a pas ete trouvee") ; sc.AddMessageToLog(message,1) ; }

		// Récupération de la StudyID_OverlayINQ100 sur le graphe 16-ticks
		StudyID_OverlayINQ100 = sc.GetStudyIDByName(sc.ChartNumber, "Overlay INQ100 sur le future (avec correction du spread)", 0) ;
		if  (StudyID_OverlayINQ100 == 0 )
		{ SCString message ;  message.Format("!! ERREUR !! La study ''Overlay INQ100 sur le future (avec correction du spread)'' n'a pas ete trouvee") ; sc.AddMessageToLog(message,1) ; }

		// Récupération de la StudyID_PivotsFut4h sur le graphe 16-ticks
		StudyID_PivotsFut4h = sc.GetStudyIDByName(sc.ChartNumber, "[4] Pivots Fut 4h (Full)", 0) ;
		if ( StudyID_PivotsFut4h == 0 ) 
		{ SCString message ;  message.Format("!! ERREUR !! La study ''[4] Pivots Fut 4h (Full)'' n'a pas ete trouvee") ; /* sc.AddMessageToLog(message,1) ;  */}

		// Récupération de la StudyID_PivotsFut1h sur le graphe 16-ticks
		StudyID_PivotsFut1h = sc.GetStudyIDByName(sc.ChartNumber, "[5] Pivots Fut 1h (Full)", 0) ;
		if ( StudyID_PivotsFut1h == 0 ) 
		{ SCString message ;  message.Format("!! ERREUR !! La study ''[5] Pivots Fut 1h (Full)'' n'a pas ete trouvee") ; /* sc.AddMessageToLog(message,1) ; */ }

		// Récupération de la StudyID_PivotsIndM sur le graphe 16-ticks
		StudyID_PivotsIndM = sc.GetStudyIDByName(sc.ChartNumber, "[A] Pivots Ind M (Full/Mid/Quart) projetes sur le future", 0) ;
		if ( StudyID_PivotsIndM == 0 ) 
		{ SCString message ;  message.Format("!! ERREUR !! La study ''[A] Pivots Ind M (Full/Mid/Quart) projetes sur le future'' n'a pas ete trouvee") ; sc.AddMessageToLog(message,1) ; }

		// Récupération de la StudyID_PivotsIndM_Eighth sur le graphe 16-ticks
		StudyID_PivotsIndM_Eighth = sc.GetStudyIDByName(sc.ChartNumber, "[B] Pivots Ind M (Eighth) projetes sur le future", 0) ;
		if ( StudyID_PivotsIndM_Eighth == 0 ) 
		{ SCString message ;  message.Format("!! ERREUR !! La study ''[B] Pivots Ind M (Eighth) projetes sur le future'' n'a pas ete trouvee") ; sc.AddMessageToLog(message,1) ; }

			// Récupération de la StudyID_PivotsIndH sur le graphe 16-ticks
		StudyID_PivotsIndH = sc.GetStudyIDByName(sc.ChartNumber, "[C] Pivots Ind H (Full/Mid/Quart) projetes sur le future", 0) ;
		if ( StudyID_PivotsIndH == 0 ) 
		{ SCString message ;  message.Format("!! ERREUR !! La study ''[C] Pivots Ind H (Full/Mid/Quart) projetes sur le future' n'a pas ete trouvee") ; sc.AddMessageToLog(message,1) ; }

		// Récupération de la StudyID_PivotsIndJ sur le graphe 16-ticks
		StudyID_PivotsIndJ = sc.GetStudyIDByName(sc.ChartNumber, "[D] Pivots Ind J (Full/Mid) projetes sur le future", 0) ;
		if ( StudyID_PivotsIndJ == 0 ) 
		{ SCString message ;  message.Format("!! ERREUR !! La study ''[D] Pivots Ind J (Full/Mid) projetes sur le future'' n'a pas ete trouvee") ; sc.AddMessageToLog(message,1) ; }

		// Récupération de la StudyID_PivotsInd4h sur le graphe 16-ticks
		StudyID_PivotsInd4h = sc.GetStudyIDByName(sc.ChartNumber, "[E] Pivots Ind 4h projetes sur le future", 0) ;
		if ( StudyID_PivotsInd4h == 0 ) 
		{ SCString message ;  message.Format("!! ERREUR !! La study ''[E] Pivots Ind 4h projetes sur le future'' n'a pas ete trouvee") ; sc.AddMessageToLog(message,1) ; }

		// Récupération de la StudyID_PivotsInd1h sur le graphe 16-ticks
		StudyID_PivotsInd1h = sc.GetStudyIDByName(sc.ChartNumber, "[F] Pivots Ind 1h projetes sur le future", 0) ;
		if ( StudyID_PivotsInd1h == 0 ) 
		{ SCString message ;  message.Format("!! ERREUR !! La study ''[F] Pivots Ind 1h projetes sur le future'' n'a pas ete trouvee") ; sc.AddMessageToLog(message,1) ; }

		// Récupération de la StudyID_PHJPBJ sur le graphe 16-ticks
		StudyID_PHJPBJ = sc.GetStudyIDByName(sc.ChartNumber, "[G] PHJ PBJ dynamiques de l'indice projetes sur le future", 0) ;
		if ( StudyID_PHJPBJ == 0 ) 
		{ SCString message ;  message.Format("!! ERREUR !! La study ''[G] PHJ PBJ dynamiques de l'indice projetes sur le future'' n'a pas ete trouvee") ; sc.AddMessageToLog(message,1) ; }

		// Récupération de la StudyID_NiveauxPsy sur le graphe 16-ticks
		StudyID_NiveauxPsy = sc.GetStudyIDByName(sc.ChartNumber, "[Z] Niveaux psys de l'indice projetes sur le future", 0) ;
		if ( StudyID_NiveauxPsy == 0 ) 
		{ SCString message ;  message.Format("!! ERREUR !! La study ''[Z] Niveaux psys de l'indice projetes sur le future'' n'a pas ete trouvee") ; sc.AddMessageToLog(message,1) ; }

	}
	
	
	// Bouton "Niv -" ou "Niv +" appuyé
	if ( sc.MenuEventID == ACS_BUTTON_109  or  sc.MenuEventID == ACS_BUTTON_110  or  External_Input_ACS_BUTTON_109.GetInt() != 0  or  External_Input_ACS_BUTTON_110.GetInt() != 0  )  
	{ 
		sc.UpdateAlways = 1 ;
		
		// Tableau de niveaux (le constructeur par défaut est appelé)
		std::array<StructureNiveau,NbNiveaux> Niveau ;
		
		// Variables /////////////////////////////////////////////////////////////////////////////////////////////
		s_sc* const ptrsc = &sc;
		
		
		// Récupération des niveaux
		{
			// Déclaration / initialisation des variables du bloc de récupération
			SCString Name ; 
			SCFloatArray ArrayPointPivot, ExtraArrayPointPivot, Array_PHJPBJ_Projetes, Array_PHJPBJ_Index ;    // ArrayPointPivot contient les prix sur future (prix projetés pour les indices)   // ExtraArrayPointPivot  contient le NiveauIndice avant projection
			int IDU;
			int SubgraphIndex;
			float NiveauFuture;
			float NiveauIndice;
			const int ChartNumber = sc.ChartNumber ;
			int TraceVectorielONOFF ;
			float Range_PeriodePrecedente ;

			
			// Récupération des PP 1h Future    // IDU : 0 à 8
			sc.GetChartStudyInputInt(ChartNumber, StudyID_PivotsFut1h, 2, TraceVectorielONOFF)  ;      // GetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int& r_IntegerValue);
			if (  TraceVectorielONOFF )         
			{
				// Récupération du PP 1h
				{
					IDU = 0 ;
					SubgraphIndex = 0 ;
					sc.GetStudyArrayUsingID(StudyID_PivotsFut1h, SubgraphIndex, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ; 
					Niveau[0].IdentifiantUnique = 0 ;
					Name.Format("PP 1h") ;  Niveau[0].Nom = Name ;
					Niveau[0].PrixSurFuture = NiveauFuture ;
					Niveau[0].PrixSurIndice = 0 ;  
				}
				
				// Récupération du R1 1h
				{
					IDU = 1 ;
					SubgraphIndex = 1 ;
					sc.GetStudyArrayUsingID(StudyID_PivotsFut1h, SubgraphIndex, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[1].IdentifiantUnique = 1 ;
					Name.Format("R1 1h") ;  Niveau[1].Nom = Name ;
					Niveau[1].PrixSurFuture = NiveauFuture ;
					Niveau[1].PrixSurIndice = 0 ;  
				}
				
				// Récupération du R2 1h
				{
					IDU = 2 ;
					SubgraphIndex = 2 ;
					sc.GetStudyArrayUsingID(StudyID_PivotsFut1h, SubgraphIndex, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[2].IdentifiantUnique = 2 ;
					Name.Format("R2 1h") ;  Niveau[2].Nom = Name ;
					Niveau[2].PrixSurFuture = NiveauFuture ;
					Niveau[2].PrixSurIndice = 0 ;  
				}
				
				// Récupération du R3 1h
				{
					IDU = 3 ;
					SubgraphIndex = 3 ;
					sc.GetStudyArrayUsingID(StudyID_PivotsFut1h, SubgraphIndex, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[3].IdentifiantUnique = 3 ;
					Name.Format("R3 1h") ;  Niveau[3].Nom = Name ;
					Niveau[3].PrixSurFuture = NiveauFuture ;
					Niveau[3].PrixSurIndice = 0 ;  
				}
				
				// Récupération du R4 1h
				{
					IDU = 4 ;
					SubgraphIndex = 4 ;
					sc.GetStudyArrayUsingID(StudyID_PivotsFut1h, SubgraphIndex, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[4].IdentifiantUnique = 4 ;
					Name.Format("R4 1h") ;  Niveau[4].Nom = Name ;
					Niveau[4].PrixSurFuture = NiveauFuture ;
					Niveau[4].PrixSurIndice = 0 ;  
				}
				
				// Récupération du S1 1h
				{
					IDU = 5 ;
					SubgraphIndex = 5 ;
					sc.GetStudyArrayUsingID(StudyID_PivotsFut1h, SubgraphIndex, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[5].IdentifiantUnique = 5 ;
					Name.Format("S1 1h") ;  Niveau[5].Nom = Name ;
					Niveau[5].PrixSurFuture = NiveauFuture ;
					Niveau[5].PrixSurIndice = 0 ;  
				}
				
				// Récupération du S2 1h
				{
					IDU = 6 ;
					SubgraphIndex = 6 ;
					sc.GetStudyArrayUsingID(StudyID_PivotsFut1h, SubgraphIndex, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[6].IdentifiantUnique = 6 ;
					Name.Format("S2 1h") ;  Niveau[6].Nom = Name ;
					Niveau[6].PrixSurFuture = NiveauFuture ;
					Niveau[6].PrixSurIndice = 0 ;  
				}
				
				// Récupération du S3 1h
				{
					IDU = 7 ;
					SubgraphIndex = 7 ;
					sc.GetStudyArrayUsingID(StudyID_PivotsFut1h, SubgraphIndex, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[7].IdentifiantUnique = 7 ;
					Name.Format("S3 1h") ;  Niveau[7].Nom = Name ;
					Niveau[7].PrixSurFuture = NiveauFuture ;
					Niveau[7].PrixSurIndice = 0 ;  
				}
				
				// Récupération du S4 1h
				{
					IDU = 8 ;
					SubgraphIndex = 8 ;
					sc.GetStudyArrayUsingID(StudyID_PivotsFut1h, SubgraphIndex, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[8].IdentifiantUnique = 8 ;
					Name.Format("S4 1h") ;  Niveau[8].Nom = Name ;
					Niveau[8].PrixSurFuture = NiveauFuture ;
					Niveau[8].PrixSurIndice = 0 ;  
				}
			}
			
			// Récupération des PP 4h Future    // IDU : 10 à 18
			sc.GetChartStudyInputInt(ChartNumber, StudyID_PivotsFut4h, 2, TraceVectorielONOFF)  ;      // GetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int& r_IntegerValue);
			if (  TraceVectorielONOFF )         
			{
				// Récupération du PP 4h
				{
					IDU = 10 ;
					SubgraphIndex = 0 ;
					sc.GetStudyArrayUsingID(StudyID_PivotsFut4h, SubgraphIndex, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Name.Format("PP 4h") ;  Niveau[IDU].Nom = Name ;
					Niveau[IDU].PrixSurFuture = NiveauFuture ;
					Niveau[IDU].PrixSurIndice = 0 ;  
				}
				
				// Récupération du R1 4h
				{
					IDU = 11 ;
					SubgraphIndex = 1 ;
					sc.GetStudyArrayUsingID(StudyID_PivotsFut4h, SubgraphIndex, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Name.Format("R1 4h") ;  Niveau[10].Nom = Name ;
					Niveau[10].PrixSurFuture = NiveauFuture ;
					Niveau[10].PrixSurIndice = 0 ;  
				}
				
				// Récupération du R2 4h
				{
					IDU = 12 ;
					SubgraphIndex = 2 ;
					sc.GetStudyArrayUsingID(StudyID_PivotsFut4h, SubgraphIndex, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[11].IdentifiantUnique = 11 ;
					Name.Format("R2 4h") ;  Niveau[11].Nom = Name ;
					Niveau[11].PrixSurFuture = NiveauFuture ;
					Niveau[11].PrixSurIndice = 0 ;  
				}
				
				// Récupération du R3 4h
				{
					IDU = 13 ;
					SubgraphIndex = 3 ;
					sc.GetStudyArrayUsingID(StudyID_PivotsFut4h, SubgraphIndex, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[12].IdentifiantUnique = 12 ;
					Name.Format("R3 4h") ;  Niveau[12].Nom = Name ;
					Niveau[12].PrixSurFuture = NiveauFuture ;
					Niveau[12].PrixSurIndice = 0 ;  
				}
				
				// Récupération du R4 4h
				{
					IDU = 14 ;
					SubgraphIndex = 4 ;
					sc.GetStudyArrayUsingID(StudyID_PivotsFut4h, SubgraphIndex, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[13].IdentifiantUnique = 13 ;
					Name.Format("R4 4h") ;  Niveau[13].Nom = Name ;
					Niveau[13].PrixSurFuture = NiveauFuture ;
					Niveau[13].PrixSurIndice = 0 ;  
				}
				
				// Récupération du S1 4h
				{
					IDU = 15 ;
					SubgraphIndex = 5 ;
					sc.GetStudyArrayUsingID(StudyID_PivotsFut4h, SubgraphIndex, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[14].IdentifiantUnique = 14 ;
					Name.Format("S1 4h") ;  Niveau[14].Nom = Name ;
					Niveau[14].PrixSurFuture = NiveauFuture ;
					Niveau[14].PrixSurIndice = 0 ;  
				}
				
				// Récupération du S2 4h
				{
					IDU = 16 ;
					SubgraphIndex = 6 ;
					sc.GetStudyArrayUsingID(StudyID_PivotsFut4h, SubgraphIndex, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[15].IdentifiantUnique = 15 ;
					Name.Format("S2 4h") ;  Niveau[15].Nom = Name ;
					Niveau[15].PrixSurFuture = NiveauFuture ;
					Niveau[15].PrixSurIndice = 0 ;  
				}
				
				// Récupération du S3 4h
				{
					IDU = 17 ;
					SubgraphIndex = 7 ;
					sc.GetStudyArrayUsingID(StudyID_PivotsFut4h, SubgraphIndex, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[16].IdentifiantUnique = 16 ;
					Name.Format("S3 4h") ;  Niveau[16].Nom = Name ;
					Niveau[16].PrixSurFuture = NiveauFuture ;
					Niveau[16].PrixSurIndice = 0 ;  
				}
				
				// Récupération du S4 4h
				{
					IDU = 18 ;
					SubgraphIndex = 8 ;
					sc.GetStudyArrayUsingID(StudyID_PivotsFut4h, SubgraphIndex, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[17].IdentifiantUnique = 17 ;
					Name.Format("S4 4h") ;  Niveau[17].Nom = Name ;
					Niveau[17].PrixSurFuture = NiveauFuture ;
					Niveau[17].PrixSurIndice = 0 ;  
				}
			}
			
			// Récupération des PP 1h Indice    // IDU : 280 à 288
			if (NiveauxIndices_PP1h_Input.GetYesNo())
			{
				// Calcul de la valeur de Range_PeriodePrecedente
				SubgraphIndex = 0 ;  int ExtraArrayIndex = 2 ;		// Array et ExtraArray de PH de la période précédente
				sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsInd1h, SubgraphIndex, ExtraArrayIndex, ExtraArrayPointPivot) ;  
				float Niveau_PH_SurIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
				ExtraArrayIndex = 3 ;		// Array de PB de la période précédente
				sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsInd1h, SubgraphIndex, ExtraArrayIndex, ExtraArrayPointPivot) ;  
				float Niveau_PB_SurIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
				Range_PeriodePrecedente = Niveau_PH_SurIndice - Niveau_PB_SurIndice ;
				
				{
					IDU = 280 ;
					SubgraphIndex = 0 ;
					sc.GetStudyArrayUsingID(StudyID_PivotsInd1h, SubgraphIndex, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;       
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsInd1h, SubgraphIndex, 0, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Name.Format("PP 1h") ;  Niveau[IDU].Nom = Name ;
					Niveau[IDU].PrixSurFuture = NiveauFuture ;
					Niveau[IDU].PrixSurIndice = NiveauIndice ;  
					Niveau[IDU].Nature = 1 ;
					Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;
				}
				{
					IDU = 281 ;
					SubgraphIndex = 1 ;
					sc.GetStudyArrayUsingID(StudyID_PivotsInd1h, SubgraphIndex, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;       
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsInd1h, SubgraphIndex, 0, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Name.Format("R1 1h") ;  Niveau[IDU].Nom = Name ;
					Niveau[IDU].PrixSurFuture = NiveauFuture ;
					Niveau[IDU].PrixSurIndice = NiveauIndice ;  
					Niveau[IDU].Nature = 1 ;
					Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;
				}
				{
					IDU = 282 ;
					SubgraphIndex = 2 ;
					sc.GetStudyArrayUsingID(StudyID_PivotsInd1h, SubgraphIndex, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;       
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsInd1h, SubgraphIndex, 0, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Name.Format("R2 1h") ;  Niveau[IDU].Nom = Name ;
					Niveau[IDU].PrixSurFuture = NiveauFuture ;
					Niveau[IDU].PrixSurIndice = NiveauIndice ;  
					Niveau[IDU].Nature = 1 ;
					Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;
				}
				{
					IDU = 283 ;
					SubgraphIndex = 3 ;
					sc.GetStudyArrayUsingID(StudyID_PivotsInd1h, SubgraphIndex, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;       
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsInd1h, SubgraphIndex, 0, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Name.Format("R3 1h") ;  Niveau[IDU].Nom = Name ;
					Niveau[IDU].PrixSurFuture = NiveauFuture ;
					Niveau[IDU].PrixSurIndice = NiveauIndice ;  
					Niveau[IDU].Nature = 1 ;
					Niveau[IDU].SousNature = 14 ;
					Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;
				}
				{
					IDU = 284 ;
					SubgraphIndex = 4 ;
					sc.GetStudyArrayUsingID(StudyID_PivotsInd1h, SubgraphIndex, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;       
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsInd1h, SubgraphIndex, 0, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Name.Format("R4 1h") ;  Niveau[IDU].Nom = Name ;
					Niveau[IDU].PrixSurFuture = NiveauFuture ;
					Niveau[IDU].PrixSurIndice = NiveauIndice ;  
					Niveau[IDU].Nature = 1 ;
					Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;
				}
				{
					IDU = 285 ;
					SubgraphIndex = 5 ;
					sc.GetStudyArrayUsingID(StudyID_PivotsInd1h, SubgraphIndex, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;       
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsInd1h, SubgraphIndex, 0, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Name.Format("S1 1h") ;  Niveau[IDU].Nom = Name ;
					Niveau[IDU].PrixSurFuture = NiveauFuture ;
					Niveau[IDU].PrixSurIndice = NiveauIndice ;  
					Niveau[IDU].Nature = 1 ;
					Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;
				}
				{
					IDU = 286 ;
					SubgraphIndex = 6 ;
					sc.GetStudyArrayUsingID(StudyID_PivotsInd1h, SubgraphIndex, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;       
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsInd1h, SubgraphIndex, 0, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Name.Format("S2 1h") ;  Niveau[IDU].Nom = Name ;
					Niveau[IDU].PrixSurFuture = NiveauFuture ;
					Niveau[IDU].PrixSurIndice = NiveauIndice ;  
					Niveau[IDU].Nature = 1 ;
					Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;
				}
				{
					IDU = 287 ;
					SubgraphIndex = 7 ;
					sc.GetStudyArrayUsingID(StudyID_PivotsInd1h, SubgraphIndex, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;       
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsInd1h, SubgraphIndex, 0, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Name.Format("S3 1h") ;  Niveau[IDU].Nom = Name ;
					Niveau[IDU].PrixSurFuture = NiveauFuture ;
					Niveau[IDU].PrixSurIndice = NiveauIndice ;  
					Niveau[IDU].Nature = 1 ;
					Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;
				}
				{
					IDU = 288 ;
					SubgraphIndex = 8 ;
					sc.GetStudyArrayUsingID(StudyID_PivotsInd1h, SubgraphIndex, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;       
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsInd1h, SubgraphIndex, 0, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Name.Format("S4 1h") ;  Niveau[IDU].Nom = Name ;
					Niveau[IDU].PrixSurFuture = NiveauFuture ;
					Niveau[IDU].PrixSurIndice = NiveauIndice ;  
					Niveau[IDU].Nature = 1 ;
					Niveau[IDU].SousNature = 14 ;
					Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;
				}
			}
			
			// Récupération des PP 4h Indice    // IDU : 290 à 298
			if (NiveauxIndices_PP4h_Input.GetYesNo())
			{
				// Calcul de la valeur de Range_PeriodePrecedente
				SubgraphIndex = 0 ;  int ExtraArrayIndex = 2 ;		// Array et ExtraArray de PH de la période précédente
				sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsInd4h, SubgraphIndex, ExtraArrayIndex, ExtraArrayPointPivot) ;  
				float Niveau_PH_SurIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
				ExtraArrayIndex = 3 ;		// Array de PB de la période précédente
				sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsInd4h, SubgraphIndex, ExtraArrayIndex, ExtraArrayPointPivot) ;  
				float Niveau_PB_SurIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
				Range_PeriodePrecedente = Niveau_PH_SurIndice - Niveau_PB_SurIndice ;

				{
					IDU = 290 ;
					SubgraphIndex = 0 ;
					sc.GetStudyArrayUsingID(StudyID_PivotsInd4h, SubgraphIndex, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;       
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsInd4h, SubgraphIndex, 0, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Name.Format("PP 4h") ;  Niveau[IDU].Nom = Name ;
					Niveau[IDU].PrixSurFuture = NiveauFuture ;
					Niveau[IDU].PrixSurIndice = NiveauIndice ;  
					Niveau[IDU].Nature = 2 ;
					Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;
				}
				{
					IDU = 291 ;
					SubgraphIndex = 1 ;
					sc.GetStudyArrayUsingID(StudyID_PivotsInd4h, SubgraphIndex, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;       
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsInd4h, SubgraphIndex, 0, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Name.Format("R1 4h") ;  Niveau[IDU].Nom = Name ;
					Niveau[IDU].PrixSurFuture = NiveauFuture ;
					Niveau[IDU].PrixSurIndice = NiveauIndice ;  
					Niveau[IDU].Nature = 2 ;
					Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;
				}
				{
					IDU = 292 ;
					SubgraphIndex = 2 ;
					sc.GetStudyArrayUsingID(StudyID_PivotsInd4h, SubgraphIndex, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;       
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsInd4h, SubgraphIndex, 0, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Name.Format("R2 4h") ;  Niveau[IDU].Nom = Name ;
					Niveau[IDU].PrixSurFuture = NiveauFuture ;
					Niveau[IDU].PrixSurIndice = NiveauIndice ;  
					Niveau[IDU].Nature = 2 ;
					Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;
				}
				{
					IDU = 293 ;
					SubgraphIndex = 3 ;
					sc.GetStudyArrayUsingID(StudyID_PivotsInd4h, SubgraphIndex, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;       
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsInd4h, SubgraphIndex, 0, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Name.Format("R3 4h") ;  Niveau[IDU].Nom = Name ;
					Niveau[IDU].PrixSurFuture = NiveauFuture ;
					Niveau[IDU].PrixSurIndice = NiveauIndice ;  
					Niveau[IDU].Nature = 2 ;
					Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;
				}
				{
					IDU = 294 ;
					SubgraphIndex = 4 ;
					sc.GetStudyArrayUsingID(StudyID_PivotsInd4h, SubgraphIndex, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;       
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsInd4h, SubgraphIndex, 0, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Name.Format("R4 4h") ;  Niveau[IDU].Nom = Name ;
					Niveau[IDU].PrixSurFuture = NiveauFuture ;
					Niveau[IDU].PrixSurIndice = NiveauIndice ;  
					Niveau[IDU].Nature = 2 ;
					Niveau[IDU].SousNature = 14 ;
					Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;
				}
				{
					IDU = 295 ;
					SubgraphIndex = 5 ;
					sc.GetStudyArrayUsingID(StudyID_PivotsInd4h, SubgraphIndex, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;       
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsInd4h, SubgraphIndex, 0, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Name.Format("S1 4h") ;  Niveau[IDU].Nom = Name ;
					Niveau[IDU].PrixSurFuture = NiveauFuture ;
					Niveau[IDU].PrixSurIndice = NiveauIndice ;  
					Niveau[IDU].Nature = 2 ;
					Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;
				}
				{
					IDU = 296 ;
					SubgraphIndex = 6 ;
					sc.GetStudyArrayUsingID(StudyID_PivotsInd4h, SubgraphIndex, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;       
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsInd4h, SubgraphIndex, 0, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Name.Format("S2 4h") ;  Niveau[IDU].Nom = Name ;
					Niveau[IDU].PrixSurFuture = NiveauFuture ;
					Niveau[IDU].PrixSurIndice = NiveauIndice ;  
					Niveau[IDU].Nature = 2 ;
					Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;
				}
				{
					IDU = 297 ;
					SubgraphIndex = 7 ;
					sc.GetStudyArrayUsingID(StudyID_PivotsInd4h, SubgraphIndex, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;       
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsInd4h, SubgraphIndex, 0, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Name.Format("S3 4h") ;  Niveau[IDU].Nom = Name ;
					Niveau[IDU].PrixSurFuture = NiveauFuture ;
					Niveau[IDU].PrixSurIndice = NiveauIndice ;  
					Niveau[IDU].Nature = 2 ;
					Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;
				}
				{
					IDU = 298 ;
					SubgraphIndex = 8 ;
					sc.GetStudyArrayUsingID(StudyID_PivotsInd4h, SubgraphIndex, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;       
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsInd4h, SubgraphIndex, 0, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Name.Format("S4 4h") ;  Niveau[IDU].Nom = Name ;
					Niveau[IDU].PrixSurFuture = NiveauFuture ;
					Niveau[IDU].PrixSurIndice = NiveauIndice ;  
					Niveau[IDU].Nature = 2 ;
					Niveau[IDU].SousNature = 14 ;
					Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;
				}
			}
			
			// Récupération des PP Journaliers (Full/Mid) Indice    // IDU : 301 à 321
			if (NiveauxIndicesAffiches_Input.GetYesNo() != 0)		// 0 == OFF
			{
				// Calcul de la valeur de Range_PeriodePrecedente
				SubgraphIndex = 1 ;		// Array de PH de la période précédente
				sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 1, ExtraArrayPointPivot) ;  
				float Niveau_PH_SurIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
				SubgraphIndex = 2 ;		// Array de PB de la période précédente
				sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 1, ExtraArrayPointPivot) ;  
				float Niveau_PB_SurIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
				Range_PeriodePrecedente = Niveau_PH_SurIndice - Niveau_PB_SurIndice ;
				
				{
					IDU = 303 ;
					SubgraphIndex = 3 ;
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Name.Format("PP J") ;  Niveau[IDU].Nom = Name ;
					Niveau[IDU].PrixSurFuture = NiveauFuture ;
					Niveau[IDU].PrixSurIndice = NiveauIndice ;  
					Niveau[IDU].Nature = 3 ;  
					Niveau[IDU].SousNature = 1 ;	
					Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
				}
				{
					IDU = 304 ;
					SubgraphIndex = 4 ;
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Name.Format("R1 J") ;  Niveau[IDU].Nom = Name ;
					Niveau[IDU].PrixSurFuture = NiveauFuture ;
					Niveau[IDU].PrixSurIndice = NiveauIndice ;  
					Niveau[IDU].Nature = 3 ;
					Niveau[IDU].SousNature = 1 ;	
					Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
				}
				{
					IDU = 305 ;
					SubgraphIndex = 5 ;
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Name.Format("R2 J") ;  Niveau[IDU].Nom = Name ;
					Niveau[IDU].PrixSurFuture = NiveauFuture ;
					Niveau[IDU].PrixSurIndice = NiveauIndice ;  
					Niveau[IDU].Nature = 3 ;
					Niveau[IDU].SousNature = 1 ;	
					Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
				}
				{
					IDU = 306 ;
					SubgraphIndex = 6 ;
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Name.Format("R3 J") ;  Niveau[IDU].Nom = Name ;
					Niveau[IDU].PrixSurFuture = NiveauFuture ;
					Niveau[IDU].PrixSurIndice = NiveauIndice ;  
					Niveau[IDU].Nature = 3 ;
					Niveau[IDU].SousNature = 1 ;	
					Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
				}
				{
					IDU = 308 ;
					SubgraphIndex = 8 ;
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Name.Format("S1 J") ;  Niveau[IDU].Nom = Name ;
					Niveau[IDU].PrixSurFuture = NiveauFuture ;
					Niveau[IDU].PrixSurIndice = NiveauIndice ;  
					Niveau[IDU].Nature = 3 ;
					Niveau[IDU].SousNature = 1 ;	
					Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
				}
				{
					IDU = 309 ;
					SubgraphIndex = 9 ;
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Name.Format("S2 J") ;  Niveau[IDU].Nom = Name ;
					Niveau[IDU].PrixSurFuture = NiveauFuture ;
					Niveau[IDU].PrixSurIndice = NiveauIndice ;  
					Niveau[IDU].Nature = 3 ;
					Niveau[IDU].SousNature = 1 ;	
					Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
				}
				{
					IDU = 310 ;
					SubgraphIndex = 10 ;
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Name.Format("S3 J") ;  Niveau[IDU].Nom = Name ;
					Niveau[IDU].PrixSurFuture = NiveauFuture ;
					Niveau[IDU].PrixSurIndice = NiveauIndice ;  
					Niveau[IDU].Nature = 3 ;
					Niveau[IDU].SousNature = 1 ;	
					Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
				}
				{
					IDU = 312 ;
					SubgraphIndex = 12 ;
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Name.Format("mR1 J") ;  Niveau[IDU].Nom = Name ;
					Niveau[IDU].PrixSurFuture = NiveauFuture ;
					Niveau[IDU].PrixSurIndice = NiveauIndice ;  
					Niveau[IDU].Nature = 3 ;
					Niveau[IDU].SousNature = 2 ;	
					Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
				}
				{
					IDU = 313 ;
					SubgraphIndex = 13 ;
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Name.Format("mR2 J") ;  Niveau[IDU].Nom = Name ;
					Niveau[IDU].PrixSurFuture = NiveauFuture ;
					Niveau[IDU].PrixSurIndice = NiveauIndice ;  
					Niveau[IDU].Nature = 3 ;
					Niveau[IDU].SousNature = 2 ;	
					Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
				}
				{
					IDU = 314 ;
					SubgraphIndex = 14 ;
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Name.Format("mR3 J") ;  Niveau[IDU].Nom = Name ;
					Niveau[IDU].PrixSurFuture = NiveauFuture ;
					Niveau[IDU].PrixSurIndice = NiveauIndice ;  
					Niveau[IDU].Nature = 3 ;
					Niveau[IDU].SousNature = 2 ;	
					Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
				}
				{
					IDU = 316 ;
					SubgraphIndex = 16 ;
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Name.Format("mS1 J") ;  Niveau[IDU].Nom = Name ;
					Niveau[IDU].PrixSurFuture = NiveauFuture ;
					Niveau[IDU].PrixSurIndice = NiveauIndice ;  
					Niveau[IDU].Nature = 3 ;
					Niveau[IDU].SousNature = 2 ;	
					Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
				}
				{
					IDU = 317 ;
					SubgraphIndex = 17 ;
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Name.Format("mS2 J") ;  Niveau[IDU].Nom = Name ;
					Niveau[IDU].PrixSurFuture = NiveauFuture ;
					Niveau[IDU].PrixSurIndice = NiveauIndice ;  
					Niveau[IDU].Nature = 3 ;
					Niveau[IDU].SousNature = 2 ;	
					Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
				}
				{
					IDU = 318 ;
					SubgraphIndex = 18 ;
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Name.Format("mS3 J") ;  Niveau[IDU].Nom = Name ;
					Niveau[IDU].PrixSurFuture = NiveauFuture ;
					Niveau[IDU].PrixSurIndice = NiveauIndice ;  
					Niveau[IDU].Nature = 3 ;
					Niveau[IDU].SousNature = 2 ;	
					Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
				}
				
				// Navigation sur PHJP et PBJP 
				if ( NiveauxIndices_PHPB_JHM_Precedent_Input.GetYesNo() )
				{
					// Calcul de la valeur de l'OpenGap (vaut 0 en absence d'opengap)
					SubgraphIndex = 0 ;
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 1, ExtraArrayPointPivot) ;  
					float Niveau_OpenGap_SurIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
					
					{
						IDU = 301 ;
						SubgraphIndex = 1 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;             // maintenant on récupère ArrayPointPivot qui contient les niveaux projetés non arrondis
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("PH JP") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 3 ;
						Niveau[IDU].SousNature = 6 ;	
						
						// Recherche d'un hypothétique OpenGap
						if ( fabs(NiveauIndice-Niveau_OpenGap_SurIndice) < 0.01 )  // on est en présence d'un opengap haussier
						{
							Name.Format("OpenGap J") ;  Niveau[IDU].Nom = Name ;	
							Niveau[IDU].SousNature = 8 ;	
						}
					}
					{
						IDU = 302 ;
						SubgraphIndex = 2 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("PB JP") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 3 ;
						Niveau[IDU].SousNature = 6 ;	
						
						// Recherche d'un hypothétique OpenGap
						if ( fabs(NiveauIndice-Niveau_OpenGap_SurIndice) < 0.01 )  // on est en présence d'un opengap baissier
						{
							Name.Format("OpenGap J") ;  Niveau[IDU].Nom = Name ;	
							Niveau[IDU].SousNature = 8 ;	
						}
					}
					
					{
						IDU = 320 ;
						SubgraphIndex = 36 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("Cloture JP") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 3 ;
						Niveau[IDU].SousNature = 7 ;	
						Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 321 ;
						SubgraphIndex = 37 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("DemiGap J") ;  Niveau[IDU].Nom = Name ;		// Note : ne pas ajouter l'opengap, il est déjà représenté par le niveau PHJP ou PBJP
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 3 ;
						Niveau[IDU].SousNature = 9 ;	
					}
				}
				
				// Navigation sur Full4 et Mid4
				if ( NiveauxIndices_Full4Mid4_Input.GetYesNo() )
				{
					{
						IDU = 307 ;
						SubgraphIndex = 7 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("R4 J") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 3 ;
						Niveau[IDU].SousNature = 14 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 311 ;
						SubgraphIndex = 11 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("S4 J") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;    
						Niveau[IDU].Nature = 3 ;
						Niveau[IDU].SousNature = 14 ;	Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 315 ;
						SubgraphIndex = 15 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("mR4 J") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 3 ;
						Niveau[IDU].SousNature = 24 ;	Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 319 ;
						SubgraphIndex = 19 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndJ, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("mS4 J") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 3 ;
						Niveau[IDU].SousNature = 24 ;	Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
				}
			}
			
			// Récupération des PP Hebdo (Full/Mid/Quart) Indice    // IDU : 330 à 364
			if (NiveauxIndicesAffiches_Input.GetYesNo() != 0)
			{	
				// Calcul de la valeur de Range_PeriodePrecedente
				SubgraphIndex = 1 ;		// Array de PH de la période précédente
				sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 1, ExtraArrayPointPivot) ;  
				float Niveau_PH_SurIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
				SubgraphIndex = 2 ;		// Array de PB de la période précédente
				sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 1, ExtraArrayPointPivot) ;  
				float Niveau_PB_SurIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
				Range_PeriodePrecedente = Niveau_PH_SurIndice - Niveau_PB_SurIndice ;

				// Navigation sur les Full/Mid, PH/PB
				{
					{
						IDU = 333 ;
						SubgraphIndex = 3 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("PP H") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 4 ;
						Niveau[IDU].SousNature = 1 ;	
						Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;
					}
					{
						IDU = 334 ;
						SubgraphIndex = 4 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("R1 H") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 4 ;
						Niveau[IDU].SousNature = 1 ;	
						Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;
					}
					{
						IDU = 335 ;
						SubgraphIndex = 5 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("R2 H") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 4 ;
						Niveau[IDU].SousNature = 1 ;	
						Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;
					}
					{
						IDU = 336 ;
						SubgraphIndex = 6 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("R3 H") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 4 ;
						Niveau[IDU].SousNature = 1 ;	
						Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;
					}
					{
						IDU = 338 ;
						SubgraphIndex = 8 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("S1 H") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 4 ;
						Niveau[IDU].SousNature = 1 ;	
						Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;
					}
					{
						IDU = 339 ;
						SubgraphIndex = 9 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("S2 H") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 4 ;
						Niveau[IDU].SousNature = 1 ;	
						Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;
					}
					{
						IDU = 340 ;
						SubgraphIndex = 10 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("S3 H") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 4 ;
						Niveau[IDU].SousNature = 1 ;	
						Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;
					}
					{
						IDU = 342 ;
						SubgraphIndex = 12 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("mR1 H") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 4 ;
						Niveau[IDU].SousNature = 2 ;	
						Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;
					}
					{
						IDU = 343 ;
						SubgraphIndex = 13 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("mR2 H") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 4 ;
						Niveau[IDU].SousNature = 2 ;	
						Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;
					}
					{
						IDU = 344 ;
						SubgraphIndex = 14 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("mR3 H") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 4 ;
						Niveau[IDU].SousNature = 2 ;	
						Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;
					}
					{
						IDU = 346 ;
						SubgraphIndex = 16 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("mS1 H") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 4 ;
						Niveau[IDU].SousNature = 2 ;	
						Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;
					}
					{
						IDU = 347 ;
						SubgraphIndex = 17 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("mS2 H") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 4 ;
						Niveau[IDU].SousNature = 2 ;	
						Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;
					}
					{
						IDU = 348 ;
						SubgraphIndex = 18 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("mS3 H") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 4 ;
						Niveau[IDU].SousNature = 2 ;	
						Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;
					}
					
					// Navigation sur PH PB 
					if ( NiveauxIndices_PHPB_JHM_Precedent_Input.GetYesNo() )
					{
						// Calcul de la valeur de l'OpenGap (vaut 0 en absence d'opengap)
						SubgraphIndex = 0 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 1, ExtraArrayPointPivot) ;  
						float Niveau_OpenGap_SurIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						
						{
							IDU = 331 ;  				
							SubgraphIndex = 1 ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Name.Format("PH HP") ;  Niveau[IDU].Nom = Name ;
							Niveau[IDU].PrixSurFuture = NiveauFuture ;
							Niveau[IDU].PrixSurIndice = NiveauIndice ;  
							Niveau[IDU].Nature = 4 ;
							Niveau[IDU].SousNature = 6 ;	

							// Recherche d'un hypothétique OpenGap
							if ( fabs(NiveauIndice-Niveau_OpenGap_SurIndice) < 0.01 )  // on est en présence d'un opengap haussier
							{
								Name.Format("OpenGap H") ;  Niveau[IDU].Nom = Name ;	
								Niveau[IDU].SousNature = 8 ;	
							}
						}
						{
							IDU = 332 ;
							SubgraphIndex = 2 ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Name.Format("PB HP") ;  Niveau[IDU].Nom = Name ;
							Niveau[IDU].PrixSurFuture = NiveauFuture ;
							Niveau[IDU].PrixSurIndice = NiveauIndice ;  
							Niveau[IDU].Nature = 4 ;
							Niveau[IDU].SousNature = 6 ;	
							
							// Recherche d'un hypothétique OpenGap
							if ( fabs(NiveauIndice-Niveau_OpenGap_SurIndice) < 0.01 )  // on est en présence d'un opengap baissier
							{
								Name.Format("OpenGap H") ;  Niveau[IDU].Nom = Name ;	
								Niveau[IDU].SousNature = 8 ;	
							}							
						}
						{
							IDU = 366 ;
							SubgraphIndex = 36 ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Name.Format("DemiGap H") ;  Niveau[IDU].Nom = Name ;
							Niveau[IDU].PrixSurFuture = NiveauFuture ;
							Niveau[IDU].PrixSurIndice = NiveauIndice ;  
							Niveau[IDU].Nature = 4 ;
							Niveau[IDU].SousNature = 9 ;	
						}
					}
					
					// Navigation sur Full4 et Mid4
					if ( NiveauxIndices_Full4Mid4_Input.GetYesNo() )
					{
						{
							IDU = 337 ;
							SubgraphIndex = 7 ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Name.Format("R4 H") ;  Niveau[IDU].Nom = Name ;
							Niveau[IDU].PrixSurFuture = NiveauFuture ;
							Niveau[IDU].PrixSurIndice = NiveauIndice ;  
							Niveau[IDU].Nature = 4 ;
							Niveau[IDU].SousNature = 14 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;
						}
						{
							IDU = 341 ;
							SubgraphIndex = 11 ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Name.Format("S4 H") ;  Niveau[IDU].Nom = Name ;
							Niveau[IDU].PrixSurFuture = NiveauFuture ;
							Niveau[IDU].PrixSurIndice = NiveauIndice ;  
							Niveau[IDU].Nature = 4 ;
							Niveau[IDU].SousNature = 14 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
						}
						{
							IDU = 345 ;
							SubgraphIndex = 15 ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Name.Format("mR4 H") ;  Niveau[IDU].Nom = Name ;
							Niveau[IDU].PrixSurFuture = NiveauFuture ;
							Niveau[IDU].PrixSurIndice = NiveauIndice ;  
							Niveau[IDU].Nature = 4 ;
							Niveau[IDU].SousNature = 24 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
						}
						{
							IDU = 349 ;
							SubgraphIndex = 19 ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Name.Format("mS4 H") ;  Niveau[IDU].Nom = Name ;
							Niveau[IDU].PrixSurFuture = NiveauFuture ;
							Niveau[IDU].PrixSurIndice = NiveauIndice ;  
							Niveau[IDU].Nature = 4 ;
							Niveau[IDU].SousNature = 24 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
						}
					}
				}
				
				// Navigation sur les quarts
				if ( NiveauxIndices_PPQuartsH_Input.GetYesNo() )
				{
					{
						IDU = 350 ;
						SubgraphIndex = 20 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("RQ1 H") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 4 ;
						Niveau[IDU].SousNature = 3 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 351 ;
						SubgraphIndex = 21 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("RQ2 H") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 4 ;  Niveau[IDU].SousNature = 3 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 352 ;
						SubgraphIndex = 22 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("RQ3 H") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 4 ;  Niveau[IDU].SousNature = 3 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 353 ;
						SubgraphIndex = 23 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("RQ4 H") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 4 ;  Niveau[IDU].SousNature = 3 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 354 ;
						SubgraphIndex = 24 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("RQ5 H") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 4 ;  Niveau[IDU].SousNature = 3 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 355 ;
						SubgraphIndex = 25 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("RQ6 H") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 4 ;  Niveau[IDU].SousNature = 3 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 358 ;
						SubgraphIndex = 28 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("SQ1 H") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 4 ;  Niveau[IDU].SousNature = 3 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 359 ;
						SubgraphIndex = 29 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("SQ2 H") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 4 ;  Niveau[IDU].SousNature = 3 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 360 ;
						SubgraphIndex = 30 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("SQ3 H") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 4 ;  Niveau[IDU].SousNature = 3 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 361 ;
						SubgraphIndex = 31 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("SQ4 H") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 4 ;  Niveau[IDU].SousNature = 3 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 362 ;
						SubgraphIndex = 32 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("SQ5 H") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 4 ;  Niveau[IDU].SousNature = 3 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 363 ;
						SubgraphIndex = 33 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("SQ6 H") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 4 ;  Niveau[IDU].SousNature = 3 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					// if ( TraceDesFull4Mid4 )
					if ( NiveauxIndices_Full4Mid4_Input.GetYesNo() )
					{
						{
							IDU = 356 ;
							SubgraphIndex = 26 ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Name.Format("RQ7 H") ;  Niveau[IDU].Nom = Name ;
							Niveau[IDU].PrixSurFuture = NiveauFuture ;
							Niveau[IDU].PrixSurIndice = NiveauIndice ;  
							Niveau[IDU].Nature = 4 ;
							Niveau[IDU].SousNature = 34 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
						}
						{
							IDU = 357 ;
							SubgraphIndex = 27 ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Name.Format("RQ8 H") ;  Niveau[IDU].Nom = Name ;
							Niveau[IDU].PrixSurFuture = NiveauFuture ;
							Niveau[IDU].PrixSurIndice = NiveauIndice ;  
							Niveau[IDU].Nature = 4 ;
							Niveau[IDU].SousNature = 34 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
						}
						{
							IDU = 364 ;
							SubgraphIndex = 34 ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Name.Format("SQ7 H") ;  Niveau[IDU].Nom = Name ;
							Niveau[IDU].PrixSurFuture = NiveauFuture ;
							Niveau[IDU].PrixSurIndice = NiveauIndice ;  
							Niveau[IDU].Nature = 4 ;
							Niveau[IDU].SousNature = 34 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
						}
						{
							IDU = 365 ;
							SubgraphIndex = 35 ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndH, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Name.Format("SQ8 H") ;  Niveau[IDU].Nom = Name ;
							Niveau[IDU].PrixSurFuture = NiveauFuture ;
							Niveau[IDU].PrixSurIndice = NiveauIndice ;  
							Niveau[IDU].Nature = 4 ;
							Niveau[IDU].SousNature = 34 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
						}
					}
				}
			}
			
			// Récupération des PP Mensuels (Full/Mid/Quart/Eighth) Indice    // IDU : 371 à 407
			if (NiveauxIndicesAffiches_Input.GetYesNo() != 0)
			{
				// Calcul de la valeur de Range_PeriodePrecedente
				SubgraphIndex = 36 ;		// Array de PH de la période précédente
				sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 1, ExtraArrayPointPivot) ;  
				float Niveau_PH_SurIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
				SubgraphIndex = 37 ;		// Array de PB de la période précédente
				sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 1, ExtraArrayPointPivot) ;  
				float Niveau_PB_SurIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
				Range_PeriodePrecedente = Niveau_PH_SurIndice - Niveau_PB_SurIndice ;

				// Navigation sur les Full/Mid, PH/PB
				{
					{
						IDU = 373 ;
						SubgraphIndex = 3 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("PP M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 1 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 374 ;
						SubgraphIndex = 4 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("R1 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 1 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 375 ;
						SubgraphIndex = 5 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("R2 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 1 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 376 ;
						SubgraphIndex = 6 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("R3 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 1 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 378 ;
						SubgraphIndex = 8 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("S1 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 1 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 379 ;
						SubgraphIndex = 9 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("S2 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 1 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 380 ;
						SubgraphIndex = 10 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("S3 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 1 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 382 ;
						SubgraphIndex = 12 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("mR1 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 2 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 383 ;
						SubgraphIndex = 13 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("mR2 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 2 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 384 ;
						SubgraphIndex = 14 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("mR3 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 2 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 386 ;
						SubgraphIndex = 16 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("mS1 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 2 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 387 ;
						SubgraphIndex = 17 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("mS2 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 2 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 388 ;
						SubgraphIndex = 18 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("mS3 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 2 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					
					// Navigation sur PH PB 
					if ( NiveauxIndices_PHPB_JHM_Precedent_Input.GetYesNo() )
					{
						{
							IDU = 371 ;
							SubgraphIndex = 1 ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Name.Format("DemiGap M") ;  Niveau[IDU].Nom = Name ;
							Niveau[IDU].PrixSurFuture = NiveauFuture ;
							Niveau[IDU].PrixSurIndice = NiveauIndice ;  
							Niveau[IDU].Nature = 5 ;
							Niveau[IDU].SousNature = 9 ;	
						}
						{
							IDU = 372 ;
							SubgraphIndex = 2 ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Name.Format("PH Absolu") ;  Niveau[IDU].Nom = Name ;
							Niveau[IDU].PrixSurFuture = NiveauFuture ;
							Niveau[IDU].PrixSurIndice = NiveauIndice ;  
							Niveau[IDU].Nature = 5 ;
							Niveau[IDU].SousNature = 10 ;	
						}
						
						// Calcul de la valeur de l'OpenGap (vaut 0 en absence d'opengap)
						SubgraphIndex = 0 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 1, ExtraArrayPointPivot) ;  
						float Niveau_OpenGap_SurIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						{
							IDU = 406 ;
							SubgraphIndex = 36 ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Name.Format("PH MP") ;  Niveau[IDU].Nom = Name ;
							Niveau[IDU].PrixSurFuture = NiveauFuture ;
							Niveau[IDU].PrixSurIndice = NiveauIndice ;  
							Niveau[IDU].Nature = 5 ;
							Niveau[IDU].SousNature = 6 ;	

							// Recherche d'un hypothétique OpenGap
							if ( fabs(NiveauIndice-Niveau_OpenGap_SurIndice) < 0.01 )  // on est en présence d'un opengap haussier
							{
								Name.Format("OpenGap M") ;  Niveau[IDU].Nom = Name ;	
								Niveau[IDU].SousNature = 8 ;	
							}							
						}
						{
							IDU = 407 ;
							SubgraphIndex = 37 ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Name.Format("PB MP") ;  Niveau[IDU].Nom = Name ;
							Niveau[IDU].PrixSurFuture = NiveauFuture ;
							Niveau[IDU].PrixSurIndice = NiveauIndice ;  
							Niveau[IDU].Nature = 5 ;
							Niveau[IDU].SousNature = 6 ;	

							// Recherche d'un hypothétique OpenGap
							if ( fabs(NiveauIndice-Niveau_OpenGap_SurIndice) < 0.01 )  // on est en présence d'un opengap baissier
							{
								Name.Format("OpenGap M") ;  Niveau[IDU].Nom = Name ;	
								Niveau[IDU].SousNature = 8 ;	
							}							
						}
					}
					
					// Navigation sur Full4 et Mid4
					if ( NiveauxIndices_Full4Mid4_Input.GetYesNo() )
					{
						{
							IDU = 377 ;
							SubgraphIndex = 7 ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Name.Format("R4 M") ;  Niveau[IDU].Nom = Name ;
							Niveau[IDU].PrixSurFuture = NiveauFuture ;
							Niveau[IDU].PrixSurIndice = NiveauIndice ;  
							Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 14 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
						}
						{
							IDU = 381 ;
							SubgraphIndex = 11 ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Name.Format("S4 M") ;  Niveau[IDU].Nom = Name ;
							Niveau[IDU].PrixSurFuture = NiveauFuture ;
							Niveau[IDU].PrixSurIndice = NiveauIndice ;  
							Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 14 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
						}
						{
							IDU = 385 ;
							SubgraphIndex = 15 ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Name.Format("mR4 M") ;  Niveau[IDU].Nom = Name ;
							Niveau[IDU].PrixSurFuture = NiveauFuture ;
							Niveau[IDU].PrixSurIndice = NiveauIndice ;  
							Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 24 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
						}
						{
							IDU = 389 ;
							SubgraphIndex = 19 ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Name.Format("mS4 M") ;  Niveau[IDU].Nom = Name ;
							Niveau[IDU].PrixSurFuture = NiveauFuture ;
							Niveau[IDU].PrixSurIndice = NiveauIndice ;  
							Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 24 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
						}
					}
				}		
				
				// Navigation sur les quarts
				if ( NiveauxIndices_PPQuartsM_Input.GetYesNo() )
				{					
					{
						IDU = 390 ;
						SubgraphIndex = 20 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("RQ1 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 3 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 391 ;
						SubgraphIndex = 21 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("RQ2 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 3 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 392 ;
						SubgraphIndex = 22 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("RQ3 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 3 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 393 ;
						SubgraphIndex = 23 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("RQ4 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 3 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 394 ;
						SubgraphIndex = 24 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("RQ5 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 3 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 395 ;
						SubgraphIndex = 25 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("RQ6 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 3 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 398 ;
						SubgraphIndex = 28 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("SQ1 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 3 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 399 ;
						SubgraphIndex = 29 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("SQ2 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 3 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 400 ;
						SubgraphIndex = 30 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("SQ3 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 3 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 401 ;
						SubgraphIndex = 31 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("SQ4 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 3 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 402 ;
						SubgraphIndex = 32 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("SQ5 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 3 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 403 ;
						SubgraphIndex = 33 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("SQ6 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 3 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					// if ( TraceDesFull4Mid4 )
					if ( NiveauxIndices_Full4Mid4_Input.GetYesNo() )
					{
						{
							IDU = 396 ;
							SubgraphIndex = 26 ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Name.Format("RQ7 M") ;  Niveau[IDU].Nom = Name ;
							Niveau[IDU].PrixSurFuture = NiveauFuture ;
							Niveau[IDU].PrixSurIndice = NiveauIndice ;  
							Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 34 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
						}
						{
							IDU = 397 ;
							SubgraphIndex = 27 ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Name.Format("RQ8 M") ;  Niveau[IDU].Nom = Name ;
							Niveau[IDU].PrixSurFuture = NiveauFuture ;
							Niveau[IDU].PrixSurIndice = NiveauIndice ;  
							Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 34 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
						}
						{
							IDU = 404 ;
							SubgraphIndex = 34 ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Name.Format("SQ7 M") ;  Niveau[IDU].Nom = Name ;
							Niveau[IDU].PrixSurFuture = NiveauFuture ;
							Niveau[IDU].PrixSurIndice = NiveauIndice ;  
							Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 34 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
						}
						{
							IDU = 405 ;
							SubgraphIndex = 35 ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Name.Format("SQ8 M") ;  Niveau[IDU].Nom = Name ;
							Niveau[IDU].PrixSurFuture = NiveauFuture ;
							Niveau[IDU].PrixSurIndice = NiveauIndice ;  
							Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 34 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
						}
					}
				}
			
			
				// Récupération des PP Mensuels Eighth Indice   // IDU : 410 à 441
				if ( NiveauxIndices_PPEighthsM_Input.GetYesNo() )
				{
					{
						IDU = 410 ;
						SubgraphIndex = 0 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("RE1 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 4 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 411 ;
						SubgraphIndex = 1 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("RE2 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 4 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 412 ;
						SubgraphIndex = 2 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("RE3 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 4 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 413 ;
						SubgraphIndex = 3 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("RE4 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 4 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 414 ;
						SubgraphIndex = 4 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("RE5 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 4 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 415 ;
						SubgraphIndex = 5 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("RE6 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 4 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 416 ;
						SubgraphIndex = 6 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("RE7 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 4 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 417 ;
						SubgraphIndex = 7 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("RE8 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 4 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 418 ;
						SubgraphIndex = 8 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("RE9 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 4 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 419 ;
						SubgraphIndex = 9 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("RE10 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 4 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 420 ;
						SubgraphIndex = 10 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("RE11 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 4 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 421 ;
						SubgraphIndex = 11 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("RE12 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 4 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 422 ;
						SubgraphIndex = 12 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("RE13 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 4 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 423 ;
						SubgraphIndex = 13 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("RE14 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 4 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 424 ;
						SubgraphIndex = 14 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("RE15 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 4 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 425 ;
						SubgraphIndex = 15 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("RE16 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 4 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 426 ;
						SubgraphIndex = 16 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("SE1 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 4 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 427 ;
						SubgraphIndex = 17 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("SE2 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 4 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 428 ;
						SubgraphIndex = 18 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("SE3 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 4 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 429 ;
						SubgraphIndex = 19 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("SE4 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 4 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 430 ;
						SubgraphIndex = 20 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("SE5 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 4 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 431 ;
						SubgraphIndex = 21 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("SE6 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 4 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 432 ;
						SubgraphIndex = 22 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("SE7 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 4 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 433 ;
						SubgraphIndex = 23 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("SE8 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 4 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 434 ;
						SubgraphIndex = 24 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("SE9 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 4 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 435 ;
						SubgraphIndex = 25 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("SE10 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 4 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 436 ;
						SubgraphIndex = 26 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("SE11 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 4 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 437 ;
						SubgraphIndex = 27 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("SE12 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 4 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 438 ;
						SubgraphIndex = 28 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("SE13 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 4 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 439 ;
						SubgraphIndex = 29 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("SE14 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 4 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 440 ;
						SubgraphIndex = 30 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("SE15 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 4 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
					{
						IDU = 441 ;
						SubgraphIndex = 31 ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 0, ArrayPointPivot) ; NiveauFuture = ArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PivotsIndM_Eighth, SubgraphIndex, 1, ExtraArrayPointPivot) ;  NiveauIndice = ExtraArrayPointPivot[sc.IndexOfLastVisibleBar] ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Name.Format("SE16 M") ;  Niveau[IDU].Nom = Name ;
						Niveau[IDU].PrixSurFuture = NiveauFuture ;
						Niveau[IDU].PrixSurIndice = NiveauIndice ;  
						Niveau[IDU].Nature = 5 ;  Niveau[IDU].SousNature = 4 ;  Niveau[IDU].RangePrecedent = Range_PeriodePrecedente ;	
					}
				}
			}

			// Récupération des PHJ et PBJ dynamiques Indice    // IDU : 447 à 448
			if ( NiveauxIndices_PHJPBJ_Input.GetYesNo() )		
			{
				{
					IDU = 447 ;
					SubgraphIndex = 1 ;
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PHJPBJ, SubgraphIndex, 0, Array_PHJPBJ_Projetes) ; NiveauFuture = Array_PHJPBJ_Projetes[sc.IndexOfLastVisibleBar] ;        
					sc.GetStudyArrayUsingID(StudyID_PHJPBJ, SubgraphIndex, Array_PHJPBJ_Index) ;  NiveauIndice = Array_PHJPBJ_Index[sc.IndexOfLastVisibleBar] ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Name.Format("PH J") ;  Niveau[IDU].Nom = Name ;
					Niveau[IDU].PrixSurFuture = NiveauFuture ;
					Niveau[IDU].PrixSurIndice = NiveauIndice ;  
					Niveau[IDU].Nature = 6 ;
				}
				{
					IDU = 448 ;
					SubgraphIndex = 2 ;
					sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_PHJPBJ, SubgraphIndex, 0, Array_PHJPBJ_Projetes) ; NiveauFuture = Array_PHJPBJ_Projetes[sc.IndexOfLastVisibleBar] ;
					sc.GetStudyArrayUsingID(StudyID_PHJPBJ, SubgraphIndex, Array_PHJPBJ_Index) ;  NiveauIndice = Array_PHJPBJ_Index[sc.IndexOfLastVisibleBar] ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Name.Format("PB J") ;  Niveau[IDU].Nom = Name ;
					Niveau[IDU].PrixSurFuture = NiveauFuture ;
					Niveau[IDU].PrixSurIndice = NiveauIndice ;  
					Niveau[IDU].Nature = 6 ;
				}
			}

			// Récupération de l'array de SpreadMoyen
			SCFloatArray SpreadMoyenArray ;
			sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_OverlayINQ100, 0, 1, SpreadMoyenArray) ;
			float fSpreadMoyen = SpreadMoyenArray[sc.IndexOfLastVisibleBar] ;
			int TimeValue = sc.BaseDateTimeIn[sc.IndexOfLastVisibleBar].GetTimeInSecondsWithoutMilliseconds()  ; //GetTimeInSecondsWithoutMilliseconds retourne une time_value	
			
			// Récupération des niveaux psys
			if (NiveauxIndicesAffiches_Input.GetYesNo() != 0)
			{
				// Récupération des niveaux dizaines Indice   // IDU : 451 à 454
				if (NiveauxIndices_10_Input.GetYesNo())
				{
					if ( TimeValue > HMS_TIME(9,0,0) and TimeValue < HMS_TIME(16,0,0) )  // n'est calculé que entre 9h00 et 16h00
					{
						float DizaineFloor = floor((sc.BaseDataIn[SC_LAST][sc.IndexOfLastVisibleBar]-fSpreadMoyen)/10)*10 ;    // arrondi à la dizaine inférieur du prix de l'indice
						{
							IDU = 451 ;
							NiveauFuture = DizaineFloor+20 + fSpreadMoyen ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Name.Format("%.0f", DizaineFloor+20) ;  Niveau[IDU].Nom = Name ;
							Niveau[IDU].PrixSurFuture = NiveauFuture ;
							Niveau[IDU].PrixSurIndice = (DizaineFloor+20) ;  
							Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;		
						}
						{
							IDU = 452 ;
							NiveauFuture = DizaineFloor+10 + fSpreadMoyen ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Name.Format("%.0f", DizaineFloor+10) ;  Niveau[IDU].Nom = Name ;
							Niveau[IDU].PrixSurFuture = NiveauFuture ;
							Niveau[IDU].PrixSurIndice = (DizaineFloor+10) ;  
							Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;	
						}
						{
							IDU = 453 ;
							NiveauFuture = DizaineFloor + fSpreadMoyen ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Name.Format("%.0f", DizaineFloor) ;  Niveau[IDU].Nom = Name ;
							Niveau[IDU].PrixSurFuture = NiveauFuture ;
							Niveau[IDU].PrixSurIndice = DizaineFloor ;  
							Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;	
							
						}
						{
							IDU = 454 ;
							NiveauFuture = DizaineFloor-10 + fSpreadMoyen ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Name.Format("%.0f", DizaineFloor-10) ;  Niveau[IDU].Nom = Name ;
							Niveau[IDU].PrixSurFuture = NiveauFuture ;
							Niveau[IDU].PrixSurIndice = (DizaineFloor-10) ;  
							Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;		
						}
					}
				}
				
				// Récupération des niveaux 25/75 Indice   // IDU : 461 à 464
				if (NiveauxIndices_25_Input.GetYesNo())
				{
					if ( TimeValue > HMS_TIME(9,0,0) and TimeValue < HMS_TIME(16,0,0) )  // n'est calculé que entre 9h00 et 16h00
					{
						float f25Floor = floor((sc.BaseDataIn[SC_LAST][sc.IndexOfLastVisibleBar]-fSpreadMoyen)/25)*25 ;    // arrondi au 25 inférieur du prix de l'indice
						{
							IDU = 461 ;
							NiveauFuture = f25Floor+50 + fSpreadMoyen ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Name.Format("%.0f", f25Floor+50) ;  Niveau[IDU].Nom = Name ;
							Niveau[IDU].PrixSurFuture = NiveauFuture ;
							Niveau[IDU].PrixSurIndice = (f25Floor+50) ;  
							Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;
						}
						{
							IDU = 462 ;
							NiveauFuture = f25Floor+25 + fSpreadMoyen ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Name.Format("%.0f", f25Floor+25) ;  Niveau[IDU].Nom = Name ;
							Niveau[IDU].PrixSurFuture = NiveauFuture ;
							Niveau[IDU].PrixSurIndice = (f25Floor+25) ;  
							Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;
						}
						{
							IDU = 463 ;
							NiveauFuture = f25Floor + fSpreadMoyen ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Name.Format("%.0f", f25Floor) ;  Niveau[IDU].Nom = Name ;
							Niveau[IDU].PrixSurFuture = NiveauFuture ;
							Niveau[IDU].PrixSurIndice = f25Floor ;  
							Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;
						}
						{
							IDU = 464 ;
							NiveauFuture = f25Floor-25 + fSpreadMoyen ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Name.Format("%.0f", f25Floor-25) ;  Niveau[IDU].Nom = Name ;
							Niveau[IDU].PrixSurFuture = NiveauFuture ;
							Niveau[IDU].PrixSurIndice = (f25Floor-25) ;  
							Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;
						}
					}
				}
				
				// Récupération des autres niveaux ronds Indice     // IDU : 466 à 553 (7000 à 9950)		 // IDU : 555 à 579 (6950 à 6050) 		// IDU : 581 à 641 (10000 à 12000 )		// IDU : 643 à 699 (12050 à 14000 )			// penser à augmenter NbNiveaux si on dépasse 700
				{
					// Récupération des niveaux forts : grosso modo les multiples de 100 et 250
					{
						{	IDU = 666 ;  Niveau[IDU].PrixSurIndice = 14000 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 665 ;  Niveau[IDU].PrixSurIndice = 13900 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 664 ;  Niveau[IDU].PrixSurIndice = 13800 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 663 ;  Niveau[IDU].PrixSurIndice = 13750 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 662 ;  Niveau[IDU].PrixSurIndice = 13700 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 661 ;  Niveau[IDU].PrixSurIndice = 13600 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 660 ;  Niveau[IDU].PrixSurIndice = 13500 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 659 ;  Niveau[IDU].PrixSurIndice = 13400 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 658 ;  Niveau[IDU].PrixSurIndice = 13300 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 657 ;  Niveau[IDU].PrixSurIndice = 13250 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 656 ;  Niveau[IDU].PrixSurIndice = 13200 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 655 ;  Niveau[IDU].PrixSurIndice = 13100 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 654 ;  Niveau[IDU].PrixSurIndice = 13000 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 653 ;  Niveau[IDU].PrixSurIndice = 12900 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 652 ;  Niveau[IDU].PrixSurIndice = 12800 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 651 ;  Niveau[IDU].PrixSurIndice = 12750 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 650 ;  Niveau[IDU].PrixSurIndice = 12700 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 649 ;  Niveau[IDU].PrixSurIndice = 12600 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 648 ;  Niveau[IDU].PrixSurIndice = 12500 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 647 ;  Niveau[IDU].PrixSurIndice = 12400 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 646 ;  Niveau[IDU].PrixSurIndice = 12300 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 645 ;  Niveau[IDU].PrixSurIndice = 12250 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 644 ;  Niveau[IDU].PrixSurIndice = 12200 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 643 ;  Niveau[IDU].PrixSurIndice = 12100 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 641 ;  Niveau[IDU].PrixSurIndice = 12000 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 638 ;  Niveau[IDU].PrixSurIndice = 11900 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 635 ;  Niveau[IDU].PrixSurIndice = 11800 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 634 ;  Niveau[IDU].PrixSurIndice = 11750 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 633 ;  Niveau[IDU].PrixSurIndice = 11700 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 628 ;  Niveau[IDU].PrixSurIndice = 11600 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 626 ;  Niveau[IDU].PrixSurIndice = 11500 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 623 ;  Niveau[IDU].PrixSurIndice = 11400 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 618 ;  Niveau[IDU].PrixSurIndice = 11300 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 617 ;  Niveau[IDU].PrixSurIndice = 11250 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 616 ;  Niveau[IDU].PrixSurIndice = 11200 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 612 ;  Niveau[IDU].PrixSurIndice = 11100 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 609 ;  Niveau[IDU].PrixSurIndice = 11000 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 607 ;  Niveau[IDU].PrixSurIndice = 10900 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 604 ;  Niveau[IDU].PrixSurIndice = 10800 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 603 ;  Niveau[IDU].PrixSurIndice = 10750 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 602 ;  Niveau[IDU].PrixSurIndice = 10700 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 597 ;  Niveau[IDU].PrixSurIndice = 10600 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 595 ;  Niveau[IDU].PrixSurIndice = 10500 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 593 ;  Niveau[IDU].PrixSurIndice = 10400 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 588 ;  Niveau[IDU].PrixSurIndice = 10300 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 587 ;  Niveau[IDU].PrixSurIndice = 10250 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 586 ;  Niveau[IDU].PrixSurIndice = 10200 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 583 ;  Niveau[IDU].PrixSurIndice = 10100 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 581 ;  Niveau[IDU].PrixSurIndice = 10000 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						// {	IDU = 552 ;  Niveau[IDU].PrixSurIndice = 9900 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						// {	IDU = 548 ;  Niveau[IDU].PrixSurIndice = 9800 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						// {	IDU = 547 ;  Niveau[IDU].PrixSurIndice = 9750 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						// {	IDU = 546 ;  Niveau[IDU].PrixSurIndice = 9700 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						// {	IDU = 542 ;  Niveau[IDU].PrixSurIndice = 9600 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						// {	IDU = 540 ;  Niveau[IDU].PrixSurIndice = 9500 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						// {	IDU = 538 ;  Niveau[IDU].PrixSurIndice = 9400 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						// {	IDU = 534 ;  Niveau[IDU].PrixSurIndice = 9300 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						// {	IDU = 533 ;  Niveau[IDU].PrixSurIndice = 9250 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						// {	IDU = 532 ;  Niveau[IDU].PrixSurIndice = 9200 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						// {	IDU = 529 ;  Niveau[IDU].PrixSurIndice = 9100 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						// {	IDU = 527 ;  Niveau[IDU].PrixSurIndice = 9000 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
					}
					
					
					// Récupération des niveaux 50
					if (NiveauxIndices_50_Input.GetYesNo())
					{
						{	IDU = 683 ;  Niveau[IDU].PrixSurIndice = 13950 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 682 ;  Niveau[IDU].PrixSurIndice = 13850 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 681 ;  Niveau[IDU].PrixSurIndice = 13650 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 680 ;  Niveau[IDU].PrixSurIndice = 13550 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 679 ;  Niveau[IDU].PrixSurIndice = 13450 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 678 ;  Niveau[IDU].PrixSurIndice = 13350 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 677 ;  Niveau[IDU].PrixSurIndice = 13150 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 676 ;  Niveau[IDU].PrixSurIndice = 13050 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 675 ;  Niveau[IDU].PrixSurIndice = 12950 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 674 ;  Niveau[IDU].PrixSurIndice = 12850 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 673 ;  Niveau[IDU].PrixSurIndice = 12650 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 672 ;  Niveau[IDU].PrixSurIndice = 12550 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 671 ;  Niveau[IDU].PrixSurIndice = 12450 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 670 ;  Niveau[IDU].PrixSurIndice = 12350 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 669 ;  Niveau[IDU].PrixSurIndice = 12150 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 668 ;  Niveau[IDU].PrixSurIndice = 12050 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 639 ;  Niveau[IDU].PrixSurIndice = 11950 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 636 ;  Niveau[IDU].PrixSurIndice = 11850 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 631 ;  Niveau[IDU].PrixSurIndice = 11650 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 627 ;  Niveau[IDU].PrixSurIndice = 11550 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 625 ;  Niveau[IDU].PrixSurIndice = 11450 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 621 ;  Niveau[IDU].PrixSurIndice = 11350 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 615 ;  Niveau[IDU].PrixSurIndice = 11150 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 611 ;  Niveau[IDU].PrixSurIndice = 11050 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 608 ;  Niveau[IDU].PrixSurIndice = 10950 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 605 ;  Niveau[IDU].PrixSurIndice = 10850 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 599 ;  Niveau[IDU].PrixSurIndice = 10650 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 596 ;  Niveau[IDU].PrixSurIndice = 10550 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 594 ;  Niveau[IDU].PrixSurIndice = 10450 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 591 ;  Niveau[IDU].PrixSurIndice = 10350 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 585 ;  Niveau[IDU].PrixSurIndice = 10150 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 582 ;  Niveau[IDU].PrixSurIndice = 10050 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						// {	IDU = 553 ;  Niveau[IDU].PrixSurIndice = 9950 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						// {	IDU = 549 ;  Niveau[IDU].PrixSurIndice = 9850 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						// {	IDU = 544 ;  Niveau[IDU].PrixSurIndice = 9650 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						// {	IDU = 541 ;  Niveau[IDU].PrixSurIndice = 9550 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						// {	IDU = 539 ;  Niveau[IDU].PrixSurIndice = 9450 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						// {	IDU = 536 ;  Niveau[IDU].PrixSurIndice = 9350 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						// {	IDU = 531 ;  Niveau[IDU].PrixSurIndice = 9150 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						// {	IDU = 528 ;  Niveau[IDU].PrixSurIndice = 9050 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
					}
					
					if (NiveauxIndices_moyens_Input.GetYesNo())
					{
						{	IDU = 699 ;  Niveau[IDU].PrixSurIndice = 13875 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 698 ;  Niveau[IDU].PrixSurIndice = 13666 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 697 ;  Niveau[IDU].PrixSurIndice = 13625 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 695 ;  Niveau[IDU].PrixSurIndice = 13375 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 694 ;  Niveau[IDU].PrixSurIndice = 13333 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 693 ;  Niveau[IDU].PrixSurIndice = 13125 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 690 ;  Niveau[IDU].PrixSurIndice = 12875 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 696 ;  Niveau[IDU].PrixSurIndice = 12857.14 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 689 ;  Niveau[IDU].PrixSurIndice = 12666 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 688 ;  Niveau[IDU].PrixSurIndice = 12625 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 687 ;  Niveau[IDU].PrixSurIndice = 12375 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 686 ;  Niveau[IDU].PrixSurIndice = 12333 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						// {	IDU = 692 ;  Niveau[IDU].PrixSurIndice = 12222 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 685 ;  Niveau[IDU].PrixSurIndice = 12125 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 637 ;  Niveau[IDU].PrixSurIndice = 11875 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 632 ;  Niveau[IDU].PrixSurIndice = 11666 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 629 ;  Niveau[IDU].PrixSurIndice = 11625 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						// {	IDU = 624 ;  Niveau[IDU].PrixSurIndice = 11428.57 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 622 ;  Niveau[IDU].PrixSurIndice = 11375 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 619 ;  Niveau[IDU].PrixSurIndice = 11333 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 614 ;  Niveau[IDU].PrixSurIndice = 11125 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						// {	IDU = 613 ;  Niveau[IDU].PrixSurIndice = 11111 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 606 ;  Niveau[IDU].PrixSurIndice = 10875 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 601 ;  Niveau[IDU].PrixSurIndice = 10666 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 598 ;  Niveau[IDU].PrixSurIndice = 10625 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 592 ;  Niveau[IDU].PrixSurIndice = 10375 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 589 ;  Niveau[IDU].PrixSurIndice = 10333 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 584 ;  Niveau[IDU].PrixSurIndice = 10125 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						// {	IDU = 551 ;  Niveau[IDU].PrixSurIndice = 9875 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						// {	IDU = 545 ;  Niveau[IDU].PrixSurIndice = 9666 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						// {	IDU = 543 ;  Niveau[IDU].PrixSurIndice = 9625 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						// {	IDU = 537 ;  Niveau[IDU].PrixSurIndice = 9375 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						// {	IDU = 535 ;  Niveau[IDU].PrixSurIndice = 9333 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						// {	IDU = 530 ;  Niveau[IDU].PrixSurIndice = 9125 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
					}
					
						/* {	IDU = 526 ;  Niveau[IDU].PrixSurIndice = 8950 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 525 ;  Niveau[IDU].PrixSurIndice = 8900 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 524 ;  Niveau[IDU].PrixSurIndice = 8888 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 523 ;  Niveau[IDU].PrixSurIndice = 8875 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 522 ;  Niveau[IDU].PrixSurIndice = 8850 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 521 ;  Niveau[IDU].PrixSurIndice = 8800 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 520 ;  Niveau[IDU].PrixSurIndice = 8750 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 519 ;  Niveau[IDU].PrixSurIndice = 8700 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 518 ;  Niveau[IDU].PrixSurIndice = 8666 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 517 ;  Niveau[IDU].PrixSurIndice = 8650 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 516 ;  Niveau[IDU].PrixSurIndice = 8625 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 515 ;  Niveau[IDU].PrixSurIndice = 8600 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 514 ;  Niveau[IDU].PrixSurIndice = 8571.43 ;  Name.Format("%.2f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 513 ;  Niveau[IDU].PrixSurIndice = 8550 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 512 ;  Niveau[IDU].PrixSurIndice = 8500 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 511 ;  Niveau[IDU].PrixSurIndice = 8450 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 509 ;  Niveau[IDU].PrixSurIndice = 8400 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 508 ;  Niveau[IDU].PrixSurIndice = 8375 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 507 ;  Niveau[IDU].PrixSurIndice = 8350 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 506 ;  Niveau[IDU].PrixSurIndice = 8333 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 505 ;  Niveau[IDU].PrixSurIndice = 8300 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 504 ;  Niveau[IDU].PrixSurIndice = 8250 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 503 ;  Niveau[IDU].PrixSurIndice = 8200 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 502 ;  Niveau[IDU].PrixSurIndice = 8150 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 501 ;  Niveau[IDU].PrixSurIndice = 8125 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 499 ;  Niveau[IDU].PrixSurIndice = 8100 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 498 ;  Niveau[IDU].PrixSurIndice = 8050 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 497 ;  Niveau[IDU].PrixSurIndice = 8000 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 496 ;  Niveau[IDU].PrixSurIndice = 7950 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 495 ;  Niveau[IDU].PrixSurIndice = 7900 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 494 ;  Niveau[IDU].PrixSurIndice = 7875 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 493 ;  Niveau[IDU].PrixSurIndice = 7850 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 492 ;  Niveau[IDU].PrixSurIndice = 7800 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 491 ;  Niveau[IDU].PrixSurIndice = 7777 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 489 ;  Niveau[IDU].PrixSurIndice = 7750 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 488 ;  Niveau[IDU].PrixSurIndice = 7700 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 487 ;  Niveau[IDU].PrixSurIndice = 7666 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 486 ;  Niveau[IDU].PrixSurIndice = 7650 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 485 ;  Niveau[IDU].PrixSurIndice = 7625 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 484 ;  Niveau[IDU].PrixSurIndice = 7600 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 483 ;  Niveau[IDU].PrixSurIndice = 7550 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 482 ;  Niveau[IDU].PrixSurIndice = 7500 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 481 ;  Niveau[IDU].PrixSurIndice = 7450 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 479 ;  Niveau[IDU].PrixSurIndice = 7400 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 478 ;  Niveau[IDU].PrixSurIndice = 7375 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 477 ;  Niveau[IDU].PrixSurIndice = 7350 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 476 ;  Niveau[IDU].PrixSurIndice = 7333 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 475 ;  Niveau[IDU].PrixSurIndice = 7300 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 474 ;  Niveau[IDU].PrixSurIndice = 7250 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 473 ;  Niveau[IDU].PrixSurIndice = 7200 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 472 ;  Niveau[IDU].PrixSurIndice = 7150 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 471 ;  Niveau[IDU].PrixSurIndice = 7142.86 ;  Name.Format("%.2f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 469 ;  Niveau[IDU].PrixSurIndice = 7125 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 468 ;  Niveau[IDU].PrixSurIndice = 7100 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 467 ;  Niveau[IDU].PrixSurIndice = 7050 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 466 ;  Niveau[IDU].PrixSurIndice = 7000 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 555 ;  Niveau[IDU].PrixSurIndice = 6950 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 556 ;  Niveau[IDU].PrixSurIndice = 6900 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 557 ;  Niveau[IDU].PrixSurIndice = 6875 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 558 ;  Niveau[IDU].PrixSurIndice = 6850 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 559 ;  Niveau[IDU].PrixSurIndice = 6800 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 560 ;  Niveau[IDU].PrixSurIndice = 6750 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 561 ;  Niveau[IDU].PrixSurIndice = 6700 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 562 ;  Niveau[IDU].PrixSurIndice = 6666 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 563 ;  Niveau[IDU].PrixSurIndice = 6650 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 564 ;  Niveau[IDU].PrixSurIndice = 6625 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 565 ;  Niveau[IDU].PrixSurIndice = 6600 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 566 ;  Niveau[IDU].PrixSurIndice = 6550 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 567 ;  Niveau[IDU].PrixSurIndice = 6500 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 568 ;  Niveau[IDU].PrixSurIndice = 6450 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 569 ;  Niveau[IDU].PrixSurIndice = 6400 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 570 ;  Niveau[IDU].PrixSurIndice = 6375 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 571 ;  Niveau[IDU].PrixSurIndice = 6350 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 572 ;  Niveau[IDU].PrixSurIndice = 6333 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 573 ;  Niveau[IDU].PrixSurIndice = 6300 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 574 ;  Niveau[IDU].PrixSurIndice = 6250 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 575 ;  Niveau[IDU].PrixSurIndice = 6200 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 576 ;  Niveau[IDU].PrixSurIndice = 6150 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 577 ;  Niveau[IDU].PrixSurIndice = 6125 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 578 ;  Niveau[IDU].PrixSurIndice = 6100 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  }
						{	IDU = 579 ;  Niveau[IDU].PrixSurIndice = 6050 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].IdentifiantUnique = IDU ; Niveau[IDU].Nom = Name ;  Niveau[IDU].PrixSurFuture = Niveau[IDU].PrixSurIndice + fSpreadMoyen ;  Niveau[IDU].Nature = 0 ;  Niveau[IDU].SousNature = CalculSousNiveau(Niveau[IDU].PrixSurIndice) ;  } */

				}
			
			}
		
		}
		
		
		// Tri puis utilisation des niveaux triés
		{
			// tri de Niveau du prix le plus bas au prix le plus élevé 
			std::sort ( Niveau.begin(), Niveau.end(), InferieurA_PrixSurFuture ) ;    // note :   le prédicat " StructureNiveau::operator> " ne fonctionne pas, car le compilateur refuse de le prendre
			
			// trouver le prix le plus bas (les entrées de l'array "Niveau" situées avant Iterateur_PremiereLigneNonNulle sont non initialisées)
			const std::array<StructureNiveau,NbNiveaux>::iterator Iterateur_PremiereLigneNonNulle = std::find_if (Niveau.begin(), Niveau.end(), PrixSurFutureEstNonNul);   // itérateur sur la première instance qui matche PrixSurFutureEstNonNul
			
			// trouver le niveau le plus proche de NiveauFuture de la study active : Iterateur_NiveauLePlusProcheDeNiveauCibleActif
			NiveauCibleActif = NiveauFutureDeLaStudyActive (ptrsc) ;  // récupération de la valeur de NiveauCibleActif sur le graphe 16-ticks
			const std::array<StructureNiveau,NbNiveaux>::iterator Iterateur_NiveauLePlusProcheDeNiveauCibleActif = std::min_element(Iterateur_PremiereLigneNonNulle,Niveau.end(), EstPlusProcheQue) ;    // ForwardIterator min_element (ForwardIterator first, ForwardIterator last, Compare comp);
			
			// Calcul de Iterateur_Niveau_ApresClic
			std::array<StructureNiveau,NbNiveaux>::iterator Iterateur_Niveau_ApresClic;
			if ( fabs ( Iterateur_NiveauLePlusProcheDeNiveauCibleActif->PrixSurFuture - NiveauCibleActif ) < 0.25 )   // le NiveauLePlusProcheDeNiveauCibleActif se trouve à moins de 0.25 point (1 pas) de NiveauCibleActif  =>  on considère que c'est le même niveau  =>  on monte d'un niveau (Niv +) ou on baisse d'un niveau (Niv -)
			{
				if ( sc.MenuEventID == ACS_BUTTON_109  or  External_Input_ACS_BUTTON_109.GetInt() != 0 )  // Bouton "Niv -" appuyé
				{	
					int NiveauxDeplaces = max(1, min(20, External_Input_ACS_BUTTON_109.GetInt() ) ) ; 		// on se déplace entre 1 et 20 niveaux
					External_Input_ACS_BUTTON_109.SetInt(0) ; // on RAZ External_Input_ACS_BUTTON_109		
					
					for ( int Deplacement = 1 ; Deplacement <= NiveauxDeplaces ; Deplacement ++ )
					{
						// Déplacement de l'itérateur
						Iterateur_Niveau_ApresClic= Iterateur_NiveauLePlusProcheDeNiveauCibleActif - Deplacement ;  
						// On vérifie qu'on n'est pas passé sous la valeur min
						if ( Iterateur_Niveau_ApresClic == Iterateur_PremiereLigneNonNulle - 1 )    // lorsqu'on arrive sur la première valeur nulle, on bascule sur la dernière valeur du tableau
						{	Iterateur_Niveau_ApresClic = Niveau.end() - 1 ;  break ;  }    // Niveau.end() est en dehors de Niveau		// on break ici, car un déplacement supplémentaire ferait sortir des limites de array
						// On vérifie qu'on a bougé, pour éviter qu'on s'embourbe sur un prix qui contient plusieurs niveaux de même valeur
						while ( Iterateur_Niveau_ApresClic->PrixSurFuture == PrixSurFuture_ApresClic.GetFloat() )
						{
							Iterateur_Niveau_ApresClic= Iterateur_Niveau_ApresClic - 1 ;  
							// on vérifie qu'on n'est pas passé sous la valeur min
							if ( Iterateur_Niveau_ApresClic == Iterateur_PremiereLigneNonNulle - 1 )    // lorsqu'on arrive sur la première valeur nulle, on bascule sur la dernière valeur du tableau
							{	Iterateur_Niveau_ApresClic = Niveau.end() - 1 ;  break ;  }    // Niveau.end() est en dehors de Niveau
						}
					}
				}
				else if ( sc.MenuEventID == ACS_BUTTON_110  or  External_Input_ACS_BUTTON_110.GetInt() != 0 )  // Bouton "Niv +" appuyé
				{
					int NiveauxDeplaces = max(1, min(20, External_Input_ACS_BUTTON_110.GetInt() ) ) ; 		// on se déplace entre 1 et 20 niveaux
					External_Input_ACS_BUTTON_110.SetInt(0) ; // on RAZ External_Input_ACS_BUTTON_110		
					
					for ( int Deplacement = 1 ; Deplacement <= NiveauxDeplaces ; Deplacement ++ )
					{
						// Déplacement de l'itérateur
						Iterateur_Niveau_ApresClic= Iterateur_NiveauLePlusProcheDeNiveauCibleActif + Deplacement ;  
						// On vérifie qu'on n'est pas sorti du tableau // Niveau.end() est en dehors de Niveau
						if ( Iterateur_Niveau_ApresClic == Niveau.end() )   // lorsqu'on sort du tableau Niveau, on bascule sur la première ligne non nulle
						{	Iterateur_Niveau_ApresClic = Iterateur_PremiereLigneNonNulle ;  break ;  }    
						// On vérifie qu'on a bougé, pour éviter qu'on s'embourbe sur un prix qui contient plusieurs niveaux de même valeur
						while ( Iterateur_Niveau_ApresClic->PrixSurFuture == PrixSurFuture_ApresClic.GetFloat()  ) 
						{
							Iterateur_Niveau_ApresClic= Iterateur_Niveau_ApresClic + 1 ;  
							// On vérifie qu'on n'est pas sorti du tableau 	// Niveau.end() est en dehors de Niveau
							if ( Iterateur_Niveau_ApresClic == Niveau.end() )   // lorsqu'on sort du tableau Niveau, on bascule sur la première ligne non nulle
							{	Iterateur_Niveau_ApresClic = Iterateur_PremiereLigneNonNulle ;  break ;  }    
						}
					}
 				}
			}			
			else  // les niveaux sont distants de plus de 0.25 tick  => NiveauCibleActif est remplacé par  NiveauLePlusProcheDeNiveauCibleActif
			{	Iterateur_Niveau_ApresClic = Iterateur_NiveauLePlusProcheDeNiveauCibleActif ;  }
		
			// Liste des inputs est mise à jour
			IdentifiantUnique_ApresClic.SetInt	(Iterateur_Niveau_ApresClic->IdentifiantUnique);
			Nom_ApresClic.SetString				(Iterateur_Niveau_ApresClic->Nom);
			PrixSurFuture_ApresClic.SetFloat	(Iterateur_Niveau_ApresClic->PrixSurFuture);
			PrixSurindice_ApresClic.SetFloat	(Iterateur_Niveau_ApresClic->PrixSurIndice);
			Nature_ApresClic.SetInt					(Iterateur_Niveau_ApresClic->Nature);
			SousNature_ApresClic.SetInt			(Iterateur_Niveau_ApresClic->SousNature);
			RangePrecedent_ApresClic.SetFloat	(Iterateur_Niveau_ApresClic->RangePrecedent);
		}
		
	}  
	else
	{	sc.UpdateAlways = 0 ;  }
	
	
	
	// GenerationFichier_csv
	if ( GenerationFichier_csv.GetYesNo()  or  sc.MenuEventID == ACS_BUTTON_111 )  
	{ 
		// RAZ de l'input GenerationFichier_csv
		GenerationFichier_csv.SetYesNo(0) ;
		
		// Tableau de niveaux (le constructeur par défaut est appelé)
		std::array<StructureNiveau,NbNiveaux> Niveau ;
		
		// Variables /////////////////////////////////////////////////////////////////////////////////////////////
		s_sc* const ptrsc = &sc;
		
		
		// Récupération des niveaux
		{
			// Déclaration / initialisation des variables du bloc de récupération
			SCString Name ; 
			int StudyID ;   // pour les niveaux future et indice
			int La_Study_Appellee_N_A_Pas_Ete_Trouvee = 0; 
			SCFloatArray ArrayPointPivot, ExtraArrayPointPivot ;    // ArrayPointPivot contient les prix sur future (prix projetés arrondis pour les indices)   // ExtraArrayPointPivot  contient le NiveauIndice avant projection
			int IDU;
			int SubgraphIndex;
			float NiveauFuture;
			float NiveauIndice;
			int ChartNumber_UT16ticks = 6 ;
			int ChartNumber_UT20s = 15 ;
			int ExternalChartIndex = sc.GetContainingIndexForDateTimeIndex(ChartNumber_UT20s, sc.ArraySize-1) ; 
			
			// Récupération des PP Journaliers (Full/Mid) Indice sur le chart en UT20s
			{
				StudyID = sc.GetStudyIDByName(ChartNumber_UT20s, "[6] Pivots Ind J (Full/Mid/Quart)", 0) ;    if ( StudyID == 0 )  { SCString message ;  message.Format("!! ERREUR !! La study appelee ''[6] Pivots Ind J (Full/Mid/Quart)'' n'a pas ete trouvee (StudyID==0).") ;  sc.AddMessageToLog(message, 1) ;  }
				
				{
					IDU = 303 ;
					SubgraphIndex = 3 ;
					ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
					sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
					Name.Format("PP J") ;  Niveau[IDU].Nom = Name ;  
				}
				{
					IDU = 304 ;
					SubgraphIndex = 4 ;
					ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
					sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
					Name.Format("R1 J") ;  Niveau[IDU].Nom = Name ;  
				}
				{
					IDU = 305 ;
					SubgraphIndex = 5 ;
					ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
					sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
					Name.Format("R2 J") ;  Niveau[IDU].Nom = Name ;  
				}
				{
					IDU = 306 ;
					SubgraphIndex = 6 ;
					ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
					sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
					Name.Format("R3 J") ;  Niveau[IDU].Nom = Name ;  
				}
				{
					IDU = 308 ;
					SubgraphIndex = 8 ;
					ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
					sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
					Name.Format("S1 J") ;  Niveau[IDU].Nom = Name ;  
				}
				{
					IDU = 309 ;
					SubgraphIndex = 9 ;
					ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
					sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
					Name.Format("S2 J") ;  Niveau[IDU].Nom = Name ;  
				}
				{
					IDU = 310 ;
					SubgraphIndex = 10 ;
					ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
					sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
					Name.Format("S3 J") ;  Niveau[IDU].Nom = Name ;  
				}
				{
					IDU = 312 ;
					SubgraphIndex = 12 ;
					ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
					sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
					Name.Format("mR1 J") ;  Niveau[IDU].Nom = Name ;  
				}
				{
					IDU = 313 ;
					SubgraphIndex = 13 ;
					ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
					sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
					Name.Format("mR2 J") ;  Niveau[IDU].Nom = Name ;  
				}
				{
					IDU = 314 ;
					SubgraphIndex = 14 ;
					ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
					sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
					Name.Format("mR3 J") ;  Niveau[IDU].Nom = Name ;  
				}
				{
					IDU = 316 ;
					SubgraphIndex = 16 ;
					ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
					sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
					Name.Format("mS1 J") ;  Niveau[IDU].Nom = Name ;  
				}
				{
					IDU = 317 ;
					SubgraphIndex = 17 ;
					ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
					sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
					Name.Format("mS2 J") ;  Niveau[IDU].Nom = Name ;  
				}
				{
					IDU = 318 ;
					SubgraphIndex = 18 ;
					ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
					sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
					Niveau[IDU].IdentifiantUnique = IDU ;
					Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
					Name.Format("mS3 J") ;  Niveau[IDU].Nom = Name ;  
				}
				if ( NiveauxIndices_PHPB_JHM_Precedent_Input.GetYesNo() )				
				{
					{
						IDU = 301 ;
						SubgraphIndex = 1 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("PH JP") ;  Niveau[IDU].Nom = Name ;
					}
					{
						IDU = 302 ;
						SubgraphIndex = 2 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("PB JP") ;  Niveau[IDU].Nom = Name ;
					}
					{
						IDU = 320 ;
						SubgraphIndex = 37 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("Demi Gap J") ;  Niveau[IDU].Nom = Name ;  
					}
				}
				if ( NiveauxIndices_Full4Mid4_Input.GetYesNo() )
				{
					{
						IDU = 307 ;
						SubgraphIndex = 7 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("R4 J") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 311 ;
						SubgraphIndex = 11 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("S4 J") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 319 ;
						SubgraphIndex = 19 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("mS4 J") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 315 ;
						SubgraphIndex = 15 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("mR4 J") ;  Niveau[IDU].Nom = Name ;  
					}
				}

			}
			
			// Récupération des PP Hebdo (Full/Mid/Quart) Indice
			{
				StudyID = sc.GetStudyIDByName(ChartNumber_UT20s, "[4] Pivots Ind H (Full/Mid/Quart)", 0) ;    if ( StudyID == 0 ) { SCString message ;  message.Format("!! ERREUR !! La study appelee ''[4] Pivots Ind H (Full/Mid/Quart)'' n'a pas ete trouvee (StudyID==0).") ;  sc.AddMessageToLog(message, 1) ;  }
				
				// Récupération des Full/Mid/Gap/DemiGap
				{
					{
						IDU = 333 ;
						SubgraphIndex = 3 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("PP H") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 334 ;
						SubgraphIndex = 4 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("R1 H") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 335 ;
						SubgraphIndex = 5 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("R2 H") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 336 ;
						SubgraphIndex = 6 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("R3 H") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 338 ;
						SubgraphIndex = 8 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("S1 H") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 339 ;
						SubgraphIndex = 9 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("S2 H") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 340 ;
						SubgraphIndex = 10 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("S3 H") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 342 ;
						SubgraphIndex = 12 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("mR1 H") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 343 ;
						SubgraphIndex = 13 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("mR2 H") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 344 ;
						SubgraphIndex = 14 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("mR3 H") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 346 ;
						SubgraphIndex = 16 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("mS1 H") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 347 ;
						SubgraphIndex = 17 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("mS2 H") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 348 ;
						SubgraphIndex = 18 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("mS3 H") ;  Niveau[IDU].Nom = Name ;  
					}
					if ( NiveauxIndices_PHPB_JHM_Precedent_Input.GetYesNo() )	
					{
						{
							IDU = 331 ;
							SubgraphIndex = 1 ;
							ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
							sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
							Name.Format("PH HP") ;  Niveau[IDU].Nom = Name ;  
						}
						{
							IDU = 332 ;
							SubgraphIndex = 2 ;
							ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
							sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
							Name.Format("PB HP") ;  Niveau[IDU].Nom = Name ;  
						}
						{
							IDU = 366 ;
							SubgraphIndex = 37 ;
							ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
							sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
							Name.Format("DemiGap H") ;  Niveau[IDU].Nom = Name ;  
						}
					}
					if ( NiveauxIndices_Full4Mid4_Input.GetYesNo() )	
					{
						{
							IDU = 337 ;
							SubgraphIndex = 7 ;
							ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
							sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
							Name.Format("R4 H") ;  Niveau[IDU].Nom = Name ;  
						}
						{
							IDU = 341 ;
							SubgraphIndex = 11 ;
							ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
							sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
							Name.Format("S4 H") ;  Niveau[IDU].Nom = Name ;  
						}
						{
							IDU = 345 ;
							SubgraphIndex = 15 ;
							ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
							sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
							Name.Format("mR4 H") ;  Niveau[IDU].Nom = Name ;  
						}
						{
							IDU = 349 ;
							SubgraphIndex = 19 ;
							ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
							sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
							Name.Format("mS4 H") ;  Niveau[IDU].Nom = Name ;  
						}
					}
				}
				
				// Récupération des Quarts
				{
					/* int TraceDesQuarts ;
					sc.GetChartStudyInputInt(ChartNumber_UT16ticks, StudyID_PrixTypique, 23, TraceDesQuarts) ;     
					if ( TraceDesQuarts )      */
					if ( NiveauxIndices_PPQuartsH_Input.GetYesNo() )
					{
						{
							IDU = 350 ;
							SubgraphIndex = 20 ;
							ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
							sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
							Name.Format("RQ1 H") ;  Niveau[IDU].Nom = Name ;  
						}
						{
							IDU = 351 ;
							SubgraphIndex = 21 ;
							ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
							sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
							Name.Format("RQ2 H") ;  Niveau[IDU].Nom = Name ;  
						}
						{
							IDU = 352 ;
							SubgraphIndex = 22 ;
							ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
							sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
							Name.Format("RQ3 H") ;  Niveau[IDU].Nom = Name ;  
						}
						{
							IDU = 353 ;
							SubgraphIndex = 23 ;
							ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
							sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
							Name.Format("RQ4 H") ;  Niveau[IDU].Nom = Name ;  
						}
						{
							IDU = 354 ;
							SubgraphIndex = 24 ;
							ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
							sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
							Name.Format("RQ5 H") ;  Niveau[IDU].Nom = Name ;  
						}
						{
							IDU = 355 ;
							SubgraphIndex = 25 ;
							ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
							sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
							Name.Format("RQ6 H") ;  Niveau[IDU].Nom = Name ;  
						}
						{
							IDU = 358 ;
							SubgraphIndex = 28 ;
							ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
							sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
							Name.Format("SQ1 H") ;  Niveau[IDU].Nom = Name ;  
						}
						{
							IDU = 359 ;
							SubgraphIndex = 29 ;
							ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
							sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
							Name.Format("SQ2 H") ;  Niveau[IDU].Nom = Name ;  
						}
						{
							IDU = 360 ;
							SubgraphIndex = 30 ;
							ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
							sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
							Name.Format("SQ3 H") ;  Niveau[IDU].Nom = Name ;  
						}
						{
							IDU = 361 ;
							SubgraphIndex = 31 ;
							ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
							sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
							Name.Format("SQ4 H") ;  Niveau[IDU].Nom = Name ;  
						}
						{
							IDU = 362 ;
							SubgraphIndex = 32 ;
							ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
							sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
							Name.Format("SQ5 H") ;  Niveau[IDU].Nom = Name ;  
						}
						{
							IDU = 363 ;
							SubgraphIndex = 33 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
							sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
							Name.Format("SQ6 H") ;  Niveau[IDU].Nom = Name ;  
						}
						// if ( TraceDesFull4Mid4 )
						if ( NiveauxIndices_Full4Mid4_Input.GetYesNo() )	
						{
							{
								IDU = 356 ;
								SubgraphIndex = 26 ;
								ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
								sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
								Niveau[IDU].IdentifiantUnique = IDU ;
								Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
								Name.Format("RQ7 H") ;  Niveau[IDU].Nom = Name ;  
							}
							{
								IDU = 357 ;
								SubgraphIndex = 27 ;
								ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
								sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
								Niveau[IDU].IdentifiantUnique = IDU ;
								Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
								Name.Format("RQ8 H") ;  Niveau[IDU].Nom = Name ;  
							}
							{
								IDU = 364 ;
								SubgraphIndex = 34 ;
								ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
								sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
								Niveau[IDU].IdentifiantUnique = IDU ;
								Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
								Name.Format("SQ7 H") ;  Niveau[IDU].Nom = Name ;  
							}
							{
								IDU = 365 ;
								SubgraphIndex = 35 ;
								ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
								sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
								Niveau[IDU].IdentifiantUnique = IDU ;
								Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
								Name.Format("SQ8 H") ;  Niveau[IDU].Nom = Name ;  
							}
						}
					}
				}
			}
			
			// Récupération des PP Mensuels (Full/Mid/Quart) Indice
			{
				StudyID = sc.GetStudyIDByName(ChartNumber_UT20s, "[1] Pivots Ind M (Full/Mid/Quart)", 0) ;    if ( StudyID == 0 )  { SCString message ;  message.Format("!! ERREUR !! La study appelee ''[1] Pivots Ind M (Full/Mid/Quart)'' n'a pas ete trouvee (StudyID==0).") ;  sc.AddMessageToLog(message, 1) ;  }
				
				// Récupération des Full/Mid/Gap/DemiGap
				{
					{
						IDU = 373 ;
						SubgraphIndex = 3 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("PP M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 374 ;
						SubgraphIndex = 4 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("R1 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 375 ;
						SubgraphIndex = 5 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("R2 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 376 ;
						SubgraphIndex = 6 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("R3 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 378 ;
						SubgraphIndex = 8 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("S1 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 379 ;
						SubgraphIndex = 9 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("S2 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 380 ;
						SubgraphIndex = 10 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("S3 M") ;  Niveau[IDU].Nom = Name ;  
					}
					
					// Suppression des Mid mensuels de fichier.csv tant que les backtests n'auront pas validé leur pertinence
					/* {
						IDU = 382 ;
						SubgraphIndex = 12 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("mR1 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 383 ;
						SubgraphIndex = 13 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("mR2 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 384 ;
						SubgraphIndex = 14 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("mR3 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 386 ;
						SubgraphIndex = 16 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("mS1 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 387 ;
						SubgraphIndex = 17 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("mS2 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 388 ;
						SubgraphIndex = 18 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("mS3 M") ;  Niveau[IDU].Nom = Name ;  
					} */
					
					if ( NiveauxIndices_PHPB_JHM_Precedent_Input.GetYesNo() )	
					{
						{
							IDU = 370 ;
							SubgraphIndex = 1 ;
							ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
							sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
							Name.Format("PH Absolu") ;  Niveau[IDU].Nom = Name ;  
						}
						{
							IDU = 371 ;
							SubgraphIndex = 2 ;
							ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
							sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
							Name.Format("PH MP") ;  Niveau[IDU].Nom = Name ;   
						}	
						{
							IDU = 407 ;   
							SubgraphIndex = 38 ;
							ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
							sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
							Name.Format("PB MP") ;  Niveau[IDU].Nom = Name ;  
						}
						{
						IDU = 406 ;   
						SubgraphIndex = 37 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("DemiGap M") ;  Niveau[IDU].Nom = Name ;  
						}
					}

					if ( NiveauxIndices_Full4Mid4_Input.GetYesNo() )
					{
						{
							IDU = 377 ;
							SubgraphIndex = 7 ;
							ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
							sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
							Name.Format("R4 M") ;  Niveau[IDU].Nom = Name ;  
						}
						{
							IDU = 381 ;
							SubgraphIndex = 11 ;
							ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
							sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
							Name.Format("S4 M") ;  Niveau[IDU].Nom = Name ;  
						}
						{
							IDU = 385 ;
							SubgraphIndex = 15 ;
							ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
							sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
							Name.Format("mR4 M") ;  Niveau[IDU].Nom = Name ;  
						}
						{
							IDU = 389 ;
							SubgraphIndex = 19 ;
							ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
							sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
							Name.Format("mS4 M") ;  Niveau[IDU].Nom = Name ;  
						}
					}
				}
				

				// Récupération des Quarts
				if ( NiveauxIndices_PPQuartsM_Input.GetYesNo() )
				{
					{
						IDU = 390 ;
						SubgraphIndex = 20 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("RQ1 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 391 ;
						SubgraphIndex = 21 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("RQ2 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 392 ;
						SubgraphIndex = 22 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("RQ3 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 393 ;
						SubgraphIndex = 23 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("RQ4 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 394 ;
						SubgraphIndex = 24 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("RQ5 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 395 ;
						SubgraphIndex = 25 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("RQ6 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 398 ;
						SubgraphIndex = 28 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("SQ1 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 399 ;
						SubgraphIndex = 29 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("SQ2 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 400 ;
						SubgraphIndex = 30 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("SQ3 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 401 ;
						SubgraphIndex = 31 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("SQ4 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 402 ;
						SubgraphIndex = 32 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("SQ5 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 403 ;
						SubgraphIndex = 33 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("SQ6 M") ;  Niveau[IDU].Nom = Name ;  
					}
					// if ( TraceDesFull4Mid4 )			
					if ( NiveauxIndices_Full4Mid4_Input.GetYesNo() )
					{
						{
							IDU = 404 ;
							SubgraphIndex = 34 ;
							ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
							sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
							Name.Format("SQ7 M") ;  Niveau[IDU].Nom = Name ;  
						}
						{
							IDU = 405 ;
							SubgraphIndex = 35 ;
							ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
							sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
							Name.Format("SQ8 M") ;  Niveau[IDU].Nom = Name ;  
						}
						{
							IDU = 396 ;
							SubgraphIndex = 26 ;
							ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
							sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
							Name.Format("RQ7 M") ;  Niveau[IDU].Nom = Name ;  
						}
						{
							IDU = 397 ;
							SubgraphIndex = 27 ;
							ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
							sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
							Niveau[IDU].IdentifiantUnique = IDU ;
							Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
							Name.Format("RQ8 M") ;  Niveau[IDU].Nom = Name ;  
						}
					}
				}
				
			}
			
			// Récupération des PP Mensuels Eighth Indice
			{
				/* StudyID = sc.GetStudyIDByName(ChartNumber_UT20s, "[2] Pivots Ind M (Eighth)", 0) ;    if ( StudyID == 0 )  { SCString message ;  message.Format("!! ERREUR !! La study appelee ''[2] Pivots Ind M (Eighth)'' n'a pas ete trouvee (StudyID==0).") ;  sc.AddMessageToLog(message, 1) ;  }
				
				int TraceDesEighths ;
				sc.GetChartStudyInputInt(ChartNumber_UT16ticks, StudyID_PrixTypique, 24, TraceDesEighths) ;     
				if ( TraceDesEighths )      */
				if ( NiveauxIndices_PPEighthsM_Input.GetYesNo() )
				{
					{
						IDU = 410 ;
						SubgraphIndex = 0 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("RE1 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 411 ;
						SubgraphIndex = 1 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("RE2 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 412 ;
						SubgraphIndex = 2 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("RE3 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 413 ;
						SubgraphIndex = 3 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("RE4 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 414 ;
						SubgraphIndex = 4 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("RE5 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 415 ;
						SubgraphIndex = 5 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("RE6 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 416 ;
						SubgraphIndex = 6 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("RE7 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 417 ;
						SubgraphIndex = 7 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("RE8 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 418 ;
						SubgraphIndex = 8 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("RE9 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 419 ;
						SubgraphIndex = 9 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("RE10 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 420 ;
						SubgraphIndex = 10 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("RE11 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 421 ;
						SubgraphIndex = 11 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("RE12 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 422 ;
						SubgraphIndex = 12 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("RE13 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 423 ;
						SubgraphIndex = 13 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("RE14 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 424 ;
						SubgraphIndex = 14 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("RE15 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 425 ;
						SubgraphIndex = 15 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("RE16 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 426 ;
						SubgraphIndex = 16 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("SE1 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 427 ;
						SubgraphIndex = 17 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("SE2 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 428 ;
						SubgraphIndex = 18 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("SE3 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 429 ;
						SubgraphIndex = 19 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("SE4 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 430 ;
						SubgraphIndex = 20 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("SE5 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 431 ;
						SubgraphIndex = 21 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("SE6 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 432 ;
						SubgraphIndex = 22 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("SE7 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 433 ;
						SubgraphIndex = 23 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("SE8 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 434 ;
						SubgraphIndex = 24 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("SE9 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 435 ;
						SubgraphIndex = 25 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("SE10 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 436 ;
						SubgraphIndex = 26 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("SE11 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 437 ;
						SubgraphIndex = 27 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("SE12 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 438 ;
						SubgraphIndex = 28 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("SE13 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 439 ;
						SubgraphIndex = 29 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("SE14 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 440 ;
						SubgraphIndex = 30 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("SE15 M") ;  Niveau[IDU].Nom = Name ;  
					}
					{
						IDU = 441 ;
						SubgraphIndex = 31 ;
						ChartInputs_ExternalArray.SetChartStudySubgraphValues(ChartNumber_UT20s, StudyID, SubgraphIndex)  ;
						sc.GetStudyArrayFromChartUsingID(ChartInputs_ExternalArray.GetChartStudySubgraphValues(), ArrayPointPivot) ;
						Niveau[IDU].IdentifiantUnique = IDU ;
						Niveau[IDU].PrixSurIndice = ArrayPointPivot[ExternalChartIndex] ;
						Name.Format("SE16 M") ;  Niveau[IDU].Nom = Name ;  
					}
				}
			}

			
			// Récupération des niveaux ronds Indice 
			{
				// Récupération des niveaux forts : grosso modo les multiples de 100 et 250
				{
					IDU = 666 ;  Niveau[IDU].PrixSurIndice = 14000 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 665 ;  Niveau[IDU].PrixSurIndice = 13900 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 664 ;  Niveau[IDU].PrixSurIndice = 13800 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 663 ;  Niveau[IDU].PrixSurIndice = 13750 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 662 ;  Niveau[IDU].PrixSurIndice = 13700 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 661 ;  Niveau[IDU].PrixSurIndice = 13600 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 660 ;  Niveau[IDU].PrixSurIndice = 13500 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 659 ;  Niveau[IDU].PrixSurIndice = 13400 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 658 ;  Niveau[IDU].PrixSurIndice = 13300 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 657 ;  Niveau[IDU].PrixSurIndice = 13250 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 656 ;  Niveau[IDU].PrixSurIndice = 13200 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 655 ;  Niveau[IDU].PrixSurIndice = 13100 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 654 ;  Niveau[IDU].PrixSurIndice = 13000 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 653 ;  Niveau[IDU].PrixSurIndice = 12900 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 652 ;  Niveau[IDU].PrixSurIndice = 12800 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 651 ;  Niveau[IDU].PrixSurIndice = 12750 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 650 ;  Niveau[IDU].PrixSurIndice = 12700 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 649 ;  Niveau[IDU].PrixSurIndice = 12600 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 648 ;  Niveau[IDU].PrixSurIndice = 12500 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 647 ;  Niveau[IDU].PrixSurIndice = 12400 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 646 ;  Niveau[IDU].PrixSurIndice = 12300 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 645 ;  Niveau[IDU].PrixSurIndice = 12250 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 644 ;  Niveau[IDU].PrixSurIndice = 12200 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 643 ;  Niveau[IDU].PrixSurIndice = 12100 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 641 ;  Niveau[IDU].PrixSurIndice = 12000 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 638 ;  Niveau[IDU].PrixSurIndice = 11900 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 635 ;  Niveau[IDU].PrixSurIndice = 11800 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 634 ;  Niveau[IDU].PrixSurIndice = 11750 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 633 ;  Niveau[IDU].PrixSurIndice = 11700 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 628 ;  Niveau[IDU].PrixSurIndice = 11600 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 626 ;  Niveau[IDU].PrixSurIndice = 11500 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 623 ;  Niveau[IDU].PrixSurIndice = 11400 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 618 ;  Niveau[IDU].PrixSurIndice = 11300 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 617 ;  Niveau[IDU].PrixSurIndice = 11250 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 616 ;  Niveau[IDU].PrixSurIndice = 11200 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 612 ;  Niveau[IDU].PrixSurIndice = 11100 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 609 ;  Niveau[IDU].PrixSurIndice = 11000 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 607 ;  Niveau[IDU].PrixSurIndice = 10900 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 604 ;  Niveau[IDU].PrixSurIndice = 10800 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 603 ;  Niveau[IDU].PrixSurIndice = 10750 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 602 ;  Niveau[IDU].PrixSurIndice = 10700 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 597 ;  Niveau[IDU].PrixSurIndice = 10600 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 595 ;  Niveau[IDU].PrixSurIndice = 10500 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 593 ;  Niveau[IDU].PrixSurIndice = 10400 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 588 ;  Niveau[IDU].PrixSurIndice = 10300 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 587 ;  Niveau[IDU].PrixSurIndice = 10250 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 586 ;  Niveau[IDU].PrixSurIndice = 10200 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 583 ;  Niveau[IDU].PrixSurIndice = 10100 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 581 ;  Niveau[IDU].PrixSurIndice = 10000 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					// IDU = 552 ;  Niveau[IDU].PrixSurIndice = 9900 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					// IDU = 548 ;  Niveau[IDU].PrixSurIndice = 9800 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					// IDU = 547 ;  Niveau[IDU].PrixSurIndice = 9750 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					// IDU = 546 ;  Niveau[IDU].PrixSurIndice = 9700 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					// IDU = 542 ;  Niveau[IDU].PrixSurIndice = 9600 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					// IDU = 540 ;  Niveau[IDU].PrixSurIndice = 9500 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					// IDU = 538 ;  Niveau[IDU].PrixSurIndice = 9400 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					// IDU = 534 ;  Niveau[IDU].PrixSurIndice = 9300 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					// IDU = 533 ;  Niveau[IDU].PrixSurIndice = 9250 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					// IDU = 532 ;  Niveau[IDU].PrixSurIndice = 9200 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					// IDU = 529 ;  Niveau[IDU].PrixSurIndice = 9100 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					// IDU = 527 ;  Niveau[IDU].PrixSurIndice = 9000 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
				}
				
				// Récupération des niveaux 50
				int TraceDes50 ;
				sc.GetChartStudyInputInt(ChartNumber_UT16ticks, StudyID_PrixTypique, 13, TraceDes50) ;     
				if ( TraceDes50 )     
				{
					IDU = 639 ;  Niveau[IDU].PrixSurIndice = 11950 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 636 ;  Niveau[IDU].PrixSurIndice = 11850 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 631 ;  Niveau[IDU].PrixSurIndice = 11650 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 627 ;  Niveau[IDU].PrixSurIndice = 11550 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 625 ;  Niveau[IDU].PrixSurIndice = 11450 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 621 ;  Niveau[IDU].PrixSurIndice = 11350 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 615 ;  Niveau[IDU].PrixSurIndice = 11150 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 611 ;  Niveau[IDU].PrixSurIndice = 11050 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 608 ;  Niveau[IDU].PrixSurIndice = 10950 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 605 ;  Niveau[IDU].PrixSurIndice = 10850 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 599 ;  Niveau[IDU].PrixSurIndice = 10650 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 596 ;  Niveau[IDU].PrixSurIndice = 10550 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 594 ;  Niveau[IDU].PrixSurIndice = 10450 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 591 ;  Niveau[IDU].PrixSurIndice = 10350 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 585 ;  Niveau[IDU].PrixSurIndice = 10150 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 582 ;  Niveau[IDU].PrixSurIndice = 10050 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 553 ;  Niveau[IDU].PrixSurIndice = 9950 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 549 ;  Niveau[IDU].PrixSurIndice = 9850 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 544 ;  Niveau[IDU].PrixSurIndice = 9650 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 541 ;  Niveau[IDU].PrixSurIndice = 9550 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 539 ;  Niveau[IDU].PrixSurIndice = 9450 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 536 ;  Niveau[IDU].PrixSurIndice = 9350 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 531 ;  Niveau[IDU].PrixSurIndice = 9150 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 528 ;  Niveau[IDU].PrixSurIndice = 9050 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
				}
				
				// Récupération des autres niveaux moyens (125, 333, ...) : grosso modo tout ce qui n'est pas un multiple de 50
				int TraceDesNiveauxMoyens ;
				sc.GetChartStudyInputInt(ChartNumber_UT16ticks, StudyID_PrixTypique, 14, TraceDesNiveauxMoyens) ;     
				if ( TraceDesNiveauxMoyens )     
				{
					IDU = 637 ;  Niveau[IDU].PrixSurIndice = 11875 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 632 ;  Niveau[IDU].PrixSurIndice = 11666 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 629 ;  Niveau[IDU].PrixSurIndice = 11625 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					// IDU = 624 ;  Niveau[IDU].PrixSurIndice = 11428.57 ;  Name.Format("%.2f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 622 ;  Niveau[IDU].PrixSurIndice = 11375 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 619 ;  Niveau[IDU].PrixSurIndice = 11333 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 614 ;  Niveau[IDU].PrixSurIndice = 11125 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					// IDU = 613 ;  Niveau[IDU].PrixSurIndice = 11111 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 606 ;  Niveau[IDU].PrixSurIndice = 10875 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 601 ;  Niveau[IDU].PrixSurIndice = 10666 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 598 ;  Niveau[IDU].PrixSurIndice = 10625 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 592 ;  Niveau[IDU].PrixSurIndice = 10375 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 589 ;  Niveau[IDU].PrixSurIndice = 10333 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 584 ;  Niveau[IDU].PrixSurIndice = 10125 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 551 ;  Niveau[IDU].PrixSurIndice = 9875 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 545 ;  Niveau[IDU].PrixSurIndice = 9666 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 543 ;  Niveau[IDU].PrixSurIndice = 9625 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 537 ;  Niveau[IDU].PrixSurIndice = 9375 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 535 ;  Niveau[IDU].PrixSurIndice = 9333 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 530 ;  Niveau[IDU].PrixSurIndice = 9125 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
				}
				
					/* IDU = 526 ;  Niveau[IDU].PrixSurIndice = 8950 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 525 ;  Niveau[IDU].PrixSurIndice = 8900 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					// IDU = xxx ;  Niveau[IDU].PrixSurIndice = 8888 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 524 ;  Niveau[IDU].PrixSurIndice = 8875 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 523 ;  Niveau[IDU].PrixSurIndice = 8850 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 522 ;  Niveau[IDU].PrixSurIndice = 8800 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 521 ;  Niveau[IDU].PrixSurIndice = 8750 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 520 ;  Niveau[IDU].PrixSurIndice = 8700 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 519 ;  Niveau[IDU].PrixSurIndice = 8666 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 518 ;  Niveau[IDU].PrixSurIndice = 8650 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 517 ;  Niveau[IDU].PrixSurIndice = 8625 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 516 ;  Niveau[IDU].PrixSurIndice = 8600 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 515 ;  Niveau[IDU].PrixSurIndice = 8571.43 ;  Name.Format("%.2f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 514 ;  Niveau[IDU].PrixSurIndice = 8550 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 513 ;  Niveau[IDU].PrixSurIndice = 8500 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 512 ;  Niveau[IDU].PrixSurIndice = 8450 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 511 ;  Niveau[IDU].PrixSurIndice = 8400 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 510 ;  Niveau[IDU].PrixSurIndice = 8375 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 509 ;  Niveau[IDU].PrixSurIndice = 8350 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 508 ;  Niveau[IDU].PrixSurIndice = 8333 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 507 ;  Niveau[IDU].PrixSurIndice = 8300 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 506 ;  Niveau[IDU].PrixSurIndice = 8250 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 505 ;  Niveau[IDU].PrixSurIndice = 8200 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 504 ;  Niveau[IDU].PrixSurIndice = 8150 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 503 ;  Niveau[IDU].PrixSurIndice = 8125 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 502 ;  Niveau[IDU].PrixSurIndice = 8100 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 501 ;  Niveau[IDU].PrixSurIndice = 8050 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 500 ;  Niveau[IDU].PrixSurIndice = 8000 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 499 ;  Niveau[IDU].PrixSurIndice = 7950 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 498 ;  Niveau[IDU].PrixSurIndice = 7900 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 497 ;  Niveau[IDU].PrixSurIndice = 7875 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 496 ;  Niveau[IDU].PrixSurIndice = 7850 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 495 ;  Niveau[IDU].PrixSurIndice = 7800 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 494 ;  Niveau[IDU].PrixSurIndice = 7777 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 493 ;  Niveau[IDU].PrixSurIndice = 7750 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 492 ;  Niveau[IDU].PrixSurIndice = 7700 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 491 ;  Niveau[IDU].PrixSurIndice = 7666 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 490 ;  Niveau[IDU].PrixSurIndice = 7650 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 489 ;  Niveau[IDU].PrixSurIndice = 7625 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 488 ;  Niveau[IDU].PrixSurIndice = 7600 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 487 ;  Niveau[IDU].PrixSurIndice = 7550 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 486 ;  Niveau[IDU].PrixSurIndice = 7500 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 485 ;  Niveau[IDU].PrixSurIndice = 7450 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 484 ;  Niveau[IDU].PrixSurIndice = 7400 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 483 ;  Niveau[IDU].PrixSurIndice = 7375 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 482 ;  Niveau[IDU].PrixSurIndice = 7350 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 481 ;  Niveau[IDU].PrixSurIndice = 7333 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 480 ;  Niveau[IDU].PrixSurIndice = 7300 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 479 ;  Niveau[IDU].PrixSurIndice = 7250 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 478 ;  Niveau[IDU].PrixSurIndice = 7200 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 477 ;  Niveau[IDU].PrixSurIndice = 7150 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 476 ;  Niveau[IDU].PrixSurIndice = 7142.86 ;  Name.Format("%.2f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 475 ;  Niveau[IDU].PrixSurIndice = 7125 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name ;
					IDU = 474 ;  Niveau[IDU].PrixSurIndice = 7100 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name;  
					IDU = 555 ;  Niveau[IDU].PrixSurIndice = 7050 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name;  
					IDU = 556 ;  Niveau[IDU].PrixSurIndice = 7000 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name;  
					IDU = 557 ;  Niveau[IDU].PrixSurIndice = 6950 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name;  
					IDU = 558 ;  Niveau[IDU].PrixSurIndice = 6900 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name;  
					IDU = 559 ;  Niveau[IDU].PrixSurIndice = 6875 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name;  
					IDU = 561 ;  Niveau[IDU].PrixSurIndice = 6850 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name;  
					IDU = 562 ;  Niveau[IDU].PrixSurIndice = 6800 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name;  
					IDU = 563 ;  Niveau[IDU].PrixSurIndice = 6750 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name;  
					IDU = 564 ;  Niveau[IDU].PrixSurIndice = 6700 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name;  
					IDU = 565 ;  Niveau[IDU].PrixSurIndice = 6666 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name;  
					IDU = 566 ;  Niveau[IDU].PrixSurIndice = 6650 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name;  
					IDU = 567 ;  Niveau[IDU].PrixSurIndice = 6625 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name;  
					IDU = 568 ;  Niveau[IDU].PrixSurIndice = 6600 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name;  
					IDU = 569 ;  Niveau[IDU].PrixSurIndice = 6550 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name;  
					IDU = 570 ;  Niveau[IDU].PrixSurIndice = 6500 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name;  
					IDU = 571 ;  Niveau[IDU].PrixSurIndice = 6450 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name;  
					IDU = 572 ;  Niveau[IDU].PrixSurIndice = 6400 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name;  
					IDU = 573 ;  Niveau[IDU].PrixSurIndice = 6375 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name;  
					IDU = 574 ;  Niveau[IDU].PrixSurIndice = 6350 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name;  
					IDU = 575 ;  Niveau[IDU].PrixSurIndice = 6333 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name;  
					IDU = 576 ;  Niveau[IDU].PrixSurIndice = 6300 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name;  
					IDU = 577 ;  Niveau[IDU].PrixSurIndice = 6250 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name;  
					IDU = 578 ;  Niveau[IDU].PrixSurIndice = 6200 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name;  
					IDU = 579 ;  Niveau[IDU].PrixSurIndice = 6150 ;  Name.Format("%.0f", Niveau[IDU].PrixSurIndice) ;  Niveau[IDU].Nom = Name;  				 */
			}

			
			// Message dans le log si une study appelée n'a pas été trouvée
			if ( La_Study_Appellee_N_A_Pas_Ete_Trouvee == 1 )
			{	SCString message ;  message.Format("!! ERREUR !! Une study appelee n'a pas ete trouvee (StudyID==0).") ;  sc.AddMessageToLog(message, 1) ;  }
		}
		
		
		// Création du fichier .csv
		{
			// tri de Niveau du prix le plus bas au prix le plus élevé 
			std::sort ( Niveau.begin(), Niveau.end(), InferieurA_PrixSurIndice ) ;    
			
			// trouver le prix le plus bas 
			const std::array<StructureNiveau,NbNiveaux>::iterator Iterateur_PremiereLigneNonNulle = std::find_if (Niveau.begin(), Niveau.end(), PrixSurIndiceEstNonNul);   // itérateur sur la première instance qui matche PrixSurIndiceEstNonNul
			
			/* // concaténer les prix dans un SCString pour les afficher dans le MessageLog
			SCString StringDePrix;  StringDePrix.Format("Prix classes du plus petit au plus grand : ");
			std::array<StructureNiveau,NbNiveaux>::iterator Iterateur = Iterateur_PremiereLigneNonNulle ;   // on utilise Iterateur pour faire la concaténation, car on ne veut pas faire varier Iterateur_PremiereLigneNonNulle
			while ( Iterateur != Niveau.end() )
			{
				StringDePrix.AppendFormat(Iterateur->Nom);
				if ( Iterateur->PrixSurIndice == 0) { StringDePrix.AppendFormat(" Fut ") ;  }  else { StringDePrix.AppendFormat(" Ind ") ; StringDePrix.AppendFormat("[%.2f]", Iterateur->PrixSurIndice) ;  }
				StringDePrix.AppendFormat(" (%.2f) ; ", Iterateur->PrixSurFuture);
				++ Iterateur;
			}
			sc.AddMessageToLog(StringDePrix, 0) ;  */
			
			// suppression du fichier .csv
			remove("Niveaux classes.csv") ;  
			
			// remplissage du fichier .csv
			{
				std::ofstream myStream("Niveaux classes.csv") ;        // il est impossible de créer un fichier en mode fstream  // il faut rajouter " std::ios::out ", ce qui revient à utiliser ofstream
				
				if ( myStream.is_open() ) 	{	SCString message ;  message.Format("Ouverture du fichier : OK") ;  sc.AddMessageToLog(message, 1) ;  }  else {	SCString message ;  message.Format("Ouverture du fichier : erreur") ;  sc.AddMessageToLog(message, 1) ;  }
				
				
				std::array<StructureNiveau,NbNiveaux>::iterator IterateurRemplissage = Niveau.end() ; 
				do
				{
					IterateurRemplissage -- ;
					
					// Création du string PrixSurIndice avec une virgule à la place du point
					std::string PrixSurIndice ;
					{
						// Conversion du float PrixSurIndice en string
						{
							std::ostringstream oss;  // flux pour écrire dans un stream
							oss << IterateurRemplissage->PrixSurIndice;       // par chance PrixSurIndice est affiché avec 6 chiffres sans que j'aie eu à le demander    // pour le modifier : http://www.cplusplus.com/reference/iomanip/setprecision/    http://www.cplusplus.com/reference/ios/ios_base/precision/
							PrixSurIndice = oss.str() ;
							// {	SCString message ;  message.Format(PrixSurIndice.c_str()) ;  sc.AddMessageToLog(message, 0) ;  }
						}
						// Remplacement du '.' dans PrixSurIndice par ','
						{
							std::replace( PrixSurIndice.begin(), PrixSurIndice.end(), '.', ',');
							// {	SCString message ;  message.Format(PrixSurIndice.c_str()) ;  sc.AddMessageToLog(message, 0) ;  }
						}
					}
					
					myStream << IterateurRemplissage->Nom << ";" << PrixSurIndice <<   "\n\n" ; 
				}
				while ( IterateurRemplissage != Iterateur_PremiereLigneNonNulle ) ;
			}
		}
	}  
	
	
	// lorsque je suis déconnecté, pour les backtests :
	if ( sc.ServerConnectionState != SCS_CONNECTED )  
	{	sc.UpdateAlways = 1 ;  }
	
}

