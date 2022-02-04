/*
v1.0 (2019/09/30) : boutons padding +10%/-10%.
v1.1 (2019/10/28) : cette study retourne le message "FENETRE ACTIVE" quand on appuie un bouton qui n'est pas reconnu de cette fenêtre.
v2.0 (2019/11/17) : les clics sur les boutons 9/11/12 sont transmis à la study AMR active.
v2.1 (2019/11/18) : les clics sur le bouton 60 (NEXT) sont transmis à la study PrixTypique et à toutes les studies AMR.
v2.1 (2019/11/21) : ajout des boutons _8 et _49.
v2.2 (2020/02/03) : ajout des boutons _52, _15, _16.
v2.3 (2020/07/18) : l'input ValeurDuNiveauActif_Input permet de gérér sc.BaseGraphScaleRangeBottom et sc.BaseGraphScaleRangeTop.
v2.4 (2020/07/18) : CTRL+Space met TriggerRecalculate à 1.
v2.5 (2020/07/21) : ValeurDuNiveauActif_Input est remplacé par ValeurMinDesStudiesAMR_Input et ValeurMinDesStudiesAMR_Input. La présente study récupère ces valeurs dans les studies AMR.
v2.6 (2020/07/22) : la study Padding n'envoie plus de trigger à AMR pour faire le check de l'état de la study active. C'est la study PrixTypique qui envoie à AMR le trigger.
v2.7 (2020/09/11) : fix du bug lors d'un appui sur Z sur charts 15 et 19 (désynchro entre charts ?) => transmission au 16-ticks de l'index du dernier chandelier visible. Idem pour CTRL+5.
v2.8 (2020/09/11) : la présente study n'est plus autorisée à gérer CTRL+5.
v2.9 (2020/09/30) : ajout du bouton _106, puis bouton _20.

*/

#include "sierrachart.h"  // When including in your source code file additional header files, they need to be placed above the #include "sierrachart.h" line. 

