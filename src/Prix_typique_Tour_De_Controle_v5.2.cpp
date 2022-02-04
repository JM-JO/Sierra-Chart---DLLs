/*
v1.0 (2019/02/05) : cet indicateur retourne le prix typique (H+L+C)/3 du chandelier.
v1.1 (2019/06/17) : rajouter un bouton/event pour augmenter le padding (Padding = Padding + 2%, lorsqu'on le presse).
v2.0 (28/6/19) : le nb d'instances AMR est illimité, et le statut Master/Slave est géré par le bouton NEXT et contrôlé par la study "Prix Typique".
v2.1 (29/6/19) : tri des instances d'AMR, de NiveauFuture le plus bas au plus élevé.
v2.2 (2/7/19) : le numéro de la study active est indiqué sur le bouton 60. Tri des instances d'AMR, de NiveauFuture le plus élevé au plus bas.
v3.0 (4/7/19) : création d'un bouton qui toggle ON/OFF le tracé de tous les niveaux futures. Idem pour les niveaux indices projetés.
v3.1 (8/7/19) : ajout du nom du niveau tradé dans le bouton NEXT. Compatibilité avec v1941 est OK.
v3.2 (12/7/19) : la study dénombre le nb de studies AMR qui sont vendeuses (resp. acheteuses), et met un popup si on démarre une 2è study acheteuse (resp. vendeuse).
v3.3 (2019/7/21) : modifs mineures.
v3.4 (2019/10/10) : on sépare TriggerWarningCritique en TriggerWarningCritique_Achat et TriggerWarningCritique_Vente. Le réarmement de ces triggers ne se fait que lorsque NbRobot[Acheteur/vendeur]Actif==1.
v3.5 (2019/11/15) : on ajoute un statut "Lean" au bouton  "Niv Ind".
v3.6 (2019/11/18) : ajout de External_Input_ACS_BUTTON_60.
v3.7 (2020/06/26) : mise en place de manual looping pour accélérer la study.
v3.8 (2020/06/26) : ajout d'un bouton pour afficher le bonus (ACS_BUTTON_17). 
v3.9 (2020/07/18) : lors d'un changement de StudyAMRActive, on indique le NiveauActif aux studies "Padding et boutons ACS" pour le recalcul du range affiché.
v4.0 (2020/07/21) : modifications pour être raccord avec Padding v2.5.
v4.1 (2020/07/22) : c'est la study PrixTypique qui envoie à AMR le trigger pour faire le check de l'état de la study active.
v4.2 (2020/07/22) : suppression de Autoloop et de tout ce qui est lié au tracé du SCSubgraphRef PrixTypique
v4.3 (2020/07/23) : ajout inputs modifiables pour statut LEAN. Ils sont transmis aux autres studies (niveaux psys, PP). 
v4.4 (2020/07/31) : ajout inputs modifiables pour pour l'affichages des Mid4 et Full4 (J/H/M).
v4.5 (2020/08/01) : bouton 106 (NiveauxIndicesAffiches_Input) : "Niv Ind Lean" devient "Niv Ind Custom". On ajoute "Niv Ind Trades" (niveaux tradés) et "Niv Ind BT" (niveaux backtestés).
v4.6 (2020/08/04) : Prix_typique transmet à Navig_niveaux l'état des niveaux à afficher (modification des inputs de Prix_typique).
v4.7 (2020/08/05) : ajout inputs modifiables pour pour l'affichages des PH et PB de la veille (J/H/M), ainsi que Close Veille.
v4.8.1 (2020/08/05) : Prix_typique transmet à chart 19 le statut de Full4/Mid4 et PHPB.
v4.9 (2020/08/05) : Prix_typique transmet aux studies Eighths le statut de NiveauxIndices_Full4Mid4_Input.
v5.0 (2020/09/18) : Tour_De_Controle transmet aux studies PHJPBJ le statut de NiveauxIndices_PHJPBJ_Input.
v5.1 (2020/09/30) : ajout de External_Input_ACS_BUTTON_106 ("Toggle ON/OFF de Niveau Indice"). Ajout de sc.RecalculateChart charts 15 et 19.
v5.2 (2021/01/14) : ajout d'un PersistentSCDateTime (SCDT_DernierChandelierVisible) pour le dernier chandelier affiché du chart #6. Cette variable persistante sera appelée par les charts #15 et #19 pour savoir le IndexOfLastVisibleBar du chart #6.






Améliorations :	
						- rajouter un input pour indiquer l'ID du robot acheteur en position dans le CO. Idem pour le robot vendeur.
						- renommer cette study en "Tour de contrôle", ou "Study unique", ou "Mother Study", ou "OverMind", ou "Master Study".
						- à plus long terme, gérer une stack de robots acheteurs (resp. vendeurs) avec leurs paramètres passés par une structure à cette study.
*/

#include "sierrachart.h"  // When including in your source code file additional header files, they need to be placed above the #include "sierrachart.h" line. 