SCDLLName("Padding et boutons ACS")


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Texte : s_UseTool    // crée un texte avec le SCString "texte" pour être affiché en coordonnées (indice,prix)
{											
	Texte(SCString texte, int index, float prix, int tailleTexte,  COLORREF couleur, int graphRegion, int Line_Number) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
	{
		AddMethod = UTAM_ADD_OR_ADJUST ;		
		AddAsUserDrawnDrawing = 1 ;		
		LineNumber = Line_Number ;
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

void DrawText (SCString texte, int index, float prix, int tailleTexte, COLORREF couleur, int LineNumber, s_sc* ptrsc)  	    // DrawText n'est utilisé que pour les affichages qui ne se mettent pas à jour à chaque appel de la fonction
{
	//Création d'instance de Texte et appel de sc.UseTool
	Texte objetTexte(texte, index, prix, tailleTexte, couleur, ptrsc->GraphRegion, LineNumber) ;
	ptrsc->UseTool(objetTexte); 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawText_FenetreActive (COLORREF couleur, s_sc* ptrsc)
{
	SCString Texte ;  Texte.Format("FENETRE ACTIVE") ;
	int  IndexDuMilieu = (ptrsc->IndexOfFirstVisibleBar + ptrsc->IndexOfLastVisibleBar ) / 2 ;
	float PrixDuMileu = ( ptrsc->YPixelCoordinateToGraphValue(ptrsc->StudyRegionTopCoordinate) + ptrsc->YPixelCoordinateToGraphValue(ptrsc->StudyRegionBottomCoordinate) ) / 2 ;
	DrawText (Texte, IndexDuMilieu, PrixDuMileu, 40, couleur, 100000, ptrsc) ;    // void DrawText (SCString texte, int index, float prix, int tailleTexte, COLORREF couleur, int LineNumber, s_sc* ptrsc)  
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int GetActiveStudyID (s_sc* ptrsc)
{
	int ActiveStudyID = 0 ;
	int ChartNumber_StudyAMR = 6 ;
	int StudyIDPrixTypique = ptrsc->GetStudyIDByName(ChartNumber_StudyAMR, "Prix typique", 0);
	if  (StudyIDPrixTypique == 0 )  
	{	SCString message ;  message.Format("!! ERREUR !! Le Chart #%d ne contient pas la study ''Prix typique''. Necessaire pour determiner ''GetActiveStudyID''.", ChartNumber_StudyAMR) ; ptrsc->AddMessageToLog(message,1) ; }
	else
	{	ptrsc->GetChartStudyInputInt(ChartNumber_StudyAMR, StudyIDPrixTypique,1, ActiveStudyID) ;  }    // int GetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int& r_IntegerValue);
	
	if ( ActiveStudyID == 0 )  { SCString message ;  message.Format("!! WARNING !! Il n'y a pas de study AMR active ( ActiveStudyID == 0 )") ; ptrsc->AddMessageToLog(message,1) ;  }
	return ActiveStudyID ;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_Padding_Et_BoutonsACS(SCStudyInterfaceRef sc)
{
	
	SCInputRef 	TriggerRecalculate_Input = sc.Input[0]; 		// modifié par NavigNiveaux et PrixTypique et AMR
	// SCInputRef 	ValeurDuNiveauActif_Input = sc.Input[1];	// modifié par NavigNiveaux  et PrixTypique
	// SCInputRef 	ValeurMinDesStudiesAMR_Input = sc.Input[2];	
	// SCInputRef 	ValeurMaxDesStudiesAMR_Input = sc.Input[3];	
	
	int& StudyID_PrixTypique = sc.GetPersistentIntFast(0);	
	int& StudyID_TPNu = sc.GetPersistentIntFast(1);	
	
	if (sc.SetDefaults)
	{
		sc.AutoLoop = 0;  
		sc.GraphName = "Padding et boutons ACS"; 		// nom appelé par NavigNiveaux et PrixTypique
		sc.ScaleRangeType = SCALE_SAMEASREGION;
		sc.GraphRegion = 0;
		sc.DisplayStudyName = 0;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 0 ;
		sc.DisplayStudyInputValues = 0;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		sc.HideStudy = 1 ;

		// Liste des inputs
		TriggerRecalculate_Input.Name = "TriggerRecalculate";
		TriggerRecalculate_Input.SetInt(0);
		// ValeurMinDesStudiesAMR_Input.Name = "Valeur Min Des Studies AMR (pts)";
		// ValeurMinDesStudiesAMR_Input.SetFloat(999999);
		// ValeurMaxDesStudiesAMR_Input.Name = "Valeur Max Des Studies AMR (pts)";
		// ValeurMaxDesStudiesAMR_Input.SetFloat(0);
		
		return;
	}  // fin du bloc de " if (sc.SetDefaults) "
	
	
	if (sc.UpdateStartIndex == 0)
	{ 
		sc.GraphRegion = 0;	
		
		// Récupération de la studyID pour Prix Typique
		StudyID_PrixTypique = sc.GetStudyIDByName(6, "Prix typique", 0);
		if  ( StudyID_PrixTypique == 0 )
		{ SCString message ;  message.Format("!! ERREUR !! Le chart #6 ne contient pas la study ''Prix typique''") ; sc.AddMessageToLog(message,1) ; }

		// Récupération de la studyID pour Calcul TPNu
		StudyID_TPNu = sc.GetStudyIDByName(6, "Calcul TPNu", 0);
		if  ( StudyID_TPNu == 0 )
		{ SCString message ;  message.Format("!! ERREUR !! Le chart #6 ne contient pas la study ''Calcul TPNu''") ; sc.AddMessageToLog(message,1) ; }

	} 
	
	
	// Statut des Control Bar Buttons
	if ( sc.MenuEventID != 0 )
	{
		sc.UpdateAlways = 1;   // pour que la study soit recalculée immédiatement (sinon il faut attendre qu'une condition d'appel se produise, il n'y a jamais d'appel si le marché est fermé)
		sc.SetCustomStudyControlBarButtonEnable(sc.MenuEventID,FALSE); 
		
		if (sc.MenuEventID == ACS_BUTTON_1)   	// Bouton "Ctrl+Space" qui met TriggerRecalculate à 1
		{
			TriggerRecalculate_Input.SetInt(1) ;
			sc.ResetAllScales  = 1 ;  // équivalent à un appui de la touche "Space"
		}

		else if ( sc.MenuEventID == ACS_BUTTON_6 )     // Bouton "-1 pas" 		// touche /
		{
			// L'input External_Input_ACS_BUTTON_6 de la study active AMR est augmenté de 1
			int ChartNumber_StudyAMR = 6 ;
			int ActiveStudyID = GetActiveStudyID (&sc) ;
			if ( ActiveStudyID != 0 )
			{ 
				int NbDeClicsEnBuffer = 0 ;
				sc.GetChartStudyInputInt(ChartNumber_StudyAMR, ActiveStudyID,59, NbDeClicsEnBuffer) ;       		// int GetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int& r_IntegerValue);
				sc.SetChartStudyInputInt(ChartNumber_StudyAMR, ActiveStudyID,59, min(NbDeClicsEnBuffer+1,10)) ;  		// int SetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int IntegerValue);
			}
			
			DrawText_FenetreActive (RGB(0,200,0), &sc) ;
		}

		else if ( sc.MenuEventID == ACS_BUTTON_7 )     // Bouton "+1 pas" 		// touche *
		{
			// L'input External_Input_ACS_BUTTON_7 de la study active AMR est augmenté de 1
			int ChartNumber_StudyAMR = 6 ;
			int ActiveStudyID = GetActiveStudyID (&sc) ;
			if ( ActiveStudyID != 0 )
			{ 
				int NbDeClicsEnBuffer = 0 ;
				sc.GetChartStudyInputInt(ChartNumber_StudyAMR, ActiveStudyID,60, NbDeClicsEnBuffer) ;       		// int GetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int& r_IntegerValue);
				sc.SetChartStudyInputInt(ChartNumber_StudyAMR, ActiveStudyID,60, min(NbDeClicsEnBuffer+1,10)) ;  		// int SetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int IntegerValue);
			}
			
			DrawText_FenetreActive (RGB(0,200,0), &sc) ;
		}
		
		else if ( sc.MenuEventID == ACS_BUTTON_8 )     // Bouton "HoraireRaz=RoundToHour(LastVisibleBar)"  : SHIFT+Z
		{
			// L'input External_Input_ACS_BUTTON_8 de la study active AMR est mis à 1
			int ChartNumber_StudyAMR = 6 ;
			int ActiveStudyID = GetActiveStudyID (&sc) ;
			if ( ActiveStudyID != 0 )
			{ 
				sc.SetChartStudyInputInt(ChartNumber_StudyAMR, ActiveStudyID, 41, 1) ;   		// int SetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int IntegerValue);
			}
			
			DrawText_FenetreActive (RGB(0,200,0), &sc) ;
		}

		else if ( sc.MenuEventID == ACS_BUTTON_9 )     // Bouton "HoraireRaz=LastVisibleBar - NbBarresVirginite"                     // Keyboard shortcut :  Z
		{
			// L'input External_Input_ACS_BUTTON_9 de la study active AMR est mis à IndexLastVisibleBar
			int ActiveStudyID = GetActiveStudyID (&sc) ;
			int IndexLastVisibleBar_Chart6 =sc.GetContainingIndexForDateTimeIndex(6, sc.IndexOfLastVisibleBar);			// int GetContainingIndexForDateTimeIndex(int ChartNumber, int DateTimeIndex);
			if ( ActiveStudyID != 0 )
			{ sc.SetChartStudyInputInt(6, ActiveStudyID, 37, IndexLastVisibleBar_Chart6) ;  } 		// int SetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int IntegerValue);
			
			if ( IndexLastVisibleBar_Chart6 == 0 ) 
			{ SCString message ;  message.Format("!! WARNING pour bouton Z !! IndexLastVisibleBar pour chart #6 vaut 0 car le chart #6 ne contient pas la seance affichee sur le chart #%d", sc.ChartNumber) ; sc.AddMessageToLog(message,1) ; }
			
			DrawText_FenetreActive (RGB(0,200,0), &sc) ;
		}
		
		else if ( sc.MenuEventID == ACS_BUTTON_10 )     // Bouton "9h30m16s"  : CTRL+Z
		{
			// L'input External_Input_ACS_BUTTON_10 de la study active AMR est mis à 1
			int ChartNumber_StudyAMR = 6 ;
			int ActiveStudyID = GetActiveStudyID (&sc) ;
			if ( ActiveStudyID != 0 )
			{ 
				sc.SetChartStudyInputInt(ChartNumber_StudyAMR, ActiveStudyID, 44, 1) ;   		// int SetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int IntegerValue);
			}
			
			DrawText_FenetreActive (RGB(0,200,0), &sc) ;
		}		

		else if ( sc.MenuEventID == ACS_BUTTON_11 )     // Bouton "-1min" 
		{
			// L'input External_Input_ACS_BUTTON_11 de la study active AMR est augmenté de 1
			int ChartNumber_StudyAMR = 6 ;
			int ActiveStudyID = GetActiveStudyID (&sc) ;
			if ( ActiveStudyID != 0 )
			{ 
				int NbDeClicsEnBuffer = 0 ;
				sc.GetChartStudyInputInt(ChartNumber_StudyAMR, ActiveStudyID,38, NbDeClicsEnBuffer) ;       		// int GetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int& r_IntegerValue);
				if (sc.ChartNumber == 15 ) { sc.SetChartStudyInputInt(ChartNumber_StudyAMR, ActiveStudyID,38, min(NbDeClicsEnBuffer+1,10)) ;  }  		// int SetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int IntegerValue);
				else if (sc.ChartNumber == 19 ) { sc.SetChartStudyInputInt(ChartNumber_StudyAMR, ActiveStudyID,38, min(NbDeClicsEnBuffer+4,40)) ;  }
			}
			
			DrawText_FenetreActive (RGB(0,200,0), &sc) ;
		}
		
		else if ( sc.MenuEventID == ACS_BUTTON_12 )     // Bouton "+1min"
		{
			// L'input External_Input_ACS_BUTTON_12 de la study active AMR est augmenté de 1
			int ChartNumber_StudyAMR = 6 ;
			int ActiveStudyID = GetActiveStudyID (&sc) ;
			if ( ActiveStudyID != 0 )
			{ 
				int NbDeClicsEnBuffer = 0 ;
				sc.GetChartStudyInputInt(ChartNumber_StudyAMR, ActiveStudyID,39, NbDeClicsEnBuffer) ;       		// int GetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int& r_IntegerValue);
				if (sc.ChartNumber == 15 )  { sc.SetChartStudyInputInt(ChartNumber_StudyAMR, ActiveStudyID,39, min(NbDeClicsEnBuffer+1,10)) ;  }  		// int SetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int IntegerValue);
				else if (sc.ChartNumber == 19 )  { sc.SetChartStudyInputInt(ChartNumber_StudyAMR, ActiveStudyID,39, min(NbDeClicsEnBuffer+4,40)) ;  }  
			}
			
			DrawText_FenetreActive (RGB(0,200,0), &sc) ;
		}		
		
		else if ( sc.MenuEventID == ACS_BUTTON_15 )     // Bouton "bonus -" 
		{
			// L'input External_Input_ACS_BUTTON_15 de la study active AMR est augmenté de 1
			int ChartNumber_StudyAMR = 6 ;
			int ActiveStudyID = GetActiveStudyID (&sc) ;
			if ( ActiveStudyID != 0 )
			{ 
				int NbDeClicsEnBuffer = 0 ;
				sc.GetChartStudyInputInt(ChartNumber_StudyAMR, ActiveStudyID,48, NbDeClicsEnBuffer) ;       					// int GetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int& r_IntegerValue);
				sc.SetChartStudyInputInt(ChartNumber_StudyAMR, ActiveStudyID,48, min(NbDeClicsEnBuffer+1,20)) ;   		// int SetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int IntegerValue);
			}
			
			DrawText_FenetreActive (RGB(0,200,0), &sc) ;
		}

		else if ( sc.MenuEventID == ACS_BUTTON_16 )     // Bouton "bonus +" 
		{
			// L'input External_Input_ACS_BUTTON_16 de la study active AMR est augmenté de 1
			int ChartNumber_StudyAMR = 6 ;
			int ActiveStudyID = GetActiveStudyID (&sc) ;
			if ( ActiveStudyID != 0 )
			{ 
				int NbDeClicsEnBuffer = 0 ;
				sc.GetChartStudyInputInt(ChartNumber_StudyAMR, ActiveStudyID, 49, NbDeClicsEnBuffer) ;       					// int GetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int& r_IntegerValue);
				sc.SetChartStudyInputInt(ChartNumber_StudyAMR, ActiveStudyID, 49, min(NbDeClicsEnBuffer+1,20)) ;   		// int SetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int IntegerValue);
			}
			
			DrawText_FenetreActive (RGB(0,200,0), &sc) ;
		}

		else if ( sc.MenuEventID == ACS_BUTTON_20 )     // Bouton "récupération externe de bonus" 
		{
			// L'input External_Input_ACS_BUTTON_20 de la study active AMR est mis à 1
			int ChartNumber_StudyAMR = 6 ;
			int ActiveStudyID = GetActiveStudyID (&sc) ;
			if ( ActiveStudyID != 0 )
			{ 
				sc.SetChartStudyInputInt(ChartNumber_StudyAMR, ActiveStudyID, 46, 1) ;   		// int SetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int IntegerValue);
			}
			
			DrawText_FenetreActive (RGB(0,200,0), &sc) ;
		}

		else if ( sc.MenuEventID == ACS_BUTTON_49 )     // Bouton "AffichageONOFF"    // Keyboard Shorcut: Inser
		{
			// L'input External_Input_ACS_BUTTON_49 de la study active AMR est augmenté de 1
			int ChartNumber_StudyAMR = 6 ;
			int ActiveStudyID = GetActiveStudyID (&sc) ;
			if ( ActiveStudyID != 0 )
			{ 
				int NbDeClicsEnBuffer = 0 ;
				sc.GetChartStudyInputInt(ChartNumber_StudyAMR, ActiveStudyID,42, NbDeClicsEnBuffer) ;       		// int GetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int& r_IntegerValue);
				sc.SetChartStudyInputInt(ChartNumber_StudyAMR, ActiveStudyID,42, min(NbDeClicsEnBuffer+1,2)) ;   		// int SetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int IntegerValue);
			}
			
			DrawText_FenetreActive (RGB(0,200,0), &sc) ;
		}		

		else if ( sc.MenuEventID == ACS_BUTTON_52 )     // Bouton "Autotrade"    // Keyboard Shorcut: A
		{
			// L'input External_Input_ACS_BUTTON_52 de la study active AMR est augmenté de 1
			int ChartNumber_StudyAMR = 6 ;
			int ActiveStudyID = GetActiveStudyID (&sc) ;
			if ( ActiveStudyID != 0 )
			{ 
				int NbDeClicsEnBuffer = 0 ;
				sc.GetChartStudyInputInt(ChartNumber_StudyAMR, ActiveStudyID,47, NbDeClicsEnBuffer) ;       		// int GetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int& r_IntegerValue);
				sc.SetChartStudyInputInt(ChartNumber_StudyAMR, ActiveStudyID,47, min(NbDeClicsEnBuffer+1,2)) ;   		// int SetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int IntegerValue);
			}
			
			DrawText_FenetreActive (RGB(0,200,0), &sc) ;
		}		

		else if ( sc.MenuEventID == ACS_BUTTON_53 )     // Bouton "CTRL+A" (refresh de prix de l'ordre limite d'entrée en position, pour la study acive)
		{
			// L'input External_Input_ACS_BUTTON_53 de la study active AMR est mis à 1
			int ChartNumber_StudyAMR = 6 ;
			int ActiveStudyID = GetActiveStudyID (&sc) ;
			if ( ActiveStudyID != 0 )  sc.SetChartStudyInputInt(ChartNumber_StudyAMR, ActiveStudyID, 50, 1) ;   		// int SetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int IntegerValue);
			
			DrawText_FenetreActive (RGB(0,200,0), &sc) ;
		}			

		else if ( sc.MenuEventID == ACS_BUTTON_54 )     // Bouton "Shift+A" (refresh de prix de l'ordre limite d'entrée en position, pour toutes les studies)
		{
			int ChartNumber_StudyAMR = 6 ;

			// On détermine parmi les 40 premières StudyID lesquelles s'appellent "ANNUL MINI REBOND", puis les inputs External_Input_ACS_BUTTON_53 de toutes les studies AMR sont mis à 1
			{
				SCString StudyName ;
				// Recherche des studies AMR
				for ( int StudyID = 1 ; StudyID < 41 ; StudyID ++ )  
				{
					StudyName = sc.GetStudyNameFromChart(ChartNumber_StudyAMR, StudyID);
					
					if ( StudyName.Compare("ANNUL MINI REBOND",0) == 0 )      // Compare returns 0 if they are the same
					{
						sc.SetChartStudyInputInt(ChartNumber_StudyAMR, StudyID, 50, 1) ;   	
					}		
				}
			}
			
			DrawText_FenetreActive (RGB(0,200,0), &sc) ;
		}			

		else if ( sc.MenuEventID == ACS_BUTTON_60 )     // Bouton "NEXT"     // L'input External_Input_ACS_BUTTON_60 de la study "Prix Typique" est augmenté de 1
		{
			int ChartNumber_StudyPrixTypique = 6 ;
			
			// On appelle "Prix typique" et on modifie son External_Input_ACS_BUTTON_60
			{ 
				int NbDeClicsEnBuffer = 0 ;
				sc.GetChartStudyInputInt(ChartNumber_StudyPrixTypique, StudyID_PrixTypique,10, NbDeClicsEnBuffer) ;       	
				sc.SetChartStudyInputInt(ChartNumber_StudyPrixTypique, StudyID_PrixTypique,10, min(NbDeClicsEnBuffer+1, 5)) ;   	// L'input External_Input_ACS_BUTTON_60 de la study "Prix typique" est augmenté de 1
			}
			
			DrawText_FenetreActive (RGB(0,200,0), &sc) ;
		}	
		
		else if ( sc.MenuEventID == ACS_BUTTON_90 )     // Bouton "KILL" 
		{
			int ChartNumber_StudyAMR = 6 ;
			
			// On détermine parmi les 40 premières StudyID lesquelles s'appellent "ANNUL MINI REBOND", puis les inputs External_Input_ACS_BUTTON_90 de toutes les studies AMR sont mis à 1
			{
				SCString StudyName ;
				// Recherche des studies AMR
				for ( int StudyID = 1 ; StudyID < 41 ; StudyID ++ )  
				{
					StudyName = sc.GetStudyNameFromChart(ChartNumber_StudyAMR, StudyID);
					
					if ( StudyName.Compare("ANNUL MINI REBOND",0) == 0 )      // Compare returns 0 if they are the same
					{
						sc.SetChartStudyInputInt(ChartNumber_StudyAMR, StudyID, 51, 1) ;   	
					}		
				}
			}
			
			DrawText_FenetreActive (RGB(0,200,0), &sc) ;
		}		

		else if (sc.MenuEventID == ACS_BUTTON_99)   		// Bouton "Pad-"
		{
			sc.BaseGraphAutoScalePaddingPercentage  = sc.BaseGraphAutoScalePaddingPercentage  - 0.10 ;
		}

		else if (sc.MenuEventID == ACS_BUTTON_100)   	// Bouton "Pad+"
		{
			sc.BaseGraphAutoScalePaddingPercentage  = sc.BaseGraphAutoScalePaddingPercentage  + 0.10 ;
		}

		else if ( sc.MenuEventID == ACS_BUTTON_106 )     // Bouton "Niveaux Affichés"     // L'input External_Input_ACS_BUTTON_106 de la study "Prix Typique" est augmenté de 1
		{
			int ChartNumber_StudyPrixTypique = 6 ;
			
			// On appelle "Prix typique" et on modifie son External_Input_ACS_BUTTON_60
			{ 
				int NbDeClicsEnBuffer = 0 ;
				sc.GetChartStudyInputInt(ChartNumber_StudyPrixTypique, StudyID_PrixTypique, 32, NbDeClicsEnBuffer) ;       	
				sc.SetChartStudyInputInt(ChartNumber_StudyPrixTypique, StudyID_PrixTypique, 32, min(NbDeClicsEnBuffer+1, 5)) ;   	// L'input External_Input_ACS_BUTTON_106 de la study "Prix typique" est augmenté de 1
			}
			
			DrawText_FenetreActive (RGB(0,200,0), &sc) ;
		}		

		else if ( sc.MenuEventID == ACS_BUTTON_109 )     // Bouton "Niv -" 
		{
			// L'input External_Input_ACS_BUTTON_109_110 de la study active AMR et l'input External_Input_ACS_BUTTON_109 de la study "Navig_niveaux" sont augmentés de 1
			int ChartNumber_StudyAMR = 6 ;
			int ActiveStudyID = GetActiveStudyID (&sc) ;
			if ( ActiveStudyID != 0 )
			{ 
				int NbDeClicsEnBuffer = 0 ;
				sc.GetChartStudyInputInt(ChartNumber_StudyAMR, ActiveStudyID,43, NbDeClicsEnBuffer) ;       		// int GetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int& r_IntegerValue);
				sc.SetChartStudyInputInt(ChartNumber_StudyAMR, ActiveStudyID,43, min(NbDeClicsEnBuffer+1, 5)) ;   		// int SetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int IntegerValue);
			}
			
			// On détermine parmi les 50 premières StudyID laquelle s'appelle "Navigation niveaux" et on modifie son External_Input_ACS_BUTTON_109
			for ( int StudyID = 1 ; StudyID < 51 ; StudyID ++ )  
			{
				SCString StudyName ;
				StudyName = sc.GetStudyNameFromChart(ChartNumber_StudyAMR, StudyID) ;
				if ( StudyName.Compare("Navigation niveaux",0) == 0 )      // returns 0 if they are the same
				{ 
					int NbDeClicsEnBuffer = 0 ;
					sc.GetChartStudyInputInt(ChartNumber_StudyAMR, StudyID, 9, NbDeClicsEnBuffer) ;       	
					sc.SetChartStudyInputInt(ChartNumber_StudyAMR, StudyID, 9, min(NbDeClicsEnBuffer+1, 5)) ;   	// L'input External_Input_ACS_BUTTON_109 de la study Navigation niveaux" est augmenté de 1
					break ;
				}
			}
			
			DrawText_FenetreActive (RGB(0,200,0), &sc) ;
		}		

		else if ( sc.MenuEventID == ACS_BUTTON_110 )     // Bouton "Niv +" 
		{
			// L'input External_Input_ACS_BUTTON_109_110 de la study active AMR et l'input External_Input_ACS_BUTTON_110 de la study "Navig_niveaux" sont augmentés de 1
			int ChartNumber_StudyAMR = 6 ;
			int ActiveStudyID = GetActiveStudyID (&sc) ;
			if ( ActiveStudyID != 0 )
			{ 
				int NbDeClicsEnBuffer = 0 ;
				sc.GetChartStudyInputInt(ChartNumber_StudyAMR, ActiveStudyID,43, NbDeClicsEnBuffer) ;       		// int GetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int& r_IntegerValue);
				sc.SetChartStudyInputInt(ChartNumber_StudyAMR, ActiveStudyID,43, min(NbDeClicsEnBuffer+1, 5)) ;   		// int SetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int IntegerValue);
			}
			
			// On détermine parmi les 50 premières StudyID laquelle s'appelle "Navigation niveaux" et on modifie son External_Input_ACS_BUTTON_110
			for ( int StudyID = 1 ; StudyID < 51 ; StudyID ++ )  
			{
				SCString StudyName ;
				StudyName = sc.GetStudyNameFromChart(ChartNumber_StudyAMR, StudyID) ;
				if ( StudyName.Compare("Navigation niveaux",0) == 0 )      // returns 0 if they are the same
				{ 
					int NbDeClicsEnBuffer = 0 ;
					sc.GetChartStudyInputInt(ChartNumber_StudyAMR, StudyID, 10, NbDeClicsEnBuffer) ;       	
					sc.SetChartStudyInputInt(ChartNumber_StudyAMR, StudyID, 10, min(NbDeClicsEnBuffer+1, 5)) ;   	// L'input External_Input_ACS_BUTTON_110 de la study Navigation niveaux" est augmenté de 1
					break ;
				}
			}
			
			DrawText_FenetreActive (RGB(0,200,0), &sc) ;
		}		

		else if ( sc.MenuEventID == ACS_BUTTON_133  or  sc.MenuEventID == ACS_BUTTON_134 )     // Bouton "navigation rapide"  : CTRL+5 et CTRL+Numpad5
		{
			// L'input External_Input_ACS_BUTTON_133_134 de la study active AMR est mis à IndexLastVisibleBar
			int ActiveStudyID = GetActiveStudyID (&sc) ;
			int IndexLastVisibleBar_Chart6 =sc.GetContainingIndexForDateTimeIndex(6, sc.IndexOfLastVisibleBar);			// int GetContainingIndexForDateTimeIndex(int ChartNumber, int DateTimeIndex);
			if ( ActiveStudyID != 0 )
			{ sc.SetChartStudyInputInt(6, ActiveStudyID, 45, IndexLastVisibleBar_Chart6) ;  } 		// int SetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int IntegerValue);
			
			if ( IndexLastVisibleBar_Chart6 == 0 ) 
			{ SCString message ;  message.Format("!! WARNING pour bouton CRL+5 !! IndexLastVisibleBar pour chart #6 vaut 0 car le chart #6 ne contient pas la seance affichee sur le chart #%d", sc.ChartNumber) ; sc.AddMessageToLog(message,1) ; }
			
			DrawText_FenetreActive (RGB(0,200,0), &sc) ;
		}		

		else if ( sc.MenuEventID == ACS_BUTTON_135 )     // Bouton "Toggle TPNu" 		// touche t
		{
			// L'input External_Input_ACS_BUTTON_135 de la study TPNu est modifié
			int  ChartNumber_Study_TPNu = 6 ;
			int Toggle_Diviseur_TPNu = 0 ;
			sc.GetChartStudyInputInt(ChartNumber_Study_TPNu, StudyID_TPNu, 6, Toggle_Diviseur_TPNu) ;       		// int GetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int& r_IntegerValue);
			sc.SetChartStudyInputInt(ChartNumber_Study_TPNu, StudyID_TPNu, 6, 1-Toggle_Diviseur_TPNu) ;  			// int SetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int IntegerValue);
			sc.RecalculateChart(ChartNumber_Study_TPNu) ;
			
			DrawText_FenetreActive (RGB(0,200,0), &sc) ;
		}

		// Boutons non gérés par la study : "Cible -1t" (6), "Cible +1t" (7), "Ajust -1t" (27), "Ajust 0" (28), "Ajust +1t" (29), "Sequence" (56), "Void" (61)
		else if ( sc.MenuEventID == ACS_BUTTON_6  or  sc.MenuEventID == ACS_BUTTON_7  or  sc.MenuEventID == ACS_BUTTON_27  or  sc.MenuEventID == ACS_BUTTON_28  or  sc.MenuEventID == ACS_BUTTON_29  or  sc.MenuEventID == ACS_BUTTON_56  or  sc.MenuEventID == ACS_BUTTON_61 )  
		{
			DrawText_FenetreActive (RGB(255,0,0), &sc) ;
		}
	} 
	else
	{ 
		sc.DeleteUserDrawnACSDrawing(sc.ChartNumber, 100000) ; 
		sc.UpdateAlways = 0; 
	}

	
	// gestion de sc.BaseGraphScaleRangeBottom et sc.BaseGraphScaleRangeTop
	if ( TriggerRecalculate_Input.GetInt() != 0  and  sc.ChartWindowIsActive )	// si ChartWindowIsActive == 0, IndexOfFirstVisibleBar = IndexOfLastVisibleBar
	{
		// Récupération de ValeurMinDesStudiesAMR et ValeurMaxDesStudiesAMR
		float ValeurMinDesStudiesAMR = 999999 ;
		float ValeurMaxDesStudiesAMR = 0 ;
		
		// On détermine parmi les 40 premières StudyID lesquelles s'appellent "ANNUL MINI REBOND", puis on récupère Void et NiveauIndiceTexte
		{
			SCString StudyName ;
			// Recherche des studies AMR
			for ( int StudyID = 1 ; StudyID < 41 ; StudyID ++ )  
			{
				StudyName = sc.GetStudyNameFromChart(6, StudyID );
				
				if ( StudyName.Compare("ANNUL MINI REBOND",0) == 0 )      // Compare returns 0 if they are the same
				{
					// récupération de la variable persistante Void
					int& Void = sc.GetPersistentIntFromChartStudy(6, StudyID, 1);		// int& GetPersistentIntFromChartStudy(int ChartNumber, int StudyID, int Key);
					
					// récupération de l'input FutureOuIndice_Input
					int FutureOuIndice_Input ;
					sc.GetChartStudyInputInt(6, StudyID, 6, FutureOuIndice_Input) ;  
					
					if ( /* Void == 0  and */  FutureOuIndice_Input == 0 ) // i.e. study visible et trading indice (FutureOuIndice_Input == 0)
					{
						// récupération de NiveauIndiceTexte et affectation à ValeurMinDesStudiesAMR ou ValeurMaxDesStudiesAMR
						double NiveauIndiceTexte ;
						sc.GetChartStudyInputFloat(6, StudyID, 10, NiveauIndiceTexte) ;  
						if ( NiveauIndiceTexte < ValeurMinDesStudiesAMR )  ValeurMinDesStudiesAMR = NiveauIndiceTexte ;
						if ( NiveauIndiceTexte > ValeurMaxDesStudiesAMR )  ValeurMaxDesStudiesAMR = NiveauIndiceTexte ;
					}
				}		
			}
		}
		
		
		// Calcul de MinBaseGraphValue et MaxBaseGraphValue (valeurs min et max du BaseGraph à l'écran)
		int FirstIndex = sc.IndexOfFirstVisibleBar ;
		int LastIndex = sc.IndexOfLastVisibleBar ;
		float MinBaseGraphValue = 999999 ;
		float MaxBaseGraphValue = 0 ;
		for ( int index = FirstIndex ; index <= LastIndex ; index ++ )
		{
			if ( MinBaseGraphValue > sc.Low[index] )  MinBaseGraphValue = sc.Low[index] ;
			if ( MaxBaseGraphValue < sc.High[index] )  MaxBaseGraphValue = sc.High[index] ;
		}
		
		
		// Calculer BaseGraphScaleRangeBottom et BaseGraphScaleRangeTop
		if ( TriggerRecalculate_Input.GetInt() == 1 )	// cas général : SCALE_USERDEFINED
		{
			sc.BaseGraphScaleRangeType = SCALE_USERDEFINED ;		// ne pas confondre avec la valeur sc.ScaleRangeType qui s'applique à la study
			
			if ( ValeurMinDesStudiesAMR < MinBaseGraphValue )  sc.BaseGraphScaleRangeBottom = ValeurMinDesStudiesAMR ;
			else sc.BaseGraphScaleRangeBottom = MinBaseGraphValue ;

			if ( ValeurMaxDesStudiesAMR > MaxBaseGraphValue )  sc.BaseGraphScaleRangeTop = ValeurMaxDesStudiesAMR ;
			else sc.BaseGraphScaleRangeTop = MaxBaseGraphValue ;
			
			// Padding pour voir le texte affiché
			sc.BaseGraphAutoScalePaddingPercentage = 0.05 ;
		}
		else if ( TriggerRecalculate_Input.GetInt() == 2 )	// cas particuier de LastCallToFunction de AMR : SCALE_AUTO
		{
			sc.BaseGraphScaleRangeType = SCALE_AUTO ;		// ne pas confondre avec la valeur sc.ScaleRangeType qui s'applique à la study
			sc.BaseGraphScaleRangeBottom = MinBaseGraphValue ;
			sc.BaseGraphScaleRangeTop = MaxBaseGraphValue ;
		}

		// RAZ de TriggerRecalculate
		TriggerRecalculate_Input.SetInt(0) ;
	}
	

	
	// lorsque je suis déconnecté d'IB, pour les backtests :
	if ( sc.ServerConnectionState != SCS_CONNECTED )  
	{	sc.UpdateAlways = 1 ;  }
	
	
}