SCDLLName("Prix typique")


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_PrixTypique(SCStudyInterfaceRef sc)
{
	
	SCInputRef 	StudyAMRActive_ID_Input = sc.Input[1];   							// appelé par AMR     // modifié par AMR              // appelé par Navig_niveaux
	SCInputRef 	NomDuNiveauTrade_FutureOuIndice = sc.Input[2]; 			// string :  nom du niveau + Fut/Ind
	SCInputRef 	NiveauxFuturesAffiches_Input = sc.Input[3];      					// appelé par Points_pivots_FNQ100    // appelé par scsf_PHPBDynamiquesForFuture    // appelé par Niveaux_psy_FNQ100    // appelé par scsf_VWAPsAffichage   // appelé par Navig_niveaux
	SCInputRef 	NiveauxIndicesAffiches_Input = sc.Input[5];   						// vaut 0 (OFF), 1 (ALL), 2 (Custom), 3 (trades), ou 4 (BT)     							// appelé par Niveaux_psy_INQ100     //   appelé par Points_pivots_INQ100		// appelé par Prix_Typique
	SCInputRef 	RobotAcheteurActif_Input = sc.Input[7];      						// appelé par AMR 
	SCInputRef 	RobotVendeurActif_Input = sc.Input[9]; 								// appelé par AMR
	SCInputRef	External_Input_ACS_BUTTON_60 = sc.Input[10];          	// numéro appelé par la study "Padding et boutons ACS"				// "NEXT"
	SCInputRef 	NiveauxIndices_10_Input = sc.Input[11];
	SCInputRef 	NiveauxIndices_25_Input = sc.Input[12];
	SCInputRef 	NiveauxIndices_50_Input = sc.Input[13];								
	SCInputRef 	NiveauxIndices_moyens_Input = sc.Input[14];					
	SCInputRef 	NiveauxIndices_PP1h_Input = sc.Input[21];						
	SCInputRef 	NiveauxIndices_PP4h_Input = sc.Input[22];
	SCInputRef 	NiveauxIndices_PPQuartsH_Input = sc.Input[23];				
	SCInputRef 	NiveauxIndices_PPEighthsM_Input = sc.Input[24];				
	SCInputRef 	NiveauxIndices_PPQuartsM_Input = sc.Input[25];				
	SCInputRef 	NiveauxIndices_Full4Mid4_Input = sc.Input[26];					
	SCInputRef 	NiveauxIndices_PHPB_JHM_Precedent_Input = sc.Input[27];					
	SCInputRef 	NiveauxIndices_PHJPBJ_Input = sc.Input[28];					
	SCInputRef 	TriggerRecalculate_AffichageNiveaux_Input = sc.Input[31];
	SCInputRef 	External_Input_ACS_BUTTON_106 = sc.Input[32];					// numéro appelé par la study "Padding et boutons ACS"		// "Toggle ON/OFF de Niveau Indice"

	
	int& TriggerWarningCritique_Achat = sc.GetPersistentIntFast(1);  
	int& TriggerWarningCritique_Vente = sc.GetPersistentIntFast(2);  
	int& StudyID_NiveauxPsys_Chart15 = sc.GetPersistentIntFast(3);  
	int& StudyID_NiveauxPP1h_Chart15 = sc.GetPersistentIntFast(4);  
	int& StudyID_NiveauxPP4h_Chart15 = sc.GetPersistentIntFast(5);  
	int& StudyID_NiveauxH_Chart15 = sc.GetPersistentIntFast(6);  
	int& StudyID_NiveauxM_Chart15 = sc.GetPersistentIntFast(7);  
	int& StudyID_NiveauxEighthM_Chart15 = sc.GetPersistentIntFast(8);  
	int& StudyID_NiveauxJ_Chart15 = sc.GetPersistentIntFast(9);  
	int& StudyID_NiveauxPsys_Projetes_Chart6 = sc.GetPersistentIntFast(11);  
	int& StudyID_NiveauxPP1h_Projetes_Chart6 = sc.GetPersistentIntFast(12);  
	int& StudyID_NiveauxPP4h_Projetes_Chart6 = sc.GetPersistentIntFast(13);  
	int& StudyID_NiveauxH_Projetes_Chart6 = sc.GetPersistentIntFast(14);  
	int& StudyID_NiveauxM_Projetes_Chart6 = sc.GetPersistentIntFast(15);  
	int& StudyID_NiveauxEighthM_Projetes_Chart6 = sc.GetPersistentIntFast(16);  
	int& StudyID_NiveauxJ_Projetes_Chart6 = sc.GetPersistentIntFast(17);  
	int& StudyID_Navig_Niveaux = sc.GetPersistentIntFast(18);  
	int& StudyID_NiveauxJ_Chart19 = sc.GetPersistentIntFast(19);  
	int& StudyID_NiveauxH_Chart19 = sc.GetPersistentIntFast(20);  
	int& StudyID_NiveauxM_Chart19 = sc.GetPersistentIntFast(21);  
	int& StudyID_NiveauxEighthH_Chart19 = sc.GetPersistentIntFast(22);  
	int& StudyID_NiveauxEighthM_Chart19 = sc.GetPersistentIntFast(23);  
	int& StudyID_PHJPBJ_Projetes_Chart6 = sc.GetPersistentIntFast(24);  
	int& StudyID_NiveauxPsys_Chart19 = sc.GetPersistentIntFast(25);  

	SCDateTime& SCDT_DernierChandelierVisible = sc.GetPersistentSCDateTime(0);		// appelé par la study "Quantieme Mvt"


	
	if (sc.SetDefaults)
	{
		sc.AutoLoop = 0;  
		sc.GraphName = "Prix typique";   // appelé par AMR      //   appelé par Points_pivots_FNQ100    // appelé par scsf_PHPBDynamiquesForFuture    // appelé par Niveaux_psy_FNQ100   // appelé par scsf_VWAPsAffichage     // appelé par Niveaux_psy_INQ100     //   appelé par Points_pivots_INQ100   // nom appelé par le DLL "Padding et boutons ACS"
		sc.ScaleRangeType = SCALE_SAMEASREGION;
		sc.GraphRegion = 1;
		sc.ValueFormat = 2;
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 0 ;
		sc.DisplayStudyInputValues = 0;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		sc.HideStudy=1;
		
		
		StudyAMRActive_ID_Input.Name = "ID de la study active";
		StudyAMRActive_ID_Input.SetInt(0);
		//NiveauxFuturesAffiches_Input.Name = "NiveauxFuturesAffiches_Input";
		NiveauxFuturesAffiches_Input.SetInt(0);  
		//NiveauxIndicesAffiches_Input.Name = "NiveauxIndicesAffiches_Input";
		NiveauxIndicesAffiches_Input.SetInt(2);  // 2 == Custom
		RobotAcheteurActif_Input.Name = "Nb de robots acheteurs actifs";
		RobotVendeurActif_Input.Name = "Nb de robots vendeurs actifs";
		External_Input_ACS_BUTTON_60.SetInt(0) ;    //  "NEXT"
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
		TriggerRecalculate_AffichageNiveaux_Input.Name = "TRIGGER RECALCULATE Affichage Niveaux";
		TriggerRecalculate_AffichageNiveaux_Input.SetYesNo(0);
		External_Input_ACS_BUTTON_106.SetInt(0) ; 

		return;
	}  // fin du bloc de " if (sc.SetDefaults) "
	
	
	
	if (sc.Index == 0)
	{ 
		sc.GraphRegion = 1;	
		
		// Set the ACS Control Bar buttons hover text
		sc.SetCustomStudyControlBarButtonHoverText(17, "Valeur du bonus");
		sc.SetCustomStudyControlBarButtonHoverText(18, "Amplitudes");
		sc.SetCustomStudyControlBarButtonHoverText(60, "ID | LVL | NEXT");
		sc.SetCustomStudyControlBarButtonHoverText(99, "Diminution du padding de 10%");
		sc.SetCustomStudyControlBarButtonHoverText(100, "Augmentation du padding de 10%");
		sc.SetCustomStudyControlBarButtonHoverText(105, "Niveaux future affiches");
		sc.SetCustomStudyControlBarButtonHoverText(106, "Niveaux indice affiches");
		
		// Set the ACS Control Bar buttons text
		SCString message ;   
		// Bouton 60
		message.Format("ID:%d | NEXT", StudyAMRActive_ID_Input.GetInt() ) ;  sc.SetCustomStudyControlBarButtonText(60, message) ; 
		// Bouton 100
		sc.SetCustomStudyControlBarButtonText(100, "Pad") ; 
		// Bouton 105
		if ( NiveauxFuturesAffiches_Input.GetInt() == 0 )  sc.SetCustomStudyControlBarButtonText(105, " Niv Fut OFF ") ; 
		else if ( NiveauxFuturesAffiches_Input.GetInt() == 1 )  sc.SetCustomStudyControlBarButtonText(105, " Niv Fut ALL ") ; 
		// Bouton 106
		if ( NiveauxIndicesAffiches_Input.GetInt() == 0 )  sc.SetCustomStudyControlBarButtonText(106, "   Niv Ind OFF  ") ; 
		else if ( NiveauxIndicesAffiches_Input.GetInt() == 1 )  sc.SetCustomStudyControlBarButtonText(106, "   Niv Ind ALL  ") ; 
		else if ( NiveauxIndicesAffiches_Input.GetInt() == 2 )  sc.SetCustomStudyControlBarButtonText(106, "Niv Ind Custom") ; 
		else if ( NiveauxIndicesAffiches_Input.GetInt() == 3 )  sc.SetCustomStudyControlBarButtonText(106, "Niv Ind Trades") ; 
		else if ( NiveauxIndicesAffiches_Input.GetInt() == 4 )  sc.SetCustomStudyControlBarButtonText(106, "    Niv Ind BT    ") ; 
		
		// Récupérations des StudyID
		{
			// Récupération de StudyID_Navig_Niveaux
			StudyID_Navig_Niveaux = sc.GetStudyIDByName(6, "Navigation niveaux", 0);
			if  (StudyID_Navig_Niveaux == 0 )
			{ SCString message ;  message.Format("!! ERREUR !! La study ''Navigation niveaux'' n'est pas disponible sur le chart #6") ; sc.AddMessageToLog(message,1) ; }
			
			// Récupération de StudyID_NiveauxPsys_Projetes_Chart6
			StudyID_NiveauxPsys_Projetes_Chart6 = sc.GetStudyIDByName(6, "[Z] Niveaux psys de l'indice projetes sur le future", 0);
			if  (StudyID_NiveauxPsys_Projetes_Chart6 == 0 )
			{ SCString message ;  message.Format("!! ERREUR !! La study ''[Z] Niveaux psys de l'indice projetes sur le future'' n'est pas disponible sur le chart #6") ; sc.AddMessageToLog(message,1) ; }

			// Récupération de StudyID_PHJPBJ_Projetes_Chart6
			StudyID_PHJPBJ_Projetes_Chart6 = sc.GetStudyIDByName(6, "[G] PHJ PBJ dynamiques de l'indice projetes sur le future", 0);
			if  (StudyID_PHJPBJ_Projetes_Chart6 == 0 )
			{ SCString message ;  message.Format("!! ERREUR !! La study ''[G] PHJ PBJ dynamiques de l'indice projetes sur le future'' n'est pas disponible sur le chart #6") ; sc.AddMessageToLog(message,1) ; }

			// Récupération de StudyID_NiveauxPP1h_Projetes_Chart6
			StudyID_NiveauxPP1h_Projetes_Chart6 = sc.GetStudyIDByName(6, "[F] Pivots Ind 1h projetes sur le future", 0);
			if  (StudyID_NiveauxPP1h_Projetes_Chart6 == 0 )
			{ SCString message ;  message.Format("!! ERREUR !! La study ''[F] Pivots Ind 1h projetes sur le future'' n'est pas disponible sur le chart #6") ; sc.AddMessageToLog(message,1) ; }

			// Récupération de StudyID_NiveauxPP4h_Projetes_Chart6
			StudyID_NiveauxPP4h_Projetes_Chart6 = sc.GetStudyIDByName(6, "[E] Pivots Ind 4h projetes sur le future", 0);
			if  (StudyID_NiveauxPP4h_Projetes_Chart6 == 0 )
			{ SCString message ;  message.Format("!! ERREUR !! La study ''[E] Pivots Ind 4h projetes sur le future'' n'est pas disponible sur le chart #6") ; sc.AddMessageToLog(message,1) ; }

			// Récupération de StudyID_NiveauxJ_Projetes_Chart6
			StudyID_NiveauxJ_Projetes_Chart6 = sc.GetStudyIDByName(6, "[D] Pivots Ind J (Full/Mid) projetes sur le future", 0);
			if  (StudyID_NiveauxJ_Projetes_Chart6 == 0 )
			{ SCString message ;  message.Format("!! ERREUR !! La study ''[D] Pivots Ind J (Full/Mid) projetes sur le future'' n'est pas disponible sur le chart #6") ; sc.AddMessageToLog(message,1) ; }

			// Récupération de StudyID_NiveauxH_Projetes_Chart6
			StudyID_NiveauxH_Projetes_Chart6 = sc.GetStudyIDByName(6, "[C] Pivots Ind H (Full/Mid/Quart) projetes sur le future", 0);
			if  (StudyID_NiveauxH_Projetes_Chart6 == 0 )
			{ SCString message ;  message.Format("!! ERREUR !! La study ''[C] Pivots Ind H (Full/Mid/Quart) projetes sur le future'' n'est pas disponible sur le chart #6") ; sc.AddMessageToLog(message,1) ; }

			// Récupération de StudyID_NiveauxM_Projetes_Chart6
			StudyID_NiveauxM_Projetes_Chart6 = sc.GetStudyIDByName(6, "[A] Pivots Ind M (Full/Mid/Quart) projetes sur le future", 0);
			if  (StudyID_NiveauxM_Projetes_Chart6 == 0 )
			{ SCString message ;  message.Format("!! ERREUR !! La study ''[A] Pivots Ind M (Full/Mid/Quart) projetes sur le future'' n'est pas disponible sur le chart #6") ; sc.AddMessageToLog(message,1) ; }

			// Récupération de StudyID_NiveauxEighthM_Projetes_Chart6
			StudyID_NiveauxEighthM_Projetes_Chart6 = sc.GetStudyIDByName(6, "[B] Pivots Ind M (Eighth) projetes sur le future", 0);
			if  (StudyID_NiveauxEighthM_Projetes_Chart6 == 0 )
			{ SCString message ;  message.Format("!! ERREUR !! La study ''[B] Pivots Ind M (Eighth) projetes sur le future'' n'est pas disponible sur le chart #6") ; sc.AddMessageToLog(message,1) ; }
		
			// Récupération de StudyID_NiveauxPsys_Chart15
			StudyID_NiveauxPsys_Chart15 = sc.GetStudyIDByName(15, "[0] Niveaux psys de l'indice traces sur l'indice", 0);
			if  (StudyID_NiveauxPsys_Chart15 == 0 )
			{ SCString message ;  message.Format("!! ERREUR !! La study ''[0] Niveaux psys de l'indice traces sur l'indice'' n'est pas disponible sur le chart #15") ; sc.AddMessageToLog(message,1) ; }

			// Récupération de StudyID_NiveauxPsys_Chart19
			StudyID_NiveauxPsys_Chart19 = sc.GetStudyIDByName(19, "[0] Niveaux psys de l'indice traces sur l'indice", 0);
			if  (StudyID_NiveauxPsys_Chart19 == 0 )
			{ SCString message ;  message.Format("!! ERREUR !! La study ''[0] Niveaux psys de l'indice traces sur l'indice'' n'est pas disponible sur le chart #19") ; sc.AddMessageToLog(message,1) ; }
			
			// Récupération de StudyID_NiveauxPP1h_Chart15
			StudyID_NiveauxPP1h_Chart15 = sc.GetStudyIDByName(15, "[8] Pivots Ind 1h (Full)", 0);
			if  (StudyID_NiveauxPP1h_Chart15 == 0 )
			{ SCString message ;  message.Format("!! ERREUR !! La study ''[8] Pivots Ind 1h (Full)'' n'est pas disponible sur le chart #15") ; sc.AddMessageToLog(message,1) ; }
			
			// Récupération de StudyID_NiveauxPP4h_Chart15
			StudyID_NiveauxPP4h_Chart15 = sc.GetStudyIDByName(15, "[7] Pivots Ind 4h (Full)", 0);
			if  (StudyID_NiveauxPP4h_Chart15 == 0 )
			{ SCString message ;  message.Format("!! ERREUR !! La study ''[7] Pivots Ind 4h (Full)'' n'est pas disponible sur le chart #15") ; sc.AddMessageToLog(message,1) ; }
			
			// Récupération de StudyID_NiveauxJ_Chart15
			StudyID_NiveauxJ_Chart15 = sc.GetStudyIDByName(15, "[6] Pivots Ind J (Full/Mid/Quart)", 0);
			if  (StudyID_NiveauxJ_Chart15 == 0 )
			{ SCString message ;  message.Format("!! ERREUR !! La study ''[6] Pivots Ind J (Full/Mid/Quart)'' n'est pas disponible sur le chart #15") ; sc.AddMessageToLog(message,1) ; }

			// Récupération de StudyID_NiveauxH_Chart15
			StudyID_NiveauxH_Chart15 = sc.GetStudyIDByName(15, "[4] Pivots Ind H (Full/Mid/Quart)", 0);
			if  (StudyID_NiveauxH_Chart15 == 0 )
			{ SCString message ;  message.Format("!! ERREUR !! La study ''[4] Pivots Ind H (Full/Mid/Quart)'' n'est pas disponible sur le chart #15") ; sc.AddMessageToLog(message,1) ; }

			// Récupération de StudyID_NiveauxM_Chart15
			StudyID_NiveauxM_Chart15 = sc.GetStudyIDByName(15, "[1] Pivots Ind M (Full/Mid/Quart)", 0);
			if  (StudyID_NiveauxM_Chart15 == 0 )
			{ SCString message ;  message.Format("!! ERREUR !! La study ''[1] Pivots Ind M (Full/Mid/Quart)'' n'est pas disponible sur le chart #15") ; sc.AddMessageToLog(message,1) ; }
			
			// Récupération de StudyID_NiveauxEighthM_Chart15
			StudyID_NiveauxEighthM_Chart15 = sc.GetStudyIDByName(15, "[2] Pivots Ind M (Eighth)", 0);
			if  (StudyID_NiveauxEighthM_Chart15 == 0 )
			{ SCString message ;  message.Format("!! ERREUR !! La study ''[2] Pivots Ind M (Eighth)'' n'est pas disponible sur le chart #15") ; sc.AddMessageToLog(message,1) ; }

			// Récupération de StudyID_NiveauxJ_Chart19
			StudyID_NiveauxJ_Chart19 = sc.GetStudyIDByName(19, "[6] Pivots Ind J (Full/Mid/Quart)", 0);
			if  (StudyID_NiveauxJ_Chart19 == 0 )
			{ SCString message ;  message.Format("!! ERREUR !! La study ''[6] Pivots Ind J (Full/Mid/Quart)'' n'est pas disponible sur le chart #19") ; sc.AddMessageToLog(message,1) ; }

			// Récupération de StudyID_NiveauxH_Chart19
			StudyID_NiveauxH_Chart19 = sc.GetStudyIDByName(19, "[4] Pivots Ind H (Full/Mid/Quart)", 0);
			if  (StudyID_NiveauxH_Chart19 == 0 )
			{ SCString message ;  message.Format("!! ERREUR !! La study ''[4] Pivots Ind H (Full/Mid/Quart)'' n'est pas disponible sur le chart #19") ; sc.AddMessageToLog(message,1) ; }

			// Récupération de StudyID_NiveauxM_Chart19
			StudyID_NiveauxM_Chart19 = sc.GetStudyIDByName(19, "[1] Pivots Ind M (Full/Mid/Quart)", 0);
			if  (StudyID_NiveauxM_Chart19 == 0 )
			{ SCString message ;  message.Format("!! ERREUR !! La study ''[1] Pivots Ind M (Full/Mid/Quart)'' n'est pas disponible sur le chart #19") ; sc.AddMessageToLog(message,1) ; }

			// Récupération de StudyID_NiveauxEighthH_Chart19
			StudyID_NiveauxEighthH_Chart19 = sc.GetStudyIDByName(19, "[5] Pivots Ind H (Eighth)", 0);
			if  (StudyID_NiveauxEighthH_Chart19 == 0 )
			{ SCString message ;  message.Format("!! ERREUR !! La study ''[5] Pivots Ind H (Eighth)'' n'est pas disponible sur le chart #19") ; sc.AddMessageToLog(message,1) ; }
			
			// Récupération de StudyID_NiveauxEighthM_Chart19
			StudyID_NiveauxEighthM_Chart19 = sc.GetStudyIDByName(19, "[2] Pivots Ind M (Eighth)", 0);
			if  (StudyID_NiveauxEighthM_Chart19 == 0 )
			{ SCString message ;  message.Format("!! ERREUR !! La study ''[2] Pivots Ind M (Eighth)' n'est pas disponible sur le chart #19") ; sc.AddMessageToLog(message,1) ; }

		}
	
	}  // fin sc.Index== 0
	


	// Statut des Control Bar Buttons, des robots actifs et SCDT_DernierChandelierVisible
	if (sc.Index == sc.ArraySize-1)
	{
	
		// Statut des Control Bar Buttons
		{
			if (sc.MenuEventID == 60  or  External_Input_ACS_BUTTON_60.GetInt() != 0)     // Bouton "NEXT"
			{
				sc.UpdateAlways = 1;   // pour que la study soit recalculée immédiatement (sinon il faut attendre qu'une condition d'appel se produise, il n'y a jamais d'appel si le marché est fermé)
				sc.SetCustomStudyControlBarButtonEnable(sc.MenuEventID,FALSE); // le bouton cliqué ne prend pas la position enfoncée
				External_Input_ACS_BUTTON_60.SetInt( max(External_Input_ACS_BUTTON_60.GetInt()-1, 0) ) ; // on retranche 1 à External_Input_ACS_BUTTON_60
				
				// Le tableau Instances stocke toutes les instances actives de AMR  (16 lignes pour 15 studies AMR au maximum ; 2 colonnes pour : StudyID, NiveauFuture).   // StudyID = Instance[xxx][0]     // NiveauFuture = Instance[xxx][1]
				float Instances[16][2] ;   
				int NbInstancesAMR = 0 ;
				
				// On détermine parmi les 50 premières StudyID lesquelles s'appellent "ANNUL MINI REBOND"
				SCString StudyName ;
				for ( int StudyID = 1 ; StudyID < 51 ; StudyID ++ )  
				{
					StudyName = sc.GetStudyNameUsingID( StudyID );
					if ( StudyName.Compare("ANNUL MINI REBOND",0) == 0 )      // returns 0 if they are the same
					{
						NbInstancesAMR ++ ;
						
						// on stocke la StudyID dans la colonne 0
						Instances[NbInstancesAMR][0] = StudyID ;   // on stocke la StudyID dans la colonne 0
						
						// on récupère NiveauFutureTexte dans sc.Input[7] de AMR
						double NiveauFutureTexte ;
						sc.GetChartStudyInputFloat(sc.ChartNumber, StudyID, 7, NiveauFutureTexte) ;  
						Instances[NbInstancesAMR][1] = NiveauFutureTexte ;   // on stocke NiveauFutureTexte dans la colonne 1
					}		
				}
				
				
				// Le tableau Instances est trié dans les lignes entre "1" et "NbInstancesAMR" : de NiveauFuture le plus bas au plus élevé.
				for ( int boucle = 1 ; boucle < NbInstancesAMR ; boucle ++ )
				{
					// On parcourt index de "boucle" à "NbInstancesAMR" pour trouver indexDuPrixLePlusEleve et PrixLePlusEleve
					int indexDuPrixLePlusEleve = boucle ;
					float PrixLePlusEleve = Instances[boucle][1] ;
					for ( int index = boucle+1 ; index <= NbInstancesAMR ; index ++ )
					{
						if ( Instances[index][1] > PrixLePlusEleve )  
						{	PrixLePlusEleve = Instances[index][1] ;  indexDuPrixLePlusEleve = index ;  }
					}
					
					// On permute la ligne "boucle" avec la ligne "indexDuPrixLePlusEleve" : 1) on copie les valeurs de la ligne "boucle", 2) on met dans la ligne "boucle" les valeurs de la ligne "indexDuPrixLePlusEleve", 3) puis on déplace les valeurs copiées dans la ligne "indexDuPrixLePlusEleve"
					float CopieStudyID = Instances[boucle][0] ;
					float CopieNiveauFuture = Instances[boucle][1] ;
					Instances[boucle][0] = Instances[indexDuPrixLePlusEleve][0] ;
					Instances[boucle][1] = Instances[indexDuPrixLePlusEleve][1] ;
					Instances[indexDuPrixLePlusEleve][0] = CopieStudyID ;
					Instances[indexDuPrixLePlusEleve][1] = CopieNiveauFuture ;
				}
				
				
				// On détermine d'abord indexStudyIDCourante : c'est l'index de la StudyAMRActive_ID_Input actuelle dans le tableau Instances, puis on détermine la NouvelleValeurDeStudyID
				if ( NbInstancesAMR != 0 )
				{
					int indexStudyIDCourante = 1 ;     // 
					for ( int index = 1 ; index <= NbInstancesAMR ; index ++ )  
					{
						if ( Instances[index][0] == StudyAMRActive_ID_Input.GetInt() )      // si ce test n'est jamais vrai (Study effacée par exemple), alors indexStudyIDCourante = 1
						{	indexStudyIDCourante = index ; }
					} 
				
					// On détermine NouvelleValeurDeStudyID
					int NouvelleValeurDeStudyID ; 
					if ( indexStudyIDCourante < NbInstancesAMR )
					{	NouvelleValeurDeStudyID = Instances[indexStudyIDCourante+1][0] ;  }  // on descend d'une ligne dans la colonne des StudyID
					else if ( indexStudyIDCourante == NbInstancesAMR ) // dernière ligne non vide du tableau Instances
					{	NouvelleValeurDeStudyID = Instances[1][0] ;  }  // on remonte à la ligne n°1 dans la colonne des StudyID
					
					// On modifie l'input de la study "prix typique"
					StudyAMRActive_ID_Input.SetInt( NouvelleValeurDeStudyID ) ;
					

					// On concatène NomDuNiveauTrade_FutureOuIndice    ////////////////////////////////////////////////////
					// On récupère d'abord NomDuNiveauTrade
					SCString NomDuNiveauTrade_FutureOuIndice ;
					sc.GetChartStudyInputString(sc.ChartNumber, StudyAMRActive_ID_Input.GetInt(), 26, NomDuNiveauTrade_FutureOuIndice) ;    // on récupère NomDuNiveauTrade dans sc.Input[26] de AMR

					// On détermine si Instrument == Future ou Indice
					int IndiceOuFuture ;  // vaut 0 pour Indice et 1 pour Future
					sc.GetChartStudyInputInt(sc.ChartNumber, StudyAMRActive_ID_Input.GetInt(), 6, IndiceOuFuture) ;    // on récupère Instrument dans sc.Input[6] de AMR     // l'utilisation de GetChartStudyInputInt est abusive car on va récupérer un input de type CustomInputIndex, mais ça fonctionne
					if ( IndiceOuFuture )  { NomDuNiveauTrade_FutureOuIndice.AppendFormat(" Fut") ;  }  else { NomDuNiveauTrade_FutureOuIndice.AppendFormat(" Ind") ;  }    //  Compare retourne 0 si les strings sont identiques					
					
					// Set the ACS Control Bar button 60 text
					SCString Texte ;
					Texte.Format("ID:%d | ", StudyAMRActive_ID_Input.GetInt() ) ;  Texte.AppendFormat(NomDuNiveauTrade_FutureOuIndice) ;  /* Texte.AppendFormat(" | NEXT") ;  */
					sc.SetCustomStudyControlBarButtonText(60, Texte) ; 
					
					// Set the ACS Control Bar button 17 text (BONUS)
					float BonusIntrinseque = sc.GetPersistentFloatFromChartStudy(6, StudyAMRActive_ID_Input.GetInt(), 0);			// float& GetPersistentFloatFromChartStudy(int ChartNumber, int StudyID, int Key);
					Texte.Format("BONUS %d", BonusIntrinseque) ;
					sc.SetCustomStudyControlBarButtonText(17, Texte) ; 
					
					// On notifie aux studies AMR le changement de la study active
					if( External_Input_ACS_BUTTON_60.GetInt() == 0 )		// tous les clics en attente ont été traités
					{
						for ( int index = 1 ; index <= NbInstancesAMR ; index ++ )  
						{
							int StudyID = Instances[index][0] ;
							sc.SetChartStudyInputInt(6, StudyID,40, 1) ;   		// int SetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int IntegerValue);
						} 
					}
				}
				else		// if NbInstancesAMR==0
				{	
					StudyAMRActive_ID_Input.SetInt(0) ; 
					sc.SetCustomStudyControlBarButtonText(60, "ID:0 | NEXT") ;  
					sc.SetCustomStudyControlBarButtonText(17, "Bonus n.d.") ; 
					sc.SetCustomStudyControlBarButtonText(18, "Amplitude n.d.") ; 
				}
				
			} // fin de if (sc.MenuEventID == 60)
			

			else if (sc.MenuEventID == 99)   // Bouton "Pad-"
			{
				sc.UpdateAlways = 1;   // pour que la study soit recalculée immédiatement (sinon il faut attendre qu'une condition d'appel se produise, il n'y a jamais d'appel si le marché est fermé)
				sc.SetCustomStudyControlBarButtonEnable(sc.MenuEventID,FALSE); // le bouton cliqué ne prend pas la position enfoncée
				
				sc.BaseGraphAutoScalePaddingPercentage  = sc.BaseGraphAutoScalePaddingPercentage  - 0.10 ;
			}

			
			else if (sc.MenuEventID == 100)   // Bouton "Pad+"
			{
				sc.UpdateAlways = 1;   // pour que la study soit recalculée immédiatement (sinon il faut attendre qu'une condition d'appel se produise, il n'y a jamais d'appel si le marché est fermé)
				sc.SetCustomStudyControlBarButtonEnable(sc.MenuEventID,FALSE); // le bouton cliqué ne prend pas la position enfoncée
				
				sc.BaseGraphAutoScalePaddingPercentage  = sc.BaseGraphAutoScalePaddingPercentage  + 0.10 ;
			}
			
			
			else if (sc.MenuEventID == 105)   // Bouton "Niv Fut OFF/ON"
			{
				sc.UpdateAlways = 1;   // pour que la study soit recalculée immédiatement (sinon il faut attendre qu'une condition d'appel se produise, il n'y a jamais d'appel si le marché est fermé)
				sc.SetCustomStudyControlBarButtonEnable(sc.MenuEventID,FALSE); // le bouton cliqué ne prend pas la position enfoncée
				
				if ( NiveauxFuturesAffiches_Input.GetInt() )   // le niveau était affiché => on le bascule vers "non affiché"
				{
					NiveauxFuturesAffiches_Input.SetInt(0) ;
					sc.SetCustomStudyControlBarButtonText(105, " Niv Fut OFF ") ; 
				}
				else  // on bascule vers "affiché"
				{
					NiveauxFuturesAffiches_Input.SetInt(1) ;
					sc.SetCustomStudyControlBarButtonText(105, " Niv Fut ALL ") ; 
				}
			}
			
			
			else if ( sc.MenuEventID == 106  or  External_Input_ACS_BUTTON_106.GetInt() != 0  or  TriggerRecalculate_AffichageNiveaux_Input.GetYesNo() )   // Bouton "Niv ind OFF / ALL / Custom / Niveaux Tradés / Backtest"
			{
				sc.UpdateAlways = 1;   // pour que la study soit recalculée immédiatement (sinon il faut attendre qu'une condition d'appel se produise, il n'y a jamais d'appel si le marché est fermé)
				sc.SetCustomStudyControlBarButtonEnable(sc.MenuEventID, FALSE); // le bouton cliqué ne prend pas la position enfoncée
				External_Input_ACS_BUTTON_106.SetInt( max(External_Input_ACS_BUTTON_106.GetInt()-1, 0) ) ; // on retranche 1 à External_Input_ACS_BUTTON_106
				sc.RecalculateChart(15) ;  sc.RecalculateChart(19) ; 		// on lance un recalcul des charts 15, 19 (et 6 automatiquement) pour que les niveaux affichés/désaffichés soient mis à jour après calcul
				
				if ( NiveauxIndicesAffiches_Input.GetInt() == 1  or  TriggerRecalculate_AffichageNiveaux_Input.GetYesNo() )   // le statut était "ALL" => on le bascule vers "CUSTOM"
				{
					TriggerRecalculate_AffichageNiveaux_Input.SetYesNo(0) ;
					NiveauxIndicesAffiches_Input.SetInt(2) ;
					sc.SetCustomStudyControlBarButtonText(106, "Niv Ind Custom") ; 
					
					// Modifications des inputs de ce qui est tracé sur le chart #15 /////
					// - niveaux psys :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxPsys_Chart15, 1, 1) ;																	// input TraceVectorielONOFF
					// - dizaines :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxPsys_Chart15, 2, NiveauxIndices_10_Input.GetYesNo()) ;			// int SetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int IntegerValue);					
					// - 25 :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxPsys_Chart15, 3, NiveauxIndices_25_Input.GetYesNo()) ;
					// - 50 :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxPsys_Chart15, 4, NiveauxIndices_50_Input.GetYesNo()) ;
					// - niveaux psys moyens :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxPsys_Chart15, 5, NiveauxIndices_moyens_Input.GetYesNo()) ;
					// - PP J :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxJ_Chart15, 2, 1) ;
					// - PP H :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxH_Chart15, 2, 1) ;
					// - PP M :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxM_Chart15, 2, 1) ;
					// - PP 1h :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxPP1h_Chart15, 2, NiveauxIndices_PP1h_Input.GetYesNo()) ;					
					// - PP 4h :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxPP4h_Chart15, 2, NiveauxIndices_PP4h_Input.GetYesNo()) ;
					// - PP Quarts H :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxH_Chart15, 3, NiveauxIndices_PPQuartsH_Input.GetYesNo()) ;
					// - PP Quarts M :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxM_Chart15, 3, NiveauxIndices_PPQuartsM_Input.GetYesNo()) ;
					// - PP Eighth M :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxEighthM_Chart15, 2, NiveauxIndices_PPEighthsM_Input.GetYesNo()) ;
					// - PP Full4/Mid4 J :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxJ_Chart15, 4, NiveauxIndices_Full4Mid4_Input.GetYesNo()) ;
					// - PP Full4/Mid4 H :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxH_Chart15, 4, NiveauxIndices_Full4Mid4_Input.GetYesNo()) ;
					// - PP Full4/Mid4 M :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxM_Chart15, 4, NiveauxIndices_Full4Mid4_Input.GetYesNo()) ;
					// - PH et PB JP + Close JP :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxJ_Chart15, 5, NiveauxIndices_PHPB_JHM_Precedent_Input.GetYesNo()) ;
					// - PH et PB HP :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxH_Chart15, 5, NiveauxIndices_PHPB_JHM_Precedent_Input.GetYesNo()) ;
					// - PH et PB MP :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxM_Chart15, 5, NiveauxIndices_PHPB_JHM_Precedent_Input.GetYesNo()) ;
					// PP RE13/16 et SE 13/16 mensuels :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxEighthM_Chart15, 4, NiveauxIndices_Full4Mid4_Input.GetYesNo()) ;

					// Modifications des inputs de ce qui est tracé sur le chart #19 /////
					// - niveaux psys :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxPsys_Chart19, 1, 1) ;																	// input TraceVectorielONOFF
					// - PP J :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxJ_Chart19, 2, 0) ;
					// - PP H :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxH_Chart19, 2, 1) ;
					// - PP M :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxM_Chart19, 2, 1) ;
					// - PP Quarts H :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxH_Chart19, 3, NiveauxIndices_PPQuartsH_Input.GetYesNo()) ;
					// - PP Quarts M :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxM_Chart19, 3, NiveauxIndices_PPQuartsM_Input.GetYesNo()) ;
					// - PP Eighth M :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxEighthM_Chart19, 2, NiveauxIndices_PPEighthsM_Input.GetYesNo()) ;
					// - PP Full4/Mid4 J :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxJ_Chart19, 4, 0) ;
					// - PP Full4/Mid4 H :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxH_Chart19, 4, NiveauxIndices_Full4Mid4_Input.GetYesNo()) ;
					// - PP Full4/Mid4 M :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxM_Chart19, 4, NiveauxIndices_Full4Mid4_Input.GetYesNo()) ;
					// - PH et PB HP :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxH_Chart19, 5, NiveauxIndices_PHPB_JHM_Precedent_Input.GetYesNo()) ;
					// - PH et PB MP :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxM_Chart19, 5, NiveauxIndices_PHPB_JHM_Precedent_Input.GetYesNo()) ;
					// PP RE13/16 et SE 13/16 hebdos :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxEighthH_Chart19, 4, NiveauxIndices_Full4Mid4_Input.GetYesNo()) ; 
					// PP RE13/16 et SE 13/16 mensuels :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxEighthM_Chart19, 4, NiveauxIndices_Full4Mid4_Input.GetYesNo()) ; 

					// Modifications des inputs de ce qui est tracé sur le chart #6 (niveaux projetés) /////
					sc.SetChartStudyInputInt(6, StudyID_NiveauxPsys_Projetes_Chart6, 1, 1) ;		// input TraceVectorielONOFF
					// - dizaines :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxPsys_Projetes_Chart6, 2, NiveauxIndices_10_Input.GetYesNo()) ;		
					// - 25 :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxPsys_Projetes_Chart6, 3, NiveauxIndices_25_Input.GetYesNo()) ;
					// - 50 :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxPsys_Projetes_Chart6, 4, NiveauxIndices_50_Input.GetYesNo()) ;
					// - niveaux psys moyens :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxPsys_Projetes_Chart6, 5, NiveauxIndices_moyens_Input.GetYesNo()) ;
					// - PHJ PBJ dynamiques :
					sc.SetChartStudyInputInt(6, StudyID_PHJPBJ_Projetes_Chart6, 3, NiveauxIndices_PHJPBJ_Input.GetYesNo()) ;
					// - PP J :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxJ_Projetes_Chart6, 2, 1) ;
					// - PP H :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxH_Projetes_Chart6, 2, 1) ;
					// - PP M :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxM_Projetes_Chart6, 2, 1) ;
					// - PP 1h :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxPP1h_Projetes_Chart6, 1, NiveauxIndices_PP1h_Input.GetYesNo()) ;
					// - PP 4h :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxPP4h_Projetes_Chart6, 1, NiveauxIndices_PP4h_Input.GetYesNo()) ;
					// - PP Quarts H :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxH_Projetes_Chart6, 3, NiveauxIndices_PPQuartsH_Input.GetYesNo()) ;
					// - PP Quarts M :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxM_Projetes_Chart6, 3, NiveauxIndices_PPQuartsM_Input.GetYesNo()) ;
					// - PP Eighth M :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxEighthM_Projetes_Chart6, 2, NiveauxIndices_PPEighthsM_Input.GetYesNo()) ;
					// - PP Full4/Mid4 J :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxJ_Projetes_Chart6, 4, NiveauxIndices_Full4Mid4_Input.GetYesNo()) ;
					// - PP Full4/Mid4 H :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxH_Projetes_Chart6, 4, NiveauxIndices_Full4Mid4_Input.GetYesNo()) ;
					// - PP Full4/Mid4 M :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxM_Projetes_Chart6, 4, NiveauxIndices_Full4Mid4_Input.GetYesNo()) ;					
					// - PH et PB JP + Close JP :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxJ_Projetes_Chart6, 5, NiveauxIndices_PHPB_JHM_Precedent_Input.GetYesNo()) ;
					// - PH et PB HP :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxH_Projetes_Chart6, 5, NiveauxIndices_PHPB_JHM_Precedent_Input.GetYesNo()) ;
					// - PH et PB MP :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxM_Projetes_Chart6, 5, NiveauxIndices_PHPB_JHM_Precedent_Input.GetYesNo()) ;			
					// PP RE13/16 et SE 13/16 mensuels :					
					sc.SetChartStudyInputInt(6, StudyID_NiveauxEighthM_Projetes_Chart6, 4, NiveauxIndices_Full4Mid4_Input.GetYesNo()) ;										

					// Modifications des inputs de Navig_Niveaux /////
					// - NiveauxIndicesAffiches_Input
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 20, NiveauxIndicesAffiches_Input.GetInt()) ;		
					// - dizaines :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 11, NiveauxIndices_10_Input.GetYesNo()) ;		
					// - 25 :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 12, NiveauxIndices_25_Input.GetYesNo()) ;
					// - 50 :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 13, NiveauxIndices_50_Input.GetYesNo()) ;
					// - niveaux psys moyens :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 14, NiveauxIndices_moyens_Input.GetYesNo()) ;
					// - PP 1h :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 21, NiveauxIndices_PP1h_Input.GetYesNo()) ;
					// - PP 4h :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 22, NiveauxIndices_PP4h_Input.GetYesNo()) ;
					// - PP Quarts H :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 23, NiveauxIndices_PPQuartsH_Input.GetYesNo()) ;
					// - PP Quarts M :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 25, NiveauxIndices_PPQuartsM_Input.GetYesNo()) ;
					// - PP Eighth M :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 24, NiveauxIndices_PPEighthsM_Input.GetYesNo()) ;
					// - PP Full4/Mid4 J/H/M :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 26, NiveauxIndices_Full4Mid4_Input.GetYesNo()) ;
					// - PHPB J/H/M + Close JP:
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 27, NiveauxIndices_PHPB_JHM_Precedent_Input.GetYesNo()) ;
					// - PHJPBJ :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 28, NiveauxIndices_PHJPBJ_Input.GetYesNo()) ;
				}
				else if ( NiveauxIndicesAffiches_Input.GetInt() == 2 )  // le statut était "CUSTOM" => on le bascule vers "NIVEAUX TRADES"
				{
					NiveauxIndicesAffiches_Input.SetInt(3) ;
					sc.SetCustomStudyControlBarButtonText(106, "Niv Ind Trades") ; 
					
					// Modifications des inputs de ce qui est tracé sur le chart #15
					// - niveaux psys :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxPsys_Chart15, 1, 1) ;			// input TraceVectorielONOFF
					// - dizaines :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxPsys_Chart15, 2, 0) ;			// int SetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int IntegerValue);					
					// - 25 :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxPsys_Chart15, 3, 0) ;
					// - 50 :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxPsys_Chart15, 4, 0) ;
					// - niveaux psys moyens :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxPsys_Chart15, 5, 0) ;
					// - PP J :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxJ_Chart15, 2, 1) ;
					// - PP H :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxH_Chart15, 2, 1) ;
					// - PP M :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxM_Chart15, 2, 1) ;
					// - PP 1h :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxPP1h_Chart15, 2, 0) ;
					// - PP 4h :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxPP4h_Chart15, 2, 0) ;
					// - PP Quarts H :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxH_Chart15, 3, 0) ;
					// - PP Quarts M :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxM_Chart15, 3, 0) ;
					// - PP Eighth M :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxEighthM_Chart15, 2, 0) ;
					// - PP Full4/Mid4 J :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxJ_Chart15, 4, 0) ;
					// - PP Full4/Mid4 H :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxH_Chart15, 4, 0) ;
					// - PP Full4/Mid4 M :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxM_Chart15, 4, 0) ;
					// - PH et PB JP + Close JP :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxJ_Chart15, 5, 0) ;
					// - PH et PB HP :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxH_Chart15, 5, 0) ;
					// - PH et PB MP :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxM_Chart15, 5, 0) ;
					// PP RE13/16 et SE 13/16 mensuels :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxEighthM_Chart15, 4, 0) ;

					// Modifications des inputs de ce qui est tracé sur le chart #19 /////
					// - niveaux psys :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxPsys_Chart19, 1, 1) ;																	// input TraceVectorielONOFF
					// - PP J :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxJ_Chart19, 2, 0) ;
					// - PP H :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxH_Chart19, 2, 1) ;
					// - PP M :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxM_Chart19, 2, 1) ;
					// - PP Quarts H :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxH_Chart19, 3, 0) ;
					// - PP Quarts M :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxM_Chart19, 3, 0) ;
					// - PP Eighth M :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxEighthM_Chart19, 2, 0) ;
					// - PP Full4/Mid4 J :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxJ_Chart19, 4, 0) ;
					// - PP Full4/Mid4 H :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxH_Chart19, 4, 0) ;
					// - PP Full4/Mid4 M :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxM_Chart19, 4, 0) ;
					// - PH et PB HP :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxH_Chart19, 5, 0) ;
					// - PH et PB MP :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxM_Chart19, 5, 0) ;
					// PP RE13/16 et SE 13/16 hebdos :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxEighthH_Chart19, 4, 0) ; 
					// PP RE13/16 et SE 13/16 mensuels :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxEighthM_Chart19, 4, 0) ; 
					
					// Modifications des inputs de ce qui est tracé sur le chart #6 (niveaux projetés)
					sc.SetChartStudyInputInt(6, StudyID_NiveauxPsys_Projetes_Chart6, 1, 1) ;		// input TraceVectorielONOFF
					// - dizaines :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxPsys_Projetes_Chart6, 2, 0) ;		
					// - 25 :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxPsys_Projetes_Chart6, 3, 0) ;
					// - 50 :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxPsys_Projetes_Chart6, 4, 0) ;
					// - niveaux psys moyens :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxPsys_Projetes_Chart6, 5, 0) ;
					// - PHJ PBJ dynamiques :
					sc.SetChartStudyInputInt(6, StudyID_PHJPBJ_Projetes_Chart6, 3, 0) ;
					// - PP J :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxJ_Projetes_Chart6, 2, 1) ;
					// - PP H :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxH_Projetes_Chart6, 2, 1) ;
					// - PP M :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxM_Projetes_Chart6, 2, 1) ;
					// - PP 1h :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxPP1h_Projetes_Chart6, 1, 0) ;
					// - PP 4h :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxPP4h_Projetes_Chart6, 1, 0) ;
					// - PP Quarts H :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxH_Projetes_Chart6, 3, 0) ;
					// - PP Quarts M :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxM_Projetes_Chart6, 3, 0) ;
					// - PP Eighth M :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxEighthM_Projetes_Chart6, 2, 0) ;
					// - PP Full4/Mid4 J :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxJ_Projetes_Chart6, 4, 0) ;
					// - PP Full4/Mid4 H :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxH_Projetes_Chart6, 4, 0) ;
					// - PP Full4/Mid4 M :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxM_Projetes_Chart6, 4, 0) ;
					// - PH et PB JP + Close JP :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxJ_Projetes_Chart6, 5, 0) ;
					// - PH et PB HP :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxH_Projetes_Chart6, 5, 0) ;
					// - PH et PB MP :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxM_Projetes_Chart6, 5, 0) ;					
					// PP RE13/16 et SE 13/16 mensuels :					
					sc.SetChartStudyInputInt(6, StudyID_NiveauxEighthM_Projetes_Chart6, 4, 0) ;					

					// Modifications des inputs de Navig_Niveaux /////
					// - NiveauxIndicesAffiches_Input
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 20, NiveauxIndicesAffiches_Input.GetInt()) ;		
					// - dizaines :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 11, 0) ;		
					// - 25 :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 12, 0) ;
					// - 50 :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 13, 0) ;
					// - niveaux psys moyens :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 14, 0) ;
					// - PP 1h :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 21, 0) ;
					// - PP 4h :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 22, 0) ;
					// - PP Quarts H :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 23, 0) ;
					// - PP Quarts M :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 25, 0) ;
					// - PP Eighth M :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 24, 0) ;
					// - PP Full4/Mid4 J/H/M :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 26, 0) ;
					// - PHPB J/H/M + Close JP:
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 27, 0) ;
					// - PHJPBJ :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 28, 0) ;
				}
				else if ( NiveauxIndicesAffiches_Input.GetInt() == 3 )  // le statut était "NIVEAUX TRADES" => on le bascule vers "NIVEAUX BACKTESTES"
				{
					NiveauxIndicesAffiches_Input.SetInt(4) ;
					sc.SetCustomStudyControlBarButtonText(106, "    Niv Ind BT    ") ; 
					
					// Modifications des inputs de ce qui est tracé sur le chart #15
					// - niveaux psys :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxPsys_Chart15, 1, 1) ;			// input TraceVectorielONOFF					
					// - dizaines :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxPsys_Chart15, 2, 0) ;			// int SetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int IntegerValue);					
					// - 25 :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxPsys_Chart15, 3, 0) ;
					// - 50 :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxPsys_Chart15, 4, 1) ;
					// - niveaux psys moyens :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxPsys_Chart15, 5, 1) ;
					// - PP J :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxJ_Chart15, 2, 1) ;
					// - PP H :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxH_Chart15, 2, 1) ;
					// - PP M :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxM_Chart15, 2, 1) ;
					// - PP 1h :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxPP1h_Chart15, 2, 1) ;
					// - PP 4h :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxPP4h_Chart15, 2, 1) ;
					// - PP Quarts H :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxH_Chart15, 3, 1) ;
					// - PP Quarts M :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxM_Chart15, 3, 1) ;
					// - PP Eighth M :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxEighthM_Chart15, 2, 1) ;
					// - PP Full4/Mid4 J :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxJ_Chart15, 4, 1) ;
					// - PP Full4/Mid4 H :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxH_Chart15, 4, 1) ;
					// - PP Full4/Mid4 M :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxM_Chart15, 4, 1) ;
					// - PH et PB JP + Close JP :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxJ_Chart15, 5, 1) ;
					// - PH et PB HP :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxH_Chart15, 5, 1) ;
					// - PH et PB MP :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxM_Chart15, 5, 1) ;
					// PP RE13/16 et SE 13/16 mensuels :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxEighthM_Chart15, 4, 1) ;
					
					// Modifications des inputs de ce qui est tracé sur le chart #19 /////
					// - niveaux psys :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxPsys_Chart19, 1, 1) ;																	// input TraceVectorielONOFF
					// - PP J :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxJ_Chart19, 2, 0) ;
					// - PP H :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxH_Chart19, 2, 1) ;
					// - PP M :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxM_Chart19, 2, 1) ;
					// - PP Quarts H :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxH_Chart19, 3, 1) ;
					// - PP Quarts M :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxM_Chart19, 3, 1) ;
					// - PP Eighth M :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxEighthM_Chart19, 2, 1) ;
					// - PP Full4/Mid4 J :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxJ_Chart19, 4, 0) ;
					// - PP Full4/Mid4 H :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxH_Chart19, 4, 1) ;
					// - PP Full4/Mid4 M :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxM_Chart19, 4, 1) ;
					// - PH et PB HP :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxH_Chart19, 5, 1) ;
					// - PH et PB MP :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxM_Chart19, 5, 1) ;
					// PP RE13/16 et SE 13/16 hebdos :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxEighthH_Chart19, 4, 1) ; 
					// PP RE13/16 et SE 13/16 mensuels :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxEighthM_Chart19, 4, 1) ; 
					
					// Modifications des inputs de ce qui est tracé sur le chart #6 (niveaux projetés)
					sc.SetChartStudyInputInt(6, StudyID_NiveauxPsys_Projetes_Chart6, 1, 1) ;		// input TraceVectorielONOFF
					// - dizaines :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxPsys_Projetes_Chart6, 2, 0) ;		
					// - 25 :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxPsys_Projetes_Chart6, 3, 0) ;
					// - 50 :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxPsys_Projetes_Chart6, 4, 1) ;
					// - niveaux psys moyens :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxPsys_Projetes_Chart6, 5, 1) ;
					// - PHJ PBJ dynamiques :
					sc.SetChartStudyInputInt(6, StudyID_PHJPBJ_Projetes_Chart6, 3, 0) ;
					// - PP J :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxJ_Projetes_Chart6, 2, 1) ;
					// - PP H :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxH_Projetes_Chart6, 2, 1) ;
					// - PP M :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxM_Projetes_Chart6, 2, 1) ;
					// - PP 1h :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxPP1h_Projetes_Chart6, 1, 1) ;
					// - PP 4h :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxPP4h_Projetes_Chart6, 1, 1) ;
					// - PP Quarts H :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxH_Projetes_Chart6, 3, 1) ;
					// - PP Quarts M :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxM_Projetes_Chart6, 3, 1) ;
					// - PP Eighth M :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxEighthM_Projetes_Chart6, 2, 1) ;
					// - PP Full4/Mid4 J :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxJ_Projetes_Chart6, 4, 1) ;
					// - PP Full4/Mid4 H :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxH_Projetes_Chart6, 4, 1) ;
					// - PP Full4/Mid4 M :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxM_Projetes_Chart6, 4, 1) ;
					// - PH et PB JP + Close JP :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxJ_Projetes_Chart6, 5, 1) ;
					// - PH et PB HP :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxH_Projetes_Chart6, 5, 1) ;
					// - PH et PB MP :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxM_Projetes_Chart6, 5, 1) ;					
					// PP RE13/16 et SE 13/16 mensuels :					
					sc.SetChartStudyInputInt(6, StudyID_NiveauxEighthM_Projetes_Chart6, 4, 1) ;					

					// Modifications des inputs de Navig_Niveaux /////
					// - NiveauxIndicesAffiches_Input
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 20, NiveauxIndicesAffiches_Input.GetInt()) ;		
					// - dizaines :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 11, 0) ;		
					// - 25 :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 12, 0) ;
					// - 50 :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 13, 1) ;
					// - niveaux psys moyens :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 14, 1) ;
					// - PP 1h :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 21, 1) ;
					// - PP 4h :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 22, 1) ;
					// - PP Quarts H :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 23, 1) ;
					// - PP Quarts M :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 25, 1) ;
					// - PP Eighth M :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 24, 1) ;
					// - PP Full4/Mid4 J/H/M :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 26, 1) ;
					// - PHPB J/H/M + Close JP:
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 27, 1) ;
					// - PHJPBJ :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 28, 0) ;
				}
				else if ( NiveauxIndicesAffiches_Input.GetInt() == 4 )  // le statut était "BACKTEST" => on le bascule vers "OFF"
				{
					NiveauxIndicesAffiches_Input.SetInt(0) ;
					sc.SetCustomStudyControlBarButtonText(106, "   Niv Ind OFF  ") ; 
					
					// Modifications des inputs de ce qui est tracé sur le chart #15
					// - niveaux psys :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxPsys_Chart15, 1, 0) ;			// input TraceVectorielONOFF
					// - dizaines :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxPsys_Chart15, 2, 0) ;			// int SetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int IntegerValue);					
					// - 25 :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxPsys_Chart15, 3, 0) ;
					// - 50 :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxPsys_Chart15, 4, 0) ;
					// - niveaux psys moyens :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxPsys_Chart15, 5, 0) ;
					// - PP J :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxJ_Chart15, 2, 0) ;
					// - PP H :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxH_Chart15, 2, 0) ;
					// - PP M :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxM_Chart15, 2, 0) ;
					// - PP 1h :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxPP1h_Chart15, 2, 0) ;
					// - PP 4h :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxPP4h_Chart15, 2, 0) ;
					// - PP Quarts H :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxH_Chart15, 3, 0) ;
					// - PP Quarts M :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxM_Chart15, 3, 0) ;
					// - PP Eighth M :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxEighthM_Chart15, 2, 0) ;
					// - PP Full4/Mid4 J :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxJ_Chart15, 4, 0) ;
					// - PP Full4/Mid4 H :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxH_Chart15, 4, 0) ;
					// - PP Full4/Mid4 M :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxM_Chart15, 4, 0) ;
					// - PH et PB JP + Close JP :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxJ_Chart15, 5, 0) ;
					// - PH et PB HP :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxH_Chart15, 5, 0) ;
					// - PH et PB MP :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxM_Chart15, 5, 0) ;
					// PP RE13/16 et SE 13/16 mensuels :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxEighthM_Chart15, 4, 0) ;

					// Modifications des inputs de ce qui est tracé sur le chart #19 /////
					// - niveaux psys :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxPsys_Chart19, 1, 0) ;																	// input TraceVectorielONOFF
					// - PP J :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxJ_Chart19, 2, 0) ;
					// - PP H :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxH_Chart19, 2, 0) ;
					// - PP M :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxM_Chart19, 2, 0) ;
					// - PP Quarts H :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxH_Chart19, 3, 0) ;
					// - PP Quarts M :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxM_Chart19, 3, 0) ;
					// - PP Eighth M :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxEighthM_Chart19, 2, 0) ;
					// - PP Full4/Mid4 J :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxJ_Chart19, 4, 0) ;
					// - PP Full4/Mid4 H :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxH_Chart19, 4, 0) ;
					// - PP Full4/Mid4 M :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxM_Chart19, 4, 0) ;
					// - PH et PB HP :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxH_Chart19, 5, 0) ;
					// - PH et PB MP :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxM_Chart19, 5, 0) ;
					// PP RE13/16 et SE 13/16 hebdos :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxEighthH_Chart19, 4, 0) ; 
					// PP RE13/16 et SE 13/16 mensuels :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxEighthM_Chart19, 4, 0) ; 
					
					// Modifications des inputs de ce qui est tracé sur le chart #6 (niveaux projetés)
					sc.SetChartStudyInputInt(6, StudyID_NiveauxPsys_Projetes_Chart6, 1, 0) ;		// input TraceVectorielONOFF
					// - dizaines :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxPsys_Projetes_Chart6, 2, 0) ;		
					// - 25 :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxPsys_Projetes_Chart6, 3, 0) ;
					// - 50 :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxPsys_Projetes_Chart6, 4, 0) ;
					// - niveaux psys moyens :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxPsys_Projetes_Chart6, 5, 0) ;
					// - PHJ PBJ dynamiques :
					sc.SetChartStudyInputInt(6, StudyID_PHJPBJ_Projetes_Chart6, 3, 0) ;
					// - PP J :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxJ_Projetes_Chart6, 2, 0) ;
					// - PP H :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxH_Projetes_Chart6, 2, 0) ;
					// - PP M :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxM_Projetes_Chart6, 2, 0) ;
					// - PP 1h :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxPP1h_Projetes_Chart6, 1, 0) ;
					// - PP 4h :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxPP4h_Projetes_Chart6, 1, 0) ;
					// - PP Quarts H :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxH_Projetes_Chart6, 3, 0) ;
					// - PP Quarts M :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxM_Projetes_Chart6, 3, 0) ;
					// - PP Eighth M :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxEighthM_Projetes_Chart6, 2, 0) ;
					// - PP Full4/Mid4 J :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxJ_Projetes_Chart6, 4, 0) ;
					// - PP Full4/Mid4 H :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxH_Projetes_Chart6, 4, 0) ;
					// - PP Full4/Mid4 M :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxM_Projetes_Chart6, 4, 0) ;
					// - PH et PB JP + Close JP :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxJ_Projetes_Chart6, 5, 0) ;
					// - PH et PB HP :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxH_Projetes_Chart6, 5, 0) ;
					// - PH et PB MP :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxM_Projetes_Chart6, 5, 0) ;					
					// PP RE13/16 et SE 13/16 mensuels :					
					sc.SetChartStudyInputInt(6, StudyID_NiveauxEighthM_Projetes_Chart6, 4, 0) ;					

					// Modifications des inputs de Navig_Niveaux /////
					// - NiveauxIndicesAffiches_Input
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 20, NiveauxIndicesAffiches_Input.GetInt()) ;		
					// - dizaines :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 11, 0) ;		
					// - 25 :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 12, 0) ;
					// - 50 :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 13, 0) ;
					// - niveaux psys moyens :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 14, 0) ;
					// - PP 1h :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 21, 0) ;
					// - PP 4h :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 22, 0) ;
					// - PP Quarts H :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 23, 0) ;
					// - PP Quarts M :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 25, 0) ;
					// - PP Eighth M :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 24, 0) ;
					// - PP Full4/Mid4 J/H/M :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 26, 0) ;
					// - PHPB J/H/M + Close JP:
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 27, 0) ;
					// - PHJPBJ :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 28, 0) ;
				}
				else if ( NiveauxIndicesAffiches_Input.GetInt() == 0 )  // le statut était "NIVEAUX OFF" => on le bascule vers "NIVEAUX ALL"
				{
					NiveauxIndicesAffiches_Input.SetInt(1) ;
					sc.SetCustomStudyControlBarButtonText(106, "   Niv Ind ALL  ") ; 
					
					// Modifications des inputs de ce qui est tracé sur le chart #15
					// - niveaux psys :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxPsys_Chart15, 1, 1) ;			// input TraceVectorielONOFF
					// - dizaines :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxPsys_Chart15, 2, 1) ;			// int SetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int IntegerValue);					
					// - 25 :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxPsys_Chart15, 3, 1) ;
					// - 50 :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxPsys_Chart15, 4, 1) ;
					// - niveaux psys moyens :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxPsys_Chart15, 5, 1) ;
					// - PP J :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxJ_Chart15, 2, 1) ;
					// - PP H :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxH_Chart15, 2, 1) ;
					// - PP M :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxM_Chart15, 2, 1) ;
					// - PP 1h :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxPP1h_Chart15, 2, 1) ;
					// - PP 4h :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxPP4h_Chart15, 2, 1) ;
					// - PP Quarts H :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxH_Chart15, 3, 1) ;
					// - PP Quarts M :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxM_Chart15, 3, 1) ;
					// - PP Eighth M :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxEighthM_Chart15, 2, 1) ;
					// - PP Full4/Mid4 J :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxJ_Chart15, 4, 1) ;
					// - PP Full4/Mid4 H :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxH_Chart15, 4, 1) ;
					// - PP Full4/Mid4 M :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxM_Chart15, 4, 1) ;
					// - PH et PB JP + Close JP :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxJ_Chart15, 5, 1) ;
					// - PH et PB HP :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxH_Chart15, 5, 1) ;
					// - PH et PB MP :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxM_Chart15, 5, 1) ;
					// PP RE13/16 et SE 13/16 mensuels :
					sc.SetChartStudyInputInt(15, StudyID_NiveauxEighthM_Chart15, 4, 1) ;
					
					// Modifications des inputs de ce qui est tracé sur le chart #19 /////
					// - niveaux psys :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxPsys_Chart19, 1, 1) ;																	// input TraceVectorielONOFF
					// - PP J :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxJ_Chart19, 2, 0) ;
					// - PP H :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxH_Chart19, 2, 1) ;
					// - PP M :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxM_Chart19, 2, 1) ;
					// - PP Full4/Mid4 J :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxJ_Chart19, 4, 0) ;
					// - PP Full4/Mid4 H :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxH_Chart19, 4, 1) ;
					// - PP Full4/Mid4 M :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxM_Chart19, 4, 1) ;
					// - PH et PB HP :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxH_Chart19, 5, 1) ;
					// - PH et PB MP :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxM_Chart19, 5, 1) ;
					// PP RE13/16 et SE 13/16 hebdos :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxEighthH_Chart19, 4, 1) ; 
					// PP RE13/16 et SE 13/16 mensuels :
					sc.SetChartStudyInputInt(19, StudyID_NiveauxEighthM_Chart19, 4, 1) ; 

					// Modifications des inputs de ce qui est tracé sur le chart #6 (niveaux projetés)
					// - niveaux psys :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxPsys_Projetes_Chart6, 1, 1) ;		// input TraceVectorielONOFF
					// - dizaines :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxPsys_Projetes_Chart6, 2, 1) ;		
					// - 25 :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxPsys_Projetes_Chart6, 3, 1) ;
					// - 50 :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxPsys_Projetes_Chart6, 4, 1) ;
					// - niveaux psys moyens :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxPsys_Projetes_Chart6, 5, 1) ;
					// - PHJ PBJ dynamiques :
					sc.SetChartStudyInputInt(6, StudyID_PHJPBJ_Projetes_Chart6, 3, 1) ;
					// - PP J :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxJ_Projetes_Chart6, 2, 1) ;
					// - PP H :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxH_Projetes_Chart6, 2, 1) ;
					// - PP M :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxM_Projetes_Chart6, 2, 1) ;
					// - PP 1h :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxPP1h_Projetes_Chart6, 1, 1) ;
					// - PP 4h :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxPP4h_Projetes_Chart6, 1, 1) ;
					// - PP Quarts H :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxH_Projetes_Chart6, 3, 1) ;
					// - PP Quarts M :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxM_Projetes_Chart6, 3, 1) ;
					// - PP Eighth M :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxEighthM_Projetes_Chart6, 2, 1) ;
					// - PP Full4/Mid4 J :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxJ_Projetes_Chart6, 4, 1) ;
					// - PP Full4/Mid4 H :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxH_Projetes_Chart6, 4, 1) ;
					// - PP Full4/Mid4 M :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxM_Projetes_Chart6, 4, 1) ;
					// - PH et PB JP + Close JP :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxJ_Projetes_Chart6, 5, 1) ;
					// - PH et PB HP :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxH_Projetes_Chart6, 5, 1) ;
					// - PH et PB MP :
					sc.SetChartStudyInputInt(6, StudyID_NiveauxM_Projetes_Chart6, 5, 1) ;					
					// PP RE13/16 et SE 13/16 mensuels :					
					sc.SetChartStudyInputInt(6, StudyID_NiveauxEighthM_Projetes_Chart6, 4, 1) ;					

					// Modifications des inputs de Navig_Niveaux /////
					// - NiveauxIndicesAffiches_Input
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 20, NiveauxIndicesAffiches_Input.GetInt()) ;		
					// - dizaines :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 11, 1) ;		
					// - 25 :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 12, 1) ;
					// - 50 :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 13, 1) ;
					// - niveaux psys moyens :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 14, 1) ;
					// - PP 1h :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 21, 1) ;
					// - PP 4h :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 22, 1) ;
					// - PP Quarts H :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 23, 1) ;
					// - PP Quarts M :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 25, 1) ;
					// - PP Eighth M :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 24, 1) ;
					// - PP Full4/Mid4 J/H/M :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 26, 1) ;
					// - PHPB J/H/M + Close JP:
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 27, 1) ;
					// - PHJPBJ :
					sc.SetChartStudyInputInt(6, StudyID_Navig_Niveaux, 28, 1) ;
				}
			}	
			
			else { sc.UpdateAlways = 0; }
		
		}  // fin de " Statut des Control Bar Buttons "
		
		
		
		// Statuts des robots actifs ; acheteur et/ou vendeur  (durée d'exécution du bloc sur le PC : 0.01 à 0.05 ms si 0 study AMR, 0.02 à 0.08 ms si 5 study AMR) (durée d'exécution du bloc sur le VPS : 0.02 à 0.03 ms si 0 study AMR, 0.04 à 0.05 ms si 5 study AMR)
		{
			// On détermine parmi les 40 premières StudyID lesquelles s'appellent "ANNUL MINI REBOND", puis on compte le nb de studies acheteuses actives et le nb de studies vendeuses actives  (en théorie il n'y a jamais plus d'une acheteuse active, ni plus d'une vendeuse active)
			SCString StudyName ;
			int NbRobotAcheteurActif = 0 ;
			int NbRobotVendeurActif = 0 ;
			
			// Recherche des studies AMR
			for ( int StudyID = 1 ; StudyID < 41 ; StudyID ++ )  
			{
				StudyName = sc.GetStudyNameUsingID( StudyID );
				
				if ( StudyName.Compare("ANNUL MINI REBOND",0) == 0 )      // Compare returns 0 if they are the same
				{
					// Cette study est-elle acheteuse active ?     // on récupère RobotPosition dans sc.Input[30] de AMR
					int RobotPosition ;    // RobotPosition vaut 0 (inactif), 1 (acheteur) ou -1 (vendeur).
					sc.GetChartStudyInputInt(sc.ChartNumber, StudyID, 30, RobotPosition) ;  
					if 			( RobotPosition == 1 )	{ NbRobotAcheteurActif ++ ;  }
					else if 	( RobotPosition == -1 )	{ NbRobotVendeurActif ++ ;  }
				}		
			}
			
			// Résultat des décomptes des robots acheteurs actifs  /////////////////////////////////////////
			if ( NbRobotAcheteurActif > 1 )
			{	
				RobotAcheteurActif_Input.SetInt(NbRobotAcheteurActif) ;  
				
				if ( TriggerWarningCritique_Achat )
				{	SCString message ;   message.Format("!! WARNING !!   Il y a %d robots acheteurs actifs", NbRobotAcheteurActif) ;    sc.AddMessageToLog(message,0) ;  TriggerWarningCritique_Achat = 0 ;  }    
			}
			else if ( NbRobotAcheteurActif == 1 )
			{	RobotAcheteurActif_Input.SetInt(1) ;  TriggerWarningCritique_Achat = 1 ;  }
			else
			{	RobotAcheteurActif_Input.SetInt(0) ;  }

			// Résultat des décomptes des robots vendeurs actifs
			if ( NbRobotVendeurActif > 1 )
			{	
				RobotVendeurActif_Input.SetInt(NbRobotVendeurActif) ;  
				
				if ( TriggerWarningCritique_Vente )
				{ SCString message ;   message.Format("!! WARNING !!   Il y a %d robots vendeurs actifs", NbRobotVendeurActif) ;    sc.AddMessageToLog(message,0) ;  TriggerWarningCritique_Vente = 0 ;  }
			}
			else if ( NbRobotVendeurActif == 1 )
			{	RobotVendeurActif_Input.SetInt(1) ;  TriggerWarningCritique_Vente = 1 ;  }
			else
			{	RobotVendeurActif_Input.SetInt(0) ;  }
		
		}  // fin du bloc " Statuts des robots actifs  "
		
		
		// SCDT_DernierChandelierVisible
		SCDT_DernierChandelierVisible = sc.BaseDateTimeIn[sc.IndexOfLastVisibleBar] ;
		
	}
	
}



