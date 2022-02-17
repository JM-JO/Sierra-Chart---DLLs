#include <chrono>
#include "sierrachart.h"


SCDLLName("Annulation mini rebond")

/* 
v0.0 (2019/3/7) : codage des inputs.
v0.1 (2019/3/7) : codage des initialisations et des calculs d'AUC et de Sigma. SigmaAccelere n'étant plus utilisé, il n'a pas été codé. Tracés des courbes : RatioAUCBis, RatioSigmaBis et SommeRatioBis.
v0.2 (2019/3/8) : indicateur tracé sur le graphique du prix.
v0.3 (2019/3/11) : ajouter des zones grisées entre SL/cible/1*TP/2*TP/3*TP/4*TP.
v0.4 (2019/3/11) : créer des CustomInputStrings (avec des valeur tous les ticks) pour le NiveauFuture.
v0.5 (2019/3/11) : le NiveauFuture est géré par deux inputs concomittamment : NiveauFutureTexte et NiveauFutureRoulette
v0.6 (2019/3/12) : ajout des marqueurs et du texte sur le graphe.
v0.7 (2019/3/13) : calcul des mvts verticaux relatifs lors de la première touche.
v0.8 (2019/3/13) : codage du compte-ticks.
v0.8.1 (2019/3/13) : améliorations et débuggage (Background en noir si données sources de Sigma sont manquantes).
v0.8.2 (2019/3/14) : HoraireRAZ est géré par deux inputs concomittamment : HoraireRAZ et HoraireRAZRoulette. L'indicateur "Annul mini rebond" pour le future est finalisé.
v1.0 (2019/3/15) : début codage de l'indicateur pour les niveaux de l'indice : les inputs et affichages ont été codés.
v1.1 (2019/3/15) : AUC devient AUC_Future. Introduction de AUC_Indice.
v1.2 (2019/3/19) : choix des niveaux à trader en menu déroulant.
v1.2.1 (2019/3/19) : complétion des PP/R/S à trader du future. OK finalisé.
v1.2.2 (2019/3/19) : complétion des PP/R/S à trader de l'indice.
v1.2.3 (2019/3/19) : modifs/corrections mineures.
v1.2.4 (2019/3/22) : modifs/corrections mineures.
v1.3.0 (2019/3/27) : les niveaux future sont arrondis au tick le plus proche.(
v1.3.1 (2019/3/27) : on sépare l'input de l'affichage ON/OFF et l'input Future/Indice
v1.3.2 (2019/3/27) : modifications dans les inputs : choix de l'UT lorsque AMRONOFF==0.
v1.3.3 (2019/3/27) : modifications dans les inputs : choix du niveau tradé lorsque AMRONOFF==0.
v1.4.0 (2019/3/28) : ajout d'un bouton "AMR ON/OFF".
v1.4.1 (2019/3/28) : ajout des boutons "CT ON/OFF" et "NewDHiLo ON/OFF".
v1.4.2 (2019/3/29) : modifications des noms des studies appelées ; ajout d'un message dans le log si la study appelée n'existe pas.
v1.4.3 (2019/3/29) : modification de l'input de NewDHiLo. HoraireRAZRoulette : l'input est passé au format 12h34m56s. Ajout de boutons "-1min" et "+1min". Ajout de boutons "B+5" et "B-5". Ajout de boutons "TP +1" et "TP -1".
v1.4.4 (2019/4/1) : ajout de boutons "CibleFut -2t" et "+2t".
v1.5.0 (1/4/19) : sur le compte-ticks, ajout d'une courbe qui interpole les droites de bascule TP0.
v1.5.1 (2/4/19) : modifications des QuelNiveauTradeInput.
v1.5.2 (2/4/19) : RAZ de QuelNiveauTradeInput lorsqu'on change d'UT.
v1.5.3 (2/4/19) : on sépare les quarts des Full/Mid dans les InputStrings.
v1.5.4 (3/4/19) : affichage de AUC/seuil du future lors du trading indice.
v1.6.0 (3/4/19) :  la contribution de AUC_Indice est multipliée par *1.6 (v1.6 au 3 avril 2019) pour fitter avec le X-volume de PRT. Le seuil AUC_Future est augmenté de 10% pour que RatioAUC_Future de SC et PRT coïncident. EnvergureMoyenneFull est passée de 9 à 8.2 pour que Sigma de SC et PRT coïncident.
v1.7.0 (4/4/19) : HoraireRAZ est amélioré (changements d'heure et de jour fonctionnent parfaitement).
v1.7.1 (5/4/19) : ajout des boutons "Ajust". La RAZ des boutons après un LastCallToFunction est gérée par une boucle for. Tracé de NiveauCible non ajusté.
v1.7.2 (5/4/19) : synchronisation du bouton "AMR ON/OFF" avec le statut "visible/hidden" de la study.
v1.7.3 (8/4/19) : RAZ des subgraph et des usetool dans sc.Index == 0.
v1.8.0 (8/4/19) : TPNu : création du mode automatique.
v1.8.1 (9/4/19) : ajout d'un bouton "TPNu Manuel" / " TPNu Auto ". Calcul des valeurs clés à IndexPremiereTouche-1.
v1.8.2 (10/4/19) : pour le tracé des indicateurs normés (AUC, Sigma et AUC+Sigma), NiveauStopLoss est remplacé par BaseDuTracé ; BaseDuTracé est calculé avec le SL_Prior du calcul précédent.
v1.8.3 (10/4/19) : on fixe une limite du nombre de barres calculées après HoraireRAZ pour éviter les calculs interminables : MaxBarresCalculees. On retourne la durée d'un calcul arrêté.
v1.8.4 (13/4/19) : BonusIntrinseque_Roulette_Input de 2 en 2 jusqu'à 50, puis de 5 en 5.
v1.9.0 (15/4/19) : on indique la première touche de BasculeTP0. On décale les affichages (sur 3 tiers).
v1.9.1 (15/4/19) : on indique la première touche de SL pour le trading sur indice. Réorganisation de l'ordre d'affichage des inputs.
v1.9.2 (16/4/19) : suppression de la gestion de StudyVisibilityState.
v1.9.3 (25/4/19) : embellissement de CreationDeHoraireRAZInputString.
v1.9.4 (16/5/19) : embellissement du code.
v2.0 (22/5/19) : début du codage du trading semi-auto. Passage d'un ordre triple lorsqu'on clique sur le bouton "Trading Auto". Ajout d'un trait vertical pointillé sur la barre où l'ordre est soumis.
v2.0.1 (23/5/19) : tracé du TPOptimum=TPNu*SI*(1+BonusExcedentaire/100) sur le graphe (TPOptimum est un TP sans arrondi).
v2.0.2 (23/5/19) : Toggle "Trading Auto" OFF : s'il n'y a pas de position ouverte et s'il y a des ordres en attente, on applique sc.FlattenAndCancelAllOrders().  Toggle "AMR OFF" : si "Trading Auto" est ON on le bascule en OFF.
v2.0.3 (23/5/19) : ajout d'un bouton KILL pour sc.FlattenAndCancelAllOrders() sans condition.
v2.0.4 (23/5/19) : lorsque SommeRatio dépasse 100% et qu'on n'est pas en position, on annule les ordres.
v2.0.5 (24/5/19) : ajout de HoraireExpiration. Si HoraireExpiration vaut 0 alors il n'y a pas d'horaire d'expiration.
v2.1.0 (24/5/19) : TPOptimumArrondi est TPOptimum arrondi au tick inférieur initialement, puis arrondi au tick supérieur lorsqu'on le dépasse, ou au tick inférieur lorsqu'on passe sous le demi-tick. 
v2.1.1 (24/5/19) : le robot reproduit les valeurs de TPOptimumArrondi (et le SL associé) dès qu'elles changent (et que SommeRatio < 100%).
v2.1.2 (24/5/19) : ajout de la variable permanente : NiveauFutureOrdreEntree (prix d'entrée demandé), qui correspond à NiveauFutureAjuste lors du démarrage du robot.
v2.2.0 (28/5/19) : QuelNiveauTradeInput n'est plus RAZ après la sélection d'un niveau tradé par la roulette.
v2.2.1 (28/5/19) : pour le trading indice, NiveauFutureOrdreEntree est calculé sur la dernière barre (en sommant NiveauIndice+SpreadMoyen+AjustementChoisi), puis un ordre d'entrée est soumis. 
v2.2.2 (28/5/19) : pour le trading indice, si NiveauFutureOrdreEntree est différent de NiveauFuture, NiveauFutureTexte est modifié et un recalcul de l'indicateur est effectué.
v2.2.3 (28/5/19) : la valeur du SL utilisé pour le robot est affectée au SL_Prior du tracé de AMR ; lancer un TriggerRecalculate.
v2.3.0 (28/5/19) : pour le trading indice, le SL est rallongé à SL'=SL*1.3+2 ; la vraie sortie a lieu lorsque l'indice dépasse SL.
v2.4.0 (5/6/19) : le malus d'ajustement (2.5% par tick, soit 10% par point) est augmenté (4% par tick, soit 16% par point).
v2.5.0 (5/6/19) : pour le trading indice, la sortie en perte (stop loss) a lieu lorsque l'indice dépasse le stop loss du future.
v2.6.0 (5/6/19) : création de NiveauIndiceSurFutureTempsReel (le prix où se balade l'indice sur le future).
v2.6.1 (5/6/19) : création de AjustementSubiPoints : c'est l'écart entre NiveauIndice et NiveauIndiceSurFutureTempsReel. Il est retranché à BonusExcedentaire.
v2.7.0 (6/6//19) : gestion du dépassement de Bascule TP0.
v2.7.1 (6/6/19) : annulation du trading auto si TPOptimumArrondi < 0.75.
v2.8.0 (7/6/19) : tester UTAM_ADD_OR_ADJUST sur un tool qui trace un incrément sur la dernière barre ; création de la fonction DrawText_UTAMAOA.
v3.0.0 (7/6/19) : affichage/recalcul en temps réel des données du graphe (en dehors de la boucle de looping, si NiveauEntree non dépassé).
v3.0.1 (7/6/19) : finalisation du travail débuté dans v3.0.0.
v3.0.2 (10/6/19) : correction de bug. QuelNiveauTradeInput doit être remis sur sa valeur indéfinie ("Indice - Choix niv") pour éviter de recalculer la valeur "initiale" d'un niveau indice lors d'un TriggerRecalculate.
v3.1.0 (11/6/19) : gestion des drawings persistants après un TriggerRecalculate avec AddAsUserDrawnDrawing (UDD) : création des fonctions DrawText_UTAMAOA_UDD et DrawMarker_UTAMAOA_UDD
v3.1.1 (12/6/19) : densification des bonus intrinsèques <26.
v3.2.0 (13/6/19) : le bloc "Dépassement Stop Loss Indice" est déplacé en dehors du bloc "Compte-ticks pour TPNu variable".
v3.2.1 (13/6/19) : la variable BasculeTP0Depassee vaut 1 si ( PremiereToucheFaite == 1) et ( SommeRatio > 1.6 ) .
v3.2.2 (14/6/19) : création de DrawSegmentPointille_UTAMAOA_UDD pour tracer qui s'arrêtent à LastIndex.
v3.3.0 (14/6/19) : faire en sorte que l'affichage des paramètres sur le graphique cesse de bouger. Ajout des annulations d'ordres à 15h50 et fermetures de position à 15h55.
v3.3.1 (14/6/19) : changements de noms des boutons (NewDHiLow devient NDHL, AMRONOFF devient AffichageONOFF). Kill passe de n°60 à n°90.
v4.0.0 (17/6/19) : on utilise une paire de studies AMR (une pour position acheteuse, l'autre pour position vendeuse). Elles partagent les mêmes boutons, sauf KILL (commun) et un bouton spécifique à chacune pour les activer/désactiver.
v4.0.1 (17/6/19) :  on met des LineNumber différents pour les positions acheteuses vs vendeuses.
v4.0.2 (18/6/19) : le bouton "Trace ON/OFF" devient Trace pour simplifier l'affichage. Les inputs de study ne sont affichés que pour la study active.
v4.0.3 (18/6/19) : le texte des boutons "Autotrade" et "NDHL" est mis à jour lorsqu'on bascule entre vente et achat. Ajout de TextTag sur l'ordre d'entrée (impossible de mettre un TextTag associé à la fonction sc.ModifyOrder) .
v4.1.0 (19/6/19) : lorsque BonusExcedentaire est insuffisant ou que SommeRatio est > 100%, il faut également être à moins de 3*TP du prix d'entrée (DistanceMiniAuDanger < 3) pour que l'ordre soit annulé.
v4.1.1 (19/6/19) : lors de TriggerRAZ, vérifier que le statut de NDHL est congruent avec le prix actuel et le prix tradé.
v4.2.0 (19/6/19) : les robots AMR "vente" et "achat" ne doivent pas se tirer dans les pattes => il faut annuler spécifiquement les ordres de l'un et de l'autre, et ne plus utiliser de FlattenAndCancelAllOrders (sauf pour KILL). Modification des paramètres du robot pour soumettre deux ordres en sens opposé.
v4.2.1 (20/6/19) : le kill du stop loss indice est remplacé par une modification de l'ordre stop du bracket.
v4.2.2 (20/6/19) : création de la variable CetteInstanceControleLesBoutons.
v4.2.3 (21/6/19) : modification du nom de certaines studies appelées (PP projetés sur future).
v4.2.4 (21/6/19) : bouton affiche "busy" lorsqu'un bouton est pressé.
v4.3.0 (21/6/19) : nouvelle architecturedu robot de trading. Suppression de : TradingAutoONOFF, TriggerPremierOrdre. Introduction de : OrdreOuvert, PositionOuverte, AutoTradeStatus, AutoTradeDemandeArret, AutoTradeFACAO.
							Architecture du robot : 1) recueil des infos (variables OrdreOuvert et PositionOuverte), 2) gestion des bugs (annulation/fermetures difficiles), 3) gestion des événements (déclencheurs de modifications d'ordres / de position), 4) gestion de AutoTradeDemandeArret et AutoTradeFACAO, 5) ouverture d'ordres.Cette version (v4.3.0.cpp) n'a pas été compilée.
v4.3.1 (25/6/19) : suite codage de v4.3.0.
v4.3.2 (25/6/19) : débuggage.
v4.3.3 (25/6/19) : gestion de tous les cas de non-démarrage du robot.
v4.3.4 (25/6/19) : débuggage.
v4.3.5 (26/6/19) : les variables BoutonStudyVenteONOFF, BoutonStudyAchatONOFF et CetteInstanceControleLesBoutons sont remplacées par des inputs pour qu'elles soient initialisées proprement dans le bloc sc.SetDefaults.
							Architecture du robot :	1) récupération de ParentOrderDetails, Target1OrderDetails et Stop1OrderDetails, pour calculer PositionOuverte et OrdreOuvert
																2) gestion des bugs 
																3) gestion des événements (déclencheurs de modifications d'ordres / de position)
																4) gestion de AutoTradeDemandeArret et AutoTradeFACAO
																5) démarrage du robot
v4.3.6 (26/6/19) : gestion du clic sur le bouton AutoTrade lorsque le statut est : 11, 100, 110, 120, 130, 140 ou 150.
v4.3.7 (26/6/19) : création de la fonction SetButton52Text() : lorsqu'on l'appelle, elle gère le texte du bouton 52 (AutoTradeStatus) en prenant en compte la valeur de CetteInstanceControleLesBoutons. Ce robot a été validé sur mon compte réel IB (ouvertures/femetures d'ordres long/court en parallèle)
v4.4.0 (26/6/19) : gestion automatique de NewDailyHighLow_Input.
v4.4.1 (27/6/19) : choix de l'input de la date via DateEtudieeRoulette. On conserve l'input DateEtudiee (mais il n'est plus affiché).
v4.4.2 (27/6/19) : exprimés en valeur non absolues : Ajustement Choisi = Prix Tradé - Niveau Projeté Statique,   Ajustement Subi = Niveau Projeté Statique - Niveau Projeté Temps Réel. Donc on a : Ajustement Choisi + Ajustement Subi = Prix Tradé - Niveau Projeté Temps Réel.
							=> remplacer " fabs(AjustementChoisiEnPas*4.0) + fabs(AjustementSubiPoints*16.0) " par " fabs(AjustementChoisiEnPas*4.0 + AjustementSubiPoints*16.0) ".
v4.4.3 (27/6/19) : la valeur de NewDHiLo n'est affichée que si on fait un NewDHiLo.
v4.4.4 (28/6/19) : la study active est highlightée.
v4.4.5 (28/6/19) : correction du bug de l'affichage de "Master/Slave | ID | ..." qui n'update pas après basculement de CetteInstanceControleLesBoutons lorsque PremiereToucheFaite>0. Utilisation de la fonction 
v5.0.0 (28/6/19) : le nb d'instances AMR est illimité, et le statut Master/Slave est géré par le bouton NEXT et contrôlé par la study "Prix Typique".
v5.0.1 (28/6/19) : correction de bugs : les LineNumber " 100xx+1000*Position " deviennent " 100xx+1000*ThisStudyID ".
v5.0.2 (1/7/19) : le bouton "Trace" devient "Affiche ON/OFF".
v5.0.3 (1/7/19) : on supprime les "ResetAllScales=1" consécutifs à des TriggerRecalculate, et on se contente de "ResetAllScales=1" lorsqu'on toggle l'affichage ON/OFF. C'est pour éviter les nombreux ResetAllScales initiés par les robots lorsqu'il y a une modification de TP.
v5.0.4 (1/7/19) : on introduit TriggerResetScale (fera un sc.ResetAllScales) lors du prochain appel du DLL.
v5.1.0 (2/7/19) : les paramètres affichés pour le niveau tradé doivent toujours être visibles avec l'espacement "1" => les colonnes -1200/-800/-400 sont remplacées par -990/-660/-330.
v5.1.1 (2/7/19) : les MARKER_TRIANGLERIGHT qui indiquent le niveau tradé et le prix tradé sont tracés avec un LineNumber et positionnés dans la colonne -1100
v5.2.0 (2/7/19) : DistanceMiniAuDanger n'est plus égal à DistMiniFuture / TPOptimum. Dorénavant DistanceMiniAuDanger = DistanceMiniAuNiveau / TPOptimum (avec une valeur minimum de 0.75 pts pour TPOptimum afin de durcir les situations de faible TPOptimum).
v5.2.1 (2/7/19) : lors du LastCallToFunction, le FlattenAndCancelAllOrders est remplacé par une DemandeArret si un ordre est ouvert, ou un popup si une position est ouverte.
v5.2.2 (2/7/19) : les changements de AutoTradeStatus reportés dans le log sont décrits de manière plus explicites (numéro + texte).
v5.3.0 (3/7/19) : on utilise la variable "Interligne" pour positionner le texte sur le graphe.
v5.3.1 (3/7/19) : lorsque la study est la study active, tous les textes en noir sont tracés en bleu.
v5.3.2 (4/7/19) : la déclaration de variable " s_sc* ptrsc = &sc; " remplacée par " s_sc* const ptrsc = &sc; ".
v5.4.0 (4/7/19) : dans les fonctions DrawXXX (DrawText, ...), la variable Graphregion est supprimée. On utilise ptrsc->GraphRegion pour la passer à la structure.
v5.4.1 (4/7/19) : le toggle ON/OFF de l'affichage est interdit si on est en position.
v5.4.2 (4/7/19) : CT est activé lorsqu'on démarre le robot ; il est impossible de désactiver le CT tant que le robot est actif.
v5.4.3 (5/7/19) : lastindex-500 ; warning si trading indice et horaire < 9h30m25s ; suppression du trait vertical bleu qui indique SeuilAUC>100%.   
v5.5.0 (6/7/19) : lorsque PremiereToucheFaite == 1, il faut que l'affichage continue de se mettre à jour (pour avoir la bonne couleur et le bon interligne). Version de travail.    
v5.5.1 (6/7/19) : lorsque PremiereToucheFaite == 1, il faut que l'affichage continue de se mettre à jour (pour avoir la bonne couleur et le bon interligne). Version compilée.    
v5.5.2 (7/7/19) : le triangle vert/rouge sur NiveauFutureAjuste et la ligne générée par DrawText_ID_NomDuNiveauTrade_Ajustement sont toujours affichés, quelque soit le statut de AffichageONOFF.
v5.5.3 (7/7/19) : éliminer les fuites de mémoires (tracés qui sont superposés à chaque appel du DLL, sans LineNumber).
v5.5.4 (7/7/19) : création d'une procédure pour effacement des s_UseTool::AddAsUserDrawnDrawing.
v5.6.0 (7/7/19) : ajout du nom du niveau tradé.
v5.6.1 (7/7/19) : lorsqu'on ajoute une instance de AMR, il y a des bugs dans l'affichage des boutons 105/106 (alors que AMR n'y touche pas !) => on ajoute un bloc de correction du texte affiché.
v5.6.2 (7/7/19) : correction de bug : arrondi au quart de point de NiveauFuture.
v5.6.3 (8/7/19) : ajout du nom du niveau tradé dans DrawText_ID_NomDuNiveauTrade_Ajustement, et dans le bouton NEXT. Version compatible avec v1941.
v5.6.4 (8/7/19) : ajout du nom du niveau tradé dans les logs du robot (changements de AutotradeStatus) et dans TextTag.
v5.6.5 (9/7/19) : ajout de AutoTradeStatus = 12. ( "Auto Trading Enabled Chart est OFF").
v5.6.6 (9/7/19) : modifications sur le positionnement des messages d'annulations d'ordres du robot.
v5.6.7 (9/7/19) : HoraireExpiration est mis automatiquement sur les PP 1h (H+55min) et sur les PP 4h (4h-15min).
v5.6.8 (9/7/19) : interdiction des modifs via bouton (+ Bonus / +1Min / +1Ajust / …) si la study n'est pas affichée.
v5.6.9 (9/7/19) : modifications de SetButton52Text.
v5.7.0 (10/7/19) : raccourcissement des textes affichés : (0 aj choisi) devient (0 aj). Permutation de lignes dans la colonne d'affichage -660. Ajout d'un message lorsqu'un robot a démarré mais qu'il n'a pas encore soumis d'ordre.
v5.7.1 (10/7/19) : ajout de blocs pour améliorer la gestion des paragraphes.
v5.7.2 (10/7/19) : gestion de bug lié à BasculeTP0Depassee et StopLossIndiceDepasse
v5.7.3 (10/7/19) : le tracé des graphes ne doit jamais s'arrêter si le robot est actif.
v5.7.4 (10/7/19) : mettre des warnings (pop up) si on modifie une study dont le robot est démarré (+1 cible / +bonus / +1 ajust / …).
v5.7.5 (11/7/19) : arrêt du manual Looping sur niveau indice après 16h : utilisation de IndexArretManualLooping.
v5.8.0 (11/7/19) : TPNuSI est multiplié par l'input MultiplicateurPrix, pour tenir compte du fait que la zone d'AUC et les TP augmentent lorsqu'augmente la valeur de l'indice (7900 aujourd'hui vs 6000 à 7000 pour la mise au point du système).
v5.8.1 (11/7/19) : création de l'input "RobotPosition".
v5.8.2 (11/7/19) : correction du bug sur RatioAUC_Prior lié à une référence sc.GetPersistentFloat utilisée deux fois. Correction de la formule pour TPNuSI_Prior.
v5.8.3 (12/7/19) : bugfix pour l'ajout d'un message lorsqu'un robot a démarré mais qu'il n'a pas encore soumis d'ordre.
v5.8.4 (12/7/19) : évènements d'arrêt : SommeRatio dépasse 100% et Distance < 2 ; SommeRatio dépasse 150% et Distance < 3 ; SommeRatio dépasse 300% et Distance < 4.
v5.8.5 (12/7/19) : ajout de blocs pour améliorer la gestion des paragraphes.
v5.9.0 (12/7/19) : solution au problème de l'algo de complexité n². On calcule IndexDeFin (correspond à IndexPremiereTouche (si touché) ou LastIndex (si pas touché). Un fichier .cpp a été compilé, le DLL fonctionne mais le .cpp a été écrasé (bug ?) par NPP.
v5.9.1 (15/7/19) : on recode v5.9.1 qui a été perdu (avec un nom de .cpp différent pour ne pas effacer le DLL v5.9.0). 
v5.9.2 (15/7/19) : calcul de NbBarresVirginite à IndexDeFin, puis calcul de IndexWarpJump (l'index à partir duquel seront calculés/affichés les indicateurs AUC, Sigma etc...). La complexité algorithmique est bien descendue à n.
v5.9.3 (15/7/19) : pour le robot : TP mini = 0,75, même si le TP OptimumArrondi est 0,5. Si OptimumArrondi < 0.75 et DangerProche, on annule toujours bien l'ordre. Le but est de soumettre moins d'ordres inutiles (TP0.5 n'a pas de sens), et de soumettre le premier ordre plus tôt même si la condition sur le TP mini n'est pas remplie.
v5.9.4 (15/7/19) : Tracer un triangle vert/rouge sur le graphe indice à HoraireRAZ sur le NiveauIndice tradé.
v5.9.5 (16/7/19) : ne pas prendre de trade avec un SL qui peut dépasser  PH/PB J => gérer via événement (ordres refermés si SLDepasse ET DangerProche ).
v5.9.6 (16/7/19) : ajout de PlaySound lorsque le robot devient inactif.
v5.9.7 (16/7/19) : lorsque AffichageONOFF bascule vers OFF via les inputs, il faut purger les UDD et remettre l'ajustement à zéro.
v5.9.8 (17/7/19) : ajout de la condition DistMiniFuture>=0 lorsqu'on fait une demande d'annulation d'ordre.
v5.9.9 (17/7/19) : on retravaille les conditions sur BonusExcedentaireSuffisant.
v6.0.0 (18/7/19) : affichage des temps : de cycle complet d'affichage / de calcul complet d'AMR pour la study active.
v6.0.1 (18/7/19) : PlaySound n'est pas appelé quand on arrête AutoTrade manuellement.
v6.0.2 (18/7/19) : on remplace l'affichage "!! Bonus exc (%.0f%%) < Malus aj (%.0f%%) !!" par "!! BonusExcedentaireSuffisant = %.0f%% !!"
v6.0.3 (18/7/19) : lorsqu'on bascule une study de OFF vers ON via ses inputs, elle devient la study active.
v6.0.4 (19/7/19) : AutoTradeStatus==12 revient à 0 après un clic si le problème est résolu.
v6.0.5 (20/7/19) : HoraireExpiration = 0 pour tous les niveaux autres que les PP1h et PP4h (RAZ de HoraireExpirationlorsqu'on change un input sur les niveau tradé)
v6.1.0 (20/7/19) : l'affichage des paramètres de la study est réalisé avec les valeurs relatives de l'écran.
v6.1.1 (22/7/19) : amélioration de l'affichage. Les triangles sur les niveaux tradés sont affichés avec des positions relatives.
v6.1.2 (23/7/19) : implémentation de la navigation sur les niveaux.
v6.1.3 (23/7/19) : débuggage de la navigation sur niveaux.
v6.2.0 (26/7/19) : rajout de warnings quand on effectue une modification interdite sur un robot actif.
v6.2.1 (26/7/19) : ajout du bouton Void pour faire disparaître une study de l'affichage. Tracé d'un triangle sur les graphes 200/2000 ticks lors du trading sur future.
v6.2.2 (31/7/2019) : empêcher un deuxième robot d'acheter (resp. vendre) si un premier robot est déjà en situation d'achat => mise en attente.
v6.2.3 (1/8/2019) : MultiplicateurPrix (qui valait 1.2 jusque là) vaut dorénavant Prix_FNQ100/6500 au moment de l'initialisation de AMR (sa valeur peut être changée manuellement).
v6.3.0 (20/8/2019) : modifs mineures.
v6.3.1 (21/8/2019) : introduction de TPBooste = TP*[2-(SommeRatio)/Seuil]. 
v6.3.2 (21/8/2019) : décalage des DrawText_RelativePosition_UTAMAOA de 25 unités vers la gauche.
v6.3.3 (22/8/2019) : le coefficient des malus d'ajustement (16% par point d'ajustement) devient une variable déclarée "float const" : CoeffMalusAjustement.
v6.3.4 (22/8/2019) : lors de la création d'une instance d'AMR, les paramètres initiaux sont pris sur "sc.IndexOfLastVisibleBar-500" (versus "LastIndex-500" précédemment). Les boutons "Niv+" / "Niv-" fonctionnent même si AffichageONOFF==0
v6.3.5 (22/8/2019) : densification de BonusIntrinseque_Roulette_Input.SetCustomInputStrings.
v6.3.6 (24/8/2019) : CoeffMalusAjustement devient un input.
v6.4.0 (24/8/2019) : on remplace " NiveauIndiceAjuste = NiveauIndice + AjustementChoisiEnPas*0.25 " par " NiveauIndiceAjuste = NiveauIndice + AjustementChoisiEnPas*0.25 + AjustementSubiPoints ". NiveauIndiceAjuste tient dorénavant compte du fait qu'il puisse dériver par rapport à NiveauIndice. Précédemment un décalage de x ticks ou le même ajustement de x ticks conduisaient à des Bonus/Malus identiques, mais les AUC pouvaient être significativement différentes !
v6.4.1 (4/9/2019) : CoeffMalusAjustement devient une variable qui vaut 90000/INQ100.
v6.4.2 (4/9/2019) : tracer SommeRatioAvecMalus = SommeRatio + MalusSommeRatio sur le graphe.
v6.4.3 (5/9/2019) : tracer SommeRatioAvecMalus = SommeRatio * (1 + MalusSommeRatio/100) sur le graphe, ainsi que MalusSommeRatio.
v6.4.4 (6/9/2019) : gestion du robot de trading lorsque BonusMoy est insuffisant. Lors du lancement du robot, le NiveauFuture est arrondi au tick le plus proche (vs le demi-point le plus proche précédemment).
v6.4.5 (6/9/2019) : les triangles rouge/vert restent affichés sur les graphes externes lorsqu'on bascule vers Affichage OFF.
v6.5.0 (6/9/2019) : séquence sur niveau : entrées avant, sur, puis après le niveau (ajustement choisi de +2pt / 0 / -2pt). Création de l'input "Sequence" qui vaut Yes/No. Création de la variable persistante "SequenceStatus".
v6.5.1 (10/9/2019) : pour la séquence sur niveau : l'ajustement choisi est calculé avec la formule suivante = TPNu * SI * MultiplicateurPrix * (1+BonusIntrinseque/4)
v6.5.2 (11/9/2019) : ajout du bouton "Sequence". Modification de SetButton52Text en SetButton52And56Text.
v6.5.3 (12/9/2019) : NiveauFutureOrdreEntree est dorénavant calculé au moment d'envoyer le premier ordre triple (et plus au moment de cliquer sur le bouton orange).
v6.5.4 (12/9/2019) : création de AppendFutOuInd. Modifications de l'affichage pour la 1ère colonne.
v6.5.5 (13/9/2019) : dépassements de HoraireExpiration ou 15h50 => l'annulation des ordres induit également SequenceStatus=0.
v6.5.6 (13/9/2019) : correction des bugs liés à IndexArretManualLooping = -1.
v6.5.7 (16/9/2019) : correction du bug observé lorsqu'on se balade de niveau en niveau : Position ne vaut ni 1 ni -1 => Position vaut alors 0 => AffichageOFF + Message à l'écran "Impossible de déterminer si la Position est acheteuse ou vendeuse"
v6.5.8 (16/9/2019) : autoriser "Affichage OFF" si AutoTradeStatus==11 (robot demarre mais pas encore d'ordre ouvert).
v6.5.9 (16/9/2019) : résolution du bug lorsque le trading indice va au-delà de 16h00. Création de SCDateHoraireFinSeanceAction (15h59m50s).
v6.6.0 (18/9/2019) : affichage en temps réel de la triplette de durées : dernier appel du DLL | dernier recalcul complet | durée depuis le dernier appel du DLL. 
v6.6.1 (19/9/2019) : si NbBarresVirginite est inférieur à 100% au moment de la touche, on affichage "(SommeRatio+Malus)/Virginite". 
v6.6.2 (21/9/2019) : variable persistante "Void" pour gérer les affichages lorsque le bouton "Void" a été cliqué.
v6.6.3 (21/9/2019) : la study AMR transmet les inputs suivants pour la study "Amplitude_Mvt" : IndexHoraireRAZ, Niveau, NatureNiveau, Position, TriggerRecalculate.
v6.6.4 (22/9/2019) : création de l'input NatureNiveau_Input.
v6.7.0 (25/9/2019) : modification majeure de l'architecture : le bloc " Statut des Control Bar Buttons " passe avant le bloc de calcul ; les ACS_Button qui le permettent sont associés à un "return;" pour gérer rapidement les appuis multiples d'un bouton. Attention : sc.UpdateAlways doit être positionné à 1 avant chaque instruction " return; ".
v6.7.1 (27/9/2019) : correction de bug (le premier trade d'une séquence n'était pas ajusté car TPNuSI valait 0 car n'était pas une variable persistante).
v6.7.2 (27/9/2019) : ajout d'un diamant sur le graphe externe (UT 20s) sur le chandelier correspondant à l'entrée en position sur future. Les niveaux indice projetés sont tracés à partir de 9h00 (le SpreadFutureIndice correspond à sa valeur à la cloture de la veille) pour naviguer sur les niveaux avant 9h30.
v6.7.3 (30/9/2019) : lorsque le bouton "+1min" est cliqué et HoraireRAZ vaut 9h30, la valeur suivante devient 9h30m25s. Idem pour "-1min" et 9h31.
v6.7.4 (30/9/2019) : création de Set_StudyAmplitudeMvt : fonction qui gère l'envoi des instructions vers "Amplitude_Mvt".
v6.7.5 (30/9/2019) : tracé d'une ligne horizontale rose sur les graphes #11, #12, #15 et #19 si le robot est actif.
v6.7.6 (02/10/2019) : le calcul de DistanceMiniAuNiveau est corrigé et simplifié.
v6.7.7 (03/10/2019) : initialisation des studies AMR : la mettre à 9h00, s'il est entre 9h00 et 9h35 au moment d'appeler sc.Defaults. Ajout d'un bouton (n°10 / raccourci clavier Z) qui met l'horaire à 9h30m25s. Lorsque "Niv+-" est cliqué et que NiveauIndice==0, on remet le future sur le prix de sc.IndexOfLastVisibleBar.
v6.7.8 (04/10/2019) : ajout de la condition  " DistMiniFuture doit être >0 pour pouvoir ouvrir une position " ; ceci empêchera de prendre un trade (dans une séquence) pour lequel le prix future a déjà été dépassé.
v6.7.9 (07/10/2019) : réécriture du bloc BasculeTP0 pour qu'il soit plus facilement compréhensible en 16-ticks (vs 8-ticks pour la version héritée de PRT).
v6.8.0 (07/10/2019) : lorsqu'une séquence est en cours et qu'on clique le bouton "Seq", on bascule sur le trade suivant (c'est une annulation manuelle), sauf si AutoTradeStatus==11.
v6.9.0 (14/10/2019) : modification de la formule de calcul de DistMiniIndice ( on remplace "NiveauIndiceAjuste" par "NiveauIndice"). Modification de la formule de calcul de Contribution_Indice ( on remplace "DistMiniIndice" par "DistMiniIndice + Position*(Ajustement Choisi + AjustementSubi)").  
v6.9.1 (14/10/2019) : une study ajoutée devient la study active. Malheureusement la fonction FindFirstMissingStudyID ne fonctionne pas (lors de l'appel du bloc sc.SetDefaults, StudyGraphInstanceID vaut encore zéro).
v6.9.2 (14/10/2019) : une study ajoutée devient la study active. Ca fonctionne.
v6.9.3 (16/10/2019) : correction d'un bug majeur introduit dans v6.9.0 : "Contribution_Indice = DistMiniIndice - Position*(Ajustement Choisi + AjustementSubi)" remplace "Contribution_Indice = DistMiniIndice + Position*(Ajustement Choisi + AjustementSubi)"
v6.9.4 (16/10/2019) : Ajout d'un bouton (n°9 / raccourci clavier CTRL+Z) qui met l'horaire à LastIndex - 1min.
v6.9.5 (17/10/2019) : correction de bug sur HoraireExpiration : deux robots actifs dans le même sens doivent s'arrêter simultanément (précédemment le premier s'arrêtait à HoraireExpiration et le second prenait le relai dans le cas particulier où il n'était pas affiché). La solution a été de modifier CreationSCDateHoraireExpiration (rajouter de 2 min de tampon).
v6.9.6 (17/10/2019) : correction de bug sur dépassement de 15h50 : deux robots actifs dans le même sens doivent s'arrêter simultanément (précédemment le premier s'arrêtait à 15h50 et le second prenait le relai dans le cas particulier où il n'était pas affiché). La solution a été de rajouter un tampon de 2 min (arrêt lancé jusqu'à 15h52).
v7.0.0 (17/10/2019) : création de l'Input HoraireNews et de la variable SCDateHoraireNewsMoins5Min. Arrêt du robot 5 min avant la news s'il n'est pas en position, et kill si besoin 20s avant la news.
v7.0.1 (21/10/2019) : ajout de AutoTradeStatus = 200 pour "OFF car 5 min avant News".
v7.0.2 (21/10/2019) : correction de DistanceMiniAuNiveau qui est fausse (elle ne tenait pas compte de l'ajustement).
v7.0.3 (22/10/2019) : afficher rond bleu (study active) sur indice, même si la study est masquée. Modifier le bouton "Void" en "Void ON/OFF".
v7.0.4 (22/10/2019) : gestion du téléscopage entre deux états contradictoires : " AutoTradeDemandeArret == 1 " et " PositionOuverte == 1 " => on annule la demande d'arrêt et on laisse le trade ouvert s'exécuter.
v7.0.5 (23/10/2019) : dans gestion des événements : ajout de conditions sur " DistMiniFuture[LastIndex] " en complément de PositionOuverte.
v7.0.5 (28/10/2019) : on remplace le nom de variable "DistanceAuDanger" par "DistanceMiniAuDanger".
v7.0.6 (29/10/2019) : le tracé rose pointillé sur les graphes externes indice et future débutent à HoraireRAZ.
v7.0.7 (29/10/2019) : modification de button_9 : HoraireRAZ se fait à "sc.LastVisibleBar-1min".
v7.0.8 (30/10/2019) : quand on appuie sur "Z" (button_9), la study devient AffichageON, si elle ne l'était pas encore. 
v7.0.9 (31/10/2019) : lors de la bascule vers TP0.25, on inscrit dans le log quel était le TP avant bascule.
v7.1.0 (31/10/2019) : lorsque Affichage=OFF, on met sc.HideStudy=1 pour accélérer l'affichage.   => une petite accélération a lieu (2ms par fonction AMR sur un tracé en dézoom max).
v7.2.0 (31/10/2019) : accélération de l'affichage de AMR : les zones grises transparentes sont tracées vectoriellement.
v7.2.1 (1/11/2019) : suppression des SubGraph devenus inutiles : Niveau1TPFT, etc...
v7.2.2 (1/11/2019) : les useTool qui ont sauté à cause du "sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0)" sont remis avec des LineNumber et en UDD.
v7.2.3 (6/11/2019) : modifs mineures.
v7.2.4 (12/11/2019) : le bouton "HoraireRaz=LastVisibleBar - 1min" modifie dorénavant l'input DateEtudiee.
v7.2.5 (13/11/2019) : un clic pour basculer sur Sequence=OFF remet l'ajustement à 0. SequenceONOFF_Input est dorénavant initialisé à OFF.
v7.3.0 (13/11/2019) : tests autour du Market Depth.
v7.3.1 (14/11/2019) : création de la variable MyEP (ma position estimée dans la queue). MyEP est affichée à côté de TPNu. Mettre une entrée dans le log à l'ouverture de l'ordre triple et à chaque fois que je gagne une place.
v7.3.2 (15/11/2019) : "SommeRatio depasse 600%" et "SommeRatioAvecMalus depasse 600%" est remplacé par "RatioAUC dépasse 600%". Idem pour remplacement par "RatioAUC dépasse 300% et DistanceAuDanger < 4" 
v7.3.3 (16/11/2019) : débuggage de Sigma et Sigma/Seuil.
v7.3.4 (17/11/2019) : tracé de SeuilSigma à la première touche sur le tracé de Sigma (sc.Graphregion=0).
v7.4.0 (17/11/2019) : création des External_Input_ACS_BUTTON_XYZ.
v7.4.1 (18/11/2019) : quand on appuie sur "-1min" (button_11) ou "+1min" (button_12), la study devient AffichageON, si elle ne l'était pas encore. 
v7.4.2 (18/11/2019) : ajout de External_Input_ACS_BUTTON_60 (NEXT).
v7.4.3 (19/11/2019) : décalage de l'algo de cloture d'ordres de 15h50 à 15h45.
v7.4.4 (20/11/2019) : DrawSegmentPointille_UTAMAOA_UDD devient DrawSegment_UTAMAOA_UDD et LineStyle est passé en argument.
v7.4.5 (21/11/2019) : le bouton 8 met HoraireRAZ au début de l'heure pile (utile pour trader les PP 1h).
v7.4.6 (21/11/2019) : optimisation de code : la fonction sc.GetStudyIDByName n'est appelée que lors des " if ( sc.UpdateStartIndex == 0  or  TriggerRecalculate == 1) ".
v7.4.7 (22/11/2019) : ajout d'un message lors de PremiereTouche, si le SL indice (resp. future) dépasse le PH/PB indice (resp. future).
v7.4.8 (23/11/2019) : résolution du bug de DrawRay qui part à gauche.
v7.4.9 (24/11/2019) : création de BasculeTP0_Test qui vaut : BasculeTP0[index] modifié par (2-RatioSigma[IndexPremiereTouche-1]).
v7.5.0 (25/11/2019) : marquage de la qté de NiveauFutureAjuste sur la projection du CO sur le graphe 16-ticks. Création des boutons 13 "-1sec" et 14 "+1sec".
v7.5.1 (26/11/2019) : on remplace partout "10xxx+1000*ThisStudyID" par "10xxx+10000*ThisStudyID".
v7.5.2 (02/12/2019) : modifications des affichages du DOM : les niveaux multiples de 1 sont passés en fond gris et le prix tradé est passé en fond rose.
v7.5.3 (03/12/2019) : mettre horaire d'expiration à H+55 min si le niveau tradé est un PP 1h.
v7.5.4 (05/12/2019) : modifs sur le bouton 9. Tracé des lignes en pointillé sur graphes externes : lignes plus épaisses pour robot avec ordre ouvert.
v7.5.5 (06/12/2019) : ajout d'un Input pour sc.SendOrdersToTradeService.
v7.5.6 (06/12/2019) : le robot est dorénavant basculé en breakeven sur la base du nouveau système de BasculeTP0.
v7.5.7 (11/12/2019) : correction de bug sur possible entrée en position après HoraireExpiration : SCDateHoraireExpiration est calculé uniquement lors du lancement du robot (lors du clic sur AutoTrade).
v7.6.0 (13/12/2019) : création de DistanceMiniRecenteAuNiveau_Future et DistanceMiniRecenteAuNiveau_Indice pour remplacer DistanceAuNiveau.
v7.6.1 (13/12/2019) : ne pas modifier les TP/SL auprès d'IB quand on est loin de la cible (DistanceMiniAuDanger > 5 ).
v7.6.2 (16/12/2019) : indication du n° de trade de la séquence (texte en rose) sur le graphe indice : Tr.0 (pas de séquence), Tr.1, Tr.2, Tr.3.
v7.7.0 (17/12/2019) : introduction de NewSigma = Sigma + 100. Introduction de NewRatioSigma = NewSigma/NewSeuilSigma + CsteNewSigma.
v7.7.1 (18/12/2019) : modifs et corrections mineures.
v7.7.2 (10/1/2020) : pour le calcul des mvts verticaux affichés sur le graphe, EnvergureMoyenne4000Ticks est récupéré via un array de Sigma (vs un input précédemment).
v7.7.3 (10/1/2020) : l'input Bouton_NewSigma de AMR est dorénavant piloté par l'input Bouton_NewSigma la study Sigma.
v8.0.0 (2020/01/23) : création d'un input pour dissocier autonome/serveur/client.
v8.0.1 (2020/01/29) : ajout de " | Actif " au nom du niveau affiché si le robot a des ordres dans le CO.
v8.0.2 (2020/01/29) : dans l'ordre triple, l'ordre stop est remplacé par un ordre stop-limit. Utilisation de Stop1Offset pour positionner l'offset entre stop et limit => ne fonctionne pas, car Stop1Offset définit l'offset entre l'ordre parent et le stop.
v8.0.3 (2020/02/01) : utilisation de StopLimitOrderLimitOffset au lieu de Stop1Offset => ne fonctionne pas, car ça semble s'appliquer à l'ordre parent (offset entre stop et limit lorsque l'ordre parent est un ordre stop-limit) => pour l'instant, je laisse tomber l'offset entre stop et limit lors de la soumission de l'ordre triple, car l'essentiel est de positionner un ordre stop-limit qui pourra être manipulé ultérieurement.
v8.0.4 (2020/02/01) : lorsque le SL indice est touché et que le stop-limit future est déplacé, on gère l'offset sur le stop-limit avec StopLimitOrderLimitOffset   => ne fonctionne pas.  
v8.0.5 (2020/02/01) : lorsque le SL indice est touché et que le stop-limit future est déplacé, on gère l'offset sur le stop-limit avec Price2 => fonctionne
v8.1.0 (2020/02/02) : on compte le nb d'occurences de studies AMR avant la présente study.
v8.1.1 (2020/02/02) : le nb d'occurences de AMR va déterminer sa position initiale sur le graphique.
v8.1.2 (2020/02/03) : ajout de External_Input_ACS_BUTTON_52 (AutoTrade).
v8.1.3 (2020/02/04) : ajout de timeBeginPeriod(1) pour avoir une résolution du timer Windows de 1ms (default timer resolution on Windows is 15.6 ms)	=> ne compile pas => nécessite d'avoir la main sur les librairies du compilateur => à retenter avec Visual Studio		// https://randomascii.wordpress.com/2013/07/08/windows-timer-resolution-megawatts-wasted/
v8.1.4 (2020/02/04) : utilisation de QueryPerformanceCounter pour mesurer des durées.
v8.2.0 (2020/02/05) : lorsque le StopLoss indice est dépassé, l'ordre de rapprochement du StopLoss future est rejeté par le serveur TT au motif "Order modification failed | SC Futures Order Routing/Data order update (Order cancel/replace reject). Info: Order modification failed | Order Modification Reject. Text: Sell order stop price must be below last trade price".
								=> Modification du bloc du robot "Dépassement de StopLoss indice" : le SL est remplacé par un FlattenPosition, suivi d'un CancelOrder.
v8.2.1 (2020/02/05) : résolution du bug de DistanceMiniAuDanger provenant de l'utilisation de sc.UpdateStartIndex (qui vaut zéro lors d'une recalculation) => ajout d'une condition sur le bloc de calcul de DistanceMiniAuDanger.
v8.2.2 (2020/02/21) : projection du texte du refresh AMR sur le chart #15 (contexte : trading sur une seule fenêtre sur VPS).
v8.3.0 (2020/02/24) : implémentation des nouvelles valeurs de "malus NRV". Ajout de l'input "New Malus NRV". Toggle avec touche "N" pour basculer entre New et Old.
v8.3.1 (2020/02/25) : implémentation des nouvelles valeurs de "NewSeuilSigma" et "CsteNewSigma". Ajout de l'input NewTuningRatioSigma_Input. Toggle avec touche "I" (comme S[i]gma) pour basculer entre New et Old.
v8.3.2 (2020/02/26) : remplacement de TPNu par TPNuSI dans le calcul de SeuilAUC. Ajout de l'input NewSeuilAUC_Input. Toggle avec touche "E" (comme S[e]uil) pour basculer entre New et Old.
v8.3.3 (2020/02/27) : modifications de Bouton "Cible -1t" et Bouton "Cible +1t", pour une exécution plus rapide lors d'appuis successifs sur ces boutons.
v8.3.4 (2020/03/04) : remplacement de TPNu par TPNu*MultiplicateurPrix dans le calcul de SeuilAUC (càd qu'on ne multiplie pas par SpreadIndex). Ajout de l'input NewNewSeuilAUC_Input. Toggle avec touche "U" (comme Se[u]il) pour basculer entre New et Old.
v8.3.5 (2020/03/05) : si DistanceAuPHPB_SurSL est inférieur à 100% au moment de la touche, on affichage "(SommeRatio+Malus)/Get_DistanceAuPHPB_SurSL²". 
v8.3.6 (2020/03/05) : la calcul "(SommeRatio*Malus)/Virginite^2" devient : "(RatioAUC/Virginite^2+RatioSigma)*Malus". 
v8.3.7 (2020/03/06) : CoeffMalusAjustement est corrigé par SpreadIndex.
v8.3.8 (2020/03/06) : CoeffMalusAjustement devient une variable permanente (vs input précédemment) et est affichée sur le graphique.
v8.3.9 (2020/03/08) : pour le calcul des malus d'ajustement, on introduit le modèle Parabolique : Contribution = ForcePivot *(1-(Ajustement/Lambda)²), avec Lambda = ForceNiveau/CoeffMalusAjustement/1.96.
v8.4.0 (2020/03/08) : le bonus utilisé pour le calcul de l'ajustement parabolique est le BonusIntrinseque, et non plus le BonusAvantAjustement.
v8.4.1 (2020/03/10) : suppression du code lié à des TP manuels.
v8.4.2 (2020/03/10) : création de l'input MalusNRV_Manuel_Input qui permet de rentrer manuellement un malus. Si l'input vaut 0, cela signifie que le malus est attribué de manière classique (ie automatiquement).
v8.4.3 (2020/03/11) : création de l'input NewContributionSpreadIndex, pour gérer ContributionSpreadIndex = SpreadIndex (old), et ContributionSpreadIndex = sqrt(2*SpreadIndex-1) (new).
v8.4.4 (2020/03/12) : déplacement du code de l'affichage des mvts verticaux vers le bloc d'affichage, afin que leur affichage s'ajuste automatiquement.
v8.4.5 (2020/03/17) : tracé d'une ligne horizontale bleue sur le 1-tick pour la study active.
v8.4.6 (2020/03/20) : la study AMR intègre directement Virginite et RatioDistanceAuPHPB dans le SommeRatio usuel. Suppression de la condition sur DistanceAuPHPB dans le robot (la condition est gérée dans SommeRatio).
v8.4.7 (2020/03/22) : affichage du VIX sur graphe de Sigma.
v8.4.8 (2020/03/22) : prévention d'un bug : EnvergureMoyenne4000Ticks ne doit jamais être nulle, sinon plantage de Sierra.
v8.4.9 (2020/03/25) : dans le calcul de DistanceMiniAuDanger, TPOptimum a été remplacé par TPCible  (l'utilisation de TPOptimum est aberrant car il peut être très supérieur à TPCible lorsque BonusExcedentaire est élevé). TPOptimumMajore est devenu TPCibleMajore.
v8.5.0 (2020/03/27) : ajout des triggers AutoTradeFACAO_MessageDansLog, ParentOrderID_EchecGetOrder_MessageDansLog, Target1OrderID_EchecGetOrder_MessageDansLog et Stop1OrderID_EchecGetOrder_MessageDansLog pour éviter de charger le log, en cas de bug.
v8.5.1 (2020/03/27) : gestion d'un bug qui se produit en simulation locale lorsque le stop loss indice est dépassé : l'ordre limite du brekeven (TP0.25) n'est pas annulé (car l'ID de l'ordre parent est perdu en sim locale).
v8.6.0 (2020/04/07) : remise à plat complète de AUC et SeuilAUC. Introduction de TP_AUC = TPNu * TailleBougie_4000Ticks / TailleBougie_4000Ticks_Reference * MultiplicateurPrix.  Avec TailleBougie_4000Ticks = f(SpreadIndex). Multiplicateur_TPNu s'appelle dorénavant MultiplicateurPrix.
v8.6.1 (2020/04/07) : modifs de l'affichage de AUC et SeuilAUC.
v8.6.2 (2020/04/07) : modifs mineures : remplacer l'input MultiplicateurPrix_Input par la variable MultiplicateurPrix, ajouter l'affichage de TPNuSI/TP_AUC sur la 2è colonne.
v8.6.3 (2020/04/12) : la touche G entraine l'écriture dans le le log des paramètres du trade en input. 
v8.6.4 (2020/04/13) : ModelePourAUC_Input prend les valeurs : Old model / New model 1 / New model 2.
v8.6.5 (2020/04/14) : simplification des modèles utilisés. On ne garde que l'input ModelePourAUC_Input (F12) et il prend les valeurs : Old model / New model 1 / New model 2.
v8.6.6 (2020/04/15) : ajout du modèle 3. SeuilAUC basé sur TPNu² seulement (SeuilAUC = 10.7*TPNu² ; sans TPNu^4 ; sans MultiplicateurPrix). Taille/Taille_Ref vaut SpreadIndex. BonusIntrinseque est divisé par SpreadIndex. La pente de BasculeTP0 est multipliée par SpreadIndex.
v8.6.7 (2020/04/16) : ajout de "CTRL + 5" : navigation vers le niveau indice correspondant à LastVisibleBar et modification de HoraireRAZ.
v8.6.8 (2020/04/16) : ajout du modèle 4. SeuilAUC basé sur TPNu² seulement (SeuilAUC = 10.7*TPNu² ; sans TPNu^4 ; sans MultiplicateurPrix). Taille/Taille_Ref vaut SpreadIndex. BonusIntrinseque est divisé par sqrt(SpreadIndex). La pente de BasculeTP0 est multipliée par sqrt(SpreadIndex).
v8.6.9 (2020/04/16) : normalisation des formules de calcul de SeuilAUC et de AUC pour le "Old Model" en le calquant sur les formules des modèles 1 et 2.
v8.7.0 (2020/04/17) : modèle 5 : basé sur Old Model. La ContributionSpreadIndex est prise égale à SpreadIndex pour TP_AUC. On remplace la formule en (TPNu*MultiplicateurPrix)^4 par TPNu^2. On garde le BonusIntrinseque intact. On garde le malus sur AUC avec la contribution pow(ContributionSpreadIndex,PuissanceSpreadIndex). Modèle 6 : idem modèle 5, sauf qu'on a supprimé le multiplicateur suivant dans le calcul d'AUC :   pow(ContributionSpreadIndex,PuissanceSpreadIndex).
v8.7.1 (2020/04/18) : mise à l'échelle de la taille du texte lorsque la résolution de l'écran baisse, avec la variable Multiplicateur_tailleTexte.
v8.7.2 (2020/04/20) : introduction de Config_tailleTexte : vaut 1 pour écran QHD, vaut 2 pour écran Full HD.
v8.7.3 (2020/04/21) : lifting du code : suppression des fonctions inusitées, inputs inusités, etc...
v8.7.4 (2020/04/22) : RAZ des boutons de la study active lors de LastCallToFunction.
v8.7.5 (2020/04/24) : ajout du modèle 7 (idem modèle 2, avec SeuilAUC = 20*TPNu^2), et du modèle 8 (idem modèle 5, avec SeuilAUC = 20*TPNu^2). Modifications des Positions_Xeme_Colonne pour écran Full HD.
v8.7.6 (2020/04/29) : tracé sur le 1-tick d'une ligne en pointillé rouge/vert pour matérialiser le niveau TP.
v8.7.7 (2020/05/04) : CTRL+Z ramène HoraireRAZ à 9h30m00s.
v8.7.8 (2020/05/10) : gestion des inputs pour le trading de PHJ/PBJ : NewDailyHighLow_InputManuel (bouton F10).
v8.7.9 (2020/05/13) : 3è colonne : afficher la dist à ISP500 et le ratio entre la dist et la dist requise (ratio distanciation).
v8.8.0 (2020/06/15) : le bouton Z "HoraireRaz=LastVisibleBar - 1min" devient "HoraireRaz=LastVisibleBar - NbBarresVirginite".
v8.8.1 (2020/06/22) : codage du modèle 9 pour AUC.
v8.8.2 (2020/06/25) : on remplace GetTimeWithoutMilliseconds par GetTimeInSecondsWithoutMilliseconds ; fix des autres bugs liés à SCDateTime.
v8.8.3 (2020/06/26) : ajout d'un bouton pour afficher le bonus (ACS_BUTTON_17).
v8.8.4 (2020/06/26) : correction de bug sur External_Input_ACS_BUTTON_133_134 qui n'était pas RAZ lorsque robot en position. 
v8.8.5 (2020/06/26) : correction de bug sur le nb de lots du CO non affiché lorsque le prix d'entrée a 0 lots.
v8.8.6 (2020/06/27) : lorsqu'on ajoute une nouvelle study AMR, son HoraireRAZ est 9h00.
v8.8.7 (2020/06/27) : ajout de External_Input_ACS_BUTTON_15 (bonus -) et External_Input_ACS_BUTTON_16 (bonus +).
v8.8.8 (2020/06/29) : accélération de SC : les calculs d'affichages pour le chart #6 ne sont exécutés que si : 1) la fenêtre du chart est visible ; et 2) le NiveauFutureAjuste est dans la fenêtre.
v8.8.9 (2020/06/30) : poursuite de v8.8.8.
v8.9.0 (2020/07/01) : des variables persistantes sont converties en variables persistantes Fast (toutes les variables float ont été converties). Modifs sur StudyID_NavigNiveaux. Débuggages divers liés à l'accélération de SC. Cette version v8.9.0 fonctionne nickel (utilisée pour trading du 2020/07/01).
v8.9.1 (2020/07/01) : poursuite de la conversion des variables persistantes en Fast.
v8.9.2 (2020/07/07) : le disque rose pour le nb de lots du CO est affiché par-dessus les disques gris.
v8.9.3 (2020/07/07) : correction bug sur ajustement de 1 pas.
v8.9.4 (2020/07/10) : lorsque le StopLoss indice a été dépassé (trade n°2 du 2020/07/09), le Flattenposition (qui envoie un ordre market) et le CancelOrder pour l'ordre stop-limit ont bien fonctionné, mais l'ordre lié TP0.25 n'a pas été annulé (j'ai dû exécuter un kill pour l'annuler).
									=> Modification du bloc du robot "Dépassement de StopLoss indice" : ajout d'un CancelOrder pour annuler le TP0.25.
v8.9.5 (2020/07/15) : popup si HoraireRAZ du robot est < 9h30.
v8.9.6 (2020/07/18) : lors d'un LastCallToFunction, si la study est active, on transmet NiveauActif=0 aux studies "Padding et boutons ACS" pour le recalcul du range affiché.
v8.9.7 (2020/07/21) : modifications pour être raccord avec Padding v2.5 + debug du marker non projeté sur charts 15/19.
v8.9.8 (2020/07/22) : fix de bug (StudyActive de PrixTypique est parfois en décalage avec AMR) => c'est la study PrixTypique (à la place de la study padding) qui envoie à AMR le trigger pour faire le check de l'état de la study active.
v8.9.9 (2020/08/03) : modification de l'affichage du bouton 17 (Bonus) si AMR n'est pas affiché.
v9.0.0 (2020/08/04) : la study AMR intègre directement la condition "SpreadIndex>1.3" dans le SommeRatio usuel.
v9.0.1 (2020/08/07) : l'appui sur CTRL+A refreshe le prix de l'ordre limite d'entrée en position pour la study active.
v9.0.2 (2020/08/10) : ajout d'une condition sur "CTRL+A" pour qu'il n'y ait pas d'envoi d'ordre de modification de prix si NiveauFutureOrdreEntree n'a pas varié. Idem pour PrixDuTP et PrixDuSL.
v9.0.3 (2020/08/11) : détection automatique de SendOrdersToTradeService (LocalSim / Ext).
v9.0.4 (2020/08/12) : création de Status Report (CTRL+S).
v9.0.5 (2020/08/13) : simplification des tests qui déterminent la valeur de OrdreOuvert. On utilise IsWorkingOrderStatus.
v9.0.6 (2020/08/13) : on modifie Calcul de PositionOuverte : ajout du cas où les child orders (TP et SL) ont été annulés.
v9.0.7 (2020/08/26) : màj du code suite à download nouvelle version de Sierra avec nouvelle arithmétique du temps.
v9.0.8 (2020/09/03) : correction de bug : la condition "SpreadIndex>1.3" n'est vraie que pour les niveaux psys.
v9.0.9 (2020/09/09) : création de Set_StudyQuantiemeMvt : fonction qui gère l'envoi des instructions vers "Quantieme_Mvt".
v9.1.0 (2020/09/10) : gestion de AffichageONOFF pour "Amplitude Mvt"  et "Quantieme Mvt" sur les charts 14 et 15.
v9.1.1 (2020/09/11) : fix du bug lors d'un appui sur Z sur charts 15 et 19 (désynchro entre charts ?) => transmission au 16-ticks de l'index du dernier chandelier visible. Idem pour CTRL+5.
v9.1.3 (2020/09/11) : fix du bug sur "CTRL+5".
v9.1.4 (2020/09/11) : suppression de la variable CorrectionBonusIntrinseque qui n'était utilisée que pour des anciens modèles pour AUC. Suppression des modes surrannés pour le calcul de BasculeTP0.
v9.1.5 (2020/09/14) : suppression des modes surranés pour ModelePourAUC_Input. On ne garde que les modèles 7 et 9.
v9.1.6 (2020/09/14) : codage du modèle 10 pour AUC.
v9.1.7 (2020/09/23) : introduction de l'input TPNu_Manuel_Input pour forcer un TPNu manuel.
v9.1.8 (2020/09/24) : on met de l'ordre dans l'input BonusIntrinseque_Roulette_Input.
v9.1.9 (2020/09/28) : introduction de SousNatureNiveau.
v9.2.0 (2020/10/01) : modification de AMR après que le DLL "Amplitude Mvt" ait été incorporé dans le DLL "Quantieme Mvt".
v9.2.1 (2020/10/05) : introduction de RangePrecedent_ApresClic
v9.2.2 (2020/10/08) : introduction de BonusIntrinseque_Exterieur_Input.
v9.2.3 (2020/10/12) : on met de l'ordre dans l'affichage de "Somme Ratio * Malus" qui devient "Somme Ratio & Malus". "MalusSommeRatio" est renommé en "MalusExcedentaire" (opposé de BonusExcedentaire et BonusExcedentaireSuffisant).
v9.2.4 (2020/10/12) : suite de 9.2.3.
v9.2.5 (2020/10/12) : fin de 9.2.3.
v9.2.6 (2020/10/12) : suppression de SommeRatio et SommeRatioProjete.
v9.2.7 (2020/10/16) : le tracé post-entrée en position est interrompu seulement si TP dépassé ou SL dépassé.
v9.2.8 (2020/10/23) : BonusIntrinseque_Exterieur_Input == -2  signifie qu'on utilise le bonus roulette.
v9.2.9 (2021/01/04) : BonusIntrinseque_Roulette_Input == 0 signifie qu'on utilise le bonus extérieur.
v9.3.0 (2021/01/07) : lorsque le bonus est < 10, on ne trace pas les courbe sur le graphe.
v9.3.1 (2021/01/08) : sur graphes #15/#19, tracer une ligne horizontale de 0 à LastIndex pour le TP et pour le SL.
v9.3.2 (2021/01/11) : pour les niveaux ronds, appui sur touche B retourne le bonus calculé pour chart 15 si mvt < 500 pdm, ou le bonus pour chart 19 si mvt > 500pdm.   ModelePourAUC_Input : ajout du modèle 11 (calculé sur le contrat DEC-20).









SOMMAIRE DES STRUCTURES ET FONCTIONS :
Font_Size
Texte_UTAMAOA
DrawText_UTAMAOA
Texte_RelativePosition_UTAMAOA
DrawText_RelativePosition_UTAMAOA
TexteAligne_UTAMAOA_UDD
DrawTextAligne_UTAMAOA_UDD
DrawTextAligne_UTAMAOA_UDD_SurGrapheExterne
TexteBasAligneGauche_UTAMAOA
TexteBasAligneDroite_UTAMAOA
TexteBasCentre_UTAMAOA_UDD
Marker_UTAMAOA
DrawMarker_UTAMAOA
Marker_UTAMAOA_UDD
DrawMarker_UTAMAOA_UDD
DrawMarker_UTAMAOA_UDD_SurGrapheExterne 
SegmentPointille
DrawSegmentPointille
Segment_UTAMAOA_UDD
DrawSegment_UTAMAOA_UDD
HorizontalRay_UTAMAOA_UDD
DrawHorizontalRay_UTAMAOA_UDD_SurGrapheExterne
Ray_UTAMAOA_UDD
DrawRay_UTAMAOA_UDD
EffacementDesUDD
EffacementDesUDD_GraphesExternes
FastHighest
FastLowest
Highest 
Lowest 
Highest_Array 
Lowest_Array 
ConversionVersSCDateHoraire
CreationDeHoraireRAZInputString
CreationDeDateEtudieeInputString
CalculDateLocale
SetButton52And56Text
DrawText_ID_NomDuNiveauTrade_Ajustement
GetActiveStudyID
QuantiemeOccurenceDeAMR
GetNombreRobotsAcheteurs
GetNombreRobotsVendeurs
BooleanHideNiveauxFuture
Set_HoraireExpiration_PP1h
Set_HoraireExpiration_PP4h
RAZHoraireExpiration
CreationSCDateHoraireExpiration
CreationSCDateHoraireNewsMoins5Min
CreationSCDateHoraireNewsMoins20Sec
Get_Nom_ApresClic
Get_PrixSurFuture_ApresClic
Get_PrixSurIndice_ApresClic
Get_NatureNiveau_ApresClic
Get_SousNatureNiveau_ApresClic
Get_RangePrecedent_ApresClic
AppendFutOuInd
Set_StudyQuantiemeMvt
Rectangle2
creationRectangle
Get_DistanceAuPHPB_SurSL 
AjustementParabolique
TailleBougie_4000Ticks
FileExists
ConversionFloat
ConversionDuInputBonusIntrinsequeRoulette
WindowsVersionNumber
scsf_AnnulationMiniRebond



ARCHITECTURE DE scsf_AnnulationMiniRebond :
Déclaration des Subgraph, Input, références, variables
Bloc sc.SetDefaults
Bloc d'initialisations ( sc.UpdateStartIndex == 0  or  TriggerRecalculate == 1)
Statut des Control Bar Buttons
Récupération des SCFloatArray externes
Bloc de calcul des indicateurs (Looping)
Bloc du robot de trading
Affichage temps réel des paramètres importants
Bloc last call to function
Report des changements de AutoTradeStatus dans le log
 */


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int Font_Size(int tailleTexte, int Config_tailleTexte)
{
	if (Config_tailleTexte ==  1) 		// écran QHD
	{ return  tailleTexte ;  }  
	else 		// écran Full HD
	{
		       if ( tailleTexte == 8 )  return 7 ;
		else if ( tailleTexte == 9 )  return 8 ;
		else if ( tailleTexte == 10 )  return 9 ;
		else if ( tailleTexte == 11 )  return 9 ;
		else if ( tailleTexte == 12 )  return 10 ;
		else if ( tailleTexte == 13 )  return 10 ;
		else return 11 ; 
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Texte_UTAMAOA : s_UseTool   
{											
	Texte_UTAMAOA (SCString texte, int index, float prix, int tailleTexte,  COLORREF couleur, int graphRegion, s_sc* ptrsc, int Line_Number, int Alignement) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
	{
		Clear(); 
		AddMethod = UTAM_ADD_OR_ADJUST ;			
		LineNumber = Line_Number ;
		DrawingType = DRAWING_TEXT;
		Text = texte ;		
		BeginIndex = index ;
		BeginValue = prix ;
		FontSize = Font_Size(tailleTexte, ptrsc->GetPersistentFloat(40)) ;
		Color = couleur ;
		Region = graphRegion ; 
		FontBold = 1 ;
		TextAlignment = Alignement ;  // options disponibles : DT_TOP, DT_VCENTER, DT_BOTTOM, DT_CENTER, DT_LEFT, DT_RIGHT
	} 
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawText_UTAMAOA (SCString texte, int index, float prix, int tailleTexte, COLORREF couleur, s_sc* ptrsc, int Line_Number)  	
{
	//Création d'instance de Texte et appel de sc.UseTool
	Texte_UTAMAOA objetTexte(texte, index, prix, tailleTexte, couleur, ptrsc->GraphRegion, ptrsc, Line_Number, DT_VCENTER | DT_CENTER);
	ptrsc->UseTool(objetTexte); 
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Texte_RelativePosition_UTAMAOA : s_UseTool   
{											
	Texte_RelativePosition_UTAMAOA (SCString texte, float RelativePositionAbcsisse, float prix, int tailleTexte,  COLORREF couleur, int graphRegion, s_sc* ptrsc, int Line_Number) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
	{
		Clear(); 
		AddMethod = UTAM_ADD_OR_ADJUST ;			
		LineNumber = Line_Number ;
		DrawingType = DRAWING_TEXT;
		Text = texte ;		
		BeginDateTime = RelativePositionAbcsisse ;
		BeginValue = prix ;
		FontSize = Font_Size(tailleTexte, ptrsc->GetPersistentFloat(40)) ;
		Color = couleur ;
		Region = graphRegion ; 
		FontBold = 1 ;
		TextAlignment = DT_VCENTER | DT_CENTER;  // options disponibles : DT_TOP, DT_VCENTER, DT_BOTTOM, DT_CENTER, DT_LEFT, DT_RIGHT
	} 
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawText_RelativePosition_UTAMAOA (SCString texte, float RelativePositionAbcsisse, float prix, int tailleTexte, COLORREF couleur, s_sc* ptrsc, int Line_Number)  	
{
	//Création d'instance de Texte et appel de sc.UseTool
	Texte_RelativePosition_UTAMAOA objetTexte(texte, RelativePositionAbcsisse, prix, tailleTexte, couleur, ptrsc->GraphRegion, ptrsc, Line_Number);
	ptrsc->UseTool(objetTexte); 
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct TexteAligne_UTAMAOA : s_UseTool    // crée un texte avec le SCString "texte" pour être affiché en coordonnées (indice,prix)
{											
	TexteAligne_UTAMAOA (SCString texte, int index, float prix, int tailleTexte,  COLORREF couleur, int chartNumber, int graphRegion, s_sc* ptrsc, int Line_Number) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
	{
		Clear(); 
		AddMethod = UTAM_ADD_OR_ADJUST ;		
		LineNumber = Line_Number ;
		DrawingType = DRAWING_TEXT;
		Text = texte ;		
		BeginIndex = index ;
		BeginValue = prix ;
		FontSize = Font_Size(tailleTexte, ptrsc->GetPersistentFloat(40)) ;
		Color = couleur ;
		Region = graphRegion ; 
		ChartNumber = chartNumber ;
		FontBold = 1 ;
		TextAlignment = DT_VCENTER | DT_RIGHT;  // options disponibles : DT_TOP, DT_VCENTER, DT_BOTTOM, DT_CENTER, DT_LEFT, DT_RIGHT
	} 
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawTextAligne_UTAMAOA (SCString texte, int index, float prix, int tailleTexte, COLORREF couleur, s_sc* ptrsc, int Line_Number)  	
{
	//Création d'instance de Texte et appel de sc.UseTool
	TexteAligne_UTAMAOA objetTexte(texte, index, prix, tailleTexte, couleur, ptrsc->ChartNumber, ptrsc->GraphRegion, ptrsc, Line_Number);
	ptrsc->UseTool(objetTexte); 
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct TexteAligne_UTAMAOA_UDD : s_UseTool    // crée un texte avec le SCString "texte" pour être affiché en coordonnées (indice,prix)
{											
	TexteAligne_UTAMAOA_UDD (SCString texte, int index, float prix, int tailleTexte,  COLORREF couleur, int chartNumber, int graphRegion, s_sc* ptrsc, int Line_Number) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
	{
		Clear(); 
		AddMethod = UTAM_ADD_OR_ADJUST ;		
		AddAsUserDrawnDrawing = 1 ;    // This member specifies that a drawing is to be added as a user drawn drawing, which allows the user to interact with the ACSIL added drawing on the chart as if it had been added by the user with one of the drawing tools on the Tools menu.
		LineNumber = Line_Number ;
		DrawingType = DRAWING_TEXT;
		Text = texte ;		
		BeginIndex = index ;
		BeginValue = prix ;
		FontSize = Font_Size(tailleTexte, ptrsc->GetPersistentFloat(40)) ;
		Color = couleur ;
		Region = graphRegion ; 
		ChartNumber = chartNumber ;
		FontBold = 1 ;
		TextAlignment = DT_VCENTER | DT_RIGHT;  // options disponibles : DT_TOP, DT_VCENTER, DT_BOTTOM, DT_CENTER, DT_LEFT, DT_RIGHT
	} 
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawTextAligne_UTAMAOA_UDD (SCString texte, int index, float prix, int tailleTexte, COLORREF couleur, s_sc* ptrsc, int Line_Number)  	
{
	//Création d'instance de Texte et appel de sc.UseTool
	TexteAligne_UTAMAOA_UDD objetTexte(texte, index, prix, tailleTexte, couleur, ptrsc->ChartNumber, ptrsc->GraphRegion, ptrsc, Line_Number);
	ptrsc->UseTool(objetTexte); 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawTextAligne_UTAMAOA_UDD_SurGrapheExterne (SCString texte, int index, float prix, int tailleTexte, COLORREF couleur, s_sc* ptrsc, int Line_Number, int chartNumber, int GraphRegion)
{
	//Création d'instance de Texte et appel de sc.UseTool
	TexteAligne_UTAMAOA_UDD objetTexte(texte, index, prix, tailleTexte, couleur, chartNumber, GraphRegion, ptrsc, Line_Number);
	ptrsc->UseTool(objetTexte); 
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct TexteBasAligneGauche_UTAMAOA : s_UseTool  
{											
	TexteBasAligneGauche_UTAMAOA (SCString texte, int tailleTexte,  COLORREF couleur, int graphRegion, s_sc* ptrsc, int Line_Number) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
	{
		Clear(); 
		AddMethod = UTAM_ADD_OR_ADJUST ;			
		LineNumber = Line_Number ;
		DrawingType = DRAWING_TEXT;
		Text = texte ;		
		BeginDateTime = 5 ;     // To use a value relative to the left side of the chart for BeginDateTime rather than an absolute Date Time value, specify an integer value from 1 to 150
		UseRelativeVerticalValues = true ;   // If UseRelativeVerticalValues is set to 1 or a nonzero value, then BeginValue is a value relative to the Chart Region itself. It is a percentage. Where 1 = 1%.
		BeginValue = 5 ;   
		FontSize = Font_Size(tailleTexte, ptrsc->GetPersistentFloat(40)) ;
		Color = couleur ;
		FontBold = 1 ;
		TextAlignment = DT_BOTTOM | DT_LEFT;  // options disponibles : DT_TOP, DT_VCENTER, DT_BOTTOM, DT_CENTER, DT_LEFT, DT_RIGHT
		Region = graphRegion ; 
	} 
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct TexteBasAligneDroite_UTAMAOA : s_UseTool  
{											
	TexteBasAligneDroite_UTAMAOA (SCString texte, int tailleTexte,  COLORREF couleur, int graphRegion, s_sc* ptrsc, int Line_Number) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
	{
		Clear(); 
		AddMethod = UTAM_ADD_OR_ADJUST ;			
		LineNumber = Line_Number ;
		DrawingType = DRAWING_TEXT;
		Text = texte ;		
		BeginDateTime = 144 ;     // To use a value relative to the left side of the chart for BeginDateTime rather than an absolute Date Time value, specify an integer value from 1 to 150
		UseRelativeVerticalValues = true ;   // If UseRelativeVerticalValues is set to 1 or a nonzero value, then BeginValue is a value relative to the Chart Region itself. It is a percentage. Where 1 = 1%.
		BeginValue = 5 ;   
		FontSize = Font_Size(tailleTexte, ptrsc->GetPersistentFloat(40)) ;
		Color = couleur ;
		FontBold = 1 ;
		TextAlignment = DT_BOTTOM | DT_RIGHT;  // options disponibles : DT_TOP, DT_VCENTER, DT_BOTTOM, DT_CENTER, DT_LEFT, DT_RIGHT
		Region = graphRegion ; 
	} 
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct TexteBasCentre_UTAMAOA_UDD : s_UseTool  
{											
	TexteBasCentre_UTAMAOA_UDD (SCString texte, int tailleTexte,  COLORREF couleur, int chartNumber, int graphRegion, s_sc* ptrsc, int Line_Number) : s_UseTool()  
	{
		Clear(); 
		AddMethod = UTAM_ADD_OR_ADJUST ;		
		AddAsUserDrawnDrawing = 1 ;    // This member specifies that a drawing is to be added as a user drawn drawing, which allows the user to interact with the ACSIL added drawing on the chart as if it had been added by the user with one of the drawing tools on the Tools menu.		
		LineNumber = Line_Number ;
		DrawingType = DRAWING_TEXT;
		Text = texte ;		
		BeginDateTime = 75 ;     // To use a value relative to the left side of the chart for BeginDateTime rather than an absolute Date Time value, specify an integer value from 1 to 150
		UseRelativeVerticalValues = true ;   // If UseRelativeVerticalValues is set to 1 or a nonzero value, then BeginValue is a value relative to the Chart Region itself. It is a percentage. Where 1 = 1%.
		BeginValue = 0.5 ;   
		FontSize = Font_Size(tailleTexte, ptrsc->GetPersistentFloat(40)) ;
		Color = couleur ;
		FontBold = 1 ;
		TextAlignment = DT_BOTTOM | DT_CENTER;  // options disponibles : DT_TOP, DT_VCENTER, DT_BOTTOM, DT_CENTER, DT_LEFT, DT_RIGHT
		ChartNumber = chartNumber ;
		Region = graphRegion ; 
	} 
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Marqueur = MARKER_POINT, MARKER_DASH, MARKER_SQUARE, MARKER_STAR, MARKER_PLUS, MARKER_X, MARKER_ARROWUP, MARKER_ARROWDOWN, MARKER_ARROWRIGHT, MARKER_ARROWLEFT, MARKER_TRIANGLEUP, MARKER_TRIANGLEDOWN, MARKER_TRIANGLERIGHT, MARKER_TRIANGLELEFT, MARKER_DIAMOND

struct Marker_UTAMAOA : s_UseTool    // crée un Marker pour être affiché en coordonnées (indice,prix)
{											
	Marker_UTAMAOA (int marqueur, int index, float prix, int tailleObjet, COLORREF couleur, int graphRegion, int Line_Number) : s_UseTool()    
	{
		Clear(); 
		AddMethod = UTAM_ADD_OR_ADJUST ;		
		LineNumber = Line_Number ;		
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

void DrawMarker_UTAMAOA (int marqueur, int index, float prix, int tailleObjet, COLORREF couleur, s_sc* ptrsc, int Line_Number)  
{
	//Création d'instance de Marker_UTAMAOA et appel de sc.UseTool
	Marker_UTAMAOA objetMarker(marqueur, index, prix, tailleObjet, couleur, ptrsc->GraphRegion, Line_Number);
	ptrsc->UseTool(objetMarker); 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Marker_RelativePosition_UTAMAOA : s_UseTool    // crée un Marker pour être affiché en coordonnées (indice,prix)
{											
	Marker_RelativePosition_UTAMAOA (int marqueur, float RelativePositionAbcsisse, float prix, int tailleObjet, COLORREF couleur, int graphRegion, int Line_Number) : s_UseTool()    
	{
		Clear(); 
		AddMethod = UTAM_ADD_OR_ADJUST ;		
		LineNumber = Line_Number ;		
		DrawingType = DRAWING_MARKER;
		MarkerType = marqueur ;
		BeginDateTime = RelativePositionAbcsisse ;
		BeginValue = prix ;
		MarkerSize = tailleObjet ;
		Color = couleur ;
		Region = graphRegion ; 
	} 
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawMarker_RelativePosition_UTAMAOA (int marqueur, float RelativePositionAbcsisse, float prix, int tailleObjet, COLORREF couleur, s_sc* ptrsc, int Line_Number)  
{
	//Création d'instance de Marker_RelativePosition_UTAMAOA et appel de sc.UseTool
	Marker_RelativePosition_UTAMAOA objetMarker(marqueur, RelativePositionAbcsisse, prix, tailleObjet, couleur, ptrsc->GraphRegion, Line_Number);
	ptrsc->UseTool(objetMarker); 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Marker_UTAMAOA_UDD : s_UseTool    // crée un Marker pour être affiché en coordonnées (indice,prix)
{											
	Marker_UTAMAOA_UDD (int marqueur, int index, float prix, int tailleObjet, COLORREF couleur, int chartNumber, int graphRegion, int Line_Number) : s_UseTool()    
	{
		Clear(); 
		AddMethod = UTAM_ADD_OR_ADJUST ;		
		AddAsUserDrawnDrawing = 1 ;    // This member specifies that a drawing is to be added as a user drawn drawing, which allows the user to interact with the ACSIL added drawing on the chart as if it had been added by the user with one of the drawing tools on the Tools menu.
		LineNumber = Line_Number ;		
		DrawingType = DRAWING_MARKER;
		MarkerType = marqueur ;
		BeginIndex = index ;
		BeginValue = prix ;
		MarkerSize = tailleObjet ;
		Color = couleur ;
		ChartNumber = chartNumber ;
		Region = graphRegion ; 
	} 
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawMarker_UTAMAOA_UDD (int marqueur, int index, float prix, int tailleObjet, COLORREF couleur, s_sc* ptrsc, int Line_Number)  
{
	//Création d'instance de Marker_UTAMAOA_UDD et appel de sc.UseTool
	Marker_UTAMAOA_UDD objetMarker(marqueur, index, prix, tailleObjet, couleur, ptrsc->ChartNumber, ptrsc->GraphRegion, Line_Number);
	ptrsc->UseTool(objetMarker); 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawMarker_UTAMAOA_UDD_SurGrapheExterne (int marqueur, int index, float prix, int tailleObjet, COLORREF couleur, s_sc* ptrsc, int Line_Number, int chartNumber, int graphRegion)      // AddAsUserDrawnDrawing (UDD) est obligatoire pour faire un tracé sur un graphe externe
{
	//Création d'instance de Marker_UTAMAOA_UDD et appel de sc.UseTool
	Marker_UTAMAOA_UDD objetMarker (marqueur, index, prix, tailleObjet, couleur, chartNumber, graphRegion, Line_Number) ;
	ptrsc->UseTool(objetMarker); 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct SegmentPointille : s_UseTool  
	{
		SegmentPointille(int index1, float prix1, int index2, float prix2, int epaisseur, COLORREF couleur, int graphRegion) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
		{
			Clear(); 
			AddMethod = UTAM_ADD_ALWAYS;	
			DrawingType = DRAWING_LINE ;
			LineStyle = LINESTYLE_DASH ;
			BeginIndex = index1 ;
			BeginValue = prix1 ;
			EndIndex = index2 ;
			EndValue = prix2 ;
			LineWidth = epaisseur;
			Color = couleur; 
			Region = graphRegion ;   // 0 par défaut
			DrawUnderneathMainGraph = true ;
		} 
	};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawSegmentPointille (int index1, float prix1, int index2, float prix2, int epaisseur, COLORREF couleur, s_sc* ptrsc)
{
	//Création d'instance de LigneHorizontale et appel de sc.UseTool
	SegmentPointille objetSegmentPointille(index1, prix1, index2, prix2, epaisseur, couleur, ptrsc->GraphRegion);
	ptrsc->UseTool(objetSegmentPointille); // c'est sc.UseTool qui est appelé
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Segment_UTAMAOA_UDD : s_UseTool  
{
	Segment_UTAMAOA_UDD (int index1, float prix1, int index2, float prix2, int epaisseur, COLORREF couleur, int chartNumber, int graphRegion, int Line_Number, int lineStyle) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
	{
	Clear() ; 
	AddMethod = UTAM_ADD_OR_ADJUST ;		
	AddAsUserDrawnDrawing = 1 ;    // This member specifies that a drawing is to be added as a user drawn drawing, which allows the user to interact with the ACSIL added drawing on the chart as if it had been added by the user with one of the drawing tools on the Tools menu.
	LineNumber = Line_Number ;		
	DrawingType = DRAWING_LINE ;
	LineStyle = (SubgraphLineStyles) lineStyle ;  
	BeginIndex = index1 ;
	BeginValue = prix1 ;
	EndIndex = index2 ;
	EndValue = prix2 ;
	LineWidth = epaisseur;
	Color = couleur; 
	ChartNumber = chartNumber ;
	Region = graphRegion ;   // 0 par défaut
	} 
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawSegment_UTAMAOA_UDD (int index1, float prix1, int index2, float prix2, int epaisseur, COLORREF couleur, s_sc* ptrsc, int Line_Number, int LineStyle)                           // LINESTYLE_DASH pour avoir un segment pointillé    // LINESTYLE_SOLID pour une ligne continue
{
	//Création d'instance de SegmentPointille_UTAMAOA_UDD et appel de sc.UseTool
	Segment_UTAMAOA_UDD objetSegment(index1, prix1, index2, prix2, epaisseur, couleur, ptrsc->ChartNumber, ptrsc->GraphRegion, Line_Number, LineStyle);
	ptrsc->UseTool(objetSegment);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct HorizontalRay_UTAMAOA_UDD : s_UseTool  
{
	HorizontalRay_UTAMAOA_UDD (int index1, float prix1, int epaisseur, COLORREF couleur, int chartNumber, int graphRegion, int Line_Number) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
	{
	Clear() ; 
	AddMethod = UTAM_ADD_OR_ADJUST ;		
	AddAsUserDrawnDrawing = true ;    // This member specifies that a drawing is to be added as a user drawn drawing, which allows the user to interact with the ACSIL added drawing on the chart as if it had been added by the user with one of the drawing tools on the Tools menu.
	LineNumber = Line_Number ;		
	DrawingType = DRAWING_HORIZONTAL_RAY ;
	LineStyle = LINESTYLE_DASH ;  // avant la version 8.4.8 : LineStyle = LINESTYLE_DASH		// de v8.4.8 à 9.3.1 : LineStyle = LINESTYLE_DOT (mais il y a des pb avec LineWidth > 1.		// après v9.3.1 : LINESTYLE_DASH
	BeginIndex = index1 ;
	BeginValue = prix1 ;
	LineWidth = epaisseur;
	Color = couleur; 
	ChartNumber = chartNumber ;
	Region = graphRegion ;   
	} 
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawHorizontalRay_UTAMAOA_UDD_SurGrapheExterne (int index1, float prix1, int epaisseur, COLORREF couleur, s_sc* ptrsc, int Line_Number, int chartNumber, int graphRegion)
{
	//Création d'instance de HorizontalRay_UTAMAOA_UDD et appel de sc.UseTool
	HorizontalRay_UTAMAOA_UDD objetSegmentPointille(index1, prix1, epaisseur, couleur, chartNumber, graphRegion, Line_Number); 
	ptrsc->UseTool(objetSegmentPointille); // c'est sc.UseTool qui est appelé
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Ray_UTAMAOA_UDD : s_UseTool  
{
	Ray_UTAMAOA_UDD(int index1, float prix1, int index2, float prix2, COLORREF couleur, int graphRegion, int Line_Number) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
	{
	Clear(); 
	AddMethod = UTAM_ADD_OR_ADJUST ;		
	AddAsUserDrawnDrawing = 1 ;    // This member specifies that a drawing is to be added as a user drawn drawing, which allows the user to interact with the ACSIL added drawing on the chart as if it had been added by the user with one of the drawing tools on the Tools menu.
	LineNumber = Line_Number ;		
	DrawingType = DRAWING_RAY;
	LineStyle = LINESTYLE_SOLID ;
	BeginIndex = index1 ;
	BeginValue = prix1 ;
	EndIndex = index2 ;
	EndValue = prix2 ;
	LineWidth = 2 ;
	Color = couleur ; 
	Region = graphRegion ;  
	} 
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawRay_UTAMAOA_UDD (int index1, float prix1, int index2, float prix2, COLORREF couleur, s_sc* ptrsc, int Line_Number)
{
	//Création d'instance de LigneHorizontale et appel de sc.UseTool
	Ray_UTAMAOA_UDD objetRay(index1, prix1, index2, prix2, couleur, ptrsc->GraphRegion, Line_Number);
	ptrsc->UseTool(objetRay); // c'est sc.UseTool qui est appelé
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void EffacementDesUDD(s_sc* ptrsc)
{
	int const ThisStudyID = ptrsc->StudyGraphInstanceID ;
	
	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10008+10000*ThisStudyID) ;  	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10009+10000*ThisStudyID) ;       	// effacement de la croix et du texte signalant que le malus d'ajustement dépasse le bonus excédentaire
	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10010+10000*ThisStudyID) ;  	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10011+10000*ThisStudyID) ;       	// effacement de la croix et du texte signalant que SommeRatioAvecMalus dépasse 100%
	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10012+10000*ThisStudyID) ;  	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10013+10000*ThisStudyID) ;       	// effacement de la croix et du texte signalant que TP est insuffisant
	//ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10014+10000*ThisStudyID) ;  	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10015+10000*ThisStudyID) ;       	// non-effacement des segments pointillés sur le niveau tradé et le prix demandé : on choisit de les laisser toujours apparents lorsque la study est actualisée
	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10020+10000*ThisStudyID) ;  	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10021+10000*ThisStudyID) ;       	// effacement de la croix et du texte signalant 15h45
	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10022+10000*ThisStudyID) ;  	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10023+10000*ThisStudyID) ;       	// effacement de la croix et du texte signalant 15h49m40s
	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10025+10000*ThisStudyID) ;  	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10026+10000*ThisStudyID) ;       	// effacement de la croix et du texte signalant HoraireExpiration
	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10027+10000*ThisStudyID) ;  	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10028+10000*ThisStudyID) ;       	// effacement de la croix et du texte signalant modification SL sur trading indice (lorsque SL dépassé)
	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10031+10000*ThisStudyID) ;  	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10032+10000*ThisStudyID) ;       	// effacement de la croix et du texte signalant Bascule TP0/TP0.25
	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10044+10000*ThisStudyID) ;  	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10045+10000*ThisStudyID) ;       	// effacement de la croix et du texte signalant que SL dépasse PH/PB
	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10048+10000*ThisStudyID) ;  	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10049+10000*ThisStudyID) ;       	// effacement de la croix et du texte signalant que BonusMoy est insuffisant
	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10057+10000*ThisStudyID) ;  	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10058+10000*ThisStudyID) ;       	// effacement de la croix et du texte signalant que DistMiniFuture <= 0
	// ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10060+10000*ThisStudyID) ; 	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10061+10000*ThisStudyID) ;  			// effacement des marqueurs TPOptimumArrondi_Prior et TPBooste
	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10062+10000*ThisStudyID) ; 	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10063+10000*ThisStudyID) ;	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10064+10000*ThisStudyID) ; 	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10065+10000*ThisStudyID) ;	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10066+10000*ThisStudyID) ; 	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10067+10000*ThisStudyID) ;	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10068+10000*ThisStudyID) ; 	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10069+10000*ThisStudyID) ;	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10070+10000*ThisStudyID) ; 	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10071+10000*ThisStudyID) ;	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10072+10000*ThisStudyID) ;		// effacement des textes des mvts verticaux
	// ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10073+10000*ThisStudyID) ; 	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10074+10000*ThisStudyID) ;	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10075+10000*ThisStudyID) ; 	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10076+10000*ThisStudyID) ;	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10077+10000*ThisStudyID) ; 	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10078+10000*ThisStudyID) ;	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10079+10000*ThisStudyID) ; 	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10080+10000*ThisStudyID) ;	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10081+10000*ThisStudyID) ; 	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10082+10000*ThisStudyID) ;			// effacement des marqueurs des mvts verticaux
	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10083+10000*ThisStudyID) ; 	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10084+10000*ThisStudyID) ;	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10085+10000*ThisStudyID) ; 	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10086+10000*ThisStudyID) ;	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10087+10000*ThisStudyID) ; 	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10088+10000*ThisStudyID) ;	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10089+10000*ThisStudyID) ; 	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10090+10000*ThisStudyID) ;	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10091+10000*ThisStudyID) ; 	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10092+10000*ThisStudyID) ;			// effacement des Ray pour la BasculeTP0
	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10114+10000*ThisStudyID) ; 																																	 		// effacement du marqueur BasculeTP0
	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10116+10000*ThisStudyID) ; 	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10117+10000*ThisStudyID) ;	ptrsc->DeleteUserDrawnACSDrawing(ptrsc->ChartNumber, 10118+10000*ThisStudyID) ;	// effacement des 3 segments traçant les bougies autour de IndexPremiereTouche
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void EffacementDesUDD_GraphesExternes(s_sc* ptrsc)
{
	int const ThisStudyID = ptrsc->StudyGraphInstanceID ;
	
	ptrsc->DeleteUserDrawnACSDrawing(11, 10042+10000*ThisStudyID) ;    // 10042 = triangle indiquant le niveau tradé
	ptrsc->DeleteUserDrawnACSDrawing(12, 10042+10000*ThisStudyID) ;
	ptrsc->DeleteUserDrawnACSDrawing(15, 10042+10000*ThisStudyID) ;
	ptrsc->DeleteUserDrawnACSDrawing(19, 10042+10000*ThisStudyID) ;
	ptrsc->DeleteUserDrawnACSDrawing(15, 10054+10000*ThisStudyID) ;    // 10054 = diamant indiquant la première touche du prix future demandé
	ptrsc->DeleteUserDrawnACSDrawing(11, 10055+10000*ThisStudyID) ;    // 10055 = disque indiquant la study actice
	ptrsc->DeleteUserDrawnACSDrawing(12, 10055+10000*ThisStudyID) ;
	ptrsc->DeleteUserDrawnACSDrawing(15, 10055+10000*ThisStudyID) ;    
	ptrsc->DeleteUserDrawnACSDrawing(19, 10055+10000*ThisStudyID) ;
	ptrsc->DeleteUserDrawnACSDrawing(15, 10121) ;    // Affichage des temps de calcul de la study active sur le chart #15
	ptrsc->DeleteUserDrawnACSDrawing(13, 10127+10000*ThisStudyID) ;	// ligne horizontale pointillé sur le chart 1-tick (prix d'entrée en position)
	ptrsc->DeleteUserDrawnACSDrawing(13, 10132+10000*ThisStudyID) ;	// ligne horizontale pointillé sur le chart 1-tick (prix du TP)
	ptrsc->DeleteUserDrawnACSDrawing(15, 10138+10000*ThisStudyID) ;	// ligne horizontale pointillé sur le chart UT20s (prix du TP)
	ptrsc->DeleteUserDrawnACSDrawing(19, 10139+10000*ThisStudyID) ;	// ligne horizontale pointillé sur le chart UT4min (prix du TP)
	ptrsc->DeleteUserDrawnACSDrawing(15, 10140+10000*ThisStudyID) ;	// ligne horizontale pointillé sur le chart UT20s (prix du SL)
	ptrsc->DeleteUserDrawnACSDrawing(19, 10141+10000*ThisStudyID) ;	// ligne horizontale pointillé sur le chart UT4min (prix du SL)
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float FastHighest (int IndexDebut, int NbChandeliersEnArriere, float iteration, s_sc* ptrsc)  
{
	if ( IndexDebut - NbChandeliersEnArriere < 0 ) return 99999999 ;	
	float iter = std::max(1.0f, iteration) ;     // la fonction max (définie dans "sierrachart.h") rentre en conflit avec la library <chrono>   =>  il faut préciser le namespace (std) et préciser que 1 est un float
	float highest = -FLT_MAX  ;
	for (float Index = IndexDebut ; Index > IndexDebut - NbChandeliersEnArriere ; Index -= iter)    // Highest et Lowest sont construits de la même manière que les fonctions du même nom de PRT
	{
		if ( ptrsc->BaseData[SC_HIGH][Index] > highest)
			highest = ptrsc->BaseData[SC_HIGH][Index] ;
	}
	return highest ;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float FastLowest (int IndexDebut, int NbChandeliersEnArriere, float iteration, s_sc* ptrsc)
{
	if ( IndexDebut - NbChandeliersEnArriere < 0 ) return -99999999 ;
	float iter = std::max(1.0f,iteration) ;
	float lowest = FLT_MAX  ;
	for (float Index = IndexDebut ; Index > IndexDebut - NbChandeliersEnArriere ; Index -= iter)
	{
		if ( ptrsc->BaseData[SC_LOW][Index] < lowest )
			lowest = ptrsc->BaseData[SC_LOW][Index] ;
	}
	return lowest ;
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

float Highest_Array (SCFloatArray& Array, int IndexDebut, int NbChandeliersEnArriere)
{
	// mettre erreur dans Log si NbChandeliers <1 // note : en l'état si NbChandeliers <1, Highest retourne -FLT_MAX
	float highest = -FLT_MAX  ;
	for (int Index = IndexDebut ; Index > IndexDebut - NbChandeliersEnArriere ; Index--)    // Highest et Lowest sont construits de la même manière que les fonctions du même nom de PRT
	{
		if ( Array[Index] > highest)  highest = Array[Index] ;
	}
	return highest ;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float Lowest_Array (SCFloatArray& Array, int IndexDebut, int NbChandeliersEnArriere)
{
	// mettre erreur dans Log si NbChandeliers <1 // note : en l'état si NbChandeliers <1, Highest retourne FLT_MAX
	float lowest = FLT_MAX  ;
	for (int Index = IndexDebut ; Index > IndexDebut - NbChandeliersEnArriere ; Index--)
	{
		if ( Array[Index] < lowest)  lowest = Array[Index] ;
	}
	return lowest ;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ConversionVersSCDateHoraire (int DateRAZ, double HoraireRAZ, SCDateTime& SCDateHoraireRAZ)
{
	int hh = floor ( HoraireRAZ /100 ) ;   // heure de HoraireRAZ
	int mm = floor ( HoraireRAZ -100*hh ) ;   // minute de HoraireRAZ
	int ss = floor ( 100*HoraireRAZ -10000*hh - 100*mm ) ;   // seconde de HoraireRAZ
	int TimeValue = HMS_TIME(hh, mm, ss) ;
	SCDateHoraireRAZ.SetDateTime(DateRAZ, TimeValue) ;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CreationDeHoraireRAZInputString (SCDateTime& SCDateHoraireRAZ, int TailleDuCustomInputDeHoraireRAZ, SCString& HoraireRAZInputString)		// Création de CustomInputStrings pour HoraireRAZRoulette
{
	int hh, mm, ss ;
	SCDateTime SCDateHoraire ;   // déclaration de la variable qui servira à exprimer chaque horaire calculé pour le string
	SCDateHoraire = SCDateHoraireRAZ ;
	SCDateHoraire += (float)TailleDuCustomInputDeHoraireRAZ / 1440 ;   // la première valeur de la chaîne qui sera construite par concaténation (construite du niveau le plus élevé au plus bas) : on ajoute TailleDuCustomInputDeHoraireRAZ * minutes  // 24*60=1440 miutes par jour
	SCDateHoraire.GetTimeHMS(hh,mm,ss) ;
	HoraireRAZInputString.Format("%dh%dm%ds", hh, mm, ss) ;  // premier chaînon du string final
	for (int index2 = TailleDuCustomInputDeHoraireRAZ - 1 ; index2 > -TailleDuCustomInputDeHoraireRAZ - 1 ; index2-- )  // concaténation des inputs
		{
			SCDateHoraire = SCDateHoraireRAZ ;
			SCDateHoraire += (float)index2 / 1440 ;   
			SCDateHoraire.GetTimeHMS(hh,mm,ss) ;
			SCString StringAjoute ;
			// StringAjoute.Format(";%dh%dm%ds", hh, mm, ss) ;
			//HoraireRAZInputString.Append(StringAjoute) ;
			if ( hh < 10 )  { StringAjoute.Format(";0%dh",hh) ;  } else { StringAjoute.Format(";%dh",hh) ;  } 
			HoraireRAZInputString.Append(StringAjoute) ;
			if ( mm < 10 )  { StringAjoute.Format("0%dm",mm) ;  } else { StringAjoute.Format("%dm",mm) ;  } 
			HoraireRAZInputString.Append(StringAjoute) ;
			if ( ss < 10 )  { StringAjoute.Format("0%ds",ss) ;  } else { StringAjoute.Format("%ds",ss) ;  } 
			HoraireRAZInputString.Append(StringAjoute) ;
		}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CreationDeDateEtudieeInputString (int TailleDuCustomInput, SCString& DateEtudieeInputString)		// Création de CustomInputStrings pour DateEtudieeRoulette
{
	int JourDeLaSemaine, JourDuMois, Mois, Annee ;
	
	// Date du PC (local time)
	time_t now = time(0) ;
	tm *ltm = localtime(&now) ;    // cf https://www.tutorialspoint.com/cplusplus/cpp_date_time.htm
	Annee = 1900 + ltm->tm_year ;
	Mois = 1 + ltm->tm_mon ;
	JourDuMois = ltm->tm_mday ;
	SCDateTime SCDateDuPC ;
	SCDateDuPC.SetDateYMD(Annee, Mois, JourDuMois) ; 
	
	// Construction de DateEtudieeInputString : on concatène "SCDateDuPC" + "SCDateDuPC-1" + "SCDateDuPC-2" + ...
	SCDateTime SCDate ;   // déclaration de la variable qui servira à exprimer chaque date calculée pour le string
	
	// Premier chaînon du string final
	SCDate = SCDateDuPC ;
	JourDeLaSemaine = SCDate.GetDayOfWeek() ;     // GetDayOfWeek() returns the day of the week for the SCDateTime variable. The return value will be one of the following: SUNDAY, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY.
	if ( JourDeLaSemaine == 0)  { DateEtudieeInputString.Format("Di ") ;  }    if ( JourDeLaSemaine == 1)  { DateEtudieeInputString.Format("Lu ") ;  }    if ( JourDeLaSemaine == 2)  { DateEtudieeInputString.Format("Ma ") ;  }    if ( JourDeLaSemaine == 3)  { DateEtudieeInputString.Format("Me ") ;  }    if ( JourDeLaSemaine == 4)  { DateEtudieeInputString.Format("Je ") ;  }    if ( JourDeLaSemaine == 5)  { DateEtudieeInputString.Format("Ve ") ;  }    if ( JourDeLaSemaine == 6)  { DateEtudieeInputString.Format("Sa ") ;  }    
	SCDate.GetDateYMD(Annee,Mois,JourDuMois) ;   // valid ranges : Month ( 1 through 12), Day (1 through [days in month])
	SCString StringAjoute ;   // déclaration du chaînon qui sera ajouté à DateEtudieeInputString
	StringAjoute.Format("%d/", JourDuMois) ;
	DateEtudieeInputString.Append(StringAjoute) ;  // ajout du jour du mois
	StringAjoute.Format("%d", Mois) ;  
	DateEtudieeInputString.Append(StringAjoute) ;  // ajout du mois
		
	// Itération de la concaténation
	for (int index2 = 1 ; index2 < TailleDuCustomInput ; index2 ++ ) 
	{
		SCDate = SCDateDuPC ;   // la première valeur de la chaîne qui sera construite par concaténation (construite du niveau le plus élevé au plus bas)   // le niveau le plus élevé est la date du PC (local time)
		SCDate -= index2 ;   // à chaque itération de la boucle for on retranche un jour
		
		JourDeLaSemaine = SCDate.GetDayOfWeek() ;   
		if ( JourDeLaSemaine == 0)  { StringAjoute.Format(";Di ") ;  }    if ( JourDeLaSemaine == 1)  { StringAjoute.Format(";Lu ") ;  }    if ( JourDeLaSemaine == 2)  { StringAjoute.Format(";Ma ") ;  }    if ( JourDeLaSemaine == 3)  { StringAjoute.Format(";Me ") ;  }    if ( JourDeLaSemaine == 4)  { StringAjoute.Format(";Je ") ;  }    if ( JourDeLaSemaine == 5)  { StringAjoute.Format(";Ve ") ;  }    if ( JourDeLaSemaine == 6)  { StringAjoute.Format(";Sa ") ;  }    
		DateEtudieeInputString.Append(StringAjoute) ;  // ajout du jour de la semaine
		SCDate.GetDateYMD(Annee,Mois,JourDuMois) ;  
		StringAjoute.Format("%d/", JourDuMois) ;  
		DateEtudieeInputString.Append(StringAjoute) ;  // ajout du jour du mois
		StringAjoute.Format("%d", Mois) ;  
		DateEtudieeInputString.Append(StringAjoute) ;  // ajout du mois
	}
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int CalculDateLocale()    // résultat au format Date values de Sierra : integer (int) values representing the number of days since December 30, 1899.
{
	int JourDuMois, Mois, Annee ;
	// Date du PC (local time)
	time_t now = time(0) ;
	tm *ltm = localtime(&now) ;    // cf https://www.tutorialspoint.com/cplusplus/cpp_date_time.htm
	Annee = 1900 + ltm->tm_year ;
	Mois = 1 + ltm->tm_mon ;
	JourDuMois = ltm->tm_mday ;
	// Conversion en objet SCDateTime
	SCDateTime SCDateDuPC ;
	SCDateDuPC.SetDateYMD(Annee, Mois, JourDuMois) ; 
	// Conversion en Date Value
	return SCDateDuPC.GetDate() ;     // GetDate() returns the date part of the SCDateTime variable. The value returned is a Date Value.
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SetButton52And56Text (s_sc* ptrsc)  	// Set the ACS Control Bar buttons 52 and 56 texts
{
	if ( ptrsc->Input[25].GetYesNo() )              // Input[25]  correspond à l'input CetteInstanceControleLesBoutons
	{
		// BOUTON 52 /////////////////////////////////
		
		int AutoTradeStatus = ptrsc->GetPersistentIntFast(28) ;
		SCString Texte ;
		
		if        ( AutoTradeStatus ==       0 )  { Texte.Format("AT : OFF (ready)") ;  }
		else if ( AutoTradeStatus ==     10 )  { Texte.Format("AT : demarre") ;  }
		else if ( AutoTradeStatus ==     11 )  { Texte.Format("AT : demarre") ;  }
		else if ( AutoTradeStatus ==     12 )  { Texte.Format("AT : pb Auto Trading Enabled est OFF") ;  }
		else if ( AutoTradeStatus ==     15 )  { Texte.Format("AT : pb soumission premier ordre") ;  }
		else if ( AutoTradeStatus ==     20 )  { Texte.Format("AT : ordre triple ouvert") ;  }  
		else if ( AutoTradeStatus ==     30 )  { Texte.Format("AT : position ouverte") ;  }
		else if ( AutoTradeStatus ==     50 )  { Texte.Format("AT : OFF (ord et pos fermes)") ;  }
		else if ( AutoTradeStatus ==     60 )  { Texte.Format("AT : annulation ordres demandee") ;  }
		else if ( AutoTradeStatus ==     70 )  { Texte.Format("AT : pb annulation ordres") ;  }
		else if ( AutoTradeStatus ==   100 )  { Texte.Format("AT : OFF car SommeRatio&Co saturent") ;  }
		else if ( AutoTradeStatus ==   110 )  { Texte.Format("AT : OFF car prix depasse") ;  }
		else if ( AutoTradeStatus ==   120 )  { Texte.Format("AT : OFF car TP insuffisant") ;  }
		else if ( AutoTradeStatus ==   130 )  { Texte.Format("AT : OFF car BonusExcedentaire insuffisant") ;  }
		else if ( AutoTradeStatus ==   140 )  { Texte.Format("AT : OFF car HoraireExpiration est depasse") ;  }
		else if ( AutoTradeStatus ==   150 )  { Texte.Format("AT : OFF car il est 15h45") ;  }
		else if ( AutoTradeStatus ==   160 )  { Texte.Format("AT : OFF car SL depasse PH/PB") ;  }
		else if ( AutoTradeStatus ==   170 )  { Texte.Format("AT : OFF car BonusMoy insuffisant") ;  }
		else if ( AutoTradeStatus ==   180 )  { Texte.Format("AT : OFF car Sequence terminee") ;  }
		else if ( AutoTradeStatus ==   190 )  { Texte.Format("AT : OFF car DistMiniFuture <= 0") ;  }
		else if ( AutoTradeStatus ==   200 )  { Texte.Format("AT : OFF car 5min avant News") ;  }
		else if ( AutoTradeStatus == 1000 )  { Texte.Format("AT : FlattenAndCancelAllOrders") ;  }
		else if ( AutoTradeStatus == 1010 )  { Texte.Format("AT : pb avec FlattenAndCancelAllOrders") ;  }
		else 												 { Texte.Format("AT : AutoTradeStatus non renseigne") ;  }
		
		ptrsc->SetCustomStudyControlBarButtonText(52, Texte);
		
		
		// BOUTON 56 //////////////////////////////
		
		int SequenceONOFF_Input = ptrsc->Input[35].GetIndex() ;    // 0 = ON, 1 = OFF
		int SequenceStatus = ptrsc->GetPersistentInt(56) ;
		
		if ( SequenceONOFF_Input == 0 )
		{
			if ( SequenceStatus == 0 )
			{ Texte.Format(" Seq : ON ") ;  }
			else if ( SequenceStatus == 1 )
			{ Texte.Format("Seq : Tr 1") ;  }
			else if ( SequenceStatus == 2 )
			{ Texte.Format("Seq : Tr 2") ;  }
			else if ( SequenceStatus == 3 )
			{ Texte.Format("Seq : Tr 3") ;  }
		}
		else
		{ Texte.Format("Seq : OFF") ;  }
		
		ptrsc->SetCustomStudyControlBarButtonText(56, Texte);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawText_ID_NomDuNiveauTrade_Ajustement (float RelativePosition, float prix, s_sc* ptrsc, int Line_Number) 
{
	int AjustementChoisiEnPas = ptrsc->GetPersistentIntFast(5) ; 
	SCString Texte ;
	
	Texte.Format("ID:%d | ", ptrsc->StudyGraphInstanceID) ;
	Texte.AppendFormat(ptrsc->Input[26].GetString()) ;      // input[26] correspond à l'input NomDuNiveauTrade
	
	if 			(AjustementChoisiEnPas > 0)	{ Texte.AppendFormat(" | +%dp", AjustementChoisiEnPas) ;  }
	else if 	(AjustementChoisiEnPas < 0)	{ Texte.AppendFormat(" | -%dp", -AjustementChoisiEnPas) ;   }   
	
	int AutoTradeStatus = ptrsc->GetPersistentIntFast(28) ;
	int RobotActif = ptrsc->GetPersistentIntFast(37);
	if ( AutoTradeStatus == 11 ) 	{ Texte.AppendFormat(" | ATD") ;  }    			// AutoTrade : demarré (robot en attente que le précédent robot libère la place)
	else if ( RobotActif ) 				{ Texte.AppendFormat(" | AT actif") ;  }    		// Le robot est actif et n'est pas en attente 

	if ( ptrsc->Input[25].GetYesNo() )  	{ DrawText_RelativePosition_UTAMAOA (Texte, RelativePosition, prix, 12, RGB(0,127,255), ptrsc, Line_Number) ; }     // Input[25]  correspond à l'input CetteInstanceControleLesBoutons
	else                                            	{ DrawText_RelativePosition_UTAMAOA (Texte, RelativePosition, prix, 12, RGB(0,0,0), ptrsc, Line_Number) ;  }	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int GetActiveStudyID (s_sc* ptrsc)
{
	int ActiveStudyID = 0 ;
	int& StudyID_PrixTypique = ptrsc->GetPersistentIntFast(53);  
	ptrsc->GetChartStudyInputInt(ptrsc->ChartNumber, StudyID_PrixTypique,1, ActiveStudyID) ;     // int GetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int& r_IntegerValue);
	return ActiveStudyID ;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int QuantiemeOccurenceDeAMR (int ThisStudyID, s_sc* ptrsc)   // Cette fonction recense le nb d'occurence des studies "ANNUL MINI REBOND" avant la CurrentStudy (y compris celle-ci)  => si la présente study est la première AMR rencontrée, alors OccurenceDeStudyAMR = 1
{
	int OccurenceDeStudyAMR = 0 ;
	SCString StudyNameAMR = "ANNUL MINI REBOND" ;
	for ( int StudyID=1 ; StudyID != ThisStudyID ; StudyID ++ )
	{	
		if ( ptrsc->GetStudyNameUsingID(StudyID) == StudyNameAMR )
		{ OccurenceDeStudyAMR++ ;  }
	}
	
	return ( OccurenceDeStudyAMR + 1 ) ;   // ajout de +1 car lorsque " StudyID == ThisStudyID " OccurenceDeStudyAMR n'est pas incrémenté
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int GetNombreRobotsAcheteurs(s_sc* ptrsc) 
{
	int NbRobotsAcheteurs = 0 ;
	int& StudyID_PrixTypique = ptrsc->GetPersistentIntFast(53);  
	ptrsc->GetChartStudyInputInt(ptrsc->ChartNumber, StudyID_PrixTypique,7, NbRobotsAcheteurs) ;     // int GetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int& r_IntegerValue);
	return NbRobotsAcheteurs ;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int GetNombreRobotsVendeurs(s_sc* ptrsc)  
{
	int NbRobotsVendeurs = 0 ;
	int& StudyID_PrixTypique = ptrsc->GetPersistentIntFast(53);  
	ptrsc->GetChartStudyInputInt(ptrsc->ChartNumber, StudyID_PrixTypique,9, NbRobotsVendeurs) ;     // int GetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int& r_IntegerValue);
	return NbRobotsVendeurs ;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int BooleanHideNiveauxFuture(s_sc* ptrsc)
{
	int ShowStudy = 1 ;
	int& StudyID_PrixTypique = ptrsc->GetPersistentIntFast(53);  
	ptrsc->GetChartStudyInputInt(ptrsc->ChartNumber, StudyID_PrixTypique, 3, ShowStudy) ;     // int GetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int& r_IntegerValue);
	return (1 - ShowStudy) ;    // HideStudy = 1 - ShowStudy
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Set_HoraireExpiration_PP1h(s_sc* ptrsc)
{
	SCDateTime LatestTime = ptrsc->LatestDateTimeForLastBar ;
	float HoraireExpiration = LatestTime.GetHour()*100 + 55 ;  // Horaire au format hhmm.ss    // calcule l'heure entière précédente et rajoute 55 minutes
	(ptrsc->Input[24]).SetFloat(HoraireExpiration) ;	    // SCInputRef 	HoraireExpiration = sc.Input[24]
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Set_HoraireExpiration_PP4h(s_sc* ptrsc)
{
	float HoraireRAZTexte = ptrsc->Input[3].GetFloat() ;   // Horaire au format hhmm.ss
	float HoraireExpiration ; // Horaire au format hhmm.ss
	if 			( HoraireRAZTexte <  100 )		{ HoraireExpiration =     45 ; }  // 00h45min
	else	if 	( HoraireRAZTexte <  500 )		{ HoraireExpiration =   445 ; }  // 4h45min
	else	if 	( HoraireRAZTexte <  900 )		{ HoraireExpiration =   845 ; }  // 8h45min
	else	if 	( HoraireRAZTexte < 1300 )	{ HoraireExpiration = 1245 ; }  // 12h45min
	else	if 	( HoraireRAZTexte < 1700 )	{ HoraireExpiration = 1645 ; }  // 16h45min
	else	if 	( HoraireRAZTexte < 2100 )	{ HoraireExpiration = 2045 ; }  // 20h45min
	else													{ HoraireExpiration =     45 ; }  // 00h45min
	(ptrsc->Input[24]).SetFloat(HoraireExpiration) ;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void RAZHoraireExpiration(s_sc* ptrsc)             
{
	(ptrsc->Input[24]).SetFloat(0) ;	    // Horaire au format hhmm.ss
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CreationSCDateHoraireExpiration(s_sc* ptrsc)
{
	int DateDuJour = ptrsc->LatestDateTimeForLastBar.GetDate() ;  // GetDate() returns the date part of the SCDateTime variable. The value returned is a Date Value. // Date values are integer (int) values representing the number of days since December 30, 1899.
	if ( ptrsc->Input[24].GetFloat() != 0 )                      // SCInputRef 	HoraireExpiration = sc.Input[24]
	{ 
		SCDateTime& SCDateHoraireExpiration = ptrsc->GetPersistentSCDateTimeFast(2) ;  
		ConversionVersSCDateHoraire(DateDuJour, ptrsc->Input[24].GetFloat(), SCDateHoraireExpiration) ;     //   SCDateHoraireExpiration = DateDuJour + HoraireExpiration    
		
		// if ( SCDateHoraireExpiration + 2.0f/1440 < ptrsc->LatestDateTimeForLastBar )  // HoraireExpiration est inférieur à l'heure actuelle (typiquement pour une position ouverte peu avant minuit)   // on ajoute 2min de marge ("2/1440") pour le cas où un robot démarre juste après SCDateHoraireExpiration (car un autre robot vient de s'arrêter et de de lui passer le relai)
		// { ConversionVersSCDateHoraire(DateDuJour+1, ptrsc->Input[24].GetFloat(), SCDateHoraireExpiration) ;  }  // alors l'horaire est décalée à J+1
		
		if ( SCDateHoraireExpiration < ptrsc->LatestDateTimeForLastBar )  // SCDateHoraireExpiration est inférieur à l'heure actuelle 
		{	SCString message ;  message.Format("ID:%d | !! ERREUR !! SCDateHoraireExpiration est inferieur a l'heure actuelle", ptrsc->StudyGraphInstanceID) ; ptrsc->AddMessageToLog(message,1) ; }
	
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CreationSCDateHoraireNewsMoins5Min(s_sc* ptrsc)
{
	int DateDuJour = ptrsc->LatestDateTimeForLastBar.GetDate() ;  // GetDate() returns the date part of the SCDateTime variable. The value returned is a Date Value. // Date values are integer (int) values representing the number of days since December 30, 1899.
	if ( ptrsc->Input[23].GetFloat() != 0 )                      // SCInputRef 	HoraireNews = sc.Input[23]
	{ 
		SCDateTime& SCDateHoraireNewsMoins5Min = ptrsc->GetPersistentSCDateTime(4) ;  
		ConversionVersSCDateHoraire(DateDuJour, ptrsc->Input[23].GetFloat(), SCDateHoraireNewsMoins5Min) ;   SCDateHoraireNewsMoins5Min -= 5.0f/1440 ;    //   SCDateHoraireNewsMoins5Min = DateDuJour + HoraireNews - 5min
		
		
		// if ( SCDateHoraireNewsMoins5Min + 2.0f/1440 < ptrsc->LatestDateTimeForLastBar )  // HoraireNews est inférieur à l'heure actuelle (typiquement pour une position ouverte peu avant minuit)   // on ajoute 2min de marge ("2/1440") pour le cas où un robot démarre juste après SCDateHoraireNewsMoins5Min (car un autre robot vient de s'arrêter et de de lui passer le relai)
		// { ConversionVersSCDateHoraire(DateDuJour+1, ptrsc->Input[23].GetFloat(), SCDateHoraireNewsMoins5Min) ;  }  // alors l'horaire est décalée à J+1
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CreationSCDateHoraireNewsMoins20Sec(s_sc* ptrsc)
{
	int DateDuJour = ptrsc->LatestDateTimeForLastBar.GetDate() ;  // GetDate() returns the date part of the SCDateTime variable. The value returned is a Date Value. // Date values are integer (int) values representing the number of days since December 30, 1899.
	if ( ptrsc->Input[23].GetFloat() != 0 )                      // SCInputRef 	HoraireNews = sc.Input[23]
	{ 
		SCDateTime& SCDateHoraireNewsMoins20Sec = ptrsc->GetPersistentSCDateTime(5) ;  
		ConversionVersSCDateHoraire(DateDuJour, ptrsc->Input[23].GetFloat(), SCDateHoraireNewsMoins20Sec) ;   SCDateHoraireNewsMoins20Sec -= 20.0f/86400 ;    //   SCDateHoraireNewsMoins20Sec = DateDuJour + HoraireNews - 20sec
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCString Get_Nom_ApresClic (s_sc* ptrsc)
{
	SCString Nom = "Erreur recuperation nom" ;  // Initialisation
	int& StudyID_NavigNiveaux = ptrsc->GetPersistentIntFast(14);  
	ptrsc->GetChartStudyInputString(ptrsc->ChartNumber, StudyID_NavigNiveaux,2, Nom) ; 
	return Nom ;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float Get_PrixSurFuture_ApresClic (s_sc* ptrsc) 
{
	double PrixSurFuture = -1 ;
	int& StudyID_NavigNiveaux = ptrsc->GetPersistentIntFast(14);  
	ptrsc->GetChartStudyInputFloat(ptrsc->ChartNumber, StudyID_NavigNiveaux, 3, PrixSurFuture) ; 
	return PrixSurFuture ;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float Get_PrixSurIndice_ApresClic (s_sc* ptrsc)
{
	double PrixSurIndice = -1 ;
	int& StudyID_NavigNiveaux = ptrsc->GetPersistentIntFast(14);  
	ptrsc->GetChartStudyInputFloat(ptrsc->ChartNumber, StudyID_NavigNiveaux, 4, PrixSurIndice) ;  
	return PrixSurIndice ;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int Get_NatureNiveau_ApresClic (s_sc* ptrsc)
{
	int NatureNiveau = -6 ;
	int& StudyID_NavigNiveaux = ptrsc->GetPersistentIntFast(14);  
	ptrsc->GetChartStudyInputInt(ptrsc->ChartNumber, StudyID_NavigNiveaux, 5, NatureNiveau) ;    // int GetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int& r_IntegerValue);
	return NatureNiveau ;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int Get_SousNatureNiveau_ApresClic (s_sc* ptrsc)
{
	int SousNatureNiveau = -6 ;
	int& StudyID_NavigNiveaux = ptrsc->GetPersistentIntFast(14);  
	ptrsc->GetChartStudyInputInt(ptrsc->ChartNumber, StudyID_NavigNiveaux, 6, SousNatureNiveau) ;    // int GetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int& r_IntegerValue);
	return SousNatureNiveau ;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float Get_RangePrecedent_ApresClic (s_sc* ptrsc)
{
	double RangePrecedent = -100 ;
	int& StudyID_NavigNiveaux = ptrsc->GetPersistentIntFast(14);  
	ptrsc->GetChartStudyInputFloat(ptrsc->ChartNumber, StudyID_NavigNiveaux, 0, RangePrecedent) ;    // int GetChartStudyInputFloat(int ChartNumber, int StudyID, int InputIndex, double& r_FloatValue);
	return RangePrecedent ;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AppendFutOuInd (SCString& Texte, int FutureOuIndice)
{
	if ( FutureOuIndice )  { Texte.AppendFormat(" Ind") ;  } 
	else { Texte.AppendFormat(" Fut") ;  }  
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Set_StudyQuantiemeMvt (s_sc* ptrsc)	
{
	// StudyID du graphe #15
	int& StudyID_QuantiemeMvt_Chart15 = ptrsc->GetPersistentIntFast(67) ;

	// StudyID du graphe #19
	int& StudyID_QuantiemeMvt_Chart19 = ptrsc->GetPersistentIntFast(68) ;
	
	// IndexHoraireRAZ ////////
	int IndexHoraireRAZ = ptrsc->GetPersistentIntFast(8);  
	// graphe #15
	int ExternalChartIndex = ptrsc->GetContainingIndexForDateTimeIndex(15, IndexHoraireRAZ) ; 
	int Result = ptrsc->SetChartStudyInputInt(15, StudyID_QuantiemeMvt_Chart15, 8, ExternalChartIndex) ;   // int sc.SetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int IntegerValue);
	if ( Result == 0 )   { SCString message ;  message.Format("ID:%d | !! ERREUR !! La study AMR n'a pas pu transmettre les Inputs pour la study ''Quantieme Mvt'' du graphe #15.", ptrsc->StudyGraphInstanceID) ;  ptrsc->AddMessageToLog(message, 1) ;  }
	// graphe #19
	ExternalChartIndex = ptrsc->GetContainingIndexForDateTimeIndex(19, IndexHoraireRAZ) ;    
	Result = ptrsc->SetChartStudyInputInt(19, StudyID_QuantiemeMvt_Chart19, 8, ExternalChartIndex) ;
	if ( Result == 0 )   { SCString message ;  message.Format("ID:%d | !! ERREUR !! La study AMR n'a pas pu transmettre les Inputs pour la study ''Quantieme Mvt'' du graphe #19.", ptrsc->StudyGraphInstanceID) ;  ptrsc->AddMessageToLog(message, 1) ;  }
	
	// ValeurNiveau ////////
	float NiveauIndice = ptrsc->GetPersistentFloatFast(7);   
	ptrsc->SetChartStudyInputFloat(15, StudyID_QuantiemeMvt_Chart15, 2, NiveauIndice) ; 
	ptrsc->SetChartStudyInputFloat(19, StudyID_QuantiemeMvt_Chart19, 2, NiveauIndice) ; 

	// NatureNiveau ////////
	int NatureNiveau = ptrsc->Input[36].GetInt() ;
	ptrsc->SetChartStudyInputInt(15, StudyID_QuantiemeMvt_Chart15, 3, NatureNiveau) ;
	ptrsc->SetChartStudyInputInt(19, StudyID_QuantiemeMvt_Chart19, 3, NatureNiveau) ; 

	// SousNatureNiveau ////////
	int SousNatureNiveau = ptrsc->Input[34].GetInt() ; 
	ptrsc->SetChartStudyInputInt(15, StudyID_QuantiemeMvt_Chart15, 4, SousNatureNiveau) ;
	ptrsc->SetChartStudyInputInt(19, StudyID_QuantiemeMvt_Chart19, 4, SousNatureNiveau) ; 

	// SousNatureNiveau ////////
	float RangePrecedent = ptrsc->Input[58].GetFloat() ; 
	ptrsc->SetChartStudyInputFloat(15, StudyID_QuantiemeMvt_Chart15, 20, RangePrecedent) ;
	ptrsc->SetChartStudyInputFloat(19, StudyID_QuantiemeMvt_Chart19, 20, RangePrecedent) ; 

	// TriggerRecalculate ////////
	int TriggerRecalculate_ExternalChart = 1 ;
	ptrsc->SetChartStudyInputInt(15, StudyID_QuantiemeMvt_Chart15, 5, TriggerRecalculate_ExternalChart) ; 
	ptrsc->SetChartStudyInputInt(19, StudyID_QuantiemeMvt_Chart19, 5, TriggerRecalculate_ExternalChart) ; 

	// Affichage ////////
	int AffichageONOFF = ptrsc->GetPersistentIntFast(4);  
	Result = ptrsc->SetChartStudyInputInt(15, StudyID_QuantiemeMvt_Chart15, 9, AffichageONOFF) ; 
	ptrsc->SetChartStudyInputInt(19, StudyID_QuantiemeMvt_Chart19, 9, AffichageONOFF) ; 

	// NomNiveau //////// 
	SCInputRef 	NomDuNiveauTrade = ptrsc->Input[26];
	SCString NomNiveau = NomDuNiveauTrade.GetString() ;   
	ptrsc->SetChartStudyInputString(15, StudyID_QuantiemeMvt_Chart15, 1, NomNiveau) ; 
	ptrsc->SetChartStudyInputString(19, StudyID_QuantiemeMvt_Chart19, 1, NomNiveau) ; 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Rectangle2 : s_UseTool             // Attention à ne pas nommer cette structure Rectangle => erreur de compilation, le terme est probablement réservé pour un autre usage
{
	Rectangle2(int beginIndex, int endIndex, float niveau1, float niveau2, COLORREF maCouleur, int region, s_sc* ptrsc) : s_UseTool()  // constructeur qui appelle le constructeur par défaut de la classe mère
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
		TransparencyLevel = 75;             // TransparencyLevel works for the following Chart Drawing types only: DRAWING_RECTANGLEHIGHLIGHT, DRAWING_RECTANGLE_EXT_HIGHLIGHT, DRAWING_ELLIPSEHIGHLIGHT, DRAWING_TRIANGLE.
		Region = region ; 
	} 
};
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void creationRectangle (int beginIndex, int endIndex, float niveau1, float niveau2, COLORREF maCouleur, s_sc* ptrsc)
{
	//Création d'instance de Rectangle2 et appel de sc.UseTool
	Rectangle2 objet(beginIndex, endIndex, niveau1, niveau2, maCouleur, ptrsc->GraphRegion, ptrsc);
	ptrsc->UseTool(objet); 
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float Get_DistanceAuPHPB_SurSL(int index, float SL, s_sc* ptrsc)             // retourne DistanceAuPHPB divisé par SL 	// exprimé en pourcentage (100% = 1.00)    // retourne 1 si la distance est supérieure à 100%      //  théoriquement cette valeur ne peut pas être négative (car un NDHL retourne 1)
{
	if ( ptrsc->Input[16].GetYesNo()  or  ptrsc->Input[36].GetInt() == 6 )   // SCInputRef NewDailyHighLow_Input = sc.Input[16]; // le niveau est un NDHL => on retourne 1 		// SCInputRef 	NatureNiveau_Input = sc.Input[36];  // le niveau est un PHJ ou PBJ => on retourne 1
	{	return 1 ; }
	else
	{
		int& FutureOuIndice = ptrsc->GetPersistentIntFast(16);             
		int& Position = ptrsc->GetPersistentIntFast(9);  		

		if (FutureOuIndice == 0)
		{	
			int& StudyID_PHPBDyn_Fut = ptrsc->GetPersistentIntFast(51);  
			float& NiveauFutureAjuste = ptrsc->GetPersistentFloatFast(5); 
			
			if (Position == 1)
			{
				// récupération de PlusBasSession
				SCFloatArray Array_PBDyn ;
				ptrsc->GetStudyArrayUsingID(StudyID_PHPBDyn_Fut, 3, Array_PBDyn) ;
				float PlusBasSession = Array_PBDyn[index] ;
				// Détermination de DistanceAuPHPB_SurSL
				return fmin( ( NiveauFutureAjuste - PlusBasSession ) / SL, 1.0f ) ;
			}
			else if (Position == -1)
			{
				// récupération de PlusHautSession
				SCFloatArray Array_PHDyn ;
				ptrsc->GetStudyArrayUsingID(StudyID_PHPBDyn_Fut, 2, Array_PHDyn) ;  
				float PlusHautSession = Array_PHDyn[index] ;
				// Détermination de DistanceAuPHPB_SurSL
				return fmin( ( PlusHautSession - NiveauFutureAjuste ) / SL, 1.0f ) ;
			}
		}
		
		else if (FutureOuIndice == 1)
		{
			int& StudyID_PHPBDyn_Ind = ptrsc->GetPersistentIntFast(52);  
			int ExternalChartIndex = ptrsc->GetContainingIndexForDateTimeIndex(15, index) ;    // c'est l'index sur le graphe #15
			SCInputRef ChartInputs_PHPBDyn_Indice = ptrsc->Input[32]; 	 // SCInputRef ChartInputs_PHPBDyn_Indice = sc.Input[32]; 
			float& NiveauIndiceAjuste = ptrsc->GetPersistentFloatFast(8); 
			
			if (Position == 1)
			{
				// récupération de PlusBasSession
				SCFloatArray Array_PBDyn ;
				ChartInputs_PHPBDyn_Indice.SetChartStudySubgraphValues(15, StudyID_PHPBDyn_Ind, 7);    // Création des ChartStudySubgraphValues    	   // SCInputRef sc.Input[].SetChartStudySubgraphValues(int ChartNumber, int StudyID, int SubgraphIndex);
				ptrsc->GetStudyArrayFromChartUsingID(ChartInputs_PHPBDyn_Indice.GetChartStudySubgraphValues(), Array_PBDyn) ;    
				float PlusBasSession = Array_PBDyn[ExternalChartIndex] ;    
				// Détermination de DistanceAuPHPB_SurSL
				return fmin( ( NiveauIndiceAjuste - PlusBasSession ) / SL, 1.0f ) ;
			}
			else if (Position == -1)
			{
				// récupération de PlusHautSession
				SCFloatArray Array_PHDyn ;
				ChartInputs_PHPBDyn_Indice.SetChartStudySubgraphValues(15, StudyID_PHPBDyn_Ind, 6);    // Création des ChartStudySubgraphValues   // SCInputRef sc.Input[].SetChartStudySubgraphValues(int ChartNumber, int StudyID, int SubgraphIndex);
				ptrsc->GetStudyArrayFromChartUsingID(ChartInputs_PHPBDyn_Indice.GetChartStudySubgraphValues(), Array_PHDyn) ;    
				float PlusHautSession = Array_PHDyn[ExternalChartIndex] ;
				// Détermination de DistanceAuPHPB_SurSL
				return fmin( ( PlusHautSession - NiveauIndiceAjuste ) / SL, 1.0f ) ;
			}
		}
	}
	return 1 ;  // pour éviter warning du compilateur
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float AjustementParabolique (float Bonus, float AjustementAbsolu, float CoeffMalusAjustement) 
{
	if ( Bonus == 0 ) return 0 ;
	else
	{
		float Lambda = Bonus / CoeffMalusAjustement / 1.96 ;
		return Bonus * pow(AjustementAbsolu/Lambda,2) ;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float TailleBougie_4000Ticks(float SpreadIndex, s_sc* ptrsc) 
{
	SCInputRef ModelePourAUC_Input = ptrsc->Input[55]; 	 		// SCInputRef ModelePourAUC_Input = sc.Input[55]; 
	
	if ( ModelePourAUC_Input.GetIndex() == 0 )				// C'est le modèle 7, basé sur l'ensemble des ranges 4000t plottés vs les SpreadIndex moyen sur 4000t		// Conférer l'onglet "UT4000t, SI v Range, 2020-1à3" dans le fichier"VIX et Envergure4000TicksSeancePrecedente et SpreadIndex.xlsx"
	{ return -0.064*SpreadIndex*SpreadIndex+27.01*SpreadIndex-15.91 ;  }			
	else if ( ModelePourAUC_Input.GetIndex() == 1 )		// C'est le modèle 9, basé sur l'ensemble des ranges 4000t plottés vs les SpreadIndex moyen sur 4000t		// Conférer l'onglet "UT4000t, SI v Range, 2020-3à6" dans le fichier"VIX et Envergure4000TicksSeancePrecedente et SpreadIndex.xlsx"
	{ return -0.736*SpreadIndex*SpreadIndex+28.385*SpreadIndex-14.471 ;  }		
	else if ( ModelePourAUC_Input.GetIndex() == 2 )		// C'est le modèle 10, basé sur l'ensemble des ranges 4000t plottés vs les SpreadIndex moyen sur 4000t		// Conférer l'onglet "UT4000t, SI v Range, 2020-6à9" dans le fichier"VIX et Envergure4000TicksSeancePrecedente et SpreadIndex.xlsx"
	{ return -3.60*SpreadIndex*SpreadIndex+38.01*SpreadIndex-22.24 ;  }		
	else if ( ModelePourAUC_Input.GetIndex() == 3 )		// C'est le modèle 11, basé sur l'ensemble des ranges 4000t plottés vs les SpreadIndex moyen sur 4000t		// Conférer l'onglet "UT4000t, SI v Range, 2020-9à12" dans le fichier"VIX et Envergure4000TicksSeancePrecedente et SpreadIndex.xlsx"
	{ return -2.05*SpreadIndex*SpreadIndex+31.58*SpreadIndex-16.6 ;  }		
	else 
	{ return 1 ;  }
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
	// Conversion of the float number into a string
	{
		std::ostringstream oss;  // flux pour écrire dans un stream
		oss << number;       // par chance number est affiché avec 6 chiffres après la virgule, sans que j'aie eu à le demander    // pour le modifier : http://www.cplusplus.com/reference/iomanip/setprecision/    http://www.cplusplus.com/reference/ios/ios_base/precision/
		MyString = oss.str() ;
	}
	// Replacement of '.' by ','   
	{
		std::replace( MyString.begin(), MyString.end(), '.', ',');
	}
	return MyString ;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int NumberOfMonitors()		// Number of display monitors on desktop
{
	return GetSystemMetrics(80) ;		// https://docs.microsoft.com/fr-fr/windows/win32/api/winuser/nf-winuser-getsystemmetrics
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float ConversionDuInputBonusIntrinsequeRoulette (int BonusIntrinseque_Roulette_Input)
{
	float BonusIntrinseque ;						  
	if 			( BonusIntrinseque_Roulette_Input == 0 )  	{ BonusIntrinseque = 650 ; }         														
	else if	( BonusIntrinseque_Roulette_Input <= 9 )  	{ BonusIntrinseque = 650 - 50 * BonusIntrinseque_Roulette_Input ; }			// index de 1 à 9 : utilisés pour le développement :   index 9 => 200 ; index 1 => 600
	else if	( BonusIntrinseque_Roulette_Input <= 29 )  	{ BonusIntrinseque = 200 - 5 * BonusIntrinseque_Roulette_Input ; }       	  	// index de 150 vaut 10     // index de 55 vaut 29
	else if 	( BonusIntrinseque_Roulette_Input <= 41 )	{ BonusIntrinseque = 112 - 2 * BonusIntrinseque_Roulette_Input ; }				// index de 52 vaut 30       // index de 30 vaut 41
	else if 	( BonusIntrinseque_Roulette_Input <= 61 )	{ BonusIntrinseque = 71 - BonusIntrinseque_Roulette_Input ; }						// index de 10 vaut 61 
	else 	  BonusIntrinseque = 9 ;
	return BonusIntrinseque ;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int WindowsVersionNumber()		// retourne la version de Windows (multipliée par 10 pour ne pas avoir un float)		// WindowsVersionNumber vaut 61 pour mon PC de bureau et 62 pour mon VPS
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SCSFExport scsf_AnnulationMiniRebond(SCStudyInterfaceRef sc) 
{
	// Subgraphs /////////////////////////////////////////////////////////////////////////////////////////////
	SCSubgraphRef Background = sc.Subgraph[0] ;  
	
	SCSubgraphRef RatioAUCProjete = sc.Subgraph[1] ;  
	SCFloatArrayRef AUC_Future = sc.Subgraph[1].Arrays[0] ;  
	SCFloatArrayRef AUC_Indice = sc.Subgraph[1].Arrays[1] ;  
	SCFloatArrayRef RatioAUC = sc.Subgraph[1].Arrays[2] ;  
	SCFloatArrayRef DistMiniFuture = sc.Subgraph[1].Arrays[3] ;  
	SCFloatArrayRef DistMiniIndice = sc.Subgraph[1].Arrays[4] ;  
	
	SCSubgraphRef RatioSigmaProjete = sc.Subgraph[2] ;  
	SCFloatArrayRef Sigma = sc.Subgraph[2].Arrays[0] ;  
	SCFloatArrayRef RatioSigma = sc.Subgraph[2].Arrays[1] ;  
	
	SCSubgraphRef SommeRatioAvecMalusProjete = sc.Subgraph[4] ;
	SCFloatArrayRef SommeRatioAvecMalus = sc.Subgraph[4].Arrays[0] ;  
	
	SCSubgraphRef MalusExcedentaireProjete = sc.Subgraph[5] ;
	SCSubgraphRef StopLossProjete = sc.Subgraph[6] ;  
	SCSubgraphRef Niveau1TP = sc.Subgraph[7] ;											// = NiveauFutureAjuste + Position * TPCible  ;  TPCible = TP_AUC, avant entrée en position ; TPCible = TPOptimumArrondi (arrondi au quart de tick), après entrée en position  		// TP_AUC = TPNu * TailleBougie4000T / TailleBougie4000T_Reference * MultiplicateurPrix    // TPOptimum =  TP_AUC] * (1+BonusExcedentaire/100)    
	SCSubgraphRef Niveau2TP = sc.Subgraph[8] ;  
	SCSubgraphRef Niveau3TP = sc.Subgraph[9] ;  
	SCSubgraphRef Niveau4TP = sc.Subgraph[10] ;  
	SCSubgraphRef BasculeTP0 = sc.Subgraph[11] ;  
	SCSubgraphRef NiveauIndiceSurFutureTempsReel = sc.Subgraph[12] ;		// courbe en vert (projection du indice tradé sur le future)
	SCSubgraphRef NiveauTPOptimumArrondi = sc.Subgraph[13] ;					// NiveauFutureAjuste + Position * TPOptimumArrondi ;   
	SCFloatArrayRef NiveauTPOptimum = sc.Subgraph[13].Arrays[0] ;								// NiveauFutureAjuste + Position * TPOptimum  ;      TPOptimum =  TPNu * SI * MultiplicateurPrix * (1+BonusExcedentaire/100)   
	

	
	// Inputs /////////////////////////////////////////////////////////////////////////////////////////////
	SCInputRef 	DateEtudiee = sc.Input[1];                              						// c'est une date value (nb de jour depuis le 1/1/1900)
	SCInputRef 	DateEtudieeRoulette = sc.Input[2];                 					// c'est un string
	SCInputRef 	HoraireRAZTexte = sc.Input[3]; 	   										// numéro appelé par Set_HoraireExpiration_PP1h et Set_HoraireExpiration_PP4h
	SCInputRef 	HoraireRAZRoulette = sc.Input[4]; 	
	SCInputRef 	FutureOuIndice_Input = sc.Input[6]; 									// numéro appelé par Padding
	SCInputRef 	NiveauFutureTexte = sc.Input[7]; 										// numéro appelé par la study Prix_typique
	SCInputRef 	NiveauFutureRoulette = sc.Input[8]; 
	SCInputRef 	NiveauIndiceTexte = sc.Input[10]; 										// numéro appelé par PrixTypique		// appelé par Padding
	SCInputRef 	PeriodeNiveauTradeInput = sc.Input[11]; 
	SCInputRef 	QuelNiveauTradeInput = sc.Input[12]; 
	SCInputRef 	TPNu_Manuel_Input = sc.Input[13]; 									// TPNu auto lorsque cet input prend la valeur 0
	SCInputRef 	BonusIntrinseque_Exterieur_Input = sc.Input[14]; 
	SCInputRef 	BonusIntrinseque_Roulette_Input = sc.Input[15]; 
	SCInputRef 	NewDailyHighLow_Input = sc.Input[16];                         	 // numéro appelé par Get_DistanceAuPHPB_SurSL
	SCInputRef 	AjustementChoisiEnPas_Input = sc.Input[19];                  
	SCInputRef 	AffichageONOFF_Input = sc.Input[20]; 
	SCInputRef 	CompteTicksONOFFInput = sc.Input[22]; 
	SCInputRef 	HoraireNews = sc.Input[23]; 												// float au format hhmm.ss          // appelé par Calendrier
	SCInputRef 	HoraireExpiration = sc.Input[24]; 	  									// numéro appelé par Set_HoraireExpiration_PP1h, Set_HoraireExpiration_PP4h, RAZHoraireExpiration, CreationSCDateHoraireExpiration
	SCInputRef 	CetteInstanceControleLesBoutons = sc.Input[25]; 				// numéro appelé par DrawText_ID_NomDuNiveauTrade_Ajustement           // numéro appelé par SetButton52And56Text
	SCInputRef 	NomDuNiveauTrade = sc.Input[26]; 									// numéro appelé par DrawText_ID_NomDuNiveauTrade_Ajustement			// numéro appelé par Set_StudyQuantiemeMvt
	SCInputRef 	RobotPosition = sc.Input[30]; 												// numéro appelé par la study Prix_typique
	SCInputRef	ChartInputs_PHPBDyn_Indice = sc.Input[32]; 	
	SCInputRef	SendOrdersToTradeService = sc.Input[33]; 	
	SCInputRef 	SousNatureNiveau_Input = sc.Input[34];       						// numéro appelé par Set_StudyQuantiemeMvt
	SCInputRef	SequenceONOFF_Input = sc.Input[35]; 	           	            	// numéro appelé par SetButton52And56Text
	SCInputRef 	NatureNiveau_Input = sc.Input[36];                    					// NatureNiveau = 0 (psy) / 1 (1h) / 2 (4h) / 3 (jour) / 4 (hebdo) / 5 (mensuel) / 6 (PHJ PBJ)		  // numéro appelé par Get_DistanceAuPHPB_SurSL
	SCInputRef	External_Input_ACS_BUTTON_9 = sc.Input[37]; 	         	// numéro appelé par la study "Padding et boutons ACS"        		// "HoraireRaz=LastVisibleBar - 1min" (Z)
	SCInputRef	External_Input_ACS_BUTTON_11 = sc.Input[38];          	// numéro appelé par la study "Padding et boutons ACS"				// "-1min"
	SCInputRef	External_Input_ACS_BUTTON_12 = sc.Input[39];          	// numéro appelé par la study "Padding et boutons ACS"				// "+1min"
	SCInputRef	External_Input_ACS_BUTTON_60 = sc.Input[40];          	// numéro appelé par la study "Prix Typique"									// "NEXT"
	SCInputRef	External_Input_ACS_BUTTON_8 = sc.Input[41]; 	         	// numéro appelé par la study "Padding et boutons ACS"        		// "RoundToHour(LastVisibleBar)"
	SCInputRef	External_Input_ACS_BUTTON_49 = sc.Input[42];         	// numéro appelé par la study "Padding et boutons ACS"        		// "AffichageONOFF"
	SCInputRef	External_Input_ACS_BUTTON_109_110 = sc.Input[43];   // numéro appelé par la study "Padding et boutons ACS"        		// "Niv -" ou "Niv +"
	SCInputRef	External_Input_ACS_BUTTON_10 = sc.Input[44];   			// numéro appelé par la study "Padding et boutons ACS"        		// "9h30m00s" (CTRL+Z)
	SCInputRef	External_Input_ACS_BUTTON_133_134 = sc.Input[45];   // numéro appelé par la study "Padding et boutons ACS"        		// "navigation rapide" (CTRL+5)
	SCInputRef	External_Input_ACS_BUTTON_20 = sc.Input[46];   			// numéro appelé par la study "Padding et boutons ACS"        		// "récupération du bonus extérieur"
	SCInputRef	External_Input_ACS_BUTTON_52 = sc.Input[47];          	// numéro appelé par la study "Padding et boutons ACS"				// "Autotrade"
	SCInputRef	External_Input_ACS_BUTTON_15 = sc.Input[48];          	// numéro appelé par la study "Padding et boutons ACS"				// "Bonus -"
	SCInputRef	External_Input_ACS_BUTTON_16 = sc.Input[49];          	// numéro appelé par la study "Padding et boutons ACS"				// "Bonus +"
	SCInputRef	External_Input_ACS_BUTTON_53 = sc.Input[50];          	// numéro appelé par la study "Padding et boutons ACS"				// "CTRL+A"
	SCInputRef	External_Input_ACS_BUTTON_90 = sc.Input[51];          	// numéro appelé par la study "Padding et boutons ACS"				// "KILL"
	SCInputRef	MalusAjustementParabolique_Input = sc.Input[52];          							
	SCInputRef	MalusNRV_Manuel_Input = sc.Input[53];          							
	SCInputRef	ModelePourAUC_Input = sc.Input[55];          						// numéro appelé par la fonction TailleBougie_4000Ticks
	SCInputRef	NewDailyHighLow_InputManuel = sc.Input[57];   				// vaut 0 pour "sans malus NewDHiLo" ; 1 pour "avec malus NewDHiLo"		
	SCInputRef 	RangePrecedent_Input = sc.Input[58];       							// numéro appelé par Set_StudyQuantiemeMvt
	SCInputRef	External_Input_ACS_BUTTON_6 = sc.Input[59];         	 	// numéro appelé par la study "Padding et boutons ACS"				// "- 1 pas"
	SCInputRef	External_Input_ACS_BUTTON_7 = sc.Input[60];          		// numéro appelé par la study "Padding et boutons ACS"				// "+ 1 pas" 
	
	
	
	
	
	// Références /////////////////////////////////////////////////////////////////////////////////////////////
	int& Void = sc.GetPersistentInt(1); 																			// Void == 1 lorsque Void est actif sinon Void vaut 0   // appelé par la study Padding
	int& AutoTradeDemandeArret = sc.GetPersistentInt(42);     									// appelé par le DLL "Detecteur_incidents" (fonction ArretDesRobotsAMR)
	int& SequenceStatus = sc.GetPersistentInt(56);      												// appelé par le DLL "Detecteur_incidents" (fonction ArretDesRobotsAMR)		// appelé par SetButton56Text            
	
	int& NbMonitors = sc.GetPersistentIntFast(0); 
	int& StudyID_SI = sc.GetPersistentIntFast(1);  
	int& StudyIDTPNu = sc.GetPersistentIntFast(2);
	int& StudyIDSigma = sc.GetPersistentIntFast(3);  
	int& AffichageONOFF = sc.GetPersistentIntFast(4);  												// appelé par Set_StudyQuantiemeMvt
	int& AjustementChoisiEnPas = sc.GetPersistentIntFast(5);									// appelé par DrawText_ID_NomDuNiveauTrade_Ajustement
	int& TriggerRAZ = sc.GetPersistentIntFast(6);  
	int& IndexHoraireRAZ = sc.GetPersistentIntFast(8);  												// appelé par Set_StudyQuantiemeMvt
	int& Position = sc.GetPersistentIntFast(9);  															// appelé par Get_DistanceAuPHPB_SurSL, Set_StudyQuantiemeMvt
	int& NbBarresVirginite = sc.GetPersistentIntFast(10);  
	int& PremiereToucheFaite = sc.GetPersistentIntFast(11);  
	int& CompteTicksONOFF = sc.GetPersistentIntFast(13);  
	int& StudyID_NavigNiveaux = sc.GetPersistentIntFast(14);  
	int& StudyIDOverlayINQ100 = sc.GetPersistentIntFast(15);  
	int& FutureOuIndice = sc.GetPersistentIntFast(16);												// appelé par la fct Get_DistanceAuPHPB_SurSL
	int& TriggerRecalculate = sc.GetPersistentIntFast(17);  
	int& IndexPremiereTouche = sc.GetPersistentIntFast(18);  
	int& IndexPeriode = sc.GetPersistentIntFast(19);  
	int& StopLossIndiceDepasse = sc.GetPersistentIntFast(20);  
	int& TriggerPremierOrdre = sc.GetPersistentIntFast(21);										// TriggerPremierOrdre = 1  lors du clic sur bouton AutoTrade      // TriggerPremierOrdre = 2 si les conditions ont été remplies pour passer un ordre      // TriggerPremierOrdre = 3 lorsque le premier ordre a été correctement soumis    // TriggerPremierOrdre = 4 lorsqu'une annulation d'ordres en attente a été correctement envoyée      //  TriggerPremierOrdre = 5 lorsqu'il n'y a plus d'ordre en attente.       //  TriggerPremierOrdre = 999 : une erreur s'est produite
	int& Target1OrderID = sc.GetPersistentIntFast(22);
	int& Stop1OrderID = sc.GetPersistentIntFast(23);
	int& TriggerModificationTPSL = sc.GetPersistentIntFast(24);
	int& Increment = sc.GetPersistentIntFast(25);
	int& LastIndexPrecedent = sc.GetPersistentIntFast(26);
	int& ParentOrderID = sc.GetPersistentIntFast(27);
	int& AutoTradeStatus = sc.GetPersistentIntFast(28);          									// appelé par les fct SetButton52And56Text et DrawText_ID_NomDuNiveauTrade_Ajustement
	int& PositionOuverte = sc.GetPersistentIntFast(29);    											// vaut 0 (avant fill), vaut 1 (après fill de l'ordre parent), vaut 2 (après fill d'un ordre child)
	int& OrdreOuvert = sc.GetPersistentIntFast(30);   													// vaut 1 tant qu'il reste un ordre non annulé
	int& AutoTradeFACAO = sc.GetPersistentIntFast(31);
	int& AutoTradeStatus_Prior = sc.GetPersistentIntFast(32);
	int& DateLocale = sc.GetPersistentIntFast(33);
	int& TriggerResetScale = sc.GetPersistentIntFast(34);
	int& BasculeTP0Depassee_TriggerDuRobot = sc.GetPersistentIntFast(35);
	int& StopLossIndiceDepasse_TriggerDuRobot = sc.GetPersistentIntFast(36);
	int& RobotActif = sc.GetPersistentIntFast(37);																// appelé par la fct DrawText_ID_NomDuNiveauTrade_Ajustement
	int& IndexArretManualLooping = sc.GetPersistentIntFast(38);
	int& IndexWarpJump = sc.GetPersistentIntFast(39);
	int& AutoTradeEstArreteManuellement = sc.GetPersistentIntFast(40);
	int& IndexQuelNiveau = sc.GetPersistentIntFast(41);
	int& DureeCalculCompletAMR = sc.GetPersistentIntFast(42); 
	int& NombreCyclesConsecutifsTropLong = sc.GetPersistentIntFast(43); 
	int& LaStudyVientDEtreRajoutee = sc.GetPersistentIntFast(45); 
	int& ExternalChartIndex_UT200t = sc.GetPersistentIntFast(46);     // graphe #11
	int& ExternalChartIndex_UT2000t = sc.GetPersistentIntFast(47);     // graphe #12
	int& ExternalChartIndex_UT20s = sc.GetPersistentIntFast(48);     // graphe #15 
	int& ExternalChartIndex_UT2min = sc.GetPersistentIntFast(49);    // graphe #19
	int& MyEP = sc.GetPersistentIntFast(50);  
	int& StudyID_PHPBDyn_Fut = sc.GetPersistentIntFast(51);  								// appelé par la fct Get_DistanceAuPHPB_SurSL
	int& StudyID_PHPBDyn_Ind = sc.GetPersistentIntFast(52);									// appelé par la fct Get_DistanceAuPHPB_SurSL
	int& StudyID_PrixTypique = sc.GetPersistentIntFast(53);										// appelé par plusieurs fct
	int& BasculeTP0Depassee = sc.GetPersistentIntFast(54);  
	int& DureeCycleComplet_FullRecalculation = sc.GetPersistentIntFast(56);       	
	int& StudyID_VIX = sc.GetPersistentIntFast(57);  
	int& AutoTradeFACAO_MessageDansLog = sc.GetPersistentIntFast(58);  
	int& ParentOrderID_EchecGetOrder_MessageDansLog = sc.GetPersistentIntFast(59);  
	int& Target1OrderID_EchecGetOrder_MessageDansLog = sc.GetPersistentIntFast(60);  
	int& Stop1OrderID_EchecGetOrder_MessageDansLog = sc.GetPersistentIntFast(61);  
	int& CouleurPosition = sc.GetPersistentIntFast(62); 
	int& CouleurPositionInverse = sc.GetPersistentIntFast(63); 
	int& StudyID_DistancePHJPBJ_SP500 = sc.GetPersistentIntFast(64); 
	int& StudyID_Padding_Chart15 = sc.GetPersistentIntFast(65) ;
	int& StudyID_Padding_Chart19 = sc.GetPersistentIntFast(66) ;
	int& StudyID_QuantiemeMvt_Chart15 = sc.GetPersistentIntFast(67) ;			// appelé par la fonction Set_StudyQuantiemeMvt
	int& StudyID_QuantiemeMvt_Chart19 = sc.GetPersistentIntFast(68) ;			// appelé par la fonction Set_StudyQuantiemeMvt
	int& TakeProfitIndiceDepasse = sc.GetPersistentIntFast(69) ;	
	
	
	__int64& ElapsedTicks_PrecedentCycle = sc.GetPersistentInt64(1); 
	__int64& ElapsedTicks_CycleMoins2 = sc.GetPersistentInt64(2);  
	__int64& ElapsedTicks_CycleMoins3 = sc.GetPersistentInt64(3);  
	__int64& ElapsedTicks_CycleMoins4 = sc.GetPersistentInt64(4);  
	

	float& BonusIntrinseque = sc.GetPersistentFloat(0);  													// appelé par la study PrixTypique
	
	float& TPCible = sc.GetPersistentFloatFast(0);  
	float& TPOptimumArrondi_Prior = sc.GetPersistentFloatFast(1);  
	float& SL_Prior = sc.GetPersistentFloatFast(2);  
	float& NiveauStopLoss = sc.GetPersistentFloatFast(3);  
	float& NiveauFuture = sc.GetPersistentFloatFast(4);         											// c'est le niveau tradé sur future (que l'on trade un niveau de l'indice ou du future) avant ajustement
	float& NiveauFutureAjuste = sc.GetPersistentFloatFast(5);  									// appelé par Get_DistanceAuPHPB_SurSL		// c'est le prix (sur future) de l'ordre à cours limite pour l'entrée en position (que l'on trade le future ou l'indice)
	float& HoraireRAZ = sc.GetPersistentFloatFast(6);               									// Horaire au format hhmm.ss : 1559.59 pour 15h59m59s
	float& NiveauIndice = sc.GetPersistentFloatFast(7);  											// appelé par Set_StudyQuantiemeMvt		// c'est le niveau cible, si on trade l'indice
	float& NiveauIndiceAjuste = sc.GetPersistentFloatFast(8);  									// appelé par Get_DistanceAuPHPB_SurSL		// c'est le niveau cible ajusté, si on trade l'indice
	float& TPOptimumArrondi = sc.GetPersistentFloatFast(9);   										// exprimé en points
	float& TP_de_la_precedente_modification_de_TP = sc.GetPersistentFloatFast(10);   // pour la gestion du TP
	float& NiveauFutureOrdreEntree = sc.GetPersistentFloatFast(11);    							// (prix d'entrée demandé), qui correspond à NiveauFutureAjuste lors de l'envoi de l'ordre triple.
	float& BonusExcedentaire_Prior = sc.GetPersistentFloatFast(12);   	
	float& BonusMoy_Prior = sc.GetPersistentFloatFast(13);  
	float& TPOptimum_Prior = sc.GetPersistentFloatFast(14);  
	float& TPNu_Prior = sc.GetPersistentFloatFast(15);  
	float& MalusDeNRV_Prior = sc.GetPersistentFloatFast(16);  
	float& AjustementSubiPoints_Prior = sc.GetPersistentFloatFast(17);  
	float& AUC_Prior = sc.GetPersistentFloatFast(18);  
	float& SeuilAUC_Prior = sc.GetPersistentFloatFast(19);  
	float& RatioAUC_Future_Prior = sc.GetPersistentFloatFast(20);  
	float& RatioAUC_Prior = sc.GetPersistentFloatFast(21);  
	float& Sigma_Prior = sc.GetPersistentFloatFast(22);  
	float& SeuilSigma_Prior = sc.GetPersistentFloatFast(23);  
	float& RatioSigma_Prior = sc.GetPersistentFloatFast(24);  
	float& SommeRatioAvecMalus_Prior = sc.GetPersistentFloatFast(25);  
	float& SpreadIndex_Prior = sc.GetPersistentFloatFast(26);  
	float& TauxVirginite_Prior  = sc.GetPersistentFloatFast(27);   										// exprimé en valeur normé à 1. Une valeur de 1 équivaut à 100%.
	float& MalusDePremierPassageJournalier_Prior = sc.GetPersistentFloatFast(28); 
	float& delta = sc.GetPersistentFloatFast(29);  
	float& DistanceAuPHPB_SurSL_Prior = sc.GetPersistentFloatFast(30);  
	float& CoeffMalusAjustement_Prior = sc.GetPersistentFloatFast(31);  
	float& BonusExcedentaireSuffisant_Prior = sc.GetPersistentFloatFast(32);  
	float& MalusAjustementAbsolu_Prior = sc.GetPersistentFloatFast(33);  
	float& NbBarresVirginite_Prior = sc.GetPersistentFloatFast(34);  
	float& SL = sc.GetPersistentFloatFast(35);  
	float& DistanceAuPHPB_SurSL = sc.GetPersistentFloatFast(36);								// vaut DistanceAuPHPB divisé par SL 	// exprimé en pourcentage (100% = 1.00)
	float& TP_AUC = sc.GetPersistentFloatFast(37);    														// exprimé en points
	float& MultiplicateurPrix = sc.GetPersistentFloatFast(38);    
	float& TP_AUC_Prior = sc.GetPersistentFloatFast(39);    
	float& Config_tailleTexte = sc.GetPersistentFloatFast(40);    										// appelé par plusieurs structures		// variable utilisée par toutes les structures qui affichent du texte
	float& Position_1ere_Colonne = sc.GetPersistentFloatFast(41);    
	float& Position_2eme_Colonne = sc.GetPersistentFloatFast(42);    
	float& Position_3eme_Colonne = sc.GetPersistentFloatFast(43);    
	float& BaseDuTrace = sc.GetPersistentFloatFast(44);  
	float& PrixDuTP = sc.GetPersistentFloatFast(45);  
	float& PrixDuSL = sc.GetPersistentFloatFast(46);  
	float& Malus_SpreadIndex_Prior = sc.GetPersistentFloatFast(47);  
	float& MalusExcedentaire_Prior = sc.GetPersistentFloatFast(48);  
	float& PrixSL = sc.GetPersistentFloatFast(49);  
	float& NiveauIndiceAjuste_Prior = sc.GetPersistentFloatFast(50);  


	SCDateTime& SCDateHoraireNewsMoins5Min = sc.GetPersistentSCDateTime(4);      		// appelé par la Study Calendrier et la fct CreationSCDateHoraireNewsMoins5Min        
	SCDateTime& SCDateHoraireNewsMoins20Sec = sc.GetPersistentSCDateTime(5);      	// appelé par la Study Calendrier et la fct CreationSCDateHoraireNewsMoins20Sec        

	
	SCDateTime& SCDateHoraireRAZ = sc.GetPersistentSCDateTimeFast(1);  
	SCDateTime& SCDateHoraireExpiration = sc.GetPersistentSCDateTimeFast(2);     				// appelé par CreationSCDateHoraireExpiration
	SCDateTime& SCDateHoraireFinSeanceAction = sc.GetPersistentSCDateTimeFast(3);  

	
	SCString& Texte_x128 = sc.GetPersistentSCString(1);  
	SCString& Texte_x64 = sc.GetPersistentSCString(2);  
	SCString& Texte_x32 = sc.GetPersistentSCString(3);  
	SCString& Texte_x16 = sc.GetPersistentSCString(4);  
	SCString& Texte_x8 = sc.GetPersistentSCString(5);  
	SCString& Texte_x4 = sc.GetPersistentSCString(6);  
	SCString& Texte_x2 = sc.GetPersistentSCString(7);  
	SCString& Texte_x1 = sc.GetPersistentSCString(8);  
	SCString& Texte_x05 = sc.GetPersistentSCString(9);  
	SCString& Texte_x025 = sc.GetPersistentSCString(10);  
	SCString& Texte_Recapitulatif = sc.GetPersistentSCString(11);  
	SCString& Texte_SL = sc.GetPersistentSCString(12);  

	


	
	// Variables /////////////////////////////////////////////////////////////////////////////////////////////
	
	int const TailleDuCustomInputDeNiveauFuture = 80 ;   // c'est la taille dans une direction à partir de NiveauFuture   // la vraie taille est 2*TailleDuCustomInputDeNiveauFuture+1
	int const TailleDuCustomInputDeHoraireRAZ = 60 ;   // c'est la taille dans une direction à partir de HoraireRAZ   // la vraie taille est 2*TailleDuCustomInputDeHoraireRAZ+1
	int const TailleDuCustomInputDeDateRAZ = 150 ;   // c'est le nombre de jours qu'on peut revenir en arrière
	int const MaxBarresCalculees = 27000 ;  
	int const LastIndex = sc.ArraySize - 1 ;
	int const ThisStudyID = sc.StudyGraphInstanceID ;  // StudyID de l'instance
	
	int PremierIndex ;
	int DateRAZ ; 
	int RecalculComplet = 0 ;
	int Quantity_NiveauFutureAjuste=0 ;
	int Level_NiveauFutureAjuste ;
	int OrdreOuvertParent = -3 ;
	int OrdreOuvertTarget1 = -3 ;
	int OrdreOuvertStop1 = -3 ;
	int ParentOrderDetails_OrderStatusCode = -3 ;
	int Target1OrderDetails_OrderStatusCode = -3 ;
	int Stop1OrderDetails_OrderStatusCode = -3 ;

	
	LARGE_INTEGER Frequency ;  QueryPerformanceFrequency(&Frequency) ; 
	LARGE_INTEGER ElapsedTicks_t0 ;  QueryPerformanceCounter(&ElapsedTicks_t0) ;  
	LARGE_INTEGER ElapsedTicks ;


	float const Puissance = 5 ;  						// identique pour future et indice
	float const Interligne = ( sc.YPixelCoordinateToGraphValue(sc.StudyRegionTopCoordinate) - sc.YPixelCoordinateToGraphValue(sc.StudyRegionBottomCoordinate) ) / 100 *sqrt(Config_tailleTexte) ;   // c'est la valeur en points (divisée par 100) entre deux lignes de textes affichées sur la région #2 du chart.		// 1*Interligne = 1% de la hauteur en points  ;  50*Interligne = la moitié de la hauteur en points
	float const CoeffMalusExcedentaire = 5 ;      // cette constante est indépendante de la valeur de l'indice. Elle multiplie BonusExcedentaire et BonusExcedentaireSuffisant pour obtenir MalusExcedentaire
	float const Offset_StopLimit = 6 ;					// le CME met 6.0 (au 30/1/2020), je n'ai pas la main dessus.
	float Malus_SpreadIndex = 0 ;
	
	
	float SpreadIndex ;
	float TPNu ;    // exprimé en ticks
	float TPOptimum ;    // exprimé en points
	float BonusExcedentaire ;
	float Contribution_Future ;
	float RatioAUCBis ;
	float AUC_Future_Bis ;
	float RatioSigmaBis ;
	// float SommeRatioBis ;
	float MalusExcedentaire ;
	float SommeRatioAvecMalusBis ;
	float MalusExcedentaireBis ;
	float Contribution_Indice ;
	float AUC_Indice_Bis ;
	float AjustementSubiPoints = 0 ;
	float MalusDeNRV ;
	float MalusDePremierPassageJournalier ;
	float BonusMoy ;
	float SeuilSigma ;
	float NewSeuilSigma ;
	float CsteNewSigma ;
	float DistanceMiniAuNiveau ;
	float DistanceMiniRecenteAuNiveau_Future ;
	float DistanceMiniRecenteAuNiveau_Indice ;
	float DistanceMiniRecenteAuNiveau ;
	float DistanceMiniAuDanger ;   // DistanceMiniAuDanger est le nb de TP auquel on se situe du niveau tradé. 
	float TPCibleMajore ;
	float BonusExcedentaireSuffisant ;
	float Clicks_BlocRobotDeTrading ;
	float Clicks_BlocAffichage ;
	float CoeffMalusAjustement ;
	float TauxVirginite ;
	float MalusAjustementAbsolu ;
	

	s_sc* const ptrsc = &sc;      // c'est un pointeur constant sur un s_sc   // attention : " const s_sc* " est un pointeur sur un s_sc constant
	
	SCString Texte ; 
	
	COLORREF CouleurDrawText ;
	
	SCFloatArray ArraySpreadIndex;
	SCFloatArray ArrayTPNu;  		// TPNu en pas
	SCFloatArray ArraySigmaUp;
	SCFloatArray ArraySigmaDown;
	SCFloatArray ArraySigmaBackground; 
	SCFloatArray INQ100AvecCorrection; 
	SCFloatArray SpreadMoyen ;
	SCFloatArray INQ100SansCorrection ;
	SCFloatArray ExternalArray_EnvergureMoyenne4000Ticks ;
	
	
	// enum Instance { Serveur, Client } ;
	
	
	
	// Bloc sc.SetDefaults /////////////////////////////////////////////////////////////////////////////////////////////
	if (sc.SetDefaults)
	{
		sc.GraphName = "ANNUL MINI REBOND" ;    // nom appelé par la study "Prix typique" lors du recensement des instances   // nom appelé par le DLL "Detecteur_incidents"         // nom appelé par le DLL "Padding et boutons ACS"
		sc.GraphRegion = 1 ;  
		sc.ScaleRangeType = SCALE_SAMEASREGION;   // par défaut c'est SCALE_AUTO
		sc.ValueFormat = 2 ;
		sc.AutoLoop = 0 ;  
		sc.CalculationPrecedence = VERY_LOW_PREC_LEVEL  ;  // le TPNu et le SpreadIndexAffichage doivent être calculés avant le Sigma. Le Sigma doit être calculé avant AnnulMiniRebond
		sc.DisplayStudyName = 1;
		sc.DisplayStudyInputValues = 1;
		sc.GlobalDisplayStudySubgraphsNameAndValue  = 0 ;
		sc.IncludeInSpreadsheet=0;
		sc.IncludeInStudySummary=0;
		sc.UsesMarketDepthData = 1;
		
		
		// Initialisations initiales lors du tout premier appel du DLL
		NiveauFuture = sc.High[sc.IndexOfLastVisibleBar-500] + 10 ;
		HoraireRAZ = 900 ;		// HoraireRAZ = sc.BaseDateTimeIn[sc.IndexOfLastVisibleBar-500].GetHour()*100 + sc.BaseDateTimeIn[sc.IndexOfLastVisibleBar-500].GetMinute()  ;   // format hhmm.ss  // 500 barres avant la dernière
		DateRAZ = sc.BaseDateTimeIn[sc.IndexOfLastVisibleBar].GetDate() ;  // DateRAZ = sc.BaseDateTimeIn[sc.IndexOfLastVisibleBar-500].GetDate() ;   // GetDate() returns the date part of the SCDateTime variable. The value returned is a Date Value. // Date values are integer (int) values representing the number of days since December 30, 1899.
		ConversionVersSCDateHoraire (DateRAZ, HoraireRAZ, SCDateHoraireRAZ) ;
		CompteTicksONOFF = 1 ;  // Le compte-ticks doit être ON pour gérer BasculeTP0
		AutoTradeStatus = 0 ; 
		PositionOuverte = 0 ;
		AutoTradeDemandeArret = 0 ; 
		AutoTradeFACAO = 0 ;  AutoTradeFACAO_MessageDansLog = 0 ;
		ParentOrderID_EchecGetOrder_MessageDansLog = 0 ;  Target1OrderID_EchecGetOrder_MessageDansLog = 0 ;  Stop1OrderID_EchecGetOrder_MessageDansLog = 0 ;
		OrdreOuvert = 0 ;
		AutoTradeStatus_Prior = 0 ;
		DateLocale = CalculDateLocale() ;     // DateLocale est un int (date value)
		TriggerResetScale = 0 ;
		RobotActif = 0 ;
		Void = 0 ;
		
		
		// Cas particulier de l'initialisation initiale : sc.IndexOfLastVisibleBar est compris entre 9h00 et 9h35 => on initialise à 9h00 pour favoriser l'initialisation des 4 studies AMR lors de la mise en place pré-séance.
		{
			float HoraireOfLastVisibleBar = sc.BaseDateTimeIn[sc.IndexOfLastVisibleBar].GetHour()*100 + sc.BaseDateTimeIn[sc.IndexOfLastVisibleBar].GetMinute() ;  // format hhmm.ss
			if ( HoraireOfLastVisibleBar > 900  and  HoraireOfLastVisibleBar < 935 )
			{
				HoraireRAZ = 900 ;
			}
		}
		
		
		// Création de CustomInputStrings pour le NiveauFuture
		SCString NiveauFutureInputString ;
		NiveauFutureInputString.Format("%.2f",NiveauFuture+TailleDuCustomInputDeNiveauFuture*0.25) ;  // la première valeur de la chaîne qui sera construite par concaténation (construite du niveau le plus élevé au plus bas)
		for (int index2 = TailleDuCustomInputDeNiveauFuture - 1 ; index2 > -TailleDuCustomInputDeNiveauFuture - 1 ; index2-- )  // concaténation des inputs
		{
			SCString StringAjoute ;
			StringAjoute.Format(";%.2f", NiveauFuture + index2*0.25) ;
			NiveauFutureInputString.Append(StringAjoute) ;
		}
		
		
		// Création de CustomInputStrings pour HoraireRAZ		
		SCString HoraireRAZInputString ;  CreationDeHoraireRAZInputString (SCDateHoraireRAZ, TailleDuCustomInputDeHoraireRAZ, HoraireRAZInputString) ;

		// Création de CustomInputStrings pour DateEtudiee		
		SCString DateEtudieeInputString ;  CreationDeDateEtudieeInputString (TailleDuCustomInputDeDateRAZ, DateEtudieeInputString) ;
		
		// SendOrdersToTradeService
		int AutoDetect_SendOrdersToTradeService = 0 ;  // par dédaut : LocalSim
		if ( WindowsVersionNumber() == 62 )  AutoDetect_SendOrdersToTradeService = 1 ;  // 62 = Windows Server 2012 R2 = VPS => Ext
		
		// Liste des inputs
		AffichageONOFF_Input.Name = "Affichage";
		AffichageONOFF_Input.SetCustomInputStrings("Affichage ON;Affichage OFF");   
		AffichageONOFF = 1 ;  AffichageONOFF_Input.SetCustomInputIndex(1-AffichageONOFF);    //    AffichageONOFF == 0  <=>  OFF  ;  AffichageONOFF == 1  <=>  Affichage ON     
		FutureOuIndice_Input.Name = "Instrument";
		FutureOuIndice_Input.SetCustomInputStrings("Trading INDICE;Trading FUTURE");   
		FutureOuIndice = 0 ;  FutureOuIndice_Input.SetCustomInputIndex(1-FutureOuIndice);    //     FutureOuIndice == 0  <=>  Future  ;  FutureOuIndice == 1  <=>  Indice 
		DateEtudiee.SetDate(DateRAZ) ;  // DateRAZ est un int (date value) 
		DateEtudieeRoulette.Name = "Date Etudiee";
		DateEtudieeRoulette.SetCustomInputStrings(DateEtudieeInputString) ;
		DateEtudieeRoulette.SetCustomInputIndex(DateLocale-DateRAZ);  // différent de 0 le WE     // note : il y a parfois un bug quand on ajoute une nouvelle instance de AMR, mais ce bug n'a pas lieu si on clique sur APPLY avant de modifier les settings
		// HoraireRAZTexte.Name = "Horaire RAZ (hhmm.ss)";
		HoraireRAZTexte.SetFloat(HoraireRAZ);	
		HoraireRAZRoulette.Name = "Horaire RAZ (hhmm.ss)";
		HoraireRAZRoulette.SetCustomInputStrings(HoraireRAZInputString) ;
		HoraireRAZRoulette.SetCustomInputIndex(TailleDuCustomInputDeHoraireRAZ);
		NiveauFutureTexte.Name = "Niveau Cible (sur future)";
		NiveauFutureTexte.SetFloat(NiveauFuture);
		NiveauFutureRoulette.Name = "Niveau Cible (roulette sur future)";
		NiveauFutureRoulette.SetCustomInputStrings(NiveauFutureInputString) ;
		NiveauFutureRoulette.SetCustomInputIndex(TailleDuCustomInputDeNiveauFuture);
		BonusIntrinseque_Exterieur_Input.Name = "Bonus Intrinseque (exterieur)";
		BonusIntrinseque_Exterieur_Input.SetFloat(56);        
		BonusIntrinseque_Roulette_Input.Name = "Bonus Intrinseque (roulette)";
		BonusIntrinseque_Roulette_Input.SetCustomInputStrings("BONUS Externe;BONUS 600;BONUS 550;BONUS 500;BONUS 450;BONUS 400;BONUS 350;BONUS 300;BONUS 250;BONUS 200;BONUS 150;BONUS 145;BONUS 140;BONUS 135;BONUS 130;BONUS 125;BONUS 120;BONUS 115;BONUS 110;BONUS 105;BONUS 100;BONUS 95;BONUS 90;BONUS 85;BONUS 80;BONUS 75;BONUS 70;BONUS 65;BONUS 60;BONUS 55;BONUS 52;BONUS 50;BONUS 48;BONUS 46;BONUS 44;BONUS 42;BONUS 40;BONUS 38;BONUS 36;BONUS 34;BONUS 32;BONUS 30;BONUS 29;BONUS 28;BONUS 27;BONUS 26;BONUS 25;BONUS 24;BONUS 23;BONUS 22;BONUS 21;BONUS 20;BONUS 19;BONUS 18;BONUS 17;BONUS 16;BONUS 15;BONUS 14;BONUS 13;BONUS 12;BONUS 11;BONUS 10") ;  
		BonusIntrinseque_Roulette_Input.SetCustomInputIndex(0);        // Bonus Externe
		TPNu_Manuel_Input.Name = "TPNu (en pas, 0 = mode Auto) " ;
		TPNu_Manuel_Input.SetFloat(0) ; 
		CompteTicksONOFFInput.SetCustomInputStrings("CT ON;CT OFF");
		CompteTicksONOFFInput.SetCustomInputIndex(0);
		NewDailyHighLow_Input.Name = "NewDailyHighLow_Input";  
		NewDailyHighLow_Input.SetYesNo(0);  
		AjustementChoisiEnPas_Input.Name = "Ajustement choisi du prix (en pas)";		// 1 pas de cotation = 0.25pt
		AjustementChoisiEnPas_Input.SetCustomInputStrings("Ajust +22 pas;Ajust +21 pas;Ajust +20 pas;Ajust +19 pas;Ajust +18 pas;Ajust +17 pas;Ajust +16 pas;Ajust +15 pas;Ajust +14 pas;Ajust +13 pas;Ajust +12 pas;Ajust +11 pas;Ajust +10 pas;Ajust +9 pas;Ajust +8 pas;Ajust +7 pas;Ajust +6 pas;Ajust +5 pas;Ajust +4 pas;Ajust +3 pas;Ajust +2 pas;Ajust +1 pas;Ajust 0 pas;Ajust -1 pas;Ajust -2 pas;Ajust -3 pas;Ajust -4 pas;Ajust -5 pas;Ajust -6 pas;Ajust -7 pas;Ajust -8 pas;Ajust -9 pas;Ajust -10 pas;Ajust -11 pas;Ajust -12 pas;Ajust -13 pas;Ajust -14 pas;Ajust -15 pas;Ajust -16 pas;Ajust -17 pas;Ajust -18 pas;Ajust -19 pas;Ajust -20 pas;Ajust -21 pas;Ajust -22 pas");
		AjustementChoisiEnPas_Input.SetCustomInputIndex(22);	
		NiveauIndiceTexte.Name = "Niveau Cible (sur indice)";
		NiveauIndiceTexte.SetFloat(NiveauIndice);
		// PeriodeNiveauTradeInput.Name = "Periode du PP/R/S trade";
		PeriodeNiveauTradeInput.SetCustomInputStrings("Future - Mensuel Quart;Future - Mensuel Full/Mid;Future - Hebdo;Future - Journalier;Future - UT 4h;Future - UT 1h;Future - Choisir periode");
		IndexPeriode = 0 ;  PeriodeNiveauTradeInput.SetCustomInputIndex(6-IndexPeriode) ; 
		// QuelNiveauTradeInput.Name = "Choix du PP/R/S trade";
		QuelNiveauTradeInput.SetCustomInputStrings("Non dispo");
		IndexQuelNiveau = 0 ; QuelNiveauTradeInput.SetCustomInputIndex(IndexQuelNiveau);  
		HoraireNews.Name = "Horaire News (hhmm.ss)";
		HoraireNews.SetFloat(0);	  // valeur initiale de 0 signifie qu'il n'y a pas d'horaire de news
		HoraireExpiration.Name = "Horaire Expiration (hhmm.ss)";
		HoraireExpiration.SetFloat(0);	  // valeur initiale de 0 signifie qu'il n'y a pas d'horaire d'expiration
		// CetteInstanceControleLesBoutons.Name = "CetteInstanceControleLesBoutons";
		CetteInstanceControleLesBoutons.SetYesNo( 0 );  // lors du lancement du DLL la study est inactive
		NomDuNiveauTrade.Name = "Nom du niveau trade";
		NomDuNiveauTrade.SetString("n.d.");
		// RobotPosition.Name = "RobotPosition";
		RobotPosition.SetInt(0);
		SendOrdersToTradeService.Name = "SendOrdersToTradeService (No=LocalSim / Yes=Ext)" ;
		SendOrdersToTradeService.SetCustomInputStrings("LocalSim;Ext");   
		SendOrdersToTradeService.SetCustomInputIndex( AutoDetect_SendOrdersToTradeService ); 
		SequenceONOFF_Input.Name = "Sequence";
		SequenceONOFF_Input.SetCustomInputStrings("Sequence ON;Sequence OFF");   
		SequenceONOFF_Input.SetCustomInputIndex(1);    //  Index = 1 correspond à Sequence = OFF  ; Index = 0 pour ON
		NatureNiveau_Input.Name = "NatureNiveau_Input" ;
		NatureNiveau_Input.SetInt(0) ;
		SousNatureNiveau_Input.Name = "Sous Nature Niveau Input" ;
		SousNatureNiveau_Input.SetInt(0) ;
		RangePrecedent_Input.Name = "Range Precedent (pts)" ;
		RangePrecedent_Input.SetFloat(0) ;
		External_Input_ACS_BUTTON_6.SetInt(0) ;   // "- 1 pas"
		External_Input_ACS_BUTTON_7.SetInt(0) ;    //  "+ 1 pas"
		External_Input_ACS_BUTTON_8.SetInt(0) ;    // "Horaire=RoundToHour(LastVisibleIndex-1min)"
		External_Input_ACS_BUTTON_9.SetInt(0) ;    // "Horaire=LastVisibleIndex-1min"
		External_Input_ACS_BUTTON_10.SetInt(0) ;
		External_Input_ACS_BUTTON_11.SetInt(0) ;   // "-1min"
		External_Input_ACS_BUTTON_12.SetInt(0) ;    //  "+1min"
		External_Input_ACS_BUTTON_15.SetInt(0) ;   // "bonus -"
		External_Input_ACS_BUTTON_16.SetInt(0) ;    //  "bonus +"
		External_Input_ACS_BUTTON_20.SetInt(0) ; 	// "récupération du bonus extérieur"
		External_Input_ACS_BUTTON_49.SetInt(0) ;    //  "AffichageONOFF"
		External_Input_ACS_BUTTON_52.SetInt(0) ;    	// "Autotrade"
		External_Input_ACS_BUTTON_53.SetInt(0) ;    	// "Refresh prix d'entrée"
		External_Input_ACS_BUTTON_60.SetInt(0) ;    //  "NEXT"
		External_Input_ACS_BUTTON_90.SetInt(0) ;		// "KILL"
		External_Input_ACS_BUTTON_109_110.SetInt(0) ;    //  "Niv -" ou "Niv +"
		External_Input_ACS_BUTTON_133_134.SetInt(0) ;    
		// MalusAjustementParabolique_Input.Name = "MalusAjustementParabolique_Input";
		MalusAjustementParabolique_Input.SetCustomInputStrings("Parabolique_NO;Parabolique_YES");   
		MalusAjustementParabolique_Input.SetCustomInputIndex(1);  
		MalusNRV_Manuel_Input.Name = "Malus NRV Manuel (0 = OFF)";
		MalusNRV_Manuel_Input.SetFloat(0);
		// ModelePourAUC_Input.Name = "Modele pour AUC et SeuilAUC";
		ModelePourAUC_Input.SetCustomInputStrings("MODEL 7;MODEL 9;MODEL 10;MODEL 11");
		ModelePourAUC_Input.SetCustomInputIndex(2);
		// NewDailyHighLow_InputManuel.Name = "NewDailyHighLow_InputManuel";
		NewDailyHighLow_InputManuel.SetCustomInputStrings("Sans malus NewDHiLo;Avec malus NewDHiLo");
		NewDailyHighLow_InputManuel.SetCustomInputIndex(1);
		

		// DisplayOrder des inputs
		BonusIntrinseque_Exterieur_Input.DisplayOrder = 1 ;
		BonusIntrinseque_Roulette_Input.DisplayOrder = 2 ;
		DateEtudieeRoulette.DisplayOrder = 3 ;
		HoraireRAZRoulette.DisplayOrder = 4 ;
		HoraireRAZTexte.DisplayOrder = 5 ;
		FutureOuIndice_Input.DisplayOrder = 6 ;
		NomDuNiveauTrade.DisplayOrder = 7 ;
		NatureNiveau_Input.DisplayOrder = 8;
		SousNatureNiveau_Input.DisplayOrder = 9 ;
		RangePrecedent_Input.DisplayOrder = 10 ;
		AjustementChoisiEnPas_Input.DisplayOrder = 11 ;
		AffichageONOFF_Input.DisplayOrder = 12 ;
		SequenceONOFF_Input.DisplayOrder = 13 ;
		
		
		// Paramètres des SCSubgraphRef
		for (int SubgraphIndex = 1 ; SubgraphIndex <= 13 ; SubgraphIndex++)
		{
			sc.Subgraph[SubgraphIndex].DrawStyle = DRAWSTYLE_LINE ;
			sc.Subgraph[SubgraphIndex].LineStyle = LINESTYLE_SOLID ;
			//sc.Subgraph[SubgraphIndex].LineLabel = LL_DISPLAY_VALUE | LL_VALUE_ALIGN_CENTER | LL_VALUE_ALIGN_VALUES_SCALE;  // n'affiche que la valeur
			sc.Subgraph[SubgraphIndex].DisplayNameValueInWindowsFlags = false ;
			sc.Subgraph[SubgraphIndex].DrawZeros = false ;
			sc.Subgraph[SubgraphIndex].LineWidth = 2;
		}	
		Background.Name = "Background"; 
		Background.DrawStyle = DRAWSTYLE_BACKGROUND; 
		Background.DisplayNameValueInWindowsFlags = false ;	
		RatioAUCProjete.Name = "AUC/Seuil Projete" ;
		RatioAUCProjete.PrimaryColor = RGB(120,120,255) ;		
		RatioSigmaProjete.Name = "Sigma/Seuil Projete" ;
		RatioSigmaProjete.PrimaryColor = RGB(255,181,106) ;		  // orange
		// SommeRatioProjete.Name = "(AUC+Sigma)/Seuil Projete" ;		
		// SommeRatioProjete.PrimaryColor = RGB(255,0,0) ;		
		// SommeRatioProjete.DrawStyle = DRAWSTYLE_IGNORE ;
		SommeRatioAvecMalusProjete.Name = "(AUC+Sigma)/Seuil*(1+Malus/100) Projete" ;		
		SommeRatioAvecMalusProjete.PrimaryColor = RGB(255,0,0) ;		
		SommeRatioAvecMalusProjete.LineWidth = 2;
		MalusExcedentaireProjete.Name = "MalusExcedentaire Projete" ;		
		MalusExcedentaireProjete.PrimaryColor = RGB(200,0,0) ;		
		MalusExcedentaireProjete.LineWidth = 1;
		StopLossProjete.DrawStyle = DRAWSTYLE_IGNORE ;
		Niveau1TP.DrawStyle = DRAWSTYLE_IGNORE ;
		Niveau2TP.DrawStyle = DRAWSTYLE_IGNORE ;
		Niveau3TP.DrawStyle = DRAWSTYLE_IGNORE ;
		Niveau4TP.DrawStyle = DRAWSTYLE_IGNORE ;
		BasculeTP0.Name = "Bascule TP0" ;
		BasculeTP0.PrimaryColor = RGB(0,0,0) ;  
		BasculeTP0.LineWidth = 2 ;
		NiveauIndiceSurFutureTempsReel.Name = "NiveauIndiceSurFutureTempsReel" ; 
		NiveauIndiceSurFutureTempsReel.LineWidth = 2 ;
		NiveauIndiceSurFutureTempsReel.PrimaryColor = RGB(0,180,0) ;  	
		NiveauTPOptimumArrondi.Name = "Niveau cible +- TPOptimumArrondi" ;
		NiveauTPOptimumArrondi.LineWidth = 1;
		NiveauTPOptimumArrondi.PrimaryColor = RGB(180,180,180) ;  
		
		
		sc.ResetAllScales = 1 ; 
		
		// Cette study devient la study active
		LaStudyVientDEtreRajoutee = 1 ;
		
	
		return ;	
		
	} // fin sc.Defaults
	
	
	
	if ( sc.UpdateStartIndex == 0  or  TriggerRecalculate == 1)
	{
		
		// Paramètres du robot de trading   // à remettre ultérieuremen dans le bloc sc.Defaults
		{
			sc.SendOrdersToTradeService = SendOrdersToTradeService.GetIndex() ;      // true pour router vers le broker, et false pour trader en simulation locale
			sc.MaximumPositionAllowed = 1 ;  // si cette variable vaut 1, on peut envoyer 1 ordre achat ET 1 ordre vente    // MaximumPositionAllowed controls the maximum Trade Position Quantity allowed when performing automated trading for a particular Symbol and Trade Account. An order that would increase the Trade Position for the Symbol and Trade Account to a quantity greater than the specified Maximum Position Allowed will be ignored/rejected.
			sc.AllowMultipleEntriesInSameDirection = true ;    // If you want to allow Multiple Entries in the Same Direction, set this variable to TRUE (Yes).
			sc.SupportReversals = true ;    // A reversal is triggered when a Buy Entry or Sell Entry order is submitted and an actual opposite Trade Position currently exists for the symbol.
			sc.AllowOppositeEntryWithOpposingPositionOrOrders = true ;  // When this variable is set to FALSE (No), then a Buy Entry or Sell Entry is not allowed if there is a Trade Position or working orders already existing in the opposite direction. Otherwise, the signal will be allowed.
			sc.CancelAllOrdersOnEntriesAndReversals= false ;    // annulation de tous les ordres en attente sur le graphe ; non mis car inutile a priori
			sc.CancelAllOrdersOnReversals = false ;
			sc.AllowEntryWithWorkingOrders = true ;    // When AllowEntryWithWorkingOrders is set to TRUE (Yes), then a Buy Entry or a Sell Entry will be allowed if there are existing working orders of any type.
			sc.AllowOnlyOneTradePerBar = false ;  
			sc.MaintainTradeStatisticsAndTradesData = true ; 
			sc.SupportAttachedOrdersForTrading = false ;      // When sc.SupportAttachedOrdersForTrading is set to TRUE(1), then the Attached Orders set on the Trade Window for the chart the trading study is applied to will be used no matter what the Use Attached Orders setting is set to on the Trade Window. However, this setting is irrelevant when Targets or Stops are set on the s_SCNewOrder data structure. In this case it is implied to be on for those Attached Orders.
			sc.UseGUIAttachedOrderSetting = false ;
			sc.CancelAllWorkingOrdersOnExit = true ;    // pas compris   // je l'ai basculé de false à true le 23/11/2019 en espérant que je n'aurai plus les messages d'erreur liés à des ordres non annulables par IB
		}
		
		
		// Forcer sc.GraphRegion
		sc.GraphRegion = 1 ;  		
		
		
		// Récupération des studyID
		{
			// Récupération de la studyID pour le SpreadIndex
			StudyID_SI = sc.GetStudyIDByName(sc.ChartNumber, "SpreadIndexAffichage", 0) ;
			if  (StudyID_SI == 0 )
			{ SCString message ;  message.Format("ID:%d | !! ERREUR !! Le chart ne contient pas la study ''SpreadIndexAffichage''", ThisStudyID) ; sc.AddMessageToLog(message,1) ; }
			
			// Récupération de la StudyID pour le TPTicks
			StudyIDTPNu = sc.GetStudyIDByName(sc.ChartNumber, "Calcul TPNu", 0) ; 
			if  (StudyIDTPNu == 0 )
			{ SCString message ;  message.Format("ID:%d | !! ERREUR !! Le chart ne contient pas la study ''Calcul TPNu''", ThisStudyID) ; sc.AddMessageToLog(message,1) ; }
		
			// Récupération de la studyID pour le Sigma
			StudyIDSigma = sc.GetStudyIDByName(sc.ChartNumber, "Sigma", 0) ;
			if  (StudyIDSigma == 0 )
			{ SCString message ;  message.Format("ID:%d | !! ERREUR !! Le chart ne contient pas la study ''Sigma''", ThisStudyID) ; sc.AddMessageToLog(message,1) ; }
		
			// Récupération de la studyID pour le INQ100SansCorrection
			StudyIDOverlayINQ100 = sc.GetStudyIDByName(sc.ChartNumber, "Overlay INQ100 sur le future (avec correction du spread)", 0) ;
			if  (StudyIDOverlayINQ100 == 0 )
			{ SCString message ;  message.Format("ID:%d | !! ERREUR !! Le chart ne contient pas la study ''Overlay INQ100 sur le future (avec correction du spread)''", ThisStudyID) ; sc.AddMessageToLog(message,1) ; }
		
			// Récupération de la studyID pour Prix Typique
			StudyID_PrixTypique = sc.GetStudyIDByName(sc.ChartNumber, "Prix typique", 0);
			if  ( StudyID_PrixTypique == 0 )
			{ SCString message ;  message.Format("ID:%d | !! ERREUR !! Le chart ne contient pas la study ''Prix typique''", sc.StudyGraphInstanceID) ; sc.AddMessageToLog(message,1) ; }
		
			// Récupération de la studyID pour Navigation niveaux
			StudyID_NavigNiveaux = sc.GetStudyIDByName(sc.ChartNumber, "Navigation niveaux", 0);
			if  ( StudyID_NavigNiveaux == 0 )
			{ SCString message ;  message.Format("ID:%d | !! ERREUR !! Le chart ne contient pas la study ''Navigation niveaux''", sc.StudyGraphInstanceID) ; sc.AddMessageToLog(message,1) ; }
		
			// Récupération des studyID pour les PHPBDyn du future et de l'indice
			if ( FutureOuIndice == 0 )
			{	
				StudyID_PHPBDyn_Fut = sc.GetStudyIDByName(sc.ChartNumber, "PH PB dynamiques du future", 0) ; 
				if ( StudyID_PHPBDyn_Fut == 0 )  
				{ SCString message ;  message.Format("ID:%d | !! ERREUR !! Le chart ne contient pas la study ''PH PB dynamiques du future''", ThisStudyID) ;  sc.AddMessageToLog(message, 1) ;  }
			}
			else if ( FutureOuIndice == 1 )
			{
				StudyID_PHPBDyn_Ind = sc.GetStudyIDByName(15, "PH PB dynamiques de l'indice", 0) ;  
				if ( StudyID_PHPBDyn_Ind == 0 )  
				{ SCString message ;  message.Format("ID:%d | !! ERREUR !! Le chart #15 ne contient pas la study ''PH PB dynamiques de l'indice''", ThisStudyID) ;  sc.AddMessageToLog(message, 1) ;  }
			}		
			
			// Récupération de la StudyID pour le VIX
			StudyID_VIX = std::max ( sc.GetStudyIDByName(sc.ChartNumber, "Symbol: $vix", 0), sc.GetStudyIDByName(sc.ChartNumber, "Symbol: $VIX", 0) ) ;
			if ( StudyID_VIX == 0 )   { SCString message ;  message.Format("!! ERREUR !! Le chart #%d ne contient pas la study ''Symbol: $vix''", sc.ChartNumber) ;  sc.AddMessageToLog(message, 1) ;  }

			// Récupération de la studyID pour la distance à PHJPBJ SP500
			StudyID_DistancePHJPBJ_SP500 = sc.GetStudyIDByName(sc.ChartNumber, "Distance aux PH/PB dynamiques (Indice+Future SP500) pour afficher sur le FNQ100", 0) ;
			if  (StudyID_DistancePHJPBJ_SP500 == 0 )
			{ SCString message ;  message.Format("ID:%d | !! ERREUR !! Le chart ne contient pas la study ''Distance aux PH/PB dynamiques (Indice+Future SP500) pour afficher sur le FNQ100''", ThisStudyID) ; sc.AddMessageToLog(message,1) ; }
		
			// Récupération de la StudyID_Padding_Chart15 sur le graphe indice UT20s
			StudyID_Padding_Chart15 = sc.GetStudyIDByName(15, "Padding et boutons ACS", 0) ;
			if ( StudyID_Padding_Chart15 == 0 ) 
			{ SCString message ;  message.Format("!! ERREUR !! La study ''Padding et boutons ACS'' n'a pas ete trouvee sur le chart #15") ; sc.AddMessageToLog(message,1) ; }
			
			// Récupération de la StudyID_Padding_Chart19 sur le graphe indice UT4min
			StudyID_Padding_Chart19 = sc.GetStudyIDByName(19, "Padding et boutons ACS", 0) ;
			if ( StudyID_Padding_Chart19 == 0 ) 
			{ SCString message ;  message.Format("!! ERREUR !! La study ''Padding et boutons ACS'' n'a pas ete trouvee sur le chart #19") ; sc.AddMessageToLog(message,1) ; }
		
			// Récupération de la StudyID_QuantiemeMvt_Chart15 sur le graphe indice UT20s
			StudyID_QuantiemeMvt_Chart15 = sc.GetStudyIDByName(15, "Quantieme Mvt", 0) ;
			if ( StudyID_QuantiemeMvt_Chart15 == 0 ) 
			{ SCString message ;  message.Format("!! ERREUR !! La study ''Quantieme Mvt'' n'a pas ete trouvee sur le chart #15") ; sc.AddMessageToLog(message,1) ; }

			// Récupération de la StudyID_QuantiemeMvt_Chart19 sur le graphe indice UT4min
			StudyID_QuantiemeMvt_Chart19 = sc.GetStudyIDByName(19, "Quantieme Mvt", 0) ;
			if ( StudyID_QuantiemeMvt_Chart19 == 0 ) 
			{ SCString message ;  message.Format("!! ERREUR !! La study ''Quantieme Mvt'' n'a pas ete trouvee sur le chart #19") ; sc.AddMessageToLog(message,1) ; }
		}
		
		
		// Si cette study vient d'être rajoutée, ou si on cliqué CTRL+5
		if (LaStudyVientDEtreRajoutee != 0)
		{
			// La study devient la study active. Si d'autres ont été rajoutées simultanément, c'est la dernière ajoutée qui deviendra active.
			sc.SetChartStudyInputInt(sc.ChartNumber, StudyID_PrixTypique, 1, ThisStudyID) ;     	// Ecriture dans l'input[1] de la study "Prix typique"			// int SetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int IntegerValue);
			
			// Détermination du spread Future-Indice 
			sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDOverlayINQ100, 0, 1, SpreadMoyen);    // // on récupère les valeurs de SpreadMoyen dans le Subgraph[0].Arrays[1] de "Overlay INQ100 sur le future (avec correction du spread)" sur le présent Chart 		// spread moyen non arrondi

			// Sélection de Trading indice
			FutureOuIndice = 1 ;  FutureOuIndice_Input.SetCustomInputIndex(1-FutureOuIndice);    //     FutureOuIndice == 0  <=>  Future  ;  FutureOuIndice == 1  <=>  Indice 

			if (LaStudyVientDEtreRajoutee == 1)		// La stydy vient d'être rajoutée
			{
				// Calcul du NiveauIndice 500 barres correspondant à HoraireRAZ
				float NiveauInd = round( sc.Close[sc.IndexOfLastVisibleBar] - SpreadMoyen[sc.IndexOfLastVisibleBar] ); 
				
				// Quantième occurence de AMR
				int OccurenceDeAMR = QuantiemeOccurenceDeAMR (ThisStudyID, ptrsc) ;				// int QuantiemeOccurenceDeAMR (int StudyID_CurrentStudy, s_sc* ptrsc)  

				// Modification de l'input "Niveau Cible (sur indice)"
							if ( OccurenceDeAMR == 1 )  { NiveauIndiceTexte.SetFloat(NiveauInd+20) ;  }
				else 	if ( OccurenceDeAMR == 2 )  { NiveauIndiceTexte.SetFloat(NiveauInd-20) ;  }
				else 	if ( OccurenceDeAMR == 3 )  { NiveauIndiceTexte.SetFloat(NiveauInd+30) ;  }
				else 	if ( OccurenceDeAMR == 4 )  { NiveauIndiceTexte.SetFloat(NiveauInd-30) ;  }
				else 	if ( OccurenceDeAMR == 5 )  { NiveauIndiceTexte.SetFloat(NiveauInd+40) ;  }
				else 	if ( OccurenceDeAMR == 6 )  { NiveauIndiceTexte.SetFloat(NiveauInd-40) ;  }
				else 	if ( OccurenceDeAMR == 7 )  { NiveauIndiceTexte.SetFloat(NiveauInd+50) ;  }
				else 	if ( OccurenceDeAMR == 8 )  { NiveauIndiceTexte.SetFloat(NiveauInd-50) ;  }
			}
			else if (LaStudyVientDEtreRajoutee == 2)		// On vient de cliquer sur CTRL+5 ( LaStudyVientDEtreRajoutee a été mis à 2 au précédent appel du DLL )
			{
				int IndexLastVisibleBar_Chart6 ;
				if ( External_Input_ACS_BUTTON_133_134.GetInt() != 0 )
				{ 
					IndexLastVisibleBar_Chart6 = External_Input_ACS_BUTTON_133_134.GetInt() ;  
					External_Input_ACS_BUTTON_133_134.SetInt(0) ;    // on met à 0 External_Input_ACS_BUTTON_133_134
				}
				else  { IndexLastVisibleBar_Chart6 = sc.IndexOfLastVisibleBar ;  }

				// Calcul du NiveauIndice à HoraireRAZ 
				float NiveauInd = round( sc.Close[IndexLastVisibleBar_Chart6] - SpreadMoyen[IndexLastVisibleBar_Chart6] ); 
				
				NiveauIndiceTexte.SetFloat(NiveauInd) ;
			}
			
			
			LaStudyVientDEtreRajoutee = 0 ;  // RAZ du déclencheur
			
			// sc.RecalculateChart(sc.ChartNumber) ; 			// The recalculation happens on a delay. It happens the next time the specified chart performs an update. 
			TriggerRecalculate = 1 ;
			return ;
		}
		
		
		// Indicateur de recalcul complet
		RecalculComplet = 1 ;
		
		
		// Rejet de certaines modifications d'inputs si le robot est actif
		{
			if ( RobotActif )
			{
				// Modification de l'instrument
				if ( FutureOuIndice == FutureOuIndice_Input.GetIndex() )    
				{ 
					FutureOuIndice_Input.SetCustomInputIndex(1-FutureOuIndice) ;    // réinjection de la valeur correcte
					SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | !! WARNING !! L'input ''Instrument'' a ete modifie alors que le robot est actif (le robot doit etre arrete pour que cette modification soit operante)") ;  sc.AddMessageToLog(message,1) ;
				}
				
				// Modification de Niveau cible sur future (mode texte)
				if ( NiveauFuture != NiveauFutureTexte.GetFloat() )    
				{ 
					NiveauFutureTexte.SetFloat(NiveauFuture) ;    // réinjection de la valeur correcte
					SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | !! WARNING !! L'input ''Niveau cible sur future (texte)'' a ete modifie alors que le robot est actif (le robot doit etre arrete pour que cette modification soit operante)") ;  sc.AddMessageToLog(message,1) ;
				}
				
				// Modification de Niveau cible sur future (mode roulette)
				if ( NiveauFutureRoulette.GetIndex() !=  TailleDuCustomInputDeNiveauFuture  )    
				{ 
					NiveauFutureRoulette.SetCustomInputIndex(TailleDuCustomInputDeNiveauFuture);   // réinjection de la valeur correcte
					SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | !! WARNING !! L'input ''Niveau cible sur future (roulette)'' a ete modifie alors que le robot est actif (le robot doit etre arrete pour que cette modification soit operante)") ;  sc.AddMessageToLog(message,1) ;
				}
				
				// Modification de Niveau cible sur indice
				if ( NiveauIndice != NiveauIndiceTexte.GetFloat() )    
				{ 
					NiveauIndiceTexte.SetFloat(NiveauIndice) ;   // réinjection de la valeur correcte
					SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | !! WARNING !! L'input ''Niveau cible sur indice'' a ete modifie alors que le robot est actif (le robot doit etre arrete pour que cette modification soit operante)") ;  sc.AddMessageToLog(message,1) ;
				}
				
				// Modification de la période du PP/R/S tradé
				if ( IndexPeriode != 6-PeriodeNiveauTradeInput.GetIndex() )    
				{ 
					PeriodeNiveauTradeInput.SetCustomInputIndex(6-IndexPeriode) ;    // réinjection de la valeur correcte
					SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | !! WARNING !! L'input ''Periode du PP/R/S trade'' a ete modifie alors que le robot est actif (le robot doit etre arrete pour que cette modification soit operante)") ;  sc.AddMessageToLog(message,1) ;
				}
				
				// Modification du choix du PP/R/S tradé
				if ( IndexQuelNiveau != QuelNiveauTradeInput.GetIndex() )    
				{ 
					QuelNiveauTradeInput.SetCustomInputIndex(IndexQuelNiveau);      // réinjection de la valeur correcte
					SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | !! WARNING !! L'input ''Choix du PP/R/S trade'' a ete modifie alors que le robot est actif (le robot doit etre arrete pour que cette modification soit operante)") ;  sc.AddMessageToLog(message,1) ;
				}
				
				// Modification de Ajustement du prix
				if ( AjustementChoisiEnPas != 22 - AjustementChoisiEnPas_Input.GetIndex() )    
				{ 
					AjustementChoisiEnPas_Input.SetCustomInputIndex(22-AjustementChoisiEnPas) ;    // réinjection de la valeur correcte
					SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | !! WARNING !! L'input ''Ajustement du prix'' a ete modifie alors que le robot est actif (le robot doit etre arrete pour que cette modification soit operante)") ;  sc.AddMessageToLog(message,1) ;
				}
				
				// Affichage basculé vers OFF
				if ( AffichageONOFF_Input.GetIndex() == 1  and  AutoTradeStatus != 11 )  //  AutoTradeStatus == 11 permet de mettre l'affichage OFF
				{ 
					AffichageONOFF_Input.SetCustomInputIndex(0) ;    // réinjection de la valeur correcte
					SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | !! WARNING !! L'input ''Affichage'' a ete bascule alors que le robot est actif (le robot doit etre arrete pour que cette modification soit operante)") ;  sc.AddMessageToLog(message,1) ;
				}
			}
		}
		
		
		// Gestion des variables dépendant du statut actif/inactif de la study
		{	
			if ( GetActiveStudyID(ptrsc) == ThisStudyID )   // l'instance est la study active
			{ 	
				CetteInstanceControleLesBoutons.SetYesNo(1) ;
				sc.DisplayStudyName = 1;
				sc.DisplayStudyInputValues = 1 ;  
			}
			else
			{ 	
				CetteInstanceControleLesBoutons.SetYesNo(0) ;  
				sc.DisplayStudyName = 0;  
				sc.DisplayStudyInputValues = 0 ; 
			}
		}			
		
		
		// Si la study n'est pas la study active et qu'elle vient de basculer son affichage de OFF vers ON via les inputs => elle devient la study active (bleue)
		{
			if ( CetteInstanceControleLesBoutons.GetYesNo() == 0 )
			{
				if ( ( AffichageONOFF == 0 )  and  ( AffichageONOFF_Input.GetIndex() == 0 ) )  // la study vient de basculer son affichage de OFF vers ON via les inputs
				{
					AffichageONOFF = 1 ;   // pour éviter une récursion
					Void = 0 ;
					sc.HideStudy=0 ;
					
					int Result = sc.SetChartStudyInputInt(sc.ChartNumber, StudyID_PrixTypique, 1, ThisStudyID) ;   // int SetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int IntegerValue);   // returns 1 if successful
					if (Result == 1 )
					{	
						sc.RecalculateChart(sc.ChartNumber) ;   // un TriggerRecalculate n'est pas suffisant, car il faut également recalculer les autres instances de AMR        // note : on pourrait se contenter d'un TriggerRecalculate car il suffit d'indiquer à l'ancienne instance active qu'elle n'est plus l'instance active ; la nouvelle instance active est suffisante pour faire la MàJ des boutons
						sc.UpdateAlways = 1 ; return ;  // sort de la fonction scsf_AnnulationMiniRebond  // le calcul aura lieu grace au sc.RecalculateChart
					}
					else {  SCString message ;   message.Format("ID:%d | Il y a eu un probleme avec SetChartStudyInputInt (modification de StudyAMRActive_ID_Input)", ThisStudyID) ;    sc.AddMessageToLog(message,1) ; }
				}
			}
		}
		
		
		// Zeroing subgraphs
		{
			// il faut RAZ le contenu des subgraphs avant de les recalculer
			for (int SubgraphIndex = 0; SubgraphIndex <= 13; SubgraphIndex++)
			{ for (int index = 0; index < sc.ArraySize; index++)  { sc.Subgraph[SubgraphIndex][index] = 0 ; }  }
			
			// Zeroing tools
			sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ;   // all non user drawn Chart Drawings on the specified ChartNumber will be deleted. The LineNumber parameter in this case is ignored and should be 0. 		
			
			// on efface les marqueurs UDD sur les graphes internes et externes
			EffacementDesUDD(ptrsc) ; 
			sc.DeleteUserDrawnACSDrawing(sc.ChartNumber, 10014+10000*ThisStudyID) ;  	sc.DeleteUserDrawnACSDrawing(sc.ChartNumber, 10015+10000*ThisStudyID) ;  // effacement des segments pointillés sur le niveau tradé et le prix demandé
			EffacementDesUDD_GraphesExternes(ptrsc);
		}

		
		// détermination du XTicks // besoin d'être nécessairement en 16-ticks
		{
			n_ACSIL::s_BarPeriod BarPeriod;
			sc.GetBarPeriodParameters(BarPeriod);
			if ( BarPeriod.IntradayChartBarPeriodType != IBPT_NUM_TRADES_PER_BAR   or   BarPeriod.IntradayChartBarPeriodParameter1 != 16  )  
			{  SCString message ;   message.Format("ID:%d | !! ERREUR !! Le graphique doit etre en 16-ticks pour tracer l'indicateur", ThisStudyID) ;    sc.AddMessageToLog(message,1) ; }
		}
		
		
		// Initialisation des variables persistantes de la study
		{			
			CompteTicksONOFF = 1 - CompteTicksONOFFInput.GetIndex() ;  
			AjustementChoisiEnPas = 22 - AjustementChoisiEnPas_Input.GetIndex() ;	  // exprimé en tick
			TriggerRAZ = 1 ;
			IndexPremiereTouche = 100000000 ;
			TPOptimumArrondi = -2 ;
			Increment = 0 ;
			AffichageONOFF = 1 - AffichageONOFF_Input.GetIndex() ; 
			sc.HideStudy=0;
			IndexArretManualLooping = 0 ;
			NombreCyclesConsecutifsTropLong = -1 ;
			delta = 0.25 ;
			Texte.Format("") ; Texte_x128 = Texte ;  Texte_x64 = Texte ;  Texte_x32 = Texte ;  Texte_x16 = Texte ;  Texte_x8 = Texte ;  Texte_x4 = Texte ;  Texte_x2 = Texte ;  Texte_x1 = Texte ;  Texte_x05 = Texte ;  Texte_x025 = Texte ;  Texte_Recapitulatif = Texte ;
			MultiplicateurPrix = sc.Close[sc.IndexOfLastVisibleBar]/6500 ;
		}	
		
		// BonusIntrinseque
		{
			if ( BonusIntrinseque_Roulette_Input.GetIndex() == 0 )		// 0 signifie "Bonus Extérieur"
			{
				BonusIntrinseque = BonusIntrinseque_Exterieur_Input.GetFloat() ;
			}
			else		// Bonus Roulette
			{
				BonusIntrinseque = ConversionDuInputBonusIntrinsequeRoulette(BonusIntrinseque_Roulette_Input.GetIndex()) ;
			}
			
			// lorsque BonusIntrinseque est < 10, on ne trace pas les courbe sur le graphe
			if ( BonusIntrinseque < 10 )
			{
				RatioAUCProjete.DrawStyle = DRAWSTYLE_IGNORE ;
				RatioSigmaProjete.DrawStyle = DRAWSTYLE_IGNORE ;
				SommeRatioAvecMalusProjete.DrawStyle = DRAWSTYLE_IGNORE ;
				MalusExcedentaireProjete.DrawStyle = DRAWSTYLE_IGNORE ;
				BasculeTP0.DrawStyle = DRAWSTYLE_IGNORE ;
				NiveauTPOptimumArrondi.DrawStyle = DRAWSTYLE_IGNORE ;
			}
			else
			{
				RatioAUCProjete.DrawStyle = DRAWSTYLE_LINE ;
				RatioSigmaProjete.DrawStyle = DRAWSTYLE_LINE ;
				SommeRatioAvecMalusProjete.DrawStyle = DRAWSTYLE_LINE ;
				MalusExcedentaireProjete.DrawStyle = DRAWSTYLE_LINE ;
				BasculeTP0.DrawStyle = DRAWSTYLE_LINE ;
				NiveauTPOptimumArrondi.DrawStyle = DRAWSTYLE_LINE ;
			}
		}
		
		// Résolution de l'écran => Config_tailleTexte
		int HauteurEnPixels = sc.StudyRegionBottomCoordinate - sc.StudyRegionTopCoordinate ;		// The top left pixel coordinate of the chart is at 0, 0. These coordinates increase moving towards the bottom and right.
		if ( HauteurEnPixels > 850 )		// 1000 est le nb de pixels dans la config standard sur écran QHD, 700 sur config Full HD		
		{ Config_tailleTexte = 1 ;  Position_1ere_Colonne = 35 ;  Position_2eme_Colonne = 60 ;  Position_3eme_Colonne = 85 ;	 }		// Config QHD
		else 
		{ Config_tailleTexte = 2 ;  Position_1ere_Colonne = 10 ;  Position_2eme_Colonne = 40 ;  Position_3eme_Colonne = 80 ;	 }		// Config Full HD 
	
		// Nb de moniteurs du bureau Windows
		NbMonitors = NumberOfMonitors() ;
	
		
		// On efface les UDD et on RAZ l'ajustement de Niveau Cible 
		if ( AffichageONOFF == 0)   
		{
			sc.HideStudy=1;
			EffacementDesUDD(ptrsc) ;
			if  (AutoTradeStatus != 11 )  AjustementChoisiEnPas_Input.SetCustomInputIndex(22) ;      
		} 

	
		// Récupération de FutureOuIndice // Réinitialisations de PeriodeNiveauTradeInput et QuelNiveauTradeInput
		if ( FutureOuIndice != 1 - FutureOuIndice_Input.GetIndex() )  // on teste si FutureOuIndice a été modifié    //     FutureOuIndice == 0  <=>  Future  ;  FutureOuIndice == 1  <=>  Indice 
		{
			FutureOuIndice = 1 - FutureOuIndice_Input.GetIndex() ;    
			QuelNiveauTradeInput.SetCustomInputStrings("Non dispo") ;  QuelNiveauTradeInput.SetCustomInputIndex(0);
			
			if (FutureOuIndice == 0 )  // trading future
			{	
				PeriodeNiveauTradeInput.SetCustomInputStrings("Future - Mensuel Quart;Future - Mensuel Full/Mid;Future - Hebdo;Future - Journalier;Future - UT 4h;Future - UT 1h;Future - Choisir periode");  PeriodeNiveauTradeInput.SetCustomInputIndex(6);  
				NiveauIndice = 0 ; 
			} 
			else if ( FutureOuIndice == 1 )   // trading indice
			{	
				PeriodeNiveauTradeInput.SetCustomInputStrings("Indice - Mensuel Eighth;Indice - Mensuel Quart;Indice - Mensuel Full/Mid;Indice - Hebdo Quart;Indice - Hebdo Full/Mid;Indice - Journalier;Indice - Choisir periode");  PeriodeNiveauTradeInput.SetCustomInputIndex(6);
			} 
		}
		
		
		// Récupération de IndexPeriode
		{
			if (FutureOuIndice == 0 )  // Future
			{
				if (IndexPeriode != 6-PeriodeNiveauTradeInput.GetIndex() )  // changement de IndexPeriode
				{
					RAZHoraireExpiration(ptrsc) ;
					IndexPeriode = 6-PeriodeNiveauTradeInput.GetIndex() ;  
					if ( IndexPeriode == 1 )  // 1h
					{	QuelNiveauTradeInput.SetCustomInputStrings("Future - Choix niv 1h;R4 1h;R3 1h;R2 1h;R1 1h;PP 1h;S1 1h;S2 1h;S3 1h;S4 1h;Future - Choix niv 1h") ;  QuelNiveauTradeInput.SetCustomInputIndex(10) ; }
					else if ( IndexPeriode == 2 )  // 4h
					{	QuelNiveauTradeInput.SetCustomInputStrings("Future - Choix niv 4h;R4 4h;R3 4h;R2 4h;R1 4h;PP 4h;S1 4h;S2 4h;S3 4h;S4 4h;Future - Choix niv 4h") ;  QuelNiveauTradeInput.SetCustomInputIndex(10) ; }
					else if ( IndexPeriode == 3 )  // Jour
					{	QuelNiveauTradeInput.SetCustomInputStrings("Future - Choix niv Journalier;R4 J;  mR4 J;R3 J;  mR3 J;R2 J;  mR2 J;R1 J;  mR1 J;PP J ******;  mS1 J;S1 J;  mS2 J;S2 J;  mS3 J;S3 J;  mS4 J;S4 J;PH JP;PB JP;Settlement JP; VWAP J;Future - Choix niv Journalier") ;   QuelNiveauTradeInput.SetCustomInputIndex(22) ;  }
					else if ( IndexPeriode == 4 )  // Hebdo
					{	QuelNiveauTradeInput.SetCustomInputStrings("Future - Choix niv Hebdo;R4 H;  mR4 H;R3 H;  mR3 H;R2 H;  mR2 H;R1 H;  mR1 H;PP H ******;  mS1 H;S1 H;  mS2 H;S2 H;  mS3 H;S3 H;  mS4 H;S4 H;PH HP;PB HP;VWAP H;Future - Choix niv Hebdo") ;  QuelNiveauTradeInput.SetCustomInputIndex(21) ; }
					else if ( IndexPeriode == 5 )  // Mensuel Full/Mid
					{	QuelNiveauTradeInput.SetCustomInputStrings("Future - Choix niv M;R4 M;  mR4 M;R3 M;  mR3 M;R2 M;  mR2 M;R1 M;  mR1 M;PP M ******;  mS1 M;S1 M;  mS2 M;S2 M;  mS3 M;S3 M;  mS4 M;S4 M;PH MP;PB MP;VWAP M;Future - Choix niv M") ;  QuelNiveauTradeInput.SetCustomInputIndex(21) ;  }
					else if ( IndexPeriode == 6 )  // Mensuel Quart
					{	QuelNiveauTradeInput.SetCustomInputStrings("Future - Choix niv M Quart;RQ8 M;RQ7 M;RQ6 M;RQ5 M;RQ4 M;RQ3 M;RQ2 M;RQ1 M;SQ1 M;SQ2 M;SQ3 M;SQ4 M;SQ5 M;SQ6 M;SQ7 M;SQ8 M;Future - Choix niv M Quart") ;  QuelNiveauTradeInput.SetCustomInputIndex(17) ;  }
				}
			}			
			
			else if ( FutureOuIndice == 1 )   // Indice
			{	
				if (IndexPeriode != 6-PeriodeNiveauTradeInput.GetIndex() )  // changement de IndexPeriode
				{
					RAZHoraireExpiration(ptrsc) ;
					IndexPeriode = 6-PeriodeNiveauTradeInput.GetIndex() ;  
					if ( IndexPeriode == 1 )  // Jour
					{	QuelNiveauTradeInput.SetCustomInputStrings("Indice - Choix niv Journalier;R4 J;  mR4 J;R3 J;  mR3 J;R2 J;  mR2 J;R1 J;  mR1 J;PP J ******;  mS1 J;S1 J;  mS2 J;S2 J;  mS3 J;S3 J;  mS4 J;S4 J;PH JP;PB JP;DemiGap JP;Indice - Choix niv Journalier") ;  QuelNiveauTradeInput.SetCustomInputIndex(21) ; }
					else 	if ( IndexPeriode == 2 )  // Hebdo Full/Mid
					{	QuelNiveauTradeInput.SetCustomInputStrings("Indice - Choix niv H Full/Mid;R4 H;  mR4 H;R3 H;  mR3 H;R2 H;  mR2 H;R1 H;  mR1 H;PP H ******;  mS1 H;S1 H;  mS2 H;S2 H;  mS3 H;S3 H;  mS4 H;S4 H;PH HP;PB HP;Indice - Choix niv H Full/Mid") ;  QuelNiveauTradeInput.SetCustomInputIndex(20) ; }
					else 	if ( IndexPeriode == 3 )  // Hebdo Quart
					{	QuelNiveauTradeInput.SetCustomInputStrings("Indice - Choix niv H Quart;RQ8 H;RQ7 H;RQ6;RQ5 H;RQ4 H;RQ3 H;RQ2 H;RQ1 H;SQ1 H;SQ2 H;SQ3 H;SQ4 H;SQ5 H;SQ6 H;SQ7 H;SQ8 H;Indice - Choix niv H Quart") ;  QuelNiveauTradeInput.SetCustomInputIndex(17) ; }
					else 	if ( IndexPeriode == 4 )  // Mensuel Full/Mid
					{	QuelNiveauTradeInput.SetCustomInputStrings("Indice - Choix niv M Full/Mid;R4 M;  mR4 M;R3 M;  mR3 M;R2 M;  mR2 M;R1 M;  mR1 M;PP M ******;  mS1 M;S1 M;  mS2 M;S2 M;  mS3 M;S3 M;  mS4 M;S4 M;PH MP;PB MP;PH absolu;Indice - Choix niv M Full/Mid") ; QuelNiveauTradeInput.SetCustomInputIndex(21) ;  }
					else 	if ( IndexPeriode == 5 )  // Mensuel Quart
					{	QuelNiveauTradeInput.SetCustomInputStrings("Indice - Choix niv M Quart;RQ8 M;RQ7 M;RQ6 M;RQ5 M;RQ4 M;RQ3 M;RQ2 M;RQ1 M;SQ1 M;SQ2 M;SQ3 M;SQ4 M;SQ5 M;SQ6 M;SQ7 M;SQ8 M;Indice - Choix niv M Quart") ; QuelNiveauTradeInput.SetCustomInputIndex(17) ;  }
					else 	if ( IndexPeriode == 6 )  // Mensuel Eighth
					{	QuelNiveauTradeInput.SetCustomInputStrings("Indice - Choix niv M Eighth;RE16;RE15;RE14;RE13;RE12;RE11;RE10;RE9;RE8;RE7;RE6;RE5;RE4;RE3;RE2;RE1;SE1;SE2;SE3;SE4;SE5;SE6;SE7;SE8;SE9;SE10;SE11;SE12;SE13;SE14;SE15;SE16;Indice - Choix niv M Eighth") ; QuelNiveauTradeInput.SetCustomInputIndex(33) ; }
				}
			}
		}
		
		
		// Gestion de DateEtudiee via DateEtudieeRoulette
		{
			DateRAZ = SCDateHoraireRAZ.GetDate() ;  // GetDate() returns the date part of the SCDateTime variable. The value returned is a Date Value. // Date values are integer (int) values representing the number of days since December 30, 1899.
			if ( ( DateLocale - DateRAZ ) != DateEtudieeRoulette.GetIndex()  )  // la roulette a été tournée depuis le précédent calcul de DateLocale
			{
				// Nouvelle valeur de DateEtudiee
				DateRAZ = DateLocale - DateEtudieeRoulette.GetIndex() ;
				DateEtudiee.SetDate(DateRAZ) ;
			}
		}

		
		// Gestion de HoraireRAZ et SCDateHoraireRAZ 
		{
			if ( HoraireRAZ != HoraireRAZTexte.GetFloat() )  
			{ 
				HoraireRAZ = HoraireRAZTexte.GetFloat() ;  	
				ConversionVersSCDateHoraire (DateEtudiee.GetDate(), HoraireRAZ, SCDateHoraireRAZ) ; 
			}
			else if ( HoraireRAZRoulette.GetIndex() !=  TailleDuCustomInputDeHoraireRAZ )  
			{
				SCDateHoraireRAZ += ( TailleDuCustomInputDeHoraireRAZ - static_cast<float> (HoraireRAZRoulette.GetIndex()) ) / 1440 ; 
				// Modifications de DateEtudiee et HoraireRAZ à partir de SCDateHoraireRAZ    
				DateEtudiee.SetDate( SCDateHoraireRAZ.GetDate() ) ;
				int Hour, Minute, Second ;
				SCDateHoraireRAZ.GetTimeHMS(Hour, Minute, Second) ;  
				HoraireRAZ = float(Hour)*100 + float(Minute) + float(Second)/100 ;  // tracé au format 1234.16 pour 12h34m16s 
			}		
			else // calcul de SCDateHoraireRAZ pertinent lorsqu'on a changé de date
			{
				int hh = floor ( HoraireRAZ /100 ) ;
				int mm = floor ( HoraireRAZ -100*hh ) ;
				int ss = floor ( 100*HoraireRAZ -10000*hh - 100*mm ) ;
				int TimeValue = HMS_TIME(hh, mm, ss) ;
				SCDateHoraireRAZ.SetDateTime(DateEtudiee.GetDate(), TimeValue) ;
			}
		}
		
		// Réinjection de la nouvelle valeur de DateRAZ dans les inputs DateEtudiee et DateEtudieeRoulette
		{
			DateRAZ = SCDateHoraireRAZ.GetDate() ;
			DateEtudiee.SetDate(DateRAZ) ;
			SCString DateEtudieeInputString ;  CreationDeDateEtudieeInputString (TailleDuCustomInputDeDateRAZ, DateEtudieeInputString) ;
			DateEtudieeRoulette.SetCustomInputStrings(DateEtudieeInputString) ;
			DateLocale = CalculDateLocale() ;     // DateLocale est un int (date value)
			DateEtudieeRoulette.SetCustomInputIndex(DateLocale - DateRAZ);
		}		
		
		
		// Réinjection de la nouvelle valeur de HoraireRAZ dans les inputs HoraireRAZTexte, HoraireRAZRoulette
		{
			HoraireRAZTexte.SetFloat(HoraireRAZ);
			SCString HoraireRAZInputString ;  CreationDeHoraireRAZInputString (SCDateHoraireRAZ, TailleDuCustomInputDeHoraireRAZ, HoraireRAZInputString) ;
			HoraireRAZRoulette.SetCustomInputStrings(HoraireRAZInputString) ; 
			HoraireRAZRoulette.SetCustomInputIndex(TailleDuCustomInputDeHoraireRAZ);
		}

		
		// On débute avec SL_Prior = 10
		{
			if ( SL_Prior == 0 ) // lors du tout premier appel de la study, SL_Prior doit valoir 0 en théorie
			{
				SL_Prior = 10 ; 
			}
			else if ( SL_Prior > 100  or  SL_Prior < 0 )    // correction d'un possible bug
			{	SL_Prior = 100 ;  }
		}
		

		// Création de SCDateHoraireNewsMoins5Min
		CreationSCDateHoraireNewsMoins5Min(ptrsc) ;

		// Création de SCDateHoraireNewsMoins20Sec
		CreationSCDateHoraireNewsMoins20Sec(ptrsc) ;
		
		// Création de SCDateHoraireFinSeanceAction
		ConversionVersSCDateHoraire (DateEtudiee.GetDate(), 1559.50, SCDateHoraireFinSeanceAction) ;     // correspond à 15h59m50s 
		
		
		// Gestion des ACS Control Bar buttons de la présente instance
		if (  CetteInstanceControleLesBoutons.GetYesNo() == 1  ) 
		{ 
			// Set the ACS Control Bar buttons hover text
			sc.SetCustomStudyControlBarButtonHoverText(2, "Toggle ON/OFF du compte-ticks");
			// 3 : Status Report																																	// touche CTRL+S
			// sc.SetCustomStudyControlBarButtonHoverText(6, "Retranche 1p au NiveauCible Future");				// touche Numpad /
			// sc.SetCustomStudyControlBarButtonHoverText(7, "Ajoute 1p au NiveauCible Future");					// touche Numpad *
			// sc.SetCustomStudyControlBarButtonHoverText(9, "HoraireRaz=LastIndex-1min");							// raccourci clavier : Z
			// sc.SetCustomStudyControlBarButtonHoverText(10, "Met HoraireRAZ à 9h30m25s");    					// raccourci clavier : CTRL+Z
			// sc.SetCustomStudyControlBarButtonHoverText(11, "Retranche 1 minute a HoraireRAZ");				// touche Numpad 4
			// sc.SetCustomStudyControlBarButtonHoverText(12, "Ajoute 1 minute a HoraireRAZ");						// touche Numpad 6
			// sc.SetCustomStudyControlBarButtonHoverText(15, "Retranche du bonus");										// touche Numpad 3
			// sc.SetCustomStudyControlBarButtonHoverText(16, "Ajoute du bonus");											// touche Numpad 9
			sc.SetCustomStudyControlBarButtonHoverText(17, "Valeur du bonus");
			// 18 : Amplitude Mouvement
			// 19 : Bonus calculé
			// 20 : Recupération externe du bonus																								//touche B
			// 22 : Numéro mouvement
			// sc.SetCustomStudyControlBarButtonHoverText(27, "Ajuste le Niveau Cible de -1 pas choisi");	// touche Numpad 1
			// sc.SetCustomStudyControlBarButtonHoverText(28, "RAZ de l'ajustement choisi");						// touche Numpad 4
			// sc.SetCustomStudyControlBarButtonHoverText(29, "Ajuste le Niveau Cible de +1 pas choisi");	// touche Numpad 7
			sc.SetCustomStudyControlBarButtonHoverText(49, "Toggle ON/OFF de l'affichage de la study");	// touche Inser
			sc.SetCustomStudyControlBarButtonHoverText(52, "Statut du Trading Auto");									// touche A
			// 53 : "Refresh Autotrade"																													// touches CTRL+A
			// sc.SetCustomStudyControlBarButtonHoverText(56, "Sequence ON/OFF");									// touche S
			//sc.SetCustomStudyControlBarButtonHoverText(60, "Next AMR Study");   										// touche Numpad 5		// géré par une autre study
			sc.SetCustomStudyControlBarButtonHoverText(61, "Void ON/OFF");												// touche suppr
			sc.SetCustomStudyControlBarButtonHoverText(90, "FlattenAndCancelAllOrders");							// touche K (Kill)
			//sc.SetCustomStudyControlBarButtonHoverText(105, "Toggle ON/OFF de Niveau Future");   // géré par une autre study
			//sc.SetCustomStudyControlBarButtonHoverText(106, "Toggle ON/OFF de Niveau Indice");   // géré par une autre study
			// 109 : "Niv -"     Numpad 2
			// 110 : "Niv +"    Numpad 8
			// 111 : "Génération fichier .csv"   		touche F8
			// 123 : Toggle modèles linéaire/Parabolique pour malus ajustement		touche P
			// 127 : Toggle Manuel/ValeurVeille/VIX pour EnvergureMoyenne4000Ticks_Input de la study Sigma    touche M
			// 129 : Toggle Modèle pour contribution SpreadIndex à AUC et SeuilAUC    touche F12
			// 130 : Old Modèle pour contribution SpreadIndex à AUC et SeuilAUC    Shift + F12
			// 131 : écriture dans le log xxx.csv des inputs du trade paper-tradé    touche G
			// 133/134 : navigation rapide vers le niveau indice correspondant à LastVisibleBar     CTRL + 5 / CTRL + Numpad5
			// 135 : Toggle entre TPNu standard et TPNu modifié			// touche T
			// 136/137 : utilisé par la study "Detecteur de micro-flash-krach sur future"										// Shift + Left/Right Arrow

			

			
			
			// Set the ACS Control Bar button 2 text
			if ( CompteTicksONOFF == 1 )  { sc.SetCustomStudyControlBarButtonText(2, " CT ON") ;  }
			else if ( CompteTicksONOFF == 0 )  { sc.SetCustomStudyControlBarButtonText(2, "CT OFF") ;  }
			
			// Set the ACS Control Bar button 3 text
			// if ( NewDailyHighLow_Input.GetYesNo() == 1 )  { sc.SetCustomStudyControlBarButtonText(3, "NDHL YES") ;  }
			// else if ( NewDailyHighLow_Input.GetYesNo() == 0 )  { sc.SetCustomStudyControlBarButtonText(3, "NDHL NO") ;  }
			
			// Set the ACS Control Bar button 6/7 text
			// sc.SetCustomStudyControlBarButtonText(6, "Cible -1t") ; 
			// sc.SetCustomStudyControlBarButtonText(7, "Cible +1t") ; 

			// Set the ACS Control Bar buttons 11/12 text
			// sc.SetCustomStudyControlBarButtonText(11, "-1min") ; 
			// sc.SetCustomStudyControlBarButtonText(12, "+1min") ; 
			
			// Set the ACS Control Bar buttons 15/16/17 text
			// sc.SetCustomStudyControlBarButtonText(15, "Bonus -") ; 
			// sc.SetCustomStudyControlBarButtonText(16, "Bonus +") ; 
			Texte.Format("BONUS %.0f", round(BonusIntrinseque)) ;  sc.SetCustomStudyControlBarButtonText(17, Texte) ; 
			
			// Set the ACS Control Bar button 27/28 text
			// sc.SetCustomStudyControlBarButtonText(27, "Ajust -1p") ; 
			// sc.SetCustomStudyControlBarButtonText(28, "Ajust=0") ; 
			// sc.SetCustomStudyControlBarButtonText(29, "Ajust +1p") ; 
			
			// Set the ACS Control Bar button 49 text
			if ( AffichageONOFF == 1 )  { sc.SetCustomStudyControlBarButtonText(49, "Affiche ON ") ;  }
			else if ( AffichageONOFF == 0 )  { sc.SetCustomStudyControlBarButtonText(49, "Affiche OFF") ;  }
			
			// Set the ACS Control Bar button 52 and 56 text 
			SetButton52And56Text (ptrsc) ;
			
			// Set the ACS Control Bar button 60 text 
			Texte.Format("ID:%d | ", ThisStudyID) ;
			Texte.AppendFormat( NomDuNiveauTrade.GetString() ) ;
			//if ( FutureOuIndice == 1 )  { Texte.AppendFormat(" Ind ") ;  }  else { Texte.AppendFormat(" Fut ") ;  }
			// Texte.AppendFormat(" | NEXT") ;
			sc.SetCustomStudyControlBarButtonText(60, Texte) ; 
			
			// Set the ACS Control Bar button 61 text
			if ( Void == 1 ) { sc.SetCustomStudyControlBarButtonText(61, "Void ON ") ;  }
			else if ( Void == 0 ) { sc.SetCustomStudyControlBarButtonText(61, "Void OFF") ;  }
			
			// Set the ACS Control Bar button 90 text
			sc.SetCustomStudyControlBarButtonText(90, "KILL") ; 
			
			// Set the ACS Control Bar buttons text   (ça ne devrait pas être à cette study de le faire, mais il y a des erreurs d'affichage si on ne le fait pas ici)
			// sc.SetCustomStudyControlBarButtonText(109, "Niv -") ; 
			// sc.SetCustomStudyControlBarButtonText(110, "Niv +") ; 
			
		} // fin de " Gestion des  ACS Control Bar buttons"
		

		// Set the ACS Control Bar button 105 text   (ça ne devrait pas être à cette study de le faire, mais il y a des erreurs d'affichage si on ne le fait pas ici)
		if ( BooleanHideNiveauxFuture(ptrsc) )  { sc.SetCustomStudyControlBarButtonText(105, "Niv Fut OFF") ;  }
		else  { sc.SetCustomStudyControlBarButtonText(105, "Niv Fut ON ") ;  }

		
		// Trading indice : l'horaire doit être supérieur à 9h00m00s et inférieur à 16h00, sinon la study est incapable de se positionner sur un niveau indice
		if ( FutureOuIndice == 1  and ( HoraireRAZ < 900  or  HoraireRAZ > 1559.59 ) )   
		{ SCString message ;  message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | !! WARNING !! HoraireRAZ est incompatible avec un positionnement sur Indice (HoraireRAZ < 900.00  or  HoraireRAZ > 1559.59)") ; sc.AddMessageToLog(message,1) ; }

	
		// Affichage de Quantieme_Mvt : synchronisation avec AffichageONOFF de la study active
		if (  CetteInstanceControleLesBoutons.GetYesNo() == 1  )  Set_StudyQuantiemeMvt(ptrsc) ;

	}  // fin sc.UpdateStartIndex== 0
	
	

	// Récupération des SCFloatArray externes via GetStudyArrayUsingID et GetStudyExtraArrayFromChartUsingID
	{
		// on récupère les valeurs du SpreadIndex dans le Subgraph[0] de "SpreadIndexAffichage" sur le présent Chart 		
		sc.GetStudyArrayUsingID(StudyID_SI, 0, ArraySpreadIndex);
		
		// on récupère les valeurs du TP non arrondi dans le Subgraph[2] de "Calcul TPNu" sur le présent Chart 
		sc.GetStudyArrayUsingID(StudyIDTPNu, 2, ArrayTPNu);
		
		// on récupère les valeurs du SigmaUp et du SigmaDown dans le Subgraph[1].Arrays[0] et le Subgraph[1].Arrays[1] de "Sigma" sur le présent Chart 
		sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDSigma, 1, 0, ArraySigmaUp);   //  int GetStudyExtraArrayFromChartUsingID(int ChartNumber, int StudyID, int SubgraphIndex, int ExtraArrayIndex, SCFloatArrayRef ExtraArrayRef);
		sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDSigma, 1, 1, ArraySigmaDown);  
		
		// on récupère les valeurs (booléen) du Background de Sigma dans le Subgraph[0] de "Sigma" sur le présent Chart 
		sc.GetStudyArrayUsingID(StudyIDSigma, 0, ArraySigmaBackground);
		
		// on récupère les valeurs de INQ100AvecCorrection dans le Subgraph[0] de "Overlay INQ100 sur le future (avec correction du spread)" sur le présent Chart 
		sc.GetStudyArrayUsingID(StudyIDOverlayINQ100, 0, INQ100AvecCorrection);	
		
		// on récupère les valeurs de INQ100SansCorrection dans le Subgraph[0].Arrays[0] de "Overlay INQ100 sur le future (avec correction du spread)" sur le présent Chart 
		sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDOverlayINQ100, 0, 0, INQ100SansCorrection);   //  int GetStudyExtraArrayFromChartUsingID(int ChartNumber, int StudyID, int SubgraphIndex, int ExtraArrayIndex, SCFloatArrayRef ExtraArrayRef);
		
		// on récupère les valeurs de SpreadMoyen dans le Subgraph[0].Arrays[1] de "Overlay INQ100 sur le future (avec correction du spread)" sur le présent Chart 
		sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDOverlayINQ100, 0, 1, SpreadMoyen);    // spread moyen non arrondi
		
		// Récupération de EnvergureMoyenne4000Ticks dans la study "Sigma" du présent graphique
		sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDSigma, 1, 4, ExternalArray_EnvergureMoyenne4000Ticks); 
	}


	// APPUI SUR UN CONTROL BAR BUTTON ///////////////////////////////////////////////////////////////////
	{
		if ( sc.MenuEventID != 0  or  External_Input_ACS_BUTTON_8.GetInt() != 0  or  External_Input_ACS_BUTTON_9.GetInt() != 0  or  External_Input_ACS_BUTTON_11.GetInt() != 0  or  External_Input_ACS_BUTTON_12.GetInt() != 0  or  External_Input_ACS_BUTTON_20.GetInt() != 0  or  External_Input_ACS_BUTTON_49.GetInt() != 0  or  External_Input_ACS_BUTTON_60.GetInt() != 0  or  External_Input_ACS_BUTTON_109_110.GetInt() != 0  or  External_Input_ACS_BUTTON_10.GetInt() != 0 or  External_Input_ACS_BUTTON_52.GetInt() != 0  or  External_Input_ACS_BUTTON_133_134.GetInt() !=0  or  External_Input_ACS_BUTTON_15.GetInt() != 0 or  External_Input_ACS_BUTTON_16.GetInt() != 0  or  External_Input_ACS_BUTTON_6.GetInt() != 0 or  External_Input_ACS_BUTTON_7.GetInt() != 0)
		{
			sc.UpdateAlways = 1;   // pour que la study soit updatée immédiatement (sinon il faut attendre qu'une condition d'appel se produise, il n'y a jamais d'appel si le marché est fermé, ou ça peut durer longtemps si marché calme)
			sc.SetCustomStudyControlBarButtonEnable(sc.MenuEventID,FALSE); // le bouton cliqué ne prend pas la position enfoncée		// apparemment ça permet aussi de vider le buffer (sinon, l'appui sur les boutons A, B, C donne : A, A, B, B, C )
			
			/* // Bouton "Toggle modèles linéaire/Parabolique pour malus ajustement" 
			if ( sc.MenuEventID == ACS_BUTTON_123 )  // P
			{	
				MalusAjustementParabolique_Input.SetCustomInputIndex(1-MalusAjustementParabolique_Input.GetIndex()) ; 
				TriggerRecalculate = 1 ;    return ;
			} */

			// Bouton "Toggle anciens/nouveaux modèles après remise à plat de AUC et SeuilAUC" 
			if ( sc.MenuEventID == ACS_BUTTON_129 )  // F12
			{	
				int Index = ModelePourAUC_Input.GetIndex() ;
				if ( Index == 1 )  Index = 2 ;  else if ( Index == 2 )  Index = 3 ;  else if ( Index == 3 )  Index = 1 ;
				ModelePourAUC_Input.SetCustomInputIndex(Index) ; 
				TriggerRecalculate = 1 ;    return ;
			}

			// RAZ du modèle 
			if ( sc.MenuEventID == ACS_BUTTON_130 )  // CTRL + F12
			{	
				ModelePourAUC_Input.SetCustomInputIndex(2) ; 
				TriggerRecalculate = 1 ;    return ;
			}
			
			// Bouton "écriture dans le log xxx.csv des inputs du trade paper-tradé" 
			if ( sc.MenuEventID == ACS_BUTTON_131 )  // touche G
			{
				SCString NameLogFile ;
				
				// Création du nom du fichier .csv 
				{
					// Nom de fichier
					NameLogFile.Format("Parametres en input des trades paper testes.csv") ;
					
					// Envoi de ce nom dans le log
					/* SCString message ;
					message.Format("Ecriture en cours dans le fichier ''%s''", NameLogFile.GetChars() ) ;    // the GetChars() function on the SCString to be able to access the internal C++ character pointer which is necessary when using this class with the SCString Format() and AppendFormat() functions.
					sc.AddMessageToLog(message,0) ; */
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
						// remplissage du fichier .csv avec  : IntradayIndex / datation officielle (horaire en SCDateTime) / DeltaTimeStamp en ms / Refresh de SC (ms) / prix / volume  / datation locale (horaire en SCDateTime) / ms de la datation locale / datation locale (jour en Date Value)
						myStream << "Date" << ";" << "Horaire (en s)" << ";" << "Nom Niveau" << ";" << "Prix (sur indice)" << ";" << "Prix (sur future)" << ";" << "Bonus" << ";" << "Nb Mouvement" << "\n" ; 
					}  
					else {	SCString message ;  message.Format("Ouverture du fichier a la creation : erreur") ;  sc.AddMessageToLog(message, 1) ;  }
				}
				
				// Ecriture des paramètres du trade dans le fichier
				{
					// préparation des paramètres qui vont être écrits dans le messagelog et le .csv
					SCString SCString_NomNiveau = NomDuNiveauTrade.GetString() ;
					std::string NomNiveau = SCString_NomNiveau.GetChars(); // conversion SCString en string
					
					// Déclaration du flux d'écriture
					std::ofstream myStream ;
					myStream.open(NameLogFile, std::ofstream::out | std::ofstream::app) ;
					if ( myStream.is_open() ) 	{	SCString message ;  message.Format("Ecriture des parametres du trade de  %s  dans le fichier : OK", SCString_NomNiveau.GetChars()) ;  sc.AddMessageToLog(message, 0) ;  }  else {	SCString message ;  message.Format("!! Ecriture des parametres du trade de  %s  dans le fichier : ERREUR !!", SCString_NomNiveau.GetChars()) ;  sc.AddMessageToLog(message, 1) ;  }
					
					// remplissage du fichier .csv
					myStream << ConversionFloat(float(SCDateHoraireRAZ.GetDate())) << ";" << ConversionFloat(SCDateHoraireRAZ.GetTimeAsSCDateTime().GetTimeInSecondsWithoutMilliseconds()) << ";" << NomNiveau << ";" << ConversionFloat(NiveauIndice) << ";" << ConversionFloat(NiveauFuture) <<";" << ConversionFloat(BonusIntrinseque) << ";" << "\n" ;          
				}
			} 

			// Bouton "Toggle NewDailyHighLow_InputManuel" 
			if ( sc.MenuEventID == ACS_BUTTON_132 )  // F10
			{	
				NewDailyHighLow_InputManuel.SetCustomInputIndex(1-NewDailyHighLow_InputManuel.GetIndex()) ; 
				TriggerRecalculate = 1 ;    return ;
			}

			// Bouton "NEXT study" 	// Numpad 5
			if ( External_Input_ACS_BUTTON_60.GetInt() != 0 )  		// " sc.MenuEventID == ACS_BUTTON_60 " est géré par la study "Tour de controle" qui modifie les External_Input_ACS_BUTTON_60 des studies AMR
			{	
				if ( ThisStudyID == GetActiveStudyID(ptrsc) )   // l'instance est la study active
				{ 	
					CetteInstanceControleLesBoutons.SetYesNo(1) ;
					sc.DisplayStudyName = 1;
					sc.DisplayStudyInputValues = 1 ;  
					
					// Set the ACS Control Bar button 2 text
					if ( CompteTicksONOFF == 1 )  { sc.SetCustomStudyControlBarButtonText(2, " CT ON") ;  }
					else if ( CompteTicksONOFF == 0 )  { sc.SetCustomStudyControlBarButtonText(2, "CT OFF") ;  }
					
					// Set the ACS Control Bar button 3 text
					if ( NewDailyHighLow_Input.GetYesNo() == 1 )  { sc.SetCustomStudyControlBarButtonText(3, "NDHL YES") ;  }
					else if ( NewDailyHighLow_Input.GetYesNo() == 0 )  { sc.SetCustomStudyControlBarButtonText(3, "NDHL NO") ;  }
					
					// Set the ACS Control Bar button 49 text
					if ( AffichageONOFF == 1 )  { sc.SetCustomStudyControlBarButtonText(49, "Affiche ON") ;  }
					else if ( AffichageONOFF == 0 )  { sc.SetCustomStudyControlBarButtonText(49, "Affiche OFF") ;  }
					
					// Set the ACS Control Bar button 52 & 56 text
					SetButton52And56Text (ptrsc) ;
					
					// Gestion de la study "Quantieme_Mvt"
					Set_StudyQuantiemeMvt(ptrsc) ;
				}
				else
				{ 	
					CetteInstanceControleLesBoutons.SetYesNo(0) ;  
					sc.DisplayStudyName = 0;  
					sc.DisplayStudyInputValues = 0 ; 
					sc.DeleteUserDrawnACSDrawing(11, 10055+10000*ThisStudyID) ;   sc.DeleteUserDrawnACSDrawing(12, 10055+10000*ThisStudyID) ;	 sc.DeleteUserDrawnACSDrawing(15, 10055+10000*ThisStudyID) ;   sc.DeleteUserDrawnACSDrawing(19, 10055+10000*ThisStudyID) ;  // efface les marqueurs de study active sur les graphes externes
				}
				// External_Input_ACS_BUTTON_60.SetInt( std::max(External_Input_ACS_BUTTON_60.GetInt()-1, 0) ) ; // on retranche 1 à External_Input_ACS_BUTTON_60
				External_Input_ACS_BUTTON_60.SetInt(0) ; // on remet à zéro
			}			
			
			
			// Autres boutons : nécessite que cette instance contrôle les boutons
			else if (  CetteInstanceControleLesBoutons.GetYesNo() == 1  )      
			{
				
				// Le robot est-il actif ?  Est-il en position ? 
				{
					if ( AutoTradeStatus != 0 and AutoTradeStatus != 50 and AutoTradeStatus != 100 and AutoTradeStatus != 110 and AutoTradeStatus != 120 and AutoTradeStatus != 130 and AutoTradeStatus != 140 and AutoTradeStatus != 150 and AutoTradeStatus != 160 and AutoTradeStatus != 170 and AutoTradeStatus != 180 and AutoTradeStatus != 190 and AutoTradeStatus != 200 and AutoTradeStatus != 1000 )    // AutoTradeStatus n'est ni "OFF" ni "ordres et position fermes" => le robot est actif
					{	
						RobotActif = 1 ;  
						if ( AutoTradeStatus != 11 )  RobotPosition.SetInt(Position) ;  else RobotPosition.SetInt(0) ;
					}
					else
					{	
						RobotActif = 0 ;  
						RobotPosition.SetInt(0) ;
					}
				}
				
				// Toggle "CT ON/OFF"
				if (sc.MenuEventID == ACS_BUTTON_2 )     
				{	
					if ( RobotActif == 0 )     // Le robot doit être inactif pour que le toggle OFF du compte-ticks soit autorisé
					{	CompteTicksONOFFInput.SetCustomInputIndex(CompteTicksONOFF);  sc.SetCustomStudyControlBarButtonText(2, "  busy  ") ;  TriggerRecalculate = 1 ;    return ;  }
					else  // le robot est actif => message dans le log 
					{  SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | !! WARNING !! Le bouton ''Toggle Compte-Ticks'' a ete clique alors que le robot est actif (le robot doit etre arrete pour effacer le compte-ticks)") ;  sc.AddMessageToLog(message,1) ;  }          
				}
				
				// Bouton "Cible -1t"
				if ( ( sc.MenuEventID == ACS_BUTTON_6  or  External_Input_ACS_BUTTON_6.GetInt() != 0 )  and  AffichageONOFF)  
				{ 	
					if ( sc.MenuEventID == ACS_BUTTON_6 )  External_Input_ACS_BUTTON_6.SetInt(1) ;
					
					if ( RobotActif == 0 )	{ NiveauFutureTexte.SetFloat( NiveauFutureTexte.GetFloat() - 0.25 * External_Input_ACS_BUTTON_6.GetInt() ) ;  External_Input_ACS_BUTTON_6.SetInt(0) ;  TriggerRecalculate = 1 ;    return ;  }
					else	{  SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | !! WARNING !! Le bouton '' Cible -1t '' a ete clique alors que le robot est actif (le robot doit etre arrete pour que ce bouton soit operant)") ;  sc.AddMessageToLog(message,1) ;  External_Input_ACS_BUTTON_6.SetInt(0) ;  }          
				}  

				// Bouton "Cible +1t"
				if ( ( sc.MenuEventID == ACS_BUTTON_7  or  External_Input_ACS_BUTTON_7.GetInt() != 0 )  and  AffichageONOFF)  
				{ 	
					if ( sc.MenuEventID == ACS_BUTTON_7 )  External_Input_ACS_BUTTON_7.SetInt(1) ;
					
					if ( RobotActif == 0 )	{ NiveauFutureTexte.SetFloat( NiveauFutureTexte.GetFloat() + 0.25 * External_Input_ACS_BUTTON_7.GetInt() ) ;  External_Input_ACS_BUTTON_7.SetInt(0) ;  TriggerRecalculate = 1 ;    return ;  }
					else	{  SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | !! WARNING !! Le bouton '' Cible -1t '' a ete clique alors que le robot est actif (le robot doit etre arrete pour que ce bouton soit operant)") ;  sc.AddMessageToLog(message,1) ;  External_Input_ACS_BUTTON_7.SetInt(0) ;  }          
				}  

				// Bouton "HoraireRaz=RoundToHour(SCDateHoraireRAZ)"                     // Keyboard shortcut :  SHIFT+Z
				if ( sc.MenuEventID == ACS_BUTTON_8  or  External_Input_ACS_BUTTON_8.GetInt() != 0 ) 
				{	
					SCDateTime RoundToHour_SCDateHoraireRAZ = SCDateHoraireRAZ ;    // on copie SCDateHoraireRAZ dans une variable temporaire
					RoundToHour_SCDateHoraireRAZ.RoundDateTimeDownToMinute() ;   // on arrondit à la minute
					float Minute = RoundToHour_SCDateHoraireRAZ.GetMinute();
					if (Minute == 0) Minute = 60 ;			// Cette instruction permet de soustraire heure entière si on est déjà sur une heure pile
					RoundToHour_SCDateHoraireRAZ = RoundToHour_SCDateHoraireRAZ - SCDateTime::MINUTES(Minute) ;   // on retranche les minutes => on vient d'obtenir le SCDateTime arrondi à l'heure pile
					
					// Réinjection de la nouvelle valeur de HoraireRAZTexte
					int Hour = RoundToHour_SCDateHoraireRAZ.GetHour() ;
					HoraireRAZTexte.SetFloat(Hour*100) ;   // Format hhmm.ss
					// Réinjection de la nouvelle valeur de DateRAZ dans les inputs DateEtudiee et DateEtudieeRoulette
					DateRAZ = RoundToHour_SCDateHoraireRAZ.GetDate() ;  
					DateEtudiee.SetDate(DateRAZ) ;
					SCString DateEtudieeInputString ;  CreationDeDateEtudieeInputString (TailleDuCustomInputDeDateRAZ, DateEtudieeInputString) ;
					DateEtudieeRoulette.SetCustomInputStrings(DateEtudieeInputString) ;
					DateLocale = CalculDateLocale() ;     // DateLocale est un int (date value)
					DateEtudieeRoulette.SetCustomInputIndex(DateLocale - DateRAZ);
					
					// On met l'affichage ON (s'il était déjà ON, ça ne change rien)
					AffichageONOFF_Input.SetCustomInputIndex(0); 
					AffichageONOFF = 1 ;
					sc.HideStudy=0;
					Void = 0 ;
					
					External_Input_ACS_BUTTON_8.SetInt(0) ;    // on met à 0 External_Input_ACS_BUTTON_8
					TriggerRecalculate = 1 ;    return ;
				}

				// Bouton "HoraireRaz=LastVisibleBar - NbBarresVirginite"                     // Keyboard shortcut :  Z 
				if ( sc.MenuEventID == ACS_BUTTON_9  or  External_Input_ACS_BUTTON_9.GetInt() != 0 )
				{	
					int IndexLastVisibleBar_Chart6 ;
					
					if ( sc.MenuEventID == ACS_BUTTON_9 )  { IndexLastVisibleBar_Chart6 = sc.IndexOfLastVisibleBar ;  }
					else 	// External_Input_ACS_BUTTON_9.GetInt() != 0 
					{
						IndexLastVisibleBar_Chart6 = External_Input_ACS_BUTTON_9.GetInt() ;
						External_Input_ACS_BUTTON_9.SetInt(0) ;
					}
					
					if ( TPNu_Manuel_Input.GetFloat() == 0 ) TPNu = ArrayTPNu[IndexLastVisibleBar_Chart6] ;  else TPNu = TPNu_Manuel_Input.GetFloat() ;
					NbBarresVirginite = ceil ( 250 * TPNu / 4 ) ; 
					SCDateTime HoraireDeRAZ = sc.BaseDateTimeIn[IndexLastVisibleBar_Chart6-2*NbBarresVirginite] ;
					
					// Réinjection de la nouvelle valeur de HoraireRAZTexte
					int Hour = HoraireDeRAZ.GetHour() ;
					int Minute = HoraireDeRAZ.GetMinute() ;
					HoraireRAZTexte.SetFloat(Hour*100+Minute) ;   // Format hhmm.ss
					
					// Réinjection de la nouvelle valeur de DateRAZ dans les inputs DateEtudiee et DateEtudieeRoulette
					DateRAZ = HoraireDeRAZ.GetDate() ;  
					DateEtudiee.SetDate(DateRAZ) ;
					SCString DateEtudieeInputString ;  CreationDeDateEtudieeInputString (TailleDuCustomInputDeDateRAZ, DateEtudieeInputString) ;
					DateEtudieeRoulette.SetCustomInputStrings(DateEtudieeInputString) ;
					DateLocale = CalculDateLocale() ;     // DateLocale est un int (date value)
					DateEtudieeRoulette.SetCustomInputIndex(DateLocale - DateRAZ);
					
					// On met l'affichage ON (s'il était déjà ON, ça ne change rien)
					AffichageONOFF_Input.SetCustomInputIndex(0); 
					AffichageONOFF = 1 ;
					sc.HideStudy=0;
					Void = 0 ;
					
					External_Input_ACS_BUTTON_9.SetInt(0) ;    // on met à 0 External_Input_ACS_BUTTON_9
					TriggerRecalculate = 1 ;    return ;
				}

				// Bouton "HoraireRaz=9h30m00s"    // Keyboard shortcut :  CTRL+Z
				if ( sc.MenuEventID == ACS_BUTTON_10  or  External_Input_ACS_BUTTON_10.GetInt() != 0 )  
				{	
					HoraireRAZTexte.SetFloat(930) ;
					
					// On met l'affichage ON (s'il était déjà ON, ça ne change rien)
					AffichageONOFF_Input.SetCustomInputIndex(0); 
					AffichageONOFF = 1 ;
					sc.HideStudy=0;
					Void = 0 ;
					
					External_Input_ACS_BUTTON_10.SetInt(0) ;    // on met à 0 External_Input_ACS_BUTTON_10
					sc.SetCustomStudyControlBarButtonText(10, " busy ") ;  TriggerRecalculate = 1 ;    return ;
				}


				// Bouton "-1min"             // Keyboard shortcut :  Numpad4
				if ( ( sc.MenuEventID == ACS_BUTTON_11  or  External_Input_ACS_BUTTON_11.GetInt() > 0 )  /* and  AffichageONOFF */ )  
				{	
					int MinutesRetranchees = std::max(1, std::min(20, External_Input_ACS_BUTTON_11.GetInt() ) ) ; 		// on retranche entre 1 et 20 minutes

					if ( HoraireRAZ == 931 )	{ HoraireRAZTexte.SetFloat(930.15625) ;  }  // 0.15625 correspond à un float exact   // HoraireRAZ = 930.16 ne fonctionnera pas
					else if ( HoraireRAZ == 930.15625 )	{ HoraireRAZTexte.SetFloat(930) ;  }
					// else 	{ HoraireRAZRoulette.SetCustomInputIndex( HoraireRAZRoulette.GetIndex() + 1 ) ;  } 
					else 	{ HoraireRAZRoulette.SetCustomInputIndex( HoraireRAZRoulette.GetIndex() + MinutesRetranchees ) ;  } 
					
					// On met l'affichage ON (s'il était déjà ON, ça ne change rien)
					AffichageONOFF_Input.SetCustomInputIndex(0); 
					AffichageONOFF = 1 ;
					sc.HideStudy=0;
					Void = 0 ;
					
					// External_Input_ACS_BUTTON_11.SetInt( std::max(External_Input_ACS_BUTTON_11.GetInt()-1, 0) ) ; // on retranche 1 à External_Input_ACS_BUTTON_11
					External_Input_ACS_BUTTON_11.SetInt(0) ; // on RAZ External_Input_ACS_BUTTON_11					
					sc.SetCustomStudyControlBarButtonText(11, " busy ") ;  TriggerRecalculate = 1 ;    return ;
				}
				
				// Bouton "+1min"        // Keyboard shortcut :  Numpad6
				if ( ( sc.MenuEventID == ACS_BUTTON_12  or  External_Input_ACS_BUTTON_12.GetInt() > 0 )  /* and  AffichageONOFF */ )  
				{
					int MinutesAjoutees = std::max(1, std::min(20, External_Input_ACS_BUTTON_12.GetInt() ) ) ; 		// on ajoute entre 1 et 20 minutes
					
					if ( HoraireRAZ == 930 )	{ HoraireRAZTexte.SetFloat(930.15625) ;  }
					else if ( HoraireRAZ == 930.15625 )	{ HoraireRAZTexte.SetFloat(931) ;  }
					// else	{ HoraireRAZRoulette.SetCustomInputIndex( HoraireRAZRoulette.GetIndex() - 1 ) ;  }  
					else	{ HoraireRAZRoulette.SetCustomInputIndex( HoraireRAZRoulette.GetIndex() - MinutesAjoutees ) ;  }  
					
					// On met l'affichage ON (s'il était déjà ON, ça ne change rien)
					AffichageONOFF_Input.SetCustomInputIndex(0); 
					AffichageONOFF = 1 ;
					sc.HideStudy=0;
					Void = 0 ;
					
					// External_Input_ACS_BUTTON_12.SetInt( std::max(External_Input_ACS_BUTTON_12.GetInt()-1, 0) ) ; // on retranche 1 à External_Input_ACS_BUTTON_12
					External_Input_ACS_BUTTON_12.SetInt(0) ; // on RAZ External_Input_ACS_BUTTON_12
					sc.SetCustomStudyControlBarButtonText(12, " busy ") ;  TriggerRecalculate = 1 ;    return ;  
				}

				// Bouton "-1sec"        // Keyboard shortcut :  CTRL+Numpad4
				if ( sc.MenuEventID == ACS_BUTTON_13  and  AffichageONOFF )  
				{	
					// soustraction d'une seconde
					SCDateTime SCDateHoraireRAZ_New = SCDateHoraireRAZ - SCDateTime::SECONDS(1) - SCDateTime::MILLISECONDS(500) ;   // on soustrait 1.1 seconde car à cause des erreurs d'arrondi, il se peut que HoraireRAZTexte ne soit pas modifié

					// Réinjection de la nouvelle valeur de HoraireRAZTexte
					int Hour = SCDateHoraireRAZ_New.GetHour() ;
					int Minute = SCDateHoraireRAZ_New.GetMinute() ;
					double Second = SCDateHoraireRAZ_New.GetSecond() ;
					HoraireRAZTexte.SetFloat(Hour*100+Minute+Second/100) ;   // Format hhmm.ss
					
					// On met l'affichage ON (s'il était déjà ON, ça ne change rien)
					AffichageONOFF_Input.SetCustomInputIndex(0); 
					AffichageONOFF = 1 ;
					sc.HideStudy=0;
					Void = 0 ;
					
					// {  SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | Le bouton '' -1sec '' a ete clique") ;  sc.AddMessageToLog(message,0) ;  }          
					TriggerRecalculate = 1 ;    return ;
				}

				// Bouton "+1sec"        // Keyboard shortcut :  CTRL+Numpad6
				if ( sc.MenuEventID == ACS_BUTTON_14  and  AffichageONOFF )  
				{	
					// addition d'une seconde
					SCDateTime SCDateHoraireRAZ_New = SCDateHoraireRAZ + SCDateTime::SECONDS(1) + SCDateTime::MILLISECONDS(500) ;   // on ajoute 1.5 seconde car à cause des erreurs d'arrondi, il se peut que HoraireRAZTexte ne soit pas modifié

					// Réinjection de la nouvelle valeur de HoraireRAZTexte
					int Hour = SCDateHoraireRAZ_New.GetHour() ;
					int Minute = SCDateHoraireRAZ_New.GetMinute() ;
					double Second = SCDateHoraireRAZ_New.GetSecond() ;
					HoraireRAZTexte.SetFloat(Hour*100+Minute+Second/100) ;   // Format hhmm.ss
					
					// On met l'affichage ON (s'il était déjà ON, ça ne change rien)
					AffichageONOFF_Input.SetCustomInputIndex(0); 
					AffichageONOFF = 1 ;
					sc.HideStudy=0;
					Void = 0 ;
					
					TriggerRecalculate = 1 ;    return ;
				}
				
				// Bouton "Bonus-"
				if ( sc.MenuEventID == ACS_BUTTON_15  or  External_Input_ACS_BUTTON_15.GetInt() > 0 )  
				{ 	
					External_Input_ACS_BUTTON_15.SetInt( std::max(External_Input_ACS_BUTTON_15.GetInt()-1, 0) ) ; // on retranche 1 à External_Input_ACS_BUTTON_15
					BonusIntrinseque_Roulette_Input.SetCustomInputIndex( BonusIntrinseque_Roulette_Input.GetIndex() + 1 ) ;
					BonusIntrinseque = ConversionDuInputBonusIntrinsequeRoulette(BonusIntrinseque_Roulette_Input.GetIndex()) ;
					Texte.Format("BONUS %.0f", BonusIntrinseque) ;  sc.SetCustomStudyControlBarButtonText(17, Texte) ; 
					
					// on ne recalcule la study que si elle est affichée
					if ( AffichageONOFF )  { TriggerRecalculate = 1 ;    return ;  }
				}  
				
				// Bouton "Bonus+" 
				if ( sc.MenuEventID == ACS_BUTTON_16  or  External_Input_ACS_BUTTON_16.GetInt() > 0 )  
				{ 	
					External_Input_ACS_BUTTON_16.SetInt( std::max(External_Input_ACS_BUTTON_16.GetInt()-1, 0) ) ; // on retranche 1 à External_Input_ACS_BUTTON_16
					BonusIntrinseque_Roulette_Input.SetCustomInputIndex( BonusIntrinseque_Roulette_Input.GetIndex() - 1 ) ;   
					BonusIntrinseque = ConversionDuInputBonusIntrinsequeRoulette(BonusIntrinseque_Roulette_Input.GetIndex()) ;
					Texte.Format("BONUS %.0f", BonusIntrinseque) ;  sc.SetCustomStudyControlBarButtonText(17, Texte) ; 
					
					// on ne recalcule la study que si elle est affichée
					if ( AffichageONOFF )  { TriggerRecalculate = 1 ;    return ;  }
				}  

				// Bouton "Récupération Bonus externe" 		touche B			
				if ( sc.MenuEventID == ACS_BUTTON_20  or  External_Input_ACS_BUTTON_20.GetInt() != 0 )  
				{ 	
					External_Input_ACS_BUTTON_20.SetInt(0) ; 	// on RAZ External_Input_ACS_BUTTON_20
					
					// Récupération de BonusIntrinseque_Exterieur_Input
					double BonusExterieur = -1 ; 
					int NatureNiveau = NatureNiveau_Input.GetInt() ;
					if ( NatureNiveau != 0 and NatureNiveau != 5 )			// sc.ChartNumber == 15
					sc.GetChartStudyInputFloat(15, StudyID_QuantiemeMvt_Chart15, 21, BonusExterieur) ;		// int GetChartStudyInputFloat(int ChartNumber, int StudyID, int InputIndex, double& r_FloatValue);
					else if ( NatureNiveau == 0 or NatureNiveau == 5 )			// sc.ChartNumber == 19
					sc.GetChartStudyInputFloat(19, StudyID_QuantiemeMvt_Chart19, 21, BonusExterieur) ;
					
					// Cas particulier (niveaux psys) : on retourne le bonus calculé pour chart 15 si mvt < 500 pdm, ou le bonus pour chart 19 si mvt > 500pdm.
					if ( NatureNiveau == 0 )
					{
						double Amplitude_pdm_Chart15 = -1 ;
						sc.GetChartStudyInputFloat(15, StudyID_QuantiemeMvt_Chart15, 22, Amplitude_pdm_Chart15) ;
						
						if ( Amplitude_pdm_Chart15 > 500 )		// on récupère BonusExterieur sur Chart #19
						{
							sc.GetChartStudyInputFloat(19, StudyID_QuantiemeMvt_Chart19, 21, BonusExterieur) ;
						}
						else		// on récupère BonusExterieur sur Chart #15
						{
							sc.GetChartStudyInputFloat(15, StudyID_QuantiemeMvt_Chart15, 21, BonusExterieur) ;
						}
					}

					// Modification du bouton 17
					Texte.Format("BONUS %.0f", round(BonusExterieur)) ;  sc.SetCustomStudyControlBarButtonText(17, Texte) ; 

					// Modification subséquente des inputs
					BonusIntrinseque_Exterieur_Input.SetFloat(BonusExterieur) ;
					BonusIntrinseque_Roulette_Input.SetCustomInputIndex(0) ;   // on met "OFF"
					
					// on ne recalcule la study que si elle est affichée
					if ( AffichageONOFF )  { TriggerRecalculate = 1 ;    return ;  }
				}  

				// Bouton "Ajust -1p"		// touche Numpad 7   
				if (sc.MenuEventID == ACS_BUTTON_27  and  AffichageONOFF)  
				{ 	
					if ( RobotActif == 0 )	{ AjustementChoisiEnPas_Input.SetCustomInputIndex( AjustementChoisiEnPas_Input.GetIndex() + 1 ) ;  sc.SetCustomStudyControlBarButtonText(27, "   busy   ") ;   TriggerRecalculate = 1 ;    return ;  }  
					else	{  SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | !! WARNING !! Le bouton '' Ajust -1p '' a ete clique alors que le robot est actif (le robot doit etre arrete pour que ce bouton soit operant)") ;  sc.AddMessageToLog(message,1) ;  }          
				} 
				
				// Bouton "Ajust=0"		// touche Numpad 4
				if (sc.MenuEventID == ACS_BUTTON_28  and  AffichageONOFF)  
				{ 	
					if ( RobotActif == 0 )	{ AjustementChoisiEnPas_Input.SetCustomInputIndex(22);  sc.SetCustomStudyControlBarButtonText(28, "  busy  ") ;  TriggerRecalculate = 1 ;    return ;  }  
					else	{  SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | !! WARNING !! Le bouton '' Ajust=0 '' a ete clique alors que le robot est actif (le robot doit etre arrete pour que ce bouton soit operant)") ;  sc.AddMessageToLog(message,1) ;  }          
				} 
				
				// Bouton "Ajust -1p"		// touche Numpad 1
				if (sc.MenuEventID == ACS_BUTTON_29  and  AffichageONOFF)  
				{ 	
					if ( RobotActif == 0 )	{ AjustementChoisiEnPas_Input.SetCustomInputIndex( AjustementChoisiEnPas_Input.GetIndex() - 1 ) ;  sc.SetCustomStudyControlBarButtonText(29, "   busy   ") ;  TriggerRecalculate = 1 ;    return ;  }  
					else	{  SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | !! WARNING !! Le bouton '' Ajust +1p '' a ete clique alors que le robot est actif (le robot doit etre arrete pour que ce bouton soit operant)") ;  sc.AddMessageToLog(message,1) ;  }          
				} 
				
				// Toggle "Affichage ON/OFF"      (touche "inser")
				if ( sc.MenuEventID == ACS_BUTTON_49   or  External_Input_ACS_BUTTON_49.GetInt() > 0 )  
				{	
					External_Input_ACS_BUTTON_49.SetInt( std::max(External_Input_ACS_BUTTON_49.GetInt()-1, 0) ) ; 		// on retranche 1 à External_Input_ACS_BUTTON_49
					
					if ( RobotActif == 0  or  AutoTradeStatus == 11 )       // Le robot doit être inactif (ou cas particulier de AutoTradeStatus == 11 ) pour que le toggle OFF de l'affichage soit autorisé
					{
						AffichageONOFF_Input.SetCustomInputIndex(AffichageONOFF);     // on inverse le booléen => la variable AffichageONOFF sera màj lors du prochain appel du DLL (TriggerRecalculate = 1)
						AffichageONOFF = 1 - AffichageONOFF ;   // on inverse le booléen
						sc.HideStudy=1-AffichageONOFF;
						Void = 0 ;
						sc.ResetAllScales = 1 ;
						// if ( AffichageONOFF == 0 )  sc.ResetAllScales = 1 ;
						
						TriggerRecalculate = 1 ;  return ;  
					}
					else  // le robot est actif => message dans le log 
					{  SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | !! WARNING !! Le bouton ''Toggle Affichage'' a ete clique alors que le robot est actif (le robot doit etre arrete pour effacer l'affichage)") ;  sc.AddMessageToLog(message,1) ;  }          
				}
				
				// Bouton AutoTrade
				if ( sc.MenuEventID == ACS_BUTTON_52  or  External_Input_ACS_BUTTON_52.GetInt() != 0 )  		// bouton A
				{
					External_Input_ACS_BUTTON_52.SetInt( std::max(External_Input_ACS_BUTTON_52.GetInt()-1, 0) ) ; // on retranche 1 à External_Input_ACS_BUTTON_52
					
					if ( AutoTradeStatus == 0 )  // OFF => démarrage du robot
					{
						if ( AffichageONOFF == 1 )  // la study doit être affichée quand on démarre AutoTrade
						{
							if ( SequenceONOFF_Input.GetIndex() == 0 )  // Sequence = ON  
							{
								AutoTradeStatus = 10 ; 
								// MyEP = 999999 ;
								AutoTradeDemandeArret = 0 ;  AutoTradeFACAO = 0 ;  AutoTradeFACAO_MessageDansLog = 0 ;  ParentOrderID_EchecGetOrder_MessageDansLog = 0 ;  Target1OrderID_EchecGetOrder_MessageDansLog = 0 ;  Stop1OrderID_EchecGetOrder_MessageDansLog = 0 ;  PrixDuTP = 0 ;  TP_de_la_precedente_modification_de_TP = 0 ;  External_Input_ACS_BUTTON_90.SetInt(0) ;     // RAZ des variables d'état
								SequenceStatus = 1;  // le 1er trade sur trois est lancé   // l'ouverture d'ordre ne se fait pas dans la même milliseconde car il y d'abord un TriggerRecalculate pour calculer le nouveau NiveauFutureAjuste
								SetButton52And56Text (ptrsc) ;
								EffacementDesUDD(ptrsc) ; 
								CreationSCDateHoraireExpiration(ptrsc) ;
								{  SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | Demarrage d'une sequence | SequenceStatus = 1") ;  sc.AddMessageToLog(message,0) ;  }    
								
								// Modification de l'ajustement choisi
								AjustementChoisiEnPas = Position * round( 4 * TP_AUC * (1+BonusIntrinseque/400.0) ) ;     // TP_AUC = TPNu * SI * MultiplicateurPrix    exprimé en points    // La valeur de TP_AUC (variable persistante) a été calculée au précédent appel du DLL.   	
								AjustementChoisiEnPas_Input.SetCustomInputIndex(22-AjustementChoisiEnPas);	 
								TriggerRecalculate = 1 ;    // demande un recalcul immédiat
								return;
							}
							else    // trade sans séquence
							{
								AutoTradeStatus = 10 ; 
								// MyEP = 999999 ;
								AutoTradeDemandeArret = 0 ;  AutoTradeFACAO = 0 ;  AutoTradeFACAO_MessageDansLog = 0 ;  ParentOrderID_EchecGetOrder_MessageDansLog = 0 ;  Target1OrderID_EchecGetOrder_MessageDansLog = 0 ;  Stop1OrderID_EchecGetOrder_MessageDansLog = 0 ;  PrixDuTP = 0 ;  TP_de_la_precedente_modification_de_TP = 0 ;  External_Input_ACS_BUTTON_90.SetInt(0) ;       // RAZ des variables d'état
								SetButton52And56Text (ptrsc) ;
								EffacementDesUDD(ptrsc) ; 
								CreationSCDateHoraireExpiration(ptrsc) ;
							}
						}
						else  // on a vraisemblablement cliqué sur AutoTrade par erreur
						{  SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | !! WARNING !! Le bouton AutoTrade a ete clique sans que la study soit tracee") ;  sc.AddMessageToLog(message,1) ;  }          
					}

					else if ( AutoTradeStatus == 50 )  // Ordres et position fermés
					{
						AutoTradeStatus = 0 ; 
						SetButton52And56Text (ptrsc) ;
						EffacementDesUDD(ptrsc) ;    // effacement des UDD sur le graphe 16 ticks
					}
					
					else if ( AutoTradeStatus == 100 or AutoTradeStatus == 110 or AutoTradeStatus == 120 or AutoTradeStatus == 130 or AutoTradeStatus == 140 or AutoTradeStatus == 150 or AutoTradeStatus == 160 or AutoTradeStatus == 170 or AutoTradeStatus == 180 or AutoTradeStatus == 190 or AutoTradeStatus == 200 )  // Aucun ordre ou position n'a été ouvert (le robot s'est arrêté avant)
					{
						AutoTradeStatus = 0 ; 
						SetButton52And56Text (ptrsc) ;
						EffacementDesUDD(ptrsc) ;  
					}
					
					else if ( AutoTradeStatus == 11 )  // Robot en attente que ses paramètres soient OK pour ouvrir position  => Aucun ordre ou position n'a été ouvert
					{
						AutoTradeStatus = 0 ; 
						SequenceStatus = 0 ;
						SetButton52And56Text (ptrsc) ;
						EffacementDesUDD(ptrsc) ; 
						Texte.Format("") ; DrawText_RelativePosition_UTAMAOA (Texte, 0, 0, 12, 0, ptrsc, 10041+10000*ThisStudyID) ;  // suppression du DrawText_RelativePosition_UTAMAOA "AutoTrade : demarre"
					}
					
					else if ( AutoTradeStatus == 12 )  // Aucun ordre ou position n'a été ouvert (le robot s'est arrêté avant : pb avec AutoTradingEnabled)
					{
						if ( sc.IsAutoTradingEnabled and sc.IsAutoTradingOptionEnabledForChart )
						{
							AutoTradeStatus = 0 ; 
							SequenceStatus = 0 ;
							SetButton52And56Text (ptrsc) ;
							EffacementDesUDD(ptrsc) ;    
						}
						else  // il y a encore un pb non résolu lié à AutoTradingEnabled  // AutoTradeStatus reste à 12
						{
							SCString message ;   message.Format("ID:%d | !! ERREUR !! Auto Trading Enabled (Global ou Chart) est OFF", ThisStudyID) ;    sc.AddMessageToLog(message,1) ; 
						}
					}
					
					else if ( AutoTradeStatus == 15 )  // Plantage de soumission d'ordre => Aucun ordre ou position n'a été ouvert
					{
						AutoTradeStatus = 0 ; 
						SetButton52And56Text (ptrsc) ;
						EffacementDesUDD(ptrsc) ; 
					}

					else if ( AutoTradeStatus == 1000 )  // après un FlattenAndCancelAllOrders
					{
						AutoTradeStatus = 0 ; 
						SetButton52And56Text (ptrsc) ;
						EffacementDesUDD(ptrsc) ;  
					}

					else // autres AutoTradeStatus  (ne pas présumer de AutoTradeStatus pour demander une fermeture)
					{
						AutoTradeDemandeArret = 1 ;
						SequenceStatus = 0 ;  // sinon la séquence reprend après l'arrêt du 1er ou du 2nd trade
						// Ne pas modifier le texte du bouton 52 car il se peut que la demande d'arrêt soit refusée
						AutoTradeEstArreteManuellement = 1 ;
					}
				}
			
				// Bouton "Sequence"
				if (sc.MenuEventID == ACS_BUTTON_56 )  
				{ 	
					if ( RobotActif == 0 )	 
					{
						SequenceONOFF_Input.SetCustomInputIndex( 1-SequenceONOFF_Input.GetIndex() ) ;  // inversion de ON/OFF
						SequenceStatus = 0 ;
						SetButton52And56Text(ptrsc);
						// RAZ de l'ajustement le cas échéant
						if ( AjustementChoisiEnPas_Input.GetIndex() != 22 )	{ AjustementChoisiEnPas_Input.SetCustomInputIndex(22);  sc.SetCustomStudyControlBarButtonText(28, "  busy  ") ;  TriggerRecalculate = 1 ;    return ;  }  
					}
					else // le robot est actif => on n'autorise le clic que si ce n'est pas un robot en file d'attente ( AutoTradeStatus != 11 )
					{  
						if ( SequenceONOFF_Input.GetIndex() == 0 )    // Sequence = ON    
						{
							if ( AutoTradeStatus != 11 )    // le bouton Seq a été cliqué alors que " Sequence = ON " et le robot a un ordre actif ( AutoTradeStatus != 11 ) => on bascule sur le trade suivant de la séquence
							{
								AutoTradeDemandeArret = 1 ;   // le clic sur "Seq : ON" n'arrête pas le robot mais lance une demande d'arrêt du trade (n°1, n°2 ou n°3)   // Ne surtout pas modifier SequenceStatus ici : le robot s'en charge    // Ne pas modifier le texte du bouton 52 car il se peut que la demande d'arrêt soit refusée
								SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | Arret manuel du ''Trade %d'' de la sequence", SequenceStatus) ;  sc.AddMessageToLog(message,0) ;
								if ( SequenceStatus == 3 )  { AutoTradeEstArreteManuellement = 1 ;  }
							}
							else if ( AutoTradeStatus == 11 )  // le bouton Seq a été cliqué alors que " Sequence = ON " et le robot est en attente ( AutoTradeStatus == 11 ) => il ne faut pas lancer de AutoTradeDemandeArret car cela conduirait à forcer à mettre un ordre dans le CO pour le trade suivant de la séquence
							{
								SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | !! WARNING !! Le bouton ''Seq'' a ete clique alors que le robot est actif et en attente (i.e. AutoTradeStatus==11). Le robot ne doit pas etre en attente pour cliquer ''Seq''.") ;  sc.AddMessageToLog(message,1) ;
							}
						}
						else // Sequence = OFF  
						{	SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | !! WARNING !! Le bouton ''Seq : OFF'' a ete clique alors que le robot est actif. Le robot doit etre arrete pour basculer sur ''Seq : ON''.") ;  sc.AddMessageToLog(message,1) ;  }
					} 
				} 
				
				// Bouton "Void"
				if (sc.MenuEventID == ACS_BUTTON_61 )  
				{ 	
					if ( RobotActif == 0 )	
					{
						Void = 1 ; // contrôle de certains affichages
						AffichageONOFF = 0 ; AffichageONOFF_Input.SetCustomInputIndex(1) ; sc.HideStudy=1;
						NiveauFutureAjuste = 0 ;  NiveauIndiceAjuste = 0 ;   // pour que les triangles soient tracés à l'ordonnée 0 par les fonctions qui tracent un affichage permanent
						EffacementDesUDD_GraphesExternes(ptrsc);
						sc.SetCustomStudyControlBarButtonText(61, "   busy   ") ;
						AutoTradeStatus = 0 ;
						TriggerRecalculate = 1 ;   
						return ;  
					}
					else
					{  SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | !! WARNING !! Le bouton '' Void '' a ete clique alors que le robot est actif (le robot doit etre arrete pour que ce bouton soit operant)") ;  sc.AddMessageToLog(message,1) ;  } 
				} 
			
				// Bouton "Niv -" ou bouton "Niv +"
				if ( sc.MenuEventID == ACS_BUTTON_109  or  sc.MenuEventID == ACS_BUTTON_110  or  External_Input_ACS_BUTTON_109_110.GetInt() != 0 )       // Quelque soit le bouton enfoncé (Niv- ou Niv+), AMR fait la même chose : récupérer le nouveau niveau   
				{ 	
					if ( RobotActif == 0 )
					{	
						// on s'assure que l'affichage est ON, on réinitialise AutoTradeStatus et AjustementChoisi
						AffichageONOFF_Input.SetCustomInputIndex(0);    
						AutoTradeStatus = 0 ;
						AffichageONOFF = 1 ;  
						sc.HideStudy=0;
						Void = 0 ;
						AjustementChoisiEnPas_Input.SetCustomInputIndex(22);  // On remet l'ajustement à 0.
						
						// Modification du niveau
						if ( NiveauFuture == 0 )  // si suite à une mauvaise manoeuvre NiveauFuture vaut 0, alors on le remet sur le dernier prix du future
						{
							float PriceOfLastVisibleBar = sc.Close[sc.IndexOfLastVisibleBar] ;
							NiveauFutureTexte.SetFloat(PriceOfLastVisibleBar) ;  
							FutureOuIndice_Input.SetCustomInputIndex(1) ; // Niveau Future
							NatureNiveau_Input.SetInt(0) ; // on considère que la nature du niveau est un niveau psy
							SousNatureNiveau_Input.SetInt(0) ; 
							RangePrecedent_Input.SetFloat(0) ; 
							Texte.Format("%.2f", PriceOfLastVisibleBar) ;  AppendFutOuInd (Texte, 0) ;  NomDuNiveauTrade.SetString(Texte) ;
						}
						else // cas général : on récupère les paramètres du Niveau_ApresClic
						{
							NiveauFuture = Get_PrixSurFuture_ApresClic(ptrsc) ;  NiveauFutureTexte.SetFloat(NiveauFuture);  
							NiveauIndice = Get_PrixSurIndice_ApresClic(ptrsc) ;  NiveauIndiceTexte.SetFloat(NiveauIndice);
							SCString Nom_ApresClic = Get_Nom_ApresClic(ptrsc) ;   
							if ( NiveauIndice == 0 )  { FutureOuIndice_Input.SetCustomInputIndex(1) ;  AppendFutOuInd(Nom_ApresClic, 0) ;  }  else  { FutureOuIndice_Input.SetCustomInputIndex(0) ;  AppendFutOuInd(Nom_ApresClic, 1) ;  }   // Ajout de "Fut" ou "Ind" selon que PrixSurIndice_ApresClic soit nul ou pas
							NomDuNiveauTrade.SetString(Nom_ApresClic);
							int NatureNiveau = Get_NatureNiveau_ApresClic(ptrsc) ;  NatureNiveau_Input.SetInt(NatureNiveau) ;			// la study NavigNiveaux est calculée avant AMR => Get_NatureNiveau_ApresClic récupère le niveau nouvellement calculé
							int SousNatureNiveau = Get_SousNatureNiveau_ApresClic(ptrsc) ;  SousNatureNiveau_Input.SetInt(SousNatureNiveau) ;			// la study NavigNiveaux est calculée avant AMR => Get_SousNatureNiveau_ApresClic récupère le niveau nouvellement calculé
							float RangePrecedent = Get_RangePrecedent_ApresClic(ptrsc) ;  RangePrecedent_Input.SetFloat(RangePrecedent) ;			// la study NavigNiveaux est calculée avant AMR => Get_RangePrecedent_ApresClic récupère le niveau nouvellement calculé
						}
						
						// Si le niveau tradé est un PP 1h, on met HoraireExpiration à H+55
						if ( NatureNiveau_Input.GetInt() == 1 )  Set_HoraireExpiration_PP1h(ptrsc) ;
						else RAZHoraireExpiration(ptrsc) ;
						
						// Finalisation
						sc.SetCustomStudyControlBarButtonText(109, "busy") ;  sc.SetCustomStudyControlBarButtonText(110, "busy") ;
						EffacementDesUDD(ptrsc) ;
						External_Input_ACS_BUTTON_109_110.SetInt( std::max(External_Input_ACS_BUTTON_109_110.GetInt()-1, 0) ) ; // on retranche 1 à External_Input_ACS_BUTTON_109_110
						TriggerRecalculate = 1 ;   return ;  
					}
					else	
					{  
						SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | !! WARNING !! Le bouton '' Niv - '' ou '' Niv + '' a ete clique alors que le robot est actif (le robot doit etre arrete pour que ce bouton soit operant)") ;  sc.AddMessageToLog(message,1) ;  
						External_Input_ACS_BUTTON_109_110.SetInt(0) ;
					}          
				}  

				// Bouton "CTRL + 5"
				if ( sc.MenuEventID == ACS_BUTTON_133  or  sc.MenuEventID == ACS_BUTTON_134  or  External_Input_ACS_BUTTON_133_134.GetInt() != 0 )     //  ACS_BUTTON_133 = CTRL + Numpad 5  		// ACS_BUTTON_134 = CTRL + 5
				{ 	
					// External_Input_ACS_BUTTON_133_134 sera RAZ dans le bloc " if ( LaStudyVientDEtreRajoutee = 2 ) ". Il ne faut pas RAZ dans ce bloc.

					if ( RobotActif == 0 )
					{	
						// au prochain appel du DLL, le trigger LaStudyVientDEtreRajoutee va faire recalculer le niveau indice le plus proche
						LaStudyVientDEtreRajoutee = 2 ; 		

						// on s'assure que l'affichage est ON, on réinitialise AutoTradeStatus, AjustementChoisi, et les principaux paramètres
						AffichageONOFF_Input.SetCustomInputIndex(0);    
						AutoTradeStatus = 0 ;
						AffichageONOFF = 1 ;  
						sc.HideStudy=0;
						Void = 0 ;
						AjustementChoisiEnPas_Input.SetCustomInputIndex(22);  // On remet l'ajustement à 0.
						EffacementDesUDD(ptrsc) ;
						
						// on en profite pour modifier également HoraireDeRAZ
						{	
							int IndexLastVisibleBar_Chart6 ;
							if ( sc.MenuEventID == ACS_BUTTON_133  or  sc.MenuEventID == ACS_BUTTON_134 )  { IndexLastVisibleBar_Chart6 = sc.IndexOfLastVisibleBar ;  }
							else 	// External_Input_ACS_BUTTON_9.GetInt() != 0 
							{
								IndexLastVisibleBar_Chart6 = External_Input_ACS_BUTTON_133_134.GetInt() ;
							}
							if ( TPNu_Manuel_Input.GetFloat() == 0 ) TPNu = ArrayTPNu[IndexLastVisibleBar_Chart6] ;  else TPNu = TPNu_Manuel_Input.GetFloat() ;
							NbBarresVirginite = ceil ( 250 * TPNu / 4 ) ; 
							SCDateTime HoraireDeRAZ = sc.BaseDateTimeIn[IndexLastVisibleBar_Chart6-2*NbBarresVirginite] ;		// on soustrait deux fois NbBarresVirginite pour gagner en visibilité
							
							// Réinjection de la nouvelle valeur de HoraireRAZTexte 
							int Hour = HoraireDeRAZ.GetHour() ;
							int Minute = HoraireDeRAZ.GetMinute() ;
							HoraireRAZTexte.SetFloat(Hour*100+Minute) ;   // Format hhmm.ss
							
							// Réinjection de la nouvelle valeur de DateRAZ dans les inputs DateEtudiee et DateEtudieeRoulette
							DateRAZ = HoraireDeRAZ.GetDate() ;  
							DateEtudiee.SetDate(DateRAZ) ;
							SCString DateEtudieeInputString ;  CreationDeDateEtudieeInputString (TailleDuCustomInputDeDateRAZ, DateEtudieeInputString) ;
							DateEtudieeRoulette.SetCustomInputStrings(DateEtudieeInputString) ;
							DateLocale = CalculDateLocale() ;     // DateLocale est un int (date value)
							DateEtudieeRoulette.SetCustomInputIndex(DateLocale - DateRAZ);
						}
						
						TriggerRecalculate = 1 ;   return ;  
					}
					else	
					{  
						SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | !! WARNING !! Le bouton ''CTRL + 5'' a ete clique alors que le robot est actif (le robot doit etre arrete pour que ce bouton soit operant)") ;  sc.AddMessageToLog(message,1) ;  
					}          
				}  
				
			}
		}
		else // pas de bouton appuyé
		{ sc.UpdateAlways = 0; }    
	}
	
	
	// Test de TriggerRecalculate pour déterminer PremierIndex
	{
		if ( TriggerRecalculate )
		{ PremierIndex = 0 ;  TriggerRecalculate = 0 ;  }
		else
		{ PremierIndex = sc.UpdateStartIndex ; }
	}

	
	// Index d'arrêt du manual looping : IndexArretManualLooping
	int LastIndexLooping ;
	{
		if ( RobotActif )																			// si RobotActif, le looping doit être poursuivi indéfiniment pour gérer le stop loss
		{	LastIndexLooping = sc.ArraySize ;  }   						
		else if ( IndexArretManualLooping == 0 )    								// valeur initiale de IndexArretManualLooping
		{	LastIndexLooping = sc.ArraySize ;  }
		else if ( IndexArretManualLooping == -1 )    								// il y a eu une condition d'arrêt du manual looping (arrêt à 16h00 pour le trading indice )
		{	LastIndexLooping = -1 ;  }
		else if ( IndexArretManualLooping == -2 )    								// il y a eu une condition d'arrêt du manual looping (Position == 0)
		{	LastIndexLooping = -2 ;  }
		else 																							// il y a eu une condition d'arrêt du manual looping ( TP ou SL dépassé à IndexArretManualLooping )
		{	LastIndexLooping = IndexArretManualLooping  ;  }					
	}
	
	
	// MANUAL LOOPING   //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for ( int index = PremierIndex ;  index < LastIndexLooping ;  index++ )        
	{
		if (  AffichageONOFF  and  sc.BaseDateTimeIn[index] >= SCDateHoraireRAZ 	)  
		{
			// Calcul de NiveauIndiceSurFutureTempsReel
			if ( FutureOuIndice == 1  and  sc.BaseDateTimeIn[index] < SCDateHoraireFinSeanceAction  and  TriggerRAZ == 0 )    // tracé jusqu'à 15h59m50s   // si TriggerRAZ vaut 1 alors NiveauIndice n'est pas encore connu. 
			{	
				NiveauIndiceSurFutureTempsReel[index] = NiveauIndice + SpreadMoyen[index] ;
				AjustementSubiPoints = NiveauFuture - NiveauIndiceSurFutureTempsReel[index] ;        // en points
				NiveauIndiceAjuste = NiveauIndice + AjustementChoisiEnPas*0.25 + AjustementSubiPoints ;
			}
			
			// Récupération du TPNu
			if ( TPNu_Manuel_Input.GetFloat() == 0 ) TPNu = ArrayTPNu[index]  ; else TPNu = TPNu_Manuel_Input.GetFloat() ;
			
			// SpreadIndex, ContributionSpreadIndex, CoeffMalusAjustement, TP_AUC
			SpreadIndex = ArraySpreadIndex[index] ;
			if ( NatureNiveau_Input.GetInt() == 0  and  SpreadIndex > 1.3 )  Malus_SpreadIndex = std::max(0.0, SpreadIndex-1.3)/0.3 ;		// uniquement pour niveaux psys		// normé à 1		// vaut 0 si SpreadIndex<1.3 ; vaut 1.0 si SpreadIndex=1.6
			TP_AUC = TPNu / 4 * MultiplicateurPrix * TailleBougie_4000Ticks(SpreadIndex, ptrsc) / TailleBougie_4000Ticks(1, ptrsc) ;		// TP_AUC est exprimé en points. C'est l'amplitude utilisée pour le calcul d'AUC.
			CoeffMalusAjustement = 90000 / sc.Close[index] / sqrt(2*SpreadIndex-1)  ; 
			
			// Calcul des variables spécifiques au TPNu
			{
				NbBarresVirginite = ceil ( 250 * TPNu / 4 ) ;    // 4000 ticks pour TPNu = 1 pt      
				if ( MalusNRV_Manuel_Input.GetFloat() == 0 )  // MalusDeNRV et MalusDePremierPassageJournalier sont calculés automatiquement
				{
					// nouveaux malus
					{
						if ( TPNu < 3 )   
						{ MalusDeNRV=5*TPNu-15 ; MalusDePremierPassageJournalier= 10 ; }
						else if ( TPNu < 4 )   
						{ MalusDeNRV=18*TPNu-54 ; MalusDePremierPassageJournalier= -6*TPNu+28 ; }
						else if ( TPNu < 5 )   
						{ MalusDeNRV=12*TPNu-30 ; MalusDePremierPassageJournalier= 11*TPNu-40 ; }
						else if ( TPNu < 6 )   
						{ MalusDeNRV=4*TPNu+10 ; MalusDePremierPassageJournalier= 6*TPNu-15 ; }
						else if ( TPNu < 7 )   
						{ MalusDeNRV=2*TPNu+22 ; MalusDePremierPassageJournalier= 8*TPNu-27 ; }
						else if ( TPNu < 8 )   
						{ MalusDeNRV=14*TPNu-62 ; MalusDePremierPassageJournalier= 6*TPNu-13 ; }
						else if ( TPNu < 12 )   
						{ MalusDeNRV=10*TPNu-30 ; MalusDePremierPassageJournalier= 5*TPNu-5 ; }
						else
						{ MalusDeNRV = 1000 ;  MalusDePremierPassageJournalier = 1000 ;  }
					}
				}
				else // MalusDeNRV est rentré manuellement
				{
					MalusDeNRV = MalusNRV_Manuel_Input.GetFloat() ;
					MalusDePremierPassageJournalier = 0 ;
				}
				TauxVirginite = fmin (1, fmax ( 0.25, (float(index - IndexHoraireRAZ)) / NbBarresVirginite ) ) ;				// Il est très peu probable qu'on lance un calcul avec TauxVirginite réel inférieur à 25%, c'est pourquoi on met un plancher de 0.25 (via fmax) dans la formule, pour éviter la division par un TauxVirginite proche de 0. On met un plafond à 1 car ce taux va servir pour diviser RatioAUC.
			}
			
			// BonusExcedentaire et  BonusExcedentaireSuffisant
			float AjustementAbsolu = fabs(0.25*AjustementChoisiEnPas+AjustementSubiPoints) ;
			float BonusAvantAjustement = BonusIntrinseque - MalusDeNRV - NewDailyHighLow_Input.GetYesNo() * MalusDePremierPassageJournalier ;
			if ( MalusAjustementParabolique_Input.GetIndex() == 0 )   // Modèle linéaire
			{
				// BonusExcedentaire (identique pour future et indice)
				BonusExcedentaire = BonusAvantAjustement - AjustementAbsolu*CoeffMalusAjustement ;
				// BonusExcedentaireSuffisant
				BonusExcedentaireSuffisant = BonusExcedentaire - AjustementAbsolu*CoeffMalusAjustement ;
			}
			else if ( MalusAjustementParabolique_Input.GetIndex() == 1 )   // Modèle Parabolique
			{
				// BonusExcedentaire (identique pour future et indice)
				BonusExcedentaire = BonusAvantAjustement - AjustementParabolique(BonusIntrinseque, AjustementAbsolu, CoeffMalusAjustement) ;		// float AjustementParabolique (float Bonus, float AjustementAbsolu, float CoeffMalusAjustement)  
				// BonusExcedentaireSuffisant
				BonusExcedentaireSuffisant = BonusExcedentaire - AjustementParabolique(BonusIntrinseque, AjustementAbsolu, CoeffMalusAjustement) ;
			}
			MalusAjustementAbsolu = BonusAvantAjustement - BonusExcedentaire ;  
			
			// Calcul de TPOptimum
			TPOptimum = TP_AUC * (1+BonusExcedentaire/100) ; 
			
			// Calcul de TPOptimumArrondi, SL, DistanceAuPHPB_SurSL
			if ( TPOptimum > TPOptimumArrondi + 0.25  or  TPOptimum < TPOptimumArrondi - 0.125 ) 		// ici TPOptimumArrondi est le TPOptimumArrondi du précédent appel de la variable persistente
			{ 
				TPOptimumArrondi = floor(4*TPOptimum)/4 ; 
				TPOptimumArrondi = std::max (0.25f, TPOptimumArrondi ) ;
			
				// Calcul du SL (nécessaire pour le robot de trading, l'affichage en temps réel du SL et pour le calcul de RatioDistanceAuPHPB) 
				if 			( TPOptimumArrondi == 0.75 )  	{ SL = 4.5 ; }
				else if 	( TPOptimumArrondi > 0.75 )  	{ SL = 4*TPOptimumArrondi + 2 ; }
				else  													{ SL = 4.5 ; }     // avant la v5.9.3, les SL pour des TP < 0.75 étaient différents, mais comme on a décidé que TP ne pouvait pas être inférieur à 0.75, on y remet le même SL que pour TP 0.75, càd SL=4.5			
			}
			DistanceAuPHPB_SurSL = Get_DistanceAuPHPB_SurSL(IndexHoraireRAZ, SL, ptrsc) ;     // Compris entre 0 et 1 (100%). Une valeur "supérieure" à 1 est ramenée à 1.  	// DistanceAuPHPB_SurSL doit être calculé à chaque index à cause du flottement du Spread FutureCash
			
			
			// Calcul des seuils de AUC_Future et AUC_Indice (pour modèles 7, 8, 9, ...)
			float SeuilAUC_Future = 46*pow(TPNu/4,2) * ( 1 + BonusExcedentaire/100 ) ;		// 46 = 20*2.3		// 3.492/1.518=2.3
			float SeuilAUC_Indice = 20*pow(TPNu/4,2) * (1 + BonusExcedentaire/100 + 3*BonusExcedentaire*fabs(BonusExcedentaire)/10000) ;
			
			
			
			// BOUCLE TRIGGERRAZ (boucle effectué une seule fois, quand on dépasse SCDateHoraireRAZ)  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			if ( TriggerRAZ ) 
			{ 
				TriggerRAZ = 0 ;
				IndexHoraireRAZ = index ;
				
				
				// CALCUL DU NIVEAU CIBLE (SI MODIFICATION D'UN INPUT)    ////////////////////////////////////////////////////////////////////
				{
					// Calcul du nouveau NiveauFuture (suite à modification de NiveauFutureTexte)
					if ( NiveauFuture != NiveauFutureTexte.GetFloat() ) 
					{
						NiveauFuture = NiveauFutureTexte.GetFloat() ;  
						if ( FutureOuIndice == 0 ) // trading future    // on ne modifie pas PeriodeNiveauTradeInput, ni QuelNiveauTradeInput si on trade l'indice, ni RAZHoraireExpiration
						{ 
							PeriodeNiveauTradeInput.SetCustomInputIndex(6) ;  QuelNiveauTradeInput.SetCustomInputIndex(0) ; 
							Texte.Format("%.2f", NiveauFuture) ;  AppendFutOuInd (Texte, FutureOuIndice) ;
							NomDuNiveauTrade.SetString(Texte) ;
							RAZHoraireExpiration(ptrsc) ; 
						}
					}
					
					// Calcul du nouveau NiveauFuture (suite à modification de NiveauFutureRoulette)
					else if ( NiveauFutureRoulette.GetIndex() !=  TailleDuCustomInputDeNiveauFuture ) 
					{
						NiveauFuture = NiveauFuture + 0.25*( TailleDuCustomInputDeNiveauFuture - static_cast<int> (NiveauFutureRoulette.GetIndex())  ) ;      // GetIndex retourne un <unsigned int>, puis <unsigned int> + <int> = <unsigned int>
						if ( FutureOuIndice == 0 ) // trading future    // on ne modifie pas PeriodeNiveauTradeInput, ni QuelNiveauTradeInput si on trade l'indice, ni RAZHoraireExpiration
						{ 
							PeriodeNiveauTradeInput.SetCustomInputIndex(6);  QuelNiveauTradeInput.SetCustomInputIndex(0);  
							Texte.Format("%.2f", NiveauFuture) ;  AppendFutOuInd (Texte, FutureOuIndice) ;
							NomDuNiveauTrade.SetString(Texte) ;
							RAZHoraireExpiration(ptrsc) ; 
						}
					}
					
					else // Calcul du NiveauFuture s'il est issu d'un point pivot Future, et  calcul du NiveauIndice (mode texte ou issu d'un point pivot indice)
					{
						IndexQuelNiveau = QuelNiveauTradeInput.GetIndex() ;  
						SCFloatArray ArrayPointPivot;
						int La_Study_Appellee_N_A_Pas_Ete_Trouvee = 0 ; 
						
						if ( FutureOuIndice == 0)   // Trading Future
						{
							int StudyIDVWAPsAffichage = sc.GetStudyIDByName(sc.ChartNumber, "VWAPs affichage JHM", 0) ;   if ( StudyIDVWAPsAffichage == 0 )  { La_Study_Appellee_N_A_Pas_Ete_Trouvee = 2 ;  }
							
							// Calcul du NiveauFuture s'il est issu d'un point pivot Future
							if ( IndexPeriode == 1 )  // 1h
							{
								int StudyIDPointPivot = sc.GetStudyIDByName(sc.ChartNumber, "[5] Pivots Fut 1h (Full)", 0) ;     if ( StudyIDPointPivot == 0 )  { La_Study_Appellee_N_A_Pas_Ete_Trouvee = 1 ;  }
								if ( IndexQuelNiveau == 1 ) // R4
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 4, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("R4 1h") ;  AppendFutOuInd (Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  Set_HoraireExpiration_PP1h(ptrsc) ; }
								else	if ( IndexQuelNiveau == 2 ) // R3
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 3, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("R3 1h") ;  AppendFutOuInd (Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  Set_HoraireExpiration_PP1h(ptrsc) ; }
								else if ( IndexQuelNiveau == 3 ) // R2
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 2, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("R2 1h") ;  AppendFutOuInd (Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  Set_HoraireExpiration_PP1h(ptrsc) ; }
								else	if ( IndexQuelNiveau == 4 ) // R1
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 1, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("R1 1h") ;  AppendFutOuInd (Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  Set_HoraireExpiration_PP1h(ptrsc) ; }
								else if ( IndexQuelNiveau == 5 ) // PP
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 0, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("PP 1h") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  Set_HoraireExpiration_PP1h(ptrsc) ; }
								else	if ( IndexQuelNiveau == 6 ) // S1
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 5, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("S1 1h") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  Set_HoraireExpiration_PP1h(ptrsc) ; }
								else if ( IndexQuelNiveau == 7 ) // S2
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 6, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("S2 1h") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  Set_HoraireExpiration_PP1h(ptrsc) ; }
								else	if ( IndexQuelNiveau == 8 ) // S3
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 7, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("S3 1h") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  Set_HoraireExpiration_PP1h(ptrsc) ; }
								else if ( IndexQuelNiveau == 9 ) // S4
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 8, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("S4 1h") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  Set_HoraireExpiration_PP1h(ptrsc) ; }    // l'écriture de " NomDuNiveauTrade.SetString(Texte) ; " à chaque bloc peut paraître redondante, mais elle est indispensable     // l'écriture de " Set_HoraireExpiration_PP1h(ptrsc) ; } " à chaque bloc peut paraître redondante, mais elle permet de ne fixer HoraireExpiration qu'au moment du choix du niveau
								QuelNiveauTradeInput.SetCustomInputIndex(10) ;
							}

							else if ( IndexPeriode == 2 )  // 4h
							{
								int StudyIDPointPivot = sc.GetStudyIDByName(sc.ChartNumber, "[4] Pivots Fut 4h (Full)", 0) ;     if ( StudyIDPointPivot == 0 )  { La_Study_Appellee_N_A_Pas_Ete_Trouvee = 1 ;  }
								if ( IndexQuelNiveau == 1 ) // R4
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 4, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("R4 4h") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  Set_HoraireExpiration_PP4h(ptrsc) ; }
								else	if ( IndexQuelNiveau == 2 ) // R3
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 3, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("R3 4h") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  Set_HoraireExpiration_PP4h(ptrsc) ; }
								else if ( IndexQuelNiveau == 3 ) // R2
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 2, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;	 Texte.Format("R2 4h") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  Set_HoraireExpiration_PP4h(ptrsc) ; }
								else	if ( IndexQuelNiveau == 4 ) // R1
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 1, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("R1 4h") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  Set_HoraireExpiration_PP4h(ptrsc) ; }
								else if ( IndexQuelNiveau == 5 ) // PP
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 0, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("PP 4h") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  Set_HoraireExpiration_PP4h(ptrsc) ; }
								else	if ( IndexQuelNiveau == 6 ) // S1
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 5, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("S1 4h") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  Set_HoraireExpiration_PP4h(ptrsc) ; }
								else if ( IndexQuelNiveau == 7 ) // S2
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 6, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("S2 4h") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  Set_HoraireExpiration_PP4h(ptrsc) ; }
								else	if ( IndexQuelNiveau == 8 ) // S3
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 7, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("S3 4h") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  Set_HoraireExpiration_PP4h(ptrsc) ; }
								else if ( IndexQuelNiveau == 9 ) // S4
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 8, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("S4 4h") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  Set_HoraireExpiration_PP4h(ptrsc) ; }
								QuelNiveauTradeInput.SetCustomInputIndex(10) ;
							}

							else if ( IndexPeriode == 3 )  // Jour
							{
								int StudyIDPointPivot = sc.GetStudyIDByName(sc.ChartNumber, "[3] Pivots Fut J (Full/Mid)", 0) ;     if ( StudyIDPointPivot == 0 )  { La_Study_Appellee_N_A_Pas_Ete_Trouvee = 1 ;  }
								if ( IndexQuelNiveau == 1 ) // R4
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 7, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("R4 J") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 2 ) // mR4
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 15, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("mR4 J") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 3 ) // R3
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 6, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("R3 J") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 4 ) // mR3
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 14, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("mR3 J") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 5 ) // R2
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 5, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("R2 J") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 6 ) // mR2
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 13, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("mR2 J") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 7 ) // R1
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 4, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("R1 J") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 8 ) // mR1
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 12, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("mR1 J") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 9 ) // PP
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 3, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("PP J") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 10 ) // mS1
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 16, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("mS1 J") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 11 ) // S1
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 8, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("S1 J") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 12 ) // mS2
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 17, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("mS2 J") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 13 ) // S2
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 9, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("S2 J") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 14 ) // mS3
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 18, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("mS3 J") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 15 ) // S3
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 10, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("S3 J") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 16 ) // mS4
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 19, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("mS4 J") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 17 ) // S4
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 11, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("S4 J") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 18 ) // PH
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 0, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("PH JP") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 19 ) // PB
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 1, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("PB JP") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 20 ) // Settlement
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 2, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("Settlement JP") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 21 ) // VWAP
								{ sc.GetStudyArrayUsingID(StudyIDVWAPsAffichage, 1, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("VWAP J") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								QuelNiveauTradeInput.SetCustomInputIndex(22) ;
							}

							else if ( IndexPeriode == 4 )  // Hebdo
							{
								int StudyIDPointPivot = sc.GetStudyIDByName(sc.ChartNumber, "[2] Pivots Fut H (Full/Mid/Quart)", 0) ;     if ( StudyIDPointPivot == 0 )  { La_Study_Appellee_N_A_Pas_Ete_Trouvee = 1 ;  }
								if ( IndexQuelNiveau == 1 ) // R4
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 7, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("R4 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 2 ) // mR4
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 15, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("mR4 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 3 ) // R3
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 6, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("R3 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 4 ) // mR3
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 14, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("mR3 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 5 ) // R2
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 5, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("R2 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 6 ) // mR2
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 13, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("mR2 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 7 ) // R1
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 4, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("R1 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 8 ) // mR1
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 12, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("mR1 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 9 ) // PP
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 3, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("PP H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 10 ) // mS1
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 16, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("mS1 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 11 ) // S1
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 8, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("S1 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 12 ) // mS2
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 17, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("mS2 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 13 ) // S2
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 9, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("S2 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 14 ) // mS3
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 18, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("mS3 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 15 ) // S3
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 10, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("S3 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 16 ) // mS4
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 19, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("mS4 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 17 ) // S4
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 11, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("S4 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 18 ) // PH
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 0, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("PH HP") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 19 ) // PB
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 1, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("PB HP") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 20 ) // VWAP
								{ sc.GetStudyArrayUsingID(StudyIDVWAPsAffichage, 2, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("VWAP H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								QuelNiveauTradeInput.SetCustomInputIndex(21) ;
							}
							
							else if ( IndexPeriode == 5 )  // Mensuel Full/Mid
							{ 
								int StudyIDPointPivot = sc.GetStudyIDByName(sc.ChartNumber, "[1] Pivots Fut M (Full/Mid/Quart)", 0) ;     if ( StudyIDPointPivot == 0 )  { La_Study_Appellee_N_A_Pas_Ete_Trouvee = 1 ;  }
								if ( IndexQuelNiveau == 1 ) // R4
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 7, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("R4 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 2 ) // mR4
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 15, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("mR4 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 3 ) // R3
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 6, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("R3 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 4 ) // mR3
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 14, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("mR3 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 5 ) // R2
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 5, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("R2 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 6 ) // mR2
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 13, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("mR2 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 7 ) // R1
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 4, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("R1 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 8 ) // mR1
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 12, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("mR1 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 9 ) // PP
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 3, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("PP M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 10 ) // mS1
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 16, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("mS1 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 11 ) // S1
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 8, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("S1 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 12 ) // mS2
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 17, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("mS2 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 13 ) // S2
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 9, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("S2 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 14 ) // mS3
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 18, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("mS3 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 15 ) // S3
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 10, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("S3 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 16 ) // mS4
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 19, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("mS4 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 17 ) // S4
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 11, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("S4 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 18 ) // PH
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 0, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("PH MP") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 19 ) // PB
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 1, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("PB MP") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 20 ) // VWAP
								{ sc.GetStudyArrayUsingID(StudyIDVWAPsAffichage, 3, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("VWAP M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								QuelNiveauTradeInput.SetCustomInputIndex(21) ;
							}

							if ( IndexPeriode == 6 )  // Mensuel Quart
							{ 
								int StudyIDPointPivot = sc.GetStudyIDByName(sc.ChartNumber, "[1] Pivots Fut M (Full/Mid/Quart)", 0) ;     if ( StudyIDPointPivot == 0 )  { La_Study_Appellee_N_A_Pas_Ete_Trouvee = 1 ;  }
								else	if ( IndexQuelNiveau == 1 ) // RQ8
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 27, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("RQ8") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 2 ) // RQ7
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 26, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("RQ7") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 3 ) // RQ6
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 25, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("RQ6") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 4 ) // RQ5
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 24, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("RQ5") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 5 ) // RQ4
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 23, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("RQ4") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 6 ) // RQ3
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 22, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("RQ3") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 7 ) // RQ2
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 21, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("RQ2") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 8 ) // RQ1
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 20, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("RQ1") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 9 ) // SQ1
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 28, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("SQ1") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 10 ) // SQ2
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 29, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("SQ2") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 11 ) // SQ3
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 30, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("SQ3") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 12 ) // SQ4
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 31, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("SQ4") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 13 ) // SQ5
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 32, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("SQ5") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 14 ) // SQ6
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 33, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("SQ6") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 15 ) // SQ7
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 34, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("SQ7") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 16 ) // SQ8
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 35, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  Texte.Format("SQ8") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								QuelNiveauTradeInput.SetCustomInputIndex(17) ;
							}
							
						}  // fin de FutureOuIndice == 0
						
						
						else if (FutureOuIndice == 1 ) // Calcul du NiveauIndice (mode texte ou issu d'un point pivot indice)
						{							
							// Calcul du nouveau NiveauIndice (suite à modification de l'input NiveauIndiceTexte)
							if ( fabs( ( NiveauIndiceTexte.GetFloat() - NiveauIndice ) ) > 0.01 )    // NiveauIndice a été modifié 
							{
								NiveauIndice = NiveauIndiceTexte.GetFloat() ;  
								NatureNiveau_Input.SetInt(0) ; // on considère que la nature du niveau est un niveau psy
								SousNatureNiveau_Input.SetInt(0) ;
								RangePrecedent_Input.SetFloat(0) ;
								// Calcul du NiveauFuture
								NiveauFuture = round ( (NiveauIndice +SpreadMoyen[index]) *2 ) /2 ;
								// Réinitialisation des inputs
								PeriodeNiveauTradeInput.SetCustomInputIndex(6) ;  QuelNiveauTradeInput.SetCustomInputStrings("Non dispo"); QuelNiveauTradeInput.SetCustomInputIndex(0) ;  
								if ( int(NiveauIndice) == NiveauIndice ) // on est sur un nombre entier
								{ Texte.Format("%.0f", NiveauIndice) ;  }
								else { Texte.Format("%.2f", NiveauIndice) ;  }
								AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ; 
							}
							
							// Calcul du NiveauIndice (issu d'un point pivot indice)
							else 	if ( IndexPeriode == 1 )  // Jour
							{ 
								int StudyIDPointPivot = sc.GetStudyIDByName(sc.ChartNumber, "[D] Pivots Ind J (Full/Mid) projetes sur le future", 0) ;     if ( StudyIDPointPivot == 0 )  { La_Study_Appellee_N_A_Pas_Ete_Trouvee = 1 ;  }
								if ( IndexQuelNiveau == 1 ) // R4
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 7, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 7, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("R4 J") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }     //  int GetStudyExtraArrayFromChartUsingID(int ChartNumber, int StudyID, int SubgraphIndex, int ExtraArrayIndex, SCFloatArrayRef ExtraArrayRef);
								else	if ( IndexQuelNiveau == 2 ) // mR4
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 15, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 15, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("mR4 J") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 3 ) // R3
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 6, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;    sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 6, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("R3 J") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 4 ) // mR3
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 14, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;    sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 14, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("mR3 J") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 5 ) // R2
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 5, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;    sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 5, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("R2 J") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 6 ) // mR2
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 13, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;    sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 13, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("mR2 J") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 7 ) // R1
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 4, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;    sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 4, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("R1 J") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 8 ) // mR1
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 12, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;    sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 12, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("mR1 J") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 9 ) // PP
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 3, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ; 	sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 3, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("PP J") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 10 ) // mS1
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 16, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 16, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("mS1 J") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 11 ) // S1
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 8, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 8, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("S1 J") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 12 ) // mS2
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 17, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 17, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("mS2 J") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 13 ) // S2
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 9, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 9, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("S2 J") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 14 ) // mS3
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 18, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 18, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("mS3 J") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 15 ) // S3
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 10, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 10, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("S3 J") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 16 ) // mS4
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 19, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 19, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("mS4 J") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 17 ) // S4
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 11, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 11, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("S4 J") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 18 ) // PH
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 1, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 1, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("PH JP") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 19 ) // PB
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 2, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 2, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("PB JP") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 20 ) // demi-gap
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 37, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 37, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("Demi-Gap") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								QuelNiveauTradeInput.SetCustomInputIndex(21) ;
								NatureNiveau_Input.SetInt(3) ;
							}
								
							else 	if ( IndexPeriode == 2 )  // Hebdo Full/Mid
							{ 
								int StudyIDPointPivot = sc.GetStudyIDByName(sc.ChartNumber, "[C] Pivots Ind H (Full/Mid/Quart) projetes sur le future", 0) ;     if ( StudyIDPointPivot == 0 )  { La_Study_Appellee_N_A_Pas_Ete_Trouvee = 1 ;  }
								if ( IndexQuelNiveau == 1 ) // R4
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 7, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 7, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("R4 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 2 ) // mR4
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 26, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 26, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("mR4 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 3 ) // R3
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 6, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;    sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 6, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("R3 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 4 ) // mR3
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 14, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;    sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 14, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("mR3 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 5 ) // R2
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 5, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;    sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 5, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("R2 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 6 ) // mR2
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 13, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;    sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 13, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("mR2 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 7 ) // R1
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 4, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;    sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 4, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("R1 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 8 ) // mR1
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 12, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;    sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 12, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("mR1 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 9 ) // PP
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 3, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ; 	sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 3, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("PP H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 10 ) // mS1
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 16, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 16, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("mS1 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 11 ) // S1
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 8, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 8, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("S1 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 12 ) // mS2
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 17, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 17, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("mS2 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 13 ) // S2
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 9, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 9, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("S2 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 14 ) // mS3
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 18, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 18, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("mS3 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 15 ) // S3
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 10, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 10, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("S3 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 16 ) // mS4
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 19, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 19, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("mS4 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 17 ) // S4
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 11, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 11, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("S4 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 18 ) // PH
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 1, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 1, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("PH HP") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 19 ) // PB
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 2, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 2, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("PB HP") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								QuelNiveauTradeInput.SetCustomInputIndex(20) ;
								NatureNiveau_Input.SetInt(4) ;
							}
							
							else 	if ( IndexPeriode == 3 )  // Hebdo Quart
							{ 
								int StudyIDPointPivot = sc.GetStudyIDByName(sc.ChartNumber, "[C] Pivots Ind H (Full/Mid/Quart) projetes sur le future", 0) ;     if ( StudyIDPointPivot == 0 )  { La_Study_Appellee_N_A_Pas_Ete_Trouvee = 1 ;  }
								if ( IndexQuelNiveau == 1 ) // RQ8
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 27, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 27, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("RQ8 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 2 ) // RQ7
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 26, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 26, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("RQ7 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 3 ) // RQ6
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 25, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 25, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("RQ6 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 4 ) // RQ5
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 24, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 24, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("RQ5 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 5 ) // RQ4
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 23, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 23, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("RQ4 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 5 ) // RQ3
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 22, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 22, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("RQ3 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 7 ) // RQ2
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 21, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 21, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("RQ2 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 8 ) // RQ1
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 20, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 20, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("RQ1 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 9 ) // SQ1
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 28, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 28, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("SQ1 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 10 ) // SQ2
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 29, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 29, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("SQ2 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 11 ) // SQ3
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 30, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 30, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("SQ3 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 12 ) // SQ4
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 31, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 31, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("SQ4 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 13 ) // SQ5
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 32, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 32, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("SQ5 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 14 ) // SQ6
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 33, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 33, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("SQ6 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 15 ) // SQ7
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 34, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 34, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("SQ7 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 16 ) // SQ8
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 0, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 0, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("SQ8 H") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								QuelNiveauTradeInput.SetCustomInputIndex(17) ;
								NatureNiveau_Input.SetInt(4) ;
							}
								
							else 	if ( IndexPeriode == 4 )  // Mensuel Full/Mid
							{ 
								int StudyIDPointPivot = sc.GetStudyIDByName(sc.ChartNumber, "[A] Pivots Ind M (Full/Mid/Quart) projetes sur le future", 0) ;     if ( StudyIDPointPivot == 0 )  { La_Study_Appellee_N_A_Pas_Ete_Trouvee = 1 ;  }
								if ( IndexQuelNiveau == 1 ) // R4
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 7, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 7, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("R4 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 2 ) // mR4
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 15, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 15, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("mR4 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 3 ) // R3
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 6, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;    sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 6, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("R3 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 4 ) // mR3
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 14, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;    sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 14, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("mR3 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 5 ) // R2
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 5, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;    sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 5, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("R2 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 6 ) // mR2
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 13, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;    sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 13, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("mR2 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 7 ) // R1
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 4, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;    sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 4, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("R1 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 8 ) // mR1
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 12, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;    sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 12, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("mR1 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 9 ) // PP
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 3, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ; 	sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 3, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("PP M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 10 ) // mS1
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 16, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 16, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("mS1 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 11 ) // S1
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 8, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 8, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("S1 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 12 ) // mS2
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 17, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 17, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("mS2 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 13 ) // S2
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 9, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 9, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("S2 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 14 ) // mS3
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 18, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 18, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("mS3 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 15 ) // S3
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 10, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 10, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("S3 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 16 ) // mS4
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 19, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 19, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("mS4 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 17 ) // S4
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 11, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 11, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("S4 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 18 ) // PH
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 1, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 1, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("PH MP") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 19 ) // PB
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 2, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 2, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("PB MP") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 20 ) // PH absolu
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 0, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 0, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("PH Absolu") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								QuelNiveauTradeInput.SetCustomInputIndex(21) ;
								NatureNiveau_Input.SetInt(5) ;
							}

							else 	if ( IndexPeriode == 5 )  // Mensuel Quart
							{ 
								int StudyIDPointPivot = sc.GetStudyIDByName(sc.ChartNumber, "[A] Pivots Ind M (Full/Mid/Quart) projetes sur le future", 0) ;     if ( StudyIDPointPivot == 0 )  { La_Study_Appellee_N_A_Pas_Ete_Trouvee = 1 ;  }
								if ( IndexQuelNiveau == 1 ) // RQ8
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 27, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 27, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("RQ8 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 2 ) // RQ7
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 26, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 26, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("RQ7 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 3 ) // RQ6
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 25, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 25, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("RQ6 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 4 ) // RQ5
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 24, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 24, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("RQ5 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 5 ) // RQ4
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 23, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 23, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("RQ4 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 6 ) // RQ3
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 22, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 22, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("RQ3 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 7 ) // RQ2
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 21, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 21, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("RQ2 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 8 ) // RQ1
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 20, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 20, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("RQ1 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 9 ) // SQ1
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 28, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 28, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("SQ1 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 10 ) // SQ2
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 29, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 29, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("SQ2 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 11 ) // SQ3
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 30, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 30, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("SQ3 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 12 ) // SQ4
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 31, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 31, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("SQ4 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 13 ) // SQ5
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 32, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 32, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("SQ5 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 14 ) // SQ6
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 33, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 33, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("SQ6 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 15 ) // SQ7
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 34, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 34, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("SQ7 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 16 ) // SQ8
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 35, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 35, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("SQ8 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								QuelNiveauTradeInput.SetCustomInputIndex(17) ;
								NatureNiveau_Input.SetInt(5) ;
							}
							
							else 	if ( IndexPeriode == 6 )  // Mensuel Eighth
							{ 
								int StudyIDPointPivot = sc.GetStudyIDByName(sc.ChartNumber, "[B] Pivots Ind M (Eighth) projetes sur le future", 0) ;     if ( StudyIDPointPivot == 0 )  { La_Study_Appellee_N_A_Pas_Ete_Trouvee = 1 ;  }
								if ( IndexQuelNiveau == 1 ) // RE16
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 15, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 15, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("RE16 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 2 ) // RE15
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 14, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 14, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("RE15 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau ==  3) // RE14
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 13, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 13, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("RE14 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 4 ) // RE13
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 12, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 12, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("RE13 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 5 ) // RE12
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 11, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 11, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("RE12 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 6 ) // RE11
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 10, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 10, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("RE11 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 7 ) // RE10
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 9, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 9, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("RE10 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 8 ) // RE9
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 8, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 8, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("RE9 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 9 ) // RE8
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 7, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 7, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("RE8 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 10 ) // RE7
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 6, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 6, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("RE7 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 11 ) // RE6
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 5, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 5, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("RE6 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 12 ) // RE5
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 4, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 4, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("RE5 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 13 ) // RE4
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 3, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 3, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("RE4 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 14 ) // RE3
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 2, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 2, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("RE3 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 15 ) // RE2
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 1, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 1, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("RE2 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 16 ) // RE1
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 0, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 0, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("RE1 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 17 ) // SE1
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 16, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 16, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("SE1 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 18 ) // SE2
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 17, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 17, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("SE2 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 19 ) // SE3
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 18, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 18, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("SE3 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 20 ) // SE4
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 19, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 19, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("SE4 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 21 ) // SE5
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 20, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 20, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("SE5 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 22 ) // SE6
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 21, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 21, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("SE6 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 23 ) // SE7
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 22, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 22, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("SE7 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 24 ) // SE8
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 23, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 23, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("SE8 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 25 ) // SE9
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 24, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 24, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("SE9 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 26 ) // SE10
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 25, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 25, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("SE10 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 27 ) // SE11
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 26, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 26, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("SE11 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 28 ) // SE12
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 27, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 27, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("SE12 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 29 ) // SE13
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 28, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 28, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("SE13 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 30 ) // SE29
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 29, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 29, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("SE14 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 31 ) // SE15
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 30, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 30, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("SE15 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								else	if ( IndexQuelNiveau == 32 ) // SE16
								{ sc.GetStudyArrayUsingID(StudyIDPointPivot, 31, ArrayPointPivot) ;  NiveauFuture = ArrayPointPivot[index] ;  sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyIDPointPivot, 31, 1, ArrayPointPivot) ;  NiveauIndice = ArrayPointPivot[index] ;  Texte.Format("SE16 M") ;  AppendFutOuInd(Texte, FutureOuIndice) ;  NomDuNiveauTrade.SetString(Texte) ;  }
								QuelNiveauTradeInput.SetCustomInputIndex(33) ;
								NatureNiveau_Input.SetInt(5) ;
							}
							
						}   // fin de FutureOuIndice == 1
						
						// Message dans le log si une study appelée n'a pas été trouvée
						if ( La_Study_Appellee_N_A_Pas_Ete_Trouvee == 1 )
						{	SCString message ;  message.Format("ID:%d | !! ERREUR !! Une study appelee n'a pas ete trouvee (StudyIDPointPivot==0).", ThisStudyID) ;  sc.AddMessageToLog(message, 1) ;  }
						else if ( La_Study_Appellee_N_A_Pas_Ete_Trouvee == 2 )
						{	SCString message ;  message.Format("ID:%d | !! ERREUR !! Une study appelee n'a pas ete trouvee (StudyIDVWAPsAffichage==0).", ThisStudyID) ;  sc.AddMessageToLog(message, 1) ;  }
					
					} 
					
				}   // fin de CALCUL DU NIVEAU CIBLE (SI MODIFICATION D'UN INPUT)
				
				
				// Initialisations de variables persistantes
				{
					NiveauFutureAjuste = NiveauFuture  + AjustementChoisiEnPas*0.25 ;  // première initialisation de NiveauFutureAjuste (après arrondi au quart de point)
					DistMiniFuture[IndexHoraireRAZ-1] = 100000 ; 
					DistMiniIndice[IndexHoraireRAZ-1] = 100000 ; 
					PremiereToucheFaite = 0 ;
					BasculeTP0Depassee = 0 ;
					StopLossIndiceDepasse = 0 ;
					TakeProfitIndiceDepasse = 0 ;
					IndexQuelNiveau = QuelNiveauTradeInput.GetIndex() ;      // utilise pour le test de rejet si robot inactif
					MultiplicateurPrix = NiveauFuture/6500 ;
				}
				
				
				// Calcul de l'index à HoraireRAZ sur les graphes future en 200/2000 ticks et sur les graphes indice UT20s et UT2min
				{
					ExternalChartIndex_UT200t = sc.GetContainingIndexForDateTimeIndex(11, IndexHoraireRAZ) ;    // graphe #11
					ExternalChartIndex_UT2000t = sc.GetContainingIndexForDateTimeIndex(12, IndexHoraireRAZ) ;    // graphe #12
					ExternalChartIndex_UT20s = sc.GetContainingIndexForDateTimeIndex(15, IndexHoraireRAZ) ;    // graphe #15 
					ExternalChartIndex_UT2min = sc.GetContainingIndexForDateTimeIndex(19, IndexHoraireRAZ) ;    // graphe #19
				}
				

				// Détermination si on attend une position à l'achat ou à la vente 
				{
					Position = 0 ;    // initialisation

					if ( FutureOuIndice == 0 )
					{
						if ( sc.Low[index] > NiveauFutureAjuste + 1  ) // on attend une position à l'achat   // on veut que le prix soit à plus de 1 pt du NiveauFutureAjuste
						{ Position = 1 ;  CouleurPosition = RGB(0,200,0);  CouleurPositionInverse = RGB(250,0,0); }
						else if ( sc.High[index] < NiveauFutureAjuste - 1 ) 
						{ Position = -1 ;  CouleurPosition = RGB(250,0,0);  CouleurPositionInverse = RGB(0,200,0);  } 
					}
					else if (FutureOuIndice == 1 )
					{
						float NiveauIndiceAjuste_ProjeteSurFuture = NiveauIndice + AjustementChoisiEnPas*0.25 + SpreadMoyen[index]  ;   // on ne peut pas utiliser NiveauIndiceSurFutureTempsReel car il n'est pas encore connu à cet index ; il ne sera calculé qu'à partir du suivant
						if ( sc.Low[index] > NiveauIndiceAjuste_ProjeteSurFuture + 3  ) // on attend une position à l'achat   // on veut que le prix soit à plus de 3 pts du NiveauFutureAjuste
						{ Position = 1 ;  CouleurPosition = RGB(0,200,0);  CouleurPositionInverse = RGB(250,0,0);  }
						else if ( sc.High[index] < NiveauIndiceAjuste_ProjeteSurFuture - 3 ) 
						{ Position = -1 ;  CouleurPosition = RGB(250,0,0);  CouleurPositionInverse = RGB(0,200,0);  } 
					}
					
					if ( Position == 0 )
					{	
						CouleurPosition = RGB(0,0,0);
						IndexArretManualLooping = -2 ;  
						QueryPerformanceCounter(&ElapsedTicks) ;  
						DureeCalculCompletAMR = ( ElapsedTicks.QuadPart - ElapsedTicks_t0.QuadPart ) *1000.0 / Frequency.QuadPart ;     // calculé en millisecondes
						if ( sc.IsFullRecalculation == 1 )  {	DureeCycleComplet_FullRecalculation = ( ElapsedTicks.QuadPart - ElapsedTicks_PrecedentCycle ) *1000.0 / Frequency.QuadPart ;  }
						ElapsedTicks_PrecedentCycle = ElapsedTicks.QuadPart  ;
						
						// Affichage de Quantieme_Mvt est OFF
						if (  CetteInstanceControleLesBoutons.GetYesNo() == 1  ) 
						{
							// Affichage OFF
							int OFF = 0 ;
							sc.SetChartStudyInputInt(15, StudyID_QuantiemeMvt_Chart15, 9, OFF) ; 
							sc.SetChartStudyInputInt(19, StudyID_QuantiemeMvt_Chart19, 9, OFF) ; 
						}
						
						sc.UpdateAlways = 1 ;  return ;   // on met cette instruction après le bloc des tracés sur les graphes externes
					}
				}
				
				
				// Arrondi de NiveauFuture au tick le plus proche selon que la position soit acheteuse ou vendeuse    // NiveauFuture est d'abord décalé de 0.01 pt pour le cas où NiveauFuture tombe pile entre deux ticks      // Puis on recalcule NiveauFutureAjuste
				{
					if ( Position == -1 )
					{ NiveauFuture = NiveauFuture + 0.01 ;  }
					else if ( Position == 1 )
					{ NiveauFuture = NiveauFuture - 0.01 ;  }
					NiveauFuture = round(4*NiveauFuture)/4 ;
					NiveauFutureAjuste = NiveauFuture  + AjustementChoisiEnPas*0.25 ;  // seconde initialisation de NiveauFutureAjuste (après arrondi au quart de point)
				}
				
				
				// Réinjection de la nouvelle valeur de NiveauFuture dans les inputs NiveauFutureTexte et NiveauFutureRoulette    // Réinjection de la nouvelle valeur de NiveauIndice dans l'input NiveauIndiceTexte et redressement du float
				{
					NiveauFutureTexte.SetFloat(NiveauFuture);
					SCString NiveauFutureInputString ;
					NiveauFutureInputString.Format("%.2f",NiveauFuture+TailleDuCustomInputDeNiveauFuture*0.25) ;  // la première valeur de la chaîne qui sera construite par concaténation (construite du niveau le plus élevé au plus bas)
						for (int index2 = TailleDuCustomInputDeNiveauFuture - 1 ; index2 > -TailleDuCustomInputDeNiveauFuture - 1 ; index2-- )  // concaténation des inputs
						{
							SCString StringAjoute ;
							StringAjoute.Format(";%.2f", NiveauFuture + index2*0.25) ;
							NiveauFutureInputString.Append(StringAjoute) ;
						}
					NiveauFutureRoulette.SetCustomInputStrings(NiveauFutureInputString) ;
					NiveauFutureRoulette.SetCustomInputIndex(TailleDuCustomInputDeNiveauFuture);
				
					NiveauIndice = round(NiveauIndice*100) /100 ;  // redressement du float pour avoir les 6 premiers chiffres avec le bon arrondi
					NiveauIndiceTexte.SetFloat(NiveauIndice) ;
				}
				
				
				// Calcul de BaseDuTrace pour le tracé des indicateurs normés (AUC, Sigma, AUC+Sigma) => il réutilise le SL_Prior (variable persistante) du trade précédent. SL_Prior n'est calculé qu'au moment d'entrer en position.
				BaseDuTrace = NiveauFutureAjuste - Position*SL_Prior ;

			
				// Déterminer le statut de NewDailyHighLow_Input
				if ( NatureNiveau_Input.GetInt() == 6 )  // le niveau est PHJ ou PBJ => on force NewDailyHighLow_Input en fonction de NatureNiveau_Input
				{
							if ( NewDailyHighLow_InputManuel.GetIndex() == 0 )  NewDailyHighLow_Input.SetYesNo(0) ;
					else if ( NewDailyHighLow_InputManuel.GetIndex() == 1 )  NewDailyHighLow_Input.SetYesNo(1) ;
				}
				else 		// cas général
				{
					// récupération de PlusHautSession et PlusBasSession
					SCFloatArray Array_PHDyn, Array_PBDyn ;
					sc.GetStudyArrayUsingID(StudyID_PHPBDyn_Fut, 2, Array_PHDyn) ;  
					sc.GetStudyArrayUsingID(StudyID_PHPBDyn_Fut, 3, Array_PBDyn) ;
					float PlusHautSession = Array_PHDyn[index] ;
					float PlusBasSession = Array_PBDyn[index] ;
				
					// Calcul de NewDailyHighLow_Input.SetYesNo
					if ( ( NiveauFutureAjuste > PlusHautSession ) or ( NiveauFutureAjuste < PlusBasSession ) )  // NiveauFutureAjuste est compris en-dehors de PlusHautSession et PlusBasSession
					{  NewDailyHighLow_Input.SetYesNo(1) ;  }
					else
					{  NewDailyHighLow_Input.SetYesNo(0) ;  }
				}


				// Set the ACS Control Bar button 3 text (NDHL YES/NO)
				if ( NewDailyHighLow_Input.GetYesNo() == 1 )  { sc.SetCustomStudyControlBarButtonText(3, "NDHL YES") ;  }
				else if ( NewDailyHighLow_Input.GetYesNo() == 0 )  { sc.SetCustomStudyControlBarButtonText(3, "NDHL NO") ;  }
				
				
				// Set the ACS Control Bar button 60 text 
				{
					if ( CetteInstanceControleLesBoutons.GetYesNo() )
					{
						Texte.Format("ID:%d | ", ThisStudyID) ;
						Texte.AppendFormat( NomDuNiveauTrade.GetString() ) ;
						//if ( FutureOuIndice == 1 )  { Texte.AppendFormat(" Ind ") ;  }  else { Texte.AppendFormat(" Fut ") ;  }
						// Texte.AppendFormat(" | NEXT") ;
						sc.SetCustomStudyControlBarButtonText(60, Texte) ; 
					}
				}			
				
				
				// La study AMR active transmet les inputs suivants à la study "Quantieme_Mvt" : IndexHoraireRAZ, Niveau, NatureNiveau, SousNatureNiveau, Position, TriggerRecalculate.
				if (  CetteInstanceControleLesBoutons.GetYesNo() == 1  )  Set_StudyQuantiemeMvt(ptrsc) ;
				
				
				// Transmission d'un TriggerRecalculate aux studies "Padding et boutons ACS" pour le recalcul du range affiché	 ////////////////////////////////////////////////////
				{
					// Transmission de TriggerRecalculate_Input = 1
					sc.SetChartStudyInputInt(15, StudyID_Padding_Chart15, 0, 1) ; 	// int SetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int IntegerValue);
					sc.SetChartStudyInputInt(19, StudyID_Padding_Chart19, 0, 1) ; 				
				}
				

				// Détermination de IndexDeFin (vaudra IndexPremiereTouche le cas échéant, sinon vaudra LastIndex)
				{
					int IndexDeFin ;
					for ( int index2 = IndexHoraireRAZ ; index2 <= LastIndex ; ++ index2 )
					{
						
						// On détermine DistMiniFuture qui est la distance la plus proche observée entre le cours du future et le NiveauFutureAjuste
						if ( Position == 1 ) 
						{	
							if ( sc.Low[index2] - NiveauFutureAjuste < DistMiniFuture[index2-1] )  // on a un nouveau plus bas
							{ DistMiniFuture[index2] = sc.Low[index2] - NiveauFutureAjuste ; }
							else  { DistMiniFuture[index2] = DistMiniFuture[index2-1] ; }
						}
						else if ( Position == -1 )
						{ 
							if ( NiveauFutureAjuste - sc.High[index2] < DistMiniFuture[index2-1] )  // on a un nouveau plus haut
							{ DistMiniFuture[index2] = NiveauFutureAjuste - sc.High[index2] ; }  
							else  { DistMiniFuture[index2] = DistMiniFuture[index2-1] ; }
						}
						
					
						// On détermine DistMiniIndice qui est la distance la plus proche observée entre le cours de l'indice et le NiveauIndice
						if (FutureOuIndice == 1  and  sc.BaseDateTimeIn[index2] < SCDateHoraireFinSeanceAction )      // tracé jusqu'à 15h59m50s
						{
							NiveauIndiceSurFutureTempsReel[index2] = NiveauIndice + SpreadMoyen[index2] ;
							AjustementSubiPoints = NiveauFuture - NiveauIndiceSurFutureTempsReel[index2] ;        // en points
							NiveauIndiceAjuste = NiveauIndice + AjustementChoisiEnPas*0.25 + AjustementSubiPoints ;
							if ( Position == 1 ) 
							{	
								if ( INQ100SansCorrection[index2] - NiveauIndice < DistMiniIndice[index2-1] )  // on a un nouveau plus bas
								{ DistMiniIndice[index2] = INQ100SansCorrection[index2] - NiveauIndice ; }
								else  { DistMiniIndice[index2] = DistMiniIndice[index2-1] ; }
							}
							else if ( Position == -1 )
							{ 
								if ( NiveauIndice - INQ100SansCorrection[index2] < DistMiniIndice[index2-1] )  // on a un nouveau plus haut
								{ DistMiniIndice[index2] = NiveauIndice - INQ100SansCorrection[index2] ; }  
								else  { DistMiniIndice[index2] = DistMiniIndice[index2-1] ; }
							}
						}
						
						IndexDeFin = index2 ;
						
						if ( DistMiniFuture[index2] <= 0 )  // première touche déjà faite
						{ 	break ;  }
						
						if ( FutureOuIndice == 1  and  sc.BaseDateTimeIn[index2] > SCDateHoraireFinSeanceAction )      // pour ne pas dépasser 15h59m50s sur indice
						{ 
							IndexArretManualLooping = -1 ;   
							QueryPerformanceCounter(&ElapsedTicks) ;  
							DureeCalculCompletAMR = ( ElapsedTicks.QuadPart - ElapsedTicks_t0.QuadPart ) *1000.0 / Frequency.QuadPart ;     // calculé en millisecondes
							if ( sc.IsFullRecalculation == 1 )  {	DureeCycleComplet_FullRecalculation = ( ElapsedTicks.QuadPart - ElapsedTicks_PrecedentCycle ) *1000.0 / Frequency.QuadPart ;  }
							ElapsedTicks_PrecedentCycle = ElapsedTicks.QuadPart  ;
							sc.UpdateAlways = 1 ;  return ;
						}
					
					}
					
					
					// on calcule IndexWarpJump
					if ( TPNu_Manuel_Input.GetFloat() == 0 ) TPNu = ArrayTPNu[IndexDeFin] ;  else TPNu = TPNu_Manuel_Input.GetFloat() ;
					NbBarresVirginite = ceil ( 250 * TPNu / 4 ) ; 
					IndexWarpJump = std::max ( IndexDeFin - 2*NbBarresVirginite , IndexHoraireRAZ ) ;  // IndexWarpJump ne doit pas être inférieur à IndexHoraireRAZ
					
					// on modifie la valeur de " index " de la boucle for
					index = IndexWarpJump - 1  ; //  on retranche 1 car après l'instruction " continue " la boucle for incrémente de +1
					continue ; // pour redémarrer la boucle à la nouvelle valeur de " index "
				}
				//  !!!!! NE RIEN METTRE DERRIERE LE PRECEDENT BLOC CAR IL SE TERMINE AVEC UNE INSTRUCTION  " CONTINUE "  !!!!!
				
			}
			// fin du bloc TRIGGERRAZ /////////////////////////////////////////////////

			
			// COEUR DU PROGRAMME : gestion de DistMiniFuture et DistMiniIndice ;  Calcul de AUC, RatioAUC, RatioSigma, SommeRatioAvecMalus  ////////////////////////////////////////////////////////////////////////////////
			{	
				// On teste si on dépasse MaxBarresCalculees
				if ( index - IndexHoraireRAZ > MaxBarresCalculees )
				{
					// message pour le log
					int Hour, Minute, Second;
					TIME_TO_HMS(sc.BaseDateTimeIn[index].GetTimeInSecondsWithoutMilliseconds(), Hour, Minute, Second);
					QueryPerformanceCounter(&ElapsedTicks) ;  
					float DureeCalculSimpleAMR_QPC = ( ElapsedTicks.QuadPart - ElapsedTicks_t0.QuadPart ) *1000.0 / Frequency.QuadPart ;     // calculé en millisecondes
					SCString message ;
					message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | !! ARRET CALCUL !! MaxBarresCalculees (%d barres) a ete depasse a %dh%dm%ds. SommeRatioAvecMalus=%.0f%%. Duree calcul = %.3f secondes.", MaxBarresCalculees, Hour, Minute, Second, SommeRatioAvecMalus[index-1]*100, DureeCalculSimpleAMR_QPC/1000 ) ;   
					sc.AddMessageToLog(message, 1) ;
					// L'affichage est mis off et la study recalculée
					AffichageONOFF_Input.SetCustomInputIndex(AffichageONOFF);  sc.HideStudy=1;
					TriggerRecalculate = 1 ;
					sc.UpdateAlways = 1 ;  return ;				
				}
				
				
				// On détermine DistMiniFuture qui est la distance la plus proche observée entre le cours du future et le NiveauFutureAjuste
				{
					if ( Position == 1 ) 
					{	
						if ( sc.Low[index] - NiveauFutureAjuste < DistMiniFuture[index-1] )  // on a un nouveau plus bas
						{ DistMiniFuture[index] = sc.Low[index] - NiveauFutureAjuste ; }
						else  { DistMiniFuture[index] = DistMiniFuture[index-1] ; }
					}
					else if ( Position == -1 )
					{ 
						if ( NiveauFutureAjuste - sc.High[index] < DistMiniFuture[index-1] )  // on a un nouveau plus haut
						{ DistMiniFuture[index] = NiveauFutureAjuste - sc.High[index] ; }  
						else  { DistMiniFuture[index] = DistMiniFuture[index-1] ; }
					}
				}
				
				
				// On détermine DistMiniIndice qui est la distance la plus proche observée entre le cours de l'indice et le NiveauIndice
				if ( FutureOuIndice == 1 )
				{
					if ( Position == 1 ) 
					{	
						if ( INQ100SansCorrection[index] - NiveauIndice < DistMiniIndice[index-1] )  // on a un nouveau plus bas
						{ DistMiniIndice[index] = INQ100SansCorrection[index] - NiveauIndice ; }
						else  { DistMiniIndice[index] = DistMiniIndice[index-1] ; }
					}
					else if ( Position == -1 )
					{ 
						if ( NiveauIndice - INQ100SansCorrection[index] < DistMiniIndice[index-1] )  // on a un nouveau plus haut
						{ DistMiniIndice[index] = NiveauIndice - INQ100SansCorrection[index] ; }  
						else  { DistMiniIndice[index] = DistMiniIndice[index-1] ; }
					}
				}
				
				// GESTION DE L'APPROCHE DE NIVEAU CIBLE FUTURE
				{
					// Niveau cible future a déjà été dépassé (dans cette bougie ou une précédente)
					if ( DistMiniFuture[index] < 0 )
					{	if ( PremiereToucheFaite == 0 )  { PremiereToucheFaite = 1 ;  } }
					// Niveau cible future est touché dans cette bougie, mais n'a encore jamais été dépassé
					else if ( sc.Low[index] == NiveauFutureAjuste or sc.High[index] == NiveauFutureAjuste )   // si on arrive à ce "else if" ça signifie que le niveau cible n'a encore jamais été dépassé car le test de " else if ( DistMiniFuture < 0 ) " est faux.
					{	if ( PremiereToucheFaite == 0  and  Position != 0 ) { PremiereToucheFaite = 1 ;  } }
				}
				
			
				// CALCUL DE L'AUC_Future ET DE L'AUC_Indice (recalculés pour chaque index)          // algorithme de complexité n*m  (n : durée depuis IndexHoraireRAZ ; m : nb de barres depuis le WarpJump)		// Pour modèles : 1, 2, 3, 4, 6, 7, 9
				AUC_Future_Bis = 0 ;  AUC_Indice_Bis = 0 ;
				{				
					for (int index2 = IndexHoraireRAZ ; index2 <= index ; index2 ++ )     
					{
						Contribution_Future = pow( Puissance, -(std::max( 0.25f, DistMiniFuture[index2] ) / TP_AUC) ) * Puissance ;   // DistMiniFuture est la distance la plus proche observée entre le cours du future et le NiveauFutureAjuste
						AUC_Future_Bis += Contribution_Future ;      
						
						Contribution_Indice = pow( Puissance, -(std::max( 0.0f, DistMiniIndice[index2] -Position*(AjustementChoisiEnPas*0.25f+AjustementSubiPoints) ) / TP_AUC) ) * Puissance ;   // on tient compte de AjustementChoisi+AjustementSubi parce que cela fera entrer avant/après le niveau indice
						AUC_Indice_Bis += Contribution_Indice ;                 
					}		
					AUC_Future[index] = AUC_Future_Bis ; 
					AUC_Indice[index] = AUC_Indice_Bis ;
				}
	
	
				
				// MVT VERTICAL NORME : on récupère MouvementVerticalSommeNormeUp et MouvementVerticalSommeNormeDown (normé à 1.00 = 100%) via la study "Sigma".
				{
					if (Position == 1 )
					{ Sigma[index] = 1.0 + ArraySigmaDown[index] ; }		// c'est "NewSigma" car on a ajouté +1
					else if (Position == -1)
					{ Sigma[index] = 1.0 + ArraySigmaUp[index] ; }
				}
				
				
				// Calculs de BonusMoy
				BonusMoy = ( BonusIntrinseque + BonusExcedentaire ) / 2 ; 

				
				// Calculs de SeuilSigma et CsteNewSigma  (issu de NewTuningRatioSigma)
				{
					if 			( BonusMoy < -12.0 )		{ NewSeuilSigma =  0.05 / 100.0 ;  }  																// NewSeuilSigma est un pourcentage (normé sur 1)
					else  										{ NewSeuilSigma = 500.0 * pow( (BonusMoy+13.0)/100.0 , 2 ) / 100.0 ;  }
					
					CsteNewSigma = -150.0/100.0 + Sigma[index] / 1.5 ; 																						// CsteNewSigma est un pourcentage (normé sur 1)
				}
				
				
				
				
				// Détermination de RATIOAUC et RATIOSIGMA (normés à 1)
				{
					float AUC, SeuilAUC ;
					if (FutureOuIndice == 0 )  { AUC = AUC_Future[index] ; SeuilAUC = SeuilAUC_Future ;  }	
					else if ( FutureOuIndice == 1 )  { AUC = AUC_Indice[index] ; SeuilAUC = SeuilAUC_Indice ;  }	

					// ON CALCULE LE RATIOAUC
					RatioAUC[index] = AUC / std::max(1.0f,SeuilAUC) ;   // SeuilAUC>0 pour éviter RatioAUC<0
					// RatioAUCBis est le RatioAUC tracé entre 0 et 200%
					RatioAUCBis = std::min(2.0f,RatioAUC[index]) ;  // utilisé pour le tracé de la courbe bleue
					
					// ON CALCULE LE RATIOSIGMA
					RatioSigma[index] = Sigma[index] / NewSeuilSigma + CsteNewSigma ;                  // RatioSigma est un pourcentage (normé sur 1), il peut être négatif
					// RatioSigmaBis est le RatioSigma tracé entre entre -50 et 200%
					RatioSigmaBis = std::max( -0.5f, std::min(2.0f, RatioSigma[index]) ) ; 

					// SOMME DE RATIOAUC ET RATIOSIGMA
					float SommeRatio_AvantMalusExcedentaire = ( RatioAUC[index]/DistanceAuPHPB_SurSL/DistanceAuPHPB_SurSL/TauxVirginite/TauxVirginite + RatioSigma[index]) / 1.8 + Malus_SpreadIndex ;  // on norme la somme à 180%    // on divise par DistanceAuPHPB_SurSL^2 (comprise entre 0 et 1) et TauxVirginite^2 (compris entre 0.25 et 1)		// Malus_SpreadIndex vaut 0 si SpreadIndex<1.3 ; vaut 1.0 si SpreadIndex=1.6.
					// Ajout de MalusExcedentaire
					if ( BonusExcedentaire < 0 )  						{ MalusExcedentaire = - ( BonusExcedentaire + BonusExcedentaireSuffisant ) * CoeffMalusExcedentaire ;  }
					else if ( BonusExcedentaireSuffisant < 0 )	{ MalusExcedentaire = - ( BonusExcedentaireSuffisant ) * CoeffMalusExcedentaire ;  }
					else MalusExcedentaire = 0 ;
					SommeRatioAvecMalus[index] = SommeRatio_AvantMalusExcedentaire * (1+ MalusExcedentaire/100) ;  		// MalusExcedentaire exprimé en %
					// SommeRatioAvecMalusBis est le SommeRatioAvecMalus tracé entre 0 et 200%
					SommeRatioAvecMalusBis = std::max( 0.0f, std::min(2.0f, SommeRatioAvecMalus[index]) ) ;
					// MalusExcedentaireBis est le MalusExcedentaire tracé entre 0 et 200%
					MalusExcedentaireBis = std::min(2.0f,MalusExcedentaire/100) ;
				}
			}
			// fin du coeur du programme  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			
			// AVANT QUE LE NiveauFuture SOIT TOUCHE OU DEPASSE /////////////////////////////////////////////
			if ( PremiereToucheFaite == 0 ) 
			{
				// TPCible = TP_AUC avant entrée, puis TPCible = TPOptimumArrondi_Prior après entrée. TPCible est utilisé pour le tracé des lignes horizontales : 1*TPCible, 2*TPCible, 3*TPCible, 4*TPCible
				TPCible = TP_AUC ; 
			}
			
			
			// PREMIERE FOIS QUE LE NiveauFuture EST TOUCHE OU DEPASSE : ON CALCULE LES VALEURS CLES DE L'INDICATEUR à [IndexPremiereTouche-1]  ///////////////////////////////////////////// 
			if ( PremiereToucheFaite == 1 ) 
			{
				PremiereToucheFaite = 2 ;
				IndexPremiereTouche = index ;
				
				// TPNu_Prior
				if ( TPNu_Manuel_Input.GetFloat() == 0 ) TPNu_Prior = ArrayTPNu[IndexPremiereTouche-1] ;  else TPNu_Prior = TPNu_Manuel_Input.GetFloat() ;
				
				// SpreadIndex_Prior,  CoeffMalusAjustement_Prior, TP_AUC_Prior
				SpreadIndex_Prior = ArraySpreadIndex[IndexPremiereTouche-1] ;
				if ( NatureNiveau_Input.GetInt() == 0  and  SpreadIndex_Prior > 1.3 )  Malus_SpreadIndex_Prior = std::max(0.0, SpreadIndex_Prior-1.3)/0.3 ;    
				CoeffMalusAjustement_Prior = 90000 / sc.Close[IndexPremiereTouche-1] / sqrt(2*SpreadIndex_Prior-1) ;
				TP_AUC_Prior = TPNu_Prior / 4 * MultiplicateurPrix * TailleBougie_4000Ticks(SpreadIndex_Prior, ptrsc) / TailleBougie_4000Ticks(1.0f, ptrsc) ; 
				
				// MalusDeNRV_Prior, MalusDePremierPassageJournalier_Prior
				if ( MalusNRV_Manuel_Input.GetFloat() == 0 )
				{
					// nouveaux malus
					{
						if ( TPNu_Prior < 3 )   
						{ MalusDeNRV_Prior=5*TPNu_Prior-15 ; MalusDePremierPassageJournalier_Prior= 10 ; }
						else if ( TPNu_Prior < 4 )   
						{ MalusDeNRV_Prior=18*TPNu_Prior-54 ; MalusDePremierPassageJournalier_Prior= -6*TPNu_Prior+28 ; }
						else if ( TPNu_Prior < 5 )   
						{ MalusDeNRV_Prior=12*TPNu_Prior-30 ; MalusDePremierPassageJournalier_Prior= 11*TPNu_Prior-40 ; }
						else if ( TPNu_Prior < 6 )   
						{ MalusDeNRV_Prior=4*TPNu_Prior+10 ; MalusDePremierPassageJournalier_Prior= 6*TPNu_Prior-15 ; }
						else if ( TPNu_Prior < 7 )   
						{ MalusDeNRV_Prior=2*TPNu_Prior+22 ; MalusDePremierPassageJournalier_Prior= 8*TPNu_Prior-27 ; }
						else if ( TPNu_Prior < 8 )   
						{ MalusDeNRV_Prior=14*TPNu_Prior-62 ; MalusDePremierPassageJournalier_Prior= 6*TPNu_Prior-13 ; }
						else if ( TPNu_Prior < 12 )   
						{ MalusDeNRV_Prior=10*TPNu_Prior-30 ; MalusDePremierPassageJournalier_Prior= 5*TPNu_Prior-5 ; }
						else
						{ MalusDeNRV_Prior = 1000 ;  MalusDePremierPassageJournalier_Prior = 1000 ;  SCString message ;  message.Format("ID:%d | !! WARNING !! TPNu_Prior > 12. NON APPLICABLE POUR ANNUL MINI REBOND", ThisStudyID) ; sc.AddMessageToLog(message,1) ; }
					}
				}
				else
				{
					MalusDeNRV_Prior = MalusNRV_Manuel_Input.GetFloat() ;
					MalusDePremierPassageJournalier_Prior = 0 ;
				}
				
				// Malus d'ajustement subi, NiveauIndiceAjuste
				if ( FutureOuIndice == 1 )
				{	AjustementSubiPoints_Prior = NiveauFuture - NiveauIndiceSurFutureTempsReel[index-1] ;  }
				else
				{	AjustementSubiPoints_Prior = 0;  }
				NiveauIndiceAjuste_Prior = NiveauIndice + AjustementChoisiEnPas*0.25 + AjustementSubiPoints_Prior ;

				// BonusExcedentaire_Prior, BonusExcedentaireSuffisant_Prior et MalusAjustementAbsolu_Prior
				float AjustementAbsolu_Prior = fabs(0.25*AjustementChoisiEnPas+AjustementSubiPoints_Prior) ;
				float BonusAvantAjustement_Prior = BonusIntrinseque - MalusDeNRV_Prior - NewDailyHighLow_Input.GetYesNo() * MalusDePremierPassageJournalier_Prior ;
				if ( MalusAjustementParabolique_Input.GetIndex() == 0 )   // Modèle linéaire
				{
					// BonusExcedentaire (identique pour future et indice)
					BonusExcedentaire_Prior = BonusAvantAjustement_Prior - AjustementAbsolu_Prior*CoeffMalusAjustement_Prior ;
					BonusExcedentaireSuffisant_Prior = BonusExcedentaire_Prior - AjustementAbsolu_Prior*CoeffMalusAjustement_Prior ;   
				}
				else if ( MalusAjustementParabolique_Input.GetIndex() == 1 )   // Modèle Parabolique
				{
					// BonusExcedentaire (identique pour future et indice)
					BonusExcedentaire_Prior = BonusAvantAjustement_Prior - AjustementParabolique(BonusIntrinseque, AjustementAbsolu_Prior, CoeffMalusAjustement_Prior) ;		// float AjustementParabolique (float Bonus, float AjustementAbsolu, float CoeffMalusAjustement) 
					BonusExcedentaireSuffisant_Prior = BonusExcedentaire_Prior - AjustementParabolique(BonusIntrinseque, AjustementAbsolu_Prior, CoeffMalusAjustement_Prior) ;
				}
				MalusAjustementAbsolu_Prior = BonusAvantAjustement_Prior - BonusExcedentaire_Prior ;  

				// BonusMoy_Prior
				BonusMoy_Prior = ( BonusIntrinseque + BonusExcedentaire_Prior ) / 2 ; 
				
				// Calcul de TPOptimum_Prior
				TPOptimum_Prior = TP_AUC_Prior*(1+BonusExcedentaire_Prior/100) ;
				
				// Calcul des seuils de AUC_Future et AUC_Indice		
				float SeuilAUC_Future_Prior ;
				float SeuilAUC_Indice_Prior ;
				{
					SeuilAUC_Future_Prior = 46*pow(TPNu_Prior/4,2) * ( 1 + BonusExcedentaire/100 ) ;		
					SeuilAUC_Indice_Prior = 20*pow(TPNu_Prior/4,2) * (1 + BonusExcedentaire/100 + 3*BonusExcedentaire*fabs(BonusExcedentaire)/10000) ;
				}
				
				// Calculs de TPOptimumArrondi_Prior (= TPOptimumArrondi à l'index précédent), SL_Prior et NiveauStopLoss (ce sont des variables persistantes qui serviront pour toute la suite du tracé) ; SL_Prior n'est pas réinitialisé en début de tracé => il servira pour calculer un NiveauStopLoss approximatif pour le prochain trade
				TPOptimumArrondi_Prior = ( NiveauTPOptimumArrondi[index-1] - NiveauFutureAjuste ) / Position ;   // car :    NiveauTPOptimumArrondi[index] = NiveauFutureAjuste + Position * TPOptimumArrondi ;
				if ( TPOptimumArrondi_Prior == 0.5 )  { SL_Prior = 3.5 ; }
				else if ( TPOptimumArrondi_Prior == 0.75 )  { SL_Prior = 4.5 ; }
				else if ( TPOptimumArrondi_Prior > 0.75 )  { SL_Prior = 4*TPOptimumArrondi_Prior + 2 ; }
				else  { SL_Prior = 9.5 ; }    // tracé par défaut si pb d'initialisation
				NiveauStopLoss = NiveauFutureAjuste - Position*SL_Prior ;
				TPCible = TPOptimumArrondi_Prior ; 
				
				// TauxVirginite = ( Nombre de barres parcourues depuis IndexHoraireRAZ ) / NbBarresVirginite
				NbBarresVirginite_Prior = ceil ( 250 * TPNu_Prior / 4 ) ;
				TauxVirginite_Prior = (float(index-1 - IndexHoraireRAZ)) / NbBarresVirginite_Prior ;
				
				// DistanceAuPHPB_SurSL_Prior
				DistanceAuPHPB_SurSL_Prior = Get_DistanceAuPHPB_SurSL(IndexHoraireRAZ, SL_Prior, ptrsc) ;  
				
				// calcul de AUC_Prior / SeuilAUC_Prior / RatioAUC_Future_Prior / RatioAUC_Prior
				if (FutureOuIndice == 0 )  { AUC_Prior = AUC_Future[index-1] ;   SeuilAUC_Prior = SeuilAUC_Future_Prior ;  }	
				else if ( FutureOuIndice == 1 )  { AUC_Prior = AUC_Indice[index-1] ;   SeuilAUC_Prior = SeuilAUC_Indice_Prior ;  RatioAUC_Future_Prior = AUC_Future[index-1] / std::max(1.0f,SeuilAUC_Future) *100 ; }	
				RatioAUC_Prior = RatioAUC[index-1]*100 ;
				
				// calcul de Sigma_Prior / SeuilSigma_Prior / RatioSigma_Prior
				Sigma_Prior = Sigma[index-1]*100 ;
				RatioSigma_Prior = RatioSigma[index-1]*100 ;
				if ( BonusMoy_Prior < -13.0 ) { SeuilSigma_Prior =  0.7 ;  }  else if ( BonusMoy_Prior < 20.0 ) { SeuilSigma_Prior =  BonusMoy_Prior * 2.1 + 28.0 ;  } else { SeuilSigma_Prior = BonusMoy_Prior * 3.5 ;  }  
				
				// valeur de SommeRatio à la première touche
				SommeRatioAvecMalus_Prior = SommeRatioAvecMalus[index-1]*100 ;
				
				// MalusExcedentaire_Prior
				if ( BonusExcedentaire_Prior < 0 )  MalusExcedentaire_Prior = - ( BonusExcedentaire_Prior + BonusExcedentaireSuffisant_Prior ) * CoeffMalusExcedentaire ;
				else if ( BonusExcedentaireSuffisant_Prior < 0 )  MalusExcedentaire_Prior = - BonusExcedentaireSuffisant_Prior * CoeffMalusExcedentaire ;
				else MalusExcedentaire_Prior = 0 ;

				// Calcul de TPBooste
				const float TPBooste = round ( 4* ( TPOptimum_Prior * (2 - SommeRatioAvecMalus[IndexPremiereTouche-1]) ) ) / 4 ;

				// Calcul de ExternalChartIndex_UT1t (index correspondant à IndexPremiereTouche sur chart 1-tick)
				int ExternalChartIndex_UT1t = sc.GetContainingIndexForDateTimeIndex(13, IndexPremiereTouche) ;    // graphe #13
				// Tracé du TP sur le chart #13 (chart 1-tick)
				DrawHorizontalRay_UTAMAOA_UDD_SurGrapheExterne (ExternalChartIndex_UT1t, NiveauFutureAjuste+Position*TPOptimumArrondi_Prior, 1, CouleurPositionInverse, ptrsc, 10132+10000*ThisStudyID, 13, 0) ;      // void DrawHorizontalRay_UTAMAOA_UDD_SurGrapheExterne (int index, float prix, int epaisseur, COLORREF couleur, s_sc* ptrsc, int Line_Number, int chartNumber, int graphRegion)


				
				/////////////////////////////   Tracé des mouvements verticaux relatifs (calcul sur [index-1] )   //////////////////////////////////
				float MouvementVertical128uple, MouvementVertical64uple, MouvementVertical32uple, MouvementVertical16uple, MouvementVerticalOctuple, MouvementVerticalQuadruple, MouvementVerticalDouble, MouvementVerticalFull, MouvementVerticalMoitie, MouvementVerticalQuart ;

				// Récupération de EnvergureMoyenne4000Ticks dans la study "Sigma" du présent graphique, et calcul de EnvergureMoyenneFull
				float EnvergureMoyenne4000Ticks = std::max(ExternalArray_EnvergureMoyenne4000Ticks[index-1],1.0f) ;
				float EnvergureMoyenneFull = EnvergureMoyenne4000Ticks*sqrt(TPNu_Prior/4) ; // 4000 ticks = 10.6 pts / 9 pts / ...

				// Calcul des mouvements à longue distance (x128 à x025) :
				if ( Position == 1 ) 
				{
					MouvementVertical128uple		= FastHighest(index-1, 128*NbBarresVirginite, sqrt(128*TPNu_Prior), ptrsc) - sc.Low[index-1] ;		// float FastHighest (int IndexDebut, int NbChandeliersEnArriere, float iteration, s_sc* ptrsc)  
					MouvementVertical64uple 		= FastHighest(index-1, 64*NbBarresVirginite, sqrt(64*TPNu_Prior), ptrsc) - sc.Low[index-1] ;
					MouvementVertical32uple 		= FastHighest(index-1, 32*NbBarresVirginite, sqrt(32*TPNu_Prior), ptrsc) - sc.Low[index-1] ;
					MouvementVertical16uple 		= FastHighest(index-1, 16*NbBarresVirginite, sqrt(16*TPNu_Prior), ptrsc) - sc.Low[index-1] ;
					MouvementVerticalOctuple 		= FastHighest(index-1, 8*NbBarresVirginite, sqrt(8*TPNu_Prior), ptrsc) - sc.Low[index-1] ;
					MouvementVerticalQuadruple 	= FastHighest(index-1, 4*NbBarresVirginite, sqrt(4*TPNu_Prior), ptrsc) - sc.Low[index-1] ;
					MouvementVerticalDouble 		= FastHighest(index-1, 2*NbBarresVirginite, sqrt(2*TPNu_Prior), ptrsc) - sc.Low[index-1] ;
					MouvementVerticalFull 			= FastHighest(index-1, NbBarresVirginite, sqrt(TPNu_Prior), ptrsc) - sc.Low[index-1] ;
					MouvementVerticalMoitie 		= FastHighest(index-1, NbBarresVirginite/2, sqrt(TPNu_Prior), ptrsc) - sc.Low[index-1] ;
					MouvementVerticalQuart 		= FastHighest(index-1, NbBarresVirginite/4, sqrt(TPNu_Prior), ptrsc) - sc.Low[index-1] ;
				}
				else if ( Position == -1 ) 
				{
					MouvementVertical128uple		=  sc.High[index-1] - FastLowest(index-1, 128*NbBarresVirginite, sqrt(128*TPNu_Prior), ptrsc);
					MouvementVertical64uple 		=  sc.High[index-1] - FastLowest(index-1, 64*NbBarresVirginite, sqrt(64*TPNu_Prior), ptrsc);
					MouvementVertical32uple 		=  sc.High[index-1] - FastLowest(index-1, 32*NbBarresVirginite, sqrt(32*TPNu_Prior), ptrsc);
					MouvementVertical16uple 		=  sc.High[index-1] - FastLowest(index-1, 16*NbBarresVirginite, sqrt(16*TPNu_Prior), ptrsc);
					MouvementVerticalOctuple 		=  sc.High[index-1] - FastLowest(index-1, 8*NbBarresVirginite, sqrt(8*TPNu_Prior), ptrsc);
					MouvementVerticalQuadruple 	=  sc.High[index-1] - FastLowest(index-1, 4*NbBarresVirginite, sqrt(4*TPNu_Prior), ptrsc);
					MouvementVerticalDouble 		=  sc.High[index-1] - FastLowest(index-1, 2*NbBarresVirginite, sqrt(2*TPNu_Prior), ptrsc);
					MouvementVerticalFull 			=  sc.High[index-1] - FastLowest(index-1, NbBarresVirginite, sqrt(TPNu_Prior), ptrsc);
					MouvementVerticalMoitie 		=  sc.High[index-1] - FastLowest(index-1, NbBarresVirginite/2, sqrt(TPNu_Prior), ptrsc);
					MouvementVerticalQuart 		=  sc.High[index-1] - FastLowest(index-1, NbBarresVirginite/4, sqrt(TPNu_Prior), ptrsc);					
				}

				// Affichage des mouvements verticaux relatifs :
				float MouvementVertical128upleNorme ;
				float MouvementVertical64upleNorme ;
				float MouvementVertical32upleNorme ;
				float MouvementVertical16upleNorme ;
				if ( Position != 0 )
				{
					// 128uple :
					MouvementVertical128upleNorme = MouvementVertical128uple/(EnvergureMoyenneFull*8*sqrt(2)) ;
					if ( fabs(MouvementVertical128upleNorme) < 100 ) 	{ Texte_x128.Format("x128=%.0f%%", MouvementVertical128upleNorme*100) ;   } 
					// 64uple :
					MouvementVertical64upleNorme = MouvementVertical64uple/(EnvergureMoyenneFull*8) ;
					if ( fabs(MouvementVertical128upleNorme) < 100 ) 	{ Texte_x64.Format("x64=%.0f%%", MouvementVertical64upleNorme*100) ;  }
					// 32uple :
					MouvementVertical32upleNorme = MouvementVertical32uple/(EnvergureMoyenneFull*4*sqrt(2)) ;
					if ( fabs(MouvementVertical32upleNorme) < 100 ) 	{ Texte_x32.Format("x32=%.0f%%", MouvementVertical32upleNorme*100) ;    }
					// 16uple :
					MouvementVertical16upleNorme = MouvementVertical16uple/(EnvergureMoyenneFull*4) ;
					if ( fabs(MouvementVertical16upleNorme) < 100 ) 	{ Texte_x16.Format("x16=%.0f%%", MouvementVertical64upleNorme*100) ;    }
					// octuple :
					float MouvementVerticalOctupleNorme = MouvementVerticalOctuple/(EnvergureMoyenneFull*2*sqrt(2)) ;
					Texte_x8.Format("x8=%.0f%%", MouvementVerticalOctupleNorme*100) ; 
					// quadruple :
					float MouvementVerticalQuadrupleNorme = MouvementVerticalQuadruple/(EnvergureMoyenneFull*2) ;
					Texte_x4.Format("x4=%.0f%%", MouvementVerticalQuadrupleNorme*100) ;
					// double :
					float MouvementVerticalDoubleNorme = MouvementVerticalDouble/(EnvergureMoyenneFull*sqrt(2)) ;
					Texte_x2.Format("x2=%.0f%%", MouvementVerticalDoubleNorme*100) ; 
					// full :
					float MouvementVerticalFullNorme = MouvementVerticalFull/EnvergureMoyenneFull ;	
					Texte_x1.Format("x1=%.0f%%", MouvementVerticalFullNorme*100) ;  
					// moitié :
					float MouvementVerticalMoitieNorme = MouvementVerticalMoitie/(EnvergureMoyenneFull/sqrt(2)) ;
					Texte_x05.Format("x0.5=%.0f%%", MouvementVerticalMoitieNorme*100) ;
					// quart :
					float MouvementVerticalQuartNorme = MouvementVerticalQuart/(EnvergureMoyenneFull/2) ;
					Texte_x025.Format("x0.25=%.0f%%", MouvementVerticalQuartNorme*100) ; 
					// récapitulatifs x128, x64, x32, x16, x8, x4, x2, x1, x0.5, x0.25 sous entrée :
					if ( fabs(MouvementVertical16upleNorme) > 100 )  // si MouvementVertical16upleNorme n'a pas pu être déterminé, alors x32, x64, x128 ne l'ont pas été non plus.
					{	Texte_Recapitulatif.Format("Mvts Verticaux (%%) : x128=n.d. / x64=n.d. / x32=n.d. / x16=n.d. / x8=%.0f / x4=%.0f / x2=%.0f /x1=%.0f / x0.5=%.0f / x0.25=%.0f  ", MouvementVerticalOctupleNorme*100, MouvementVerticalQuadrupleNorme*100,MouvementVerticalDoubleNorme*100,MouvementVerticalFullNorme*100,MouvementVerticalMoitieNorme*100,MouvementVerticalQuartNorme*100) ;  }
					else if ( fabs(MouvementVertical32upleNorme) > 100 )
					{	Texte_Recapitulatif.Format("Mvts Verticaux (%%) : x128=n.d. / x64=n.d. / x32=n.d. / x16=%.0f / x8=%.0f / x4=%.0f / x2=%.0f /x1=%.0f / x0.5=%.0f / x0.25=%.0f  ",MouvementVertical16upleNorme*100, MouvementVerticalOctupleNorme*100,MouvementVerticalQuadrupleNorme*100,MouvementVerticalDoubleNorme*100,MouvementVerticalFullNorme*100,MouvementVerticalMoitieNorme*100,MouvementVerticalQuartNorme*100) ;  }
					else if ( fabs(MouvementVertical64upleNorme) > 100 )
					{	Texte_Recapitulatif.Format("Mvts Verticaux (%%) : x128=n.d. / x64=n.d. / x32=%.0f / x16=%.0f / x8=%.0f / x4=%.0f / x2=%.0f /x1=%.0f / x0.5=%.0f / x0.25=%.0f  ",MouvementVertical32upleNorme*100,MouvementVertical16upleNorme*100, MouvementVerticalOctupleNorme*100,MouvementVerticalQuadrupleNorme*100,MouvementVerticalDoubleNorme*100,MouvementVerticalFullNorme*100,MouvementVerticalMoitieNorme*100,MouvementVerticalQuartNorme*100) ;  }
					else if ( fabs(MouvementVertical128upleNorme) > 100 )
					{	Texte_Recapitulatif.Format("Mvts Verticaux (%%) : x128=n.d. / x64=%.0f / x32=%.0f / x16=%.0f / x8=%.0f / x4=%.0f / x2=%.0f /x1=%.0f / x0.5=%.0f / x0.25=%.0f  ",MouvementVertical64upleNorme*100,MouvementVertical32upleNorme*100,MouvementVertical16upleNorme*100, MouvementVerticalOctupleNorme*100,MouvementVerticalQuadrupleNorme*100,MouvementVerticalDoubleNorme*100,MouvementVerticalFullNorme*100,MouvementVerticalMoitieNorme*100,MouvementVerticalQuartNorme*100) ;  }
					else 
					{	Texte_Recapitulatif.Format("Mvts Verticaux (%%) : x128=%.0f / x64=%.0f / x32=%.0f / x16=%.0f / x8=%.0f / x4=%.0f / x2=%.0f /x1=%.0f / x0.5=%.0f / x0.25=%.0f  ",MouvementVertical128upleNorme*100,MouvementVertical64upleNorme*100,MouvementVertical32upleNorme*100,MouvementVertical16upleNorme*100,MouvementVerticalOctupleNorme*100,MouvementVerticalQuadrupleNorme*100,MouvementVerticalDoubleNorme*100,MouvementVerticalFullNorme*100,MouvementVerticalMoitieNorme*100,MouvementVerticalQuartNorme*100) ;  }
				}


				
				
				// TRACES DES RAYS DE L'INDICATEUR COMPTE-TICKS
				if ( CompteTicksONOFF == 1 )
				{
					if ( SommeRatioAvecMalus[index-1] < 1.6 )       // Les DrawRay_UTAMAOA_UDD sont tracés entre les points (IndexPremiereTouche,NiveauStopLoss) et (IndexPremiereTouche+X, NiveauStopLoss+2*Position*StopLoss). Ils interceptent la ligne NiveauFutureAjuste à " index == IndexPremiereTouche + X/2 ".
					{
						/* // Composante Y du vecteur (X,Y) qui définit la direction du DrawRay au départ du point (index, NiveauStopLoss).  // X est le compte-ticks
						float ComposanteY = 2*Position*SL_Prior/(1+BonusExcedentaire_Prior/100)/(2-SommeRatioAvecMalus[index-1]) ;
						// ligne violette
						if ( TPNu_Prior <= 3 )
						{ DrawRay_UTAMAOA_UDD(index, NiveauStopLoss, index+55, NiveauStopLoss+ComposanteY, RGB(178,102,255), ptrsc, 10083+10000*ThisStudyID) ;  }         // void DrawRay_UTAMAOA_UDD (int index1, float prix1, int index2, float prix2, COLORREF couleur, s_sc* ptrsc, int Line_Number)
						// ligne verte
						if ( TPNu_Prior <= 4 )
						{ DrawRay_UTAMAOA_UDD(index, NiveauStopLoss, index+65, NiveauStopLoss+ComposanteY, RGB(12,237,12), ptrsc, 10084+10000*ThisStudyID) ;  }  
						// ligne jaune
						if ( TPNu_Prior <= 5 )
						{ DrawRay_UTAMAOA_UDD(index, NiveauStopLoss, index+80, NiveauStopLoss+ComposanteY, RGB(235,235,0), ptrsc, 10085+10000*ThisStudyID) ;  }
						// ligne orange (800 ticks si bonus = 0)
						if ( TPNu_Prior  >= 3 and TPNu_Prior <= 6 )
						{ DrawRay_UTAMAOA_UDD(index, NiveauStopLoss, index+100, NiveauStopLoss+ComposanteY, RGB(250,180,100), ptrsc, 10086+10000*ThisStudyID) ;  }
						// ligne rouge
						if ( TPNu_Prior  >= 4 and TPNu_Prior <= 7 )     
						{ DrawRay_UTAMAOA_UDD(index, NiveauStopLoss, index+150/2, NiveauStopLoss+ComposanteY/2, RGB(255,0,0), ptrsc, 10087+10000*ThisStudyID) ;  }     // attention : il y a un bug quand on trace en temps réel avec un index+X, pour X>150   => diviser par 2 les composantes (X, Y) du vecteur du DrawRay pour tomber sur X/2 inférieur à 150
						// ligne brune     
						if ( TPNu_Prior  >= 5 and TPNu_Prior <= 8 )   
						{ DrawRay_UTAMAOA_UDD(index, NiveauStopLoss, index+200/2, NiveauStopLoss+ComposanteY/2, RGB(168,84,0), ptrsc, 10088+10000*ThisStudyID) ;  }
						// ligne grise claire
						if ( TPNu_Prior  >= 6 and TPNu_Prior <= 9 )
						{ DrawRay_UTAMAOA_UDD(index, NiveauStopLoss, index+250/2, NiveauStopLoss+ComposanteY/2, RGB(200,200,200), ptrsc, 10089+10000*ThisStudyID) ;  }
						// ligne grise foncée
						if ( TPNu_Prior  >= 7 and TPNu_Prior <= 10 )       
						{ DrawRay_UTAMAOA_UDD(index, NiveauStopLoss, index+300/2, NiveauStopLoss+ComposanteY/2, RGB(165,165,165), ptrsc, 10090+10000*ThisStudyID) ;  }
						// ligne noire claire
						if ( TPNu_Prior  >= 8 and TPNu_Prior <= 11 )
						{ DrawRay_UTAMAOA_UDD(index, NiveauStopLoss, index+350/3, NiveauStopLoss+ComposanteY/3, RGB(140,140,140), ptrsc, 10091+10000*ThisStudyID) ;  }
						// ligne noire foncée
						if ( TPNu_Prior  >= 9 and TPNu_Prior <= 12 )
						{ DrawRay_UTAMAOA_UDD(index, NiveauStopLoss, index+400/3, NiveauStopLoss+ComposanteY/3, RGB(105,105,105), ptrsc, 10092+10000*ThisStudyID) ;  } */
					}
					else
					{ 
						BasculeTP0Depassee = 1 ;
					}
					
				} // fin du compte-ticks

				// Background de la première touche en gris
				Background[index] = 1 ;  
				Background.DataColor[index] = RGB(210,210,210) ; 

				// Tracé de trait vertical à 0.5*NbBarresVirginite avant IndexPremiereTouche
				int indexTraitVertical = IndexPremiereTouche-0.5*NbBarresVirginite_Prior-1 ;
				Background[indexTraitVertical] = 1 ;  
				Background.DataColor[indexTraitVertical] = RGB(210,210,210) ; 
				
				// Affichage d'un diamant sur le graphes externe UT20s sur le chandelier correspondant à l'entrée en position sur future.
				if ( FutureOuIndice == 1 )
				{
					int ExternalChartIndex_UT20s_Diamond = sc.GetContainingIndexForDateTimeIndex(15, IndexPremiereTouche) ;    // graphe #15
					DrawMarker_UTAMAOA_UDD_SurGrapheExterne (MARKER_DIAMOND, ExternalChartIndex_UT20s_Diamond, NiveauIndice+(float)AjustementChoisiEnPas/4+AjustementSubiPoints, 6, CouleurPosition, ptrsc, 10054+10000*ThisStudyID, 15, 0) ;      
				}
				
			} // fin de " if (PremiereToucheFaite==1) "  /////////////////////////////////////////////
			
		
			// Tracé de BasculeTP0 ; détermination de BasculeTP0Depassee    //////////////////         
			if ( PremiereToucheFaite > 0  and  BasculeTP0Depassee == 0  and  CompteTicksONOFF == 1  and  SommeRatioAvecMalus[IndexPremiereTouche-1] < 1.6 )
			{
				float NbChandelierPourIntercepterTP0 ;   // cette valeur est exprimée en nb de chandeliers de 16 ticks
				float TPNu_PostTouche ;
				if ( TPNu_Manuel_Input.GetFloat() == 0 ) TPNu_PostTouche = ArrayTPNu[index] ;  else TPNu_PostTouche = TPNu_Manuel_Input.GetFloat() ;
				if ( TPNu_PostTouche > 5 )  		{	NbChandelierPourIntercepterTP0 =  25 * TPNu_PostTouche - 75 ;  }  // TP supérieur à ligne orange (Nb=50)
				else if (TPNu_PostTouche > 4 )	{	NbChandelierPourIntercepterTP0 =  10 * TPNu_PostTouche ;  }  // TP entre ligne jaune (40) et orange (50)
				else if (TPNu_PostTouche > 3 )	{	NbChandelierPourIntercepterTP0 =  7.5 * TPNu_PostTouche + 10 ;  }  // TP entre ligne verte (32.5) et jaune (40)
				else if (TPNu_PostTouche > 2 )	{	NbChandelierPourIntercepterTP0 =  5 * TPNu_PostTouche + 17.5 ;  }  // TP entre ligne violette (27.5) et verte (32.5)
				else 								{	NbChandelierPourIntercepterTP0 =  5 * TPNu_PostTouche + 17.5 ;  }  // idem précédent, par défaut de mieux
				
				// Pour modèles : 0, 1, 2, 5, 6, 7, 8, 9
				BasculeTP0[index] 	= NiveauStopLoss + Position * SL_Prior * ( index - IndexPremiereTouche ) / NbChandelierPourIntercepterTP0 /(1+BonusExcedentaire/100)/(2-fmaxf(0,SommeRatioAvecMalus[IndexPremiereTouche-1]))/(fmaxf((2.2-RatioSigma[IndexPremiereTouche-1]), 0.01)) ;         	// CompteTicks = index - IndexPremiereTouche
				
				// Dépassement de BasculeTP0 (Future et Indice)
				{
					if (FutureOuIndice == 0 )  // trading sur future
					{
						if ( Position == -1  and  sc.High[index] > BasculeTP0[index] )
						{
							BasculeTP0Depassee = 1 ;
							DrawMarker_UTAMAOA_UDD (MARKER_TRIANGLEUP, index, NiveauFutureAjuste, 10, RGB(100,100,100), ptrsc, 10114+10000*ThisStudyID) ;                               // void DrawMarker_UTAMAOA_UDD (int marqueur, int index, float prix, int tailleObjet, COLORREF couleur, s_sc* ptrsc, int Line_Number)  
						}
						else if ( Position == 1  and  sc.Low[index] < BasculeTP0[index] )
						{
							BasculeTP0Depassee = 1 ;
							DrawMarker_UTAMAOA_UDD (MARKER_TRIANGLEDOWN, index, NiveauFutureAjuste, 10, RGB(100,100,100), ptrsc, 10114+10000*ThisStudyID) ;
						}
					}
					else if  (FutureOuIndice == 1 )  // trading sur indice
					{
						if ( Position == -1  and  INQ100AvecCorrection[index] > BasculeTP0[index] )
						{
							BasculeTP0Depassee = 1 ;
							// DrawMarker_UTAMAOA_UDD (MARKER_TRIANGLEUP, index, NiveauFutureAjuste, 10, RGB(100,100,100), ptrsc, 10114+10000*ThisStudyID) ;
							Background[index] = 1 ;  Background.DataColor[index] = RGB(210,210,210) ;
						}
						else if ( Position == 1  and  INQ100AvecCorrection[index] < BasculeTP0[index] )
						{
							BasculeTP0Depassee = 1 ;
							// DrawMarker_UTAMAOA_UDD (MARKER_TRIANGLEDOWN, index, NiveauFutureAjuste, 10, RGB(100,100,100), ptrsc, 10114+10000*ThisStudyID) ;
							Background[index] = 1 ;  Background.DataColor[index] = RGB(210,210,210) ;
						}
					}
				}				
			}
		
		

			// Dépassement de StopLoss indice lors du trading sur indice     //////////////////
			if ( PremiereToucheFaite > 0  and  StopLossIndiceDepasse == 0  and  FutureOuIndice == 1 )  
			{
				if ( Position == -1  and  INQ100AvecCorrection[index] > NiveauStopLoss )
				{
					StopLossIndiceDepasse = 1 ;
					Background[index] = 1 ;  Background.DataColor[index] = RGB(210,210,210) ;
				}
				else if ( Position == 1  and  INQ100AvecCorrection[index] < NiveauStopLoss )
				{
					StopLossIndiceDepasse = 1 ;
					Background[index] = 1 ;  Background.DataColor[index] = RGB(210,210,210) ;
				}
			}
			if ( StopLossIndiceDepasse  and  ! RobotActif )
			{
				IndexArretManualLooping = index ;
				if ( Position == 1 ) PrixSL = sc.Low[IndexArretManualLooping] ;  else if ( Position == -1 ) PrixSL = sc.High[IndexArretManualLooping] ;
				Texte_SL.Format ("SL = %.2f pts", fabs( NiveauFutureAjuste - PrixSL) ) ;
				break ;		// sortie du manual looping
			}
			

			// Dépassement de TakeProfit indice lors du trading sur indice     //////////////////
			if ( PremiereToucheFaite > 0  and  TakeProfitIndiceDepasse == 0  and  FutureOuIndice == 1 )  
			{
				if ( Position == -1  and  sc.Low[index] < NiveauFutureAjuste+Position*TPOptimumArrondi_Prior )
				{
					TakeProfitIndiceDepasse = 1 ;
					Background[index] = 1 ;  Background.DataColor[index] = RGB(210,210,210) ;
				}
				else if ( Position == 1  and  sc.High[index] > NiveauFutureAjuste+Position*TPOptimumArrondi_Prior )
				{
					TakeProfitIndiceDepasse = 1 ;
					Background[index] = 1 ;  Background.DataColor[index] = RGB(210,210,210) ;
				}
			}
			if ( TakeProfitIndiceDepasse  and  ! RobotActif )
			{
				IndexArretManualLooping = index ;
				break ;		// sortie du manual looping
			}
		
		
		
			// Gestion de l'affichage des Subgraph
			{
				// Projections de RatioAUC, RatioSigma, SommeRatio sur le graphique      //////////////////
				RatioAUCProjete[index] = BaseDuTrace + RatioAUCBis * ( NiveauFutureAjuste - BaseDuTrace ) ;
				RatioSigmaProjete[index] = BaseDuTrace + RatioSigmaBis * ( NiveauFutureAjuste - BaseDuTrace ) ;
				SommeRatioAvecMalusProjete[index] = BaseDuTrace + SommeRatioAvecMalusBis * ( NiveauFutureAjuste - BaseDuTrace ) ;
				MalusExcedentaireProjete[index] = BaseDuTrace + MalusExcedentaireBis * ( NiveauFutureAjuste - BaseDuTrace ) ;
				
				
				// Projections de NiveauStopLoss, NiveauFutureAjuste, 1*TPCible, 2*TPCible, 3*TPCible, 4*TPCible, TPOptimum   //////////////////
				if ( PremiereToucheFaite == 0 )  { StopLossProjete[index] = BaseDuTrace ;  }  else { StopLossProjete[index] = NiveauStopLoss ; }  // on pourrait aussi ne pas tracer StopLossProjete avant entrée en position ?
				Niveau1TP[index] = NiveauFutureAjuste + Position * 1 * TPCible ;
				Niveau2TP[index] = NiveauFutureAjuste + Position * 2 * TPCible ;
				Niveau3TP[index] = NiveauFutureAjuste + Position * 3 * TPCible ; 
				Niveau4TP[index] = NiveauFutureAjuste + Position * 4 * TPCible ;    
				NiveauTPOptimum[index] = NiveauFutureAjuste + Position * TPOptimum ;
				NiveauTPOptimumArrondi[index] = NiveauFutureAjuste + Position * TPOptimumArrondi ;
				
				// Background en noir si données sources de Sigma sont manquantes   //////////////////
				if ( ArraySigmaBackground[index] == 1 )
				{ 	Background[index] = 1 ;  Background.DataColor[index] = RGB(100,100,100) ;  }	
			}		
		
			
/* 			// IndexArretManualLooping : doit être rééxaminée après chaque index
			{
				if ( RobotActif )
				{	IndexArretManualLooping = sc.ArraySize ;    }   // si RobotActif, le looping doit être poursuivi indéfiniment pour gérer le stop loss
				else 
				{	IndexArretManualLooping = std::min(sc.ArraySize, IndexPremiereTouche+3*NbBarresVirginite)  ;  }
			} */
			
		} // fin boucle exécutée sur toutes les bougies après HoraireRAZ

	} 
	// fin du MANUAL LOOPING //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	
	// Variables utiles en dehors du bloc de looping       ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	{
		// TriggerResetScale
		if (TriggerResetScale )    //  le Trigger a été déclenché à l'appel n-1 du DLL ; on veut qu'il soit exécuté à la fin de l'appel durant lequel a lieu un Recalcuate
		{
			sc.ResetAllScales = 1 ;
			TriggerResetScale = 0 ;
		}
	}

	
	// Le robot est-il actif ?  Est-il en position ?     On doit refaire cette détermination après le bloc "Statut des control bar buttons"
	if ( AutoTradeStatus != 0 and AutoTradeStatus != 50 and AutoTradeStatus != 100 and AutoTradeStatus != 110 and AutoTradeStatus != 120 and AutoTradeStatus != 130 and AutoTradeStatus != 140 and AutoTradeStatus != 150 and AutoTradeStatus != 160 and AutoTradeStatus != 170 and AutoTradeStatus != 180 and AutoTradeStatus != 190  and AutoTradeStatus != 200  and AutoTradeStatus != 1000 )    // AutoTradeStatus n'est ni "OFF" ni "ordres et position fermes" => le robot est actif
	{	
		RobotActif = 1 ;  
		if ( AutoTradeStatus != 11 )  RobotPosition.SetInt(Position) ;  else RobotPosition.SetInt(0) ; 
	}
	else  // robot inactif
	{	
		if ( SequenceONOFF_Input.GetIndex() == 0 )  // Sequence
		{
			if ( SequenceStatus == 1 )    // le robot est inactif et le premier trade est fini 
			{
				SequenceStatus = 2;  // le 2ème trade sur trois est lancé
				RobotActif = 1 ;  
				RobotPosition.SetInt(Position) ;
				AutoTradeStatus = 10 ; 
				// MyEP = 999999 ;
				SetButton52And56Text (ptrsc) ;   // "AutoTrade : demarre (Seq : trade 2)"
				{  SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | SequenceStatus = 2") ;  sc.AddMessageToLog(message,0) ;  }    

				// Modification de l'ajustement choisi
				AjustementChoisiEnPas = 0;   // on décale de 0 ticks
				AjustementChoisiEnPas_Input.SetCustomInputIndex(22);	 
				TriggerRecalculate = 1 ;  sc.UpdateAlways = 1 ;  return;
			}
			else if ( SequenceStatus == 2 )   // le robot est inactif et le 2ème trade est fini 
			{
				SequenceStatus = 3;  // le 3ème trade sur trois est lancé
				RobotActif = 1 ;  
				RobotPosition.SetInt(Position) ;
				AutoTradeStatus = 10 ; 
				// MyEP = 999999 ;
				SetButton52And56Text (ptrsc) ;   // "AutoTrade : demarre (Seq : trade 3)"
				{  SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | SequenceStatus = 3") ;  sc.AddMessageToLog(message,0) ;  }    
				
				// Modification de l'ajustement choisi
				AjustementChoisiEnPas = - Position * round( 4 * TP_AUC * (1+BonusIntrinseque/400.0) ) ;   
				AjustementChoisiEnPas_Input.SetCustomInputIndex(22-AjustementChoisiEnPas);	 
				TriggerRecalculate = 1 ;  sc.UpdateAlways = 1 ;  return;
			}
			else if ( SequenceStatus == 3 )   // le robot est inactif et le 3ème trade est fini 
			{
				SequenceStatus = 0;  
				RobotActif = 0 ;  
				RobotPosition.SetInt(0) ;
				AutoTradeStatus = 180 ; 
				SetButton52And56Text (ptrsc) ;   // "AutoTrade : OFF car Seq finie"
				{  SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | La sequence est terminee | SequenceStatus = 0") ;  sc.AddMessageToLog(message,0) ;  }    
			}
			else // SequenceStatus == 0
			{
				RobotActif = 0 ;
				RobotPosition.SetInt(0) ;
			}
		}
		else
		{
			RobotActif = 0 ;
			RobotPosition.SetInt(0) ;
		}
		
	}


	// On détermine la couleur des tracés DrawText (nécessaire dès le robot)
	{
		if (  CetteInstanceControleLesBoutons.GetYesNo() == 1  )  
			{ CouleurDrawText = RGB(0,127,255) ; }
		else
			{ CouleurDrawText = RGB(0,0,0) ; }
	}

	
	// ROBOT DE TRADING //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	{
		LARGE_INTEGER ElapsedTicks_BlocRobotDeTrading ;  QueryPerformanceCounter(&ElapsedTicks_BlocRobotDeTrading) ;  
		
		if ( RobotActif )    // AutoTradeStatus n'est ni "OFF" ni "ordres et position fermes" => le robot est actif
		{
			
			// 0) // Calculs préliminaires
			if ( ! sc.IsFullRecalculation )   // à cause de sc.UpdateStartIndex
			{
				// Calcul de Danger Proche
				TPCibleMajore = fmax (TPCible, 0.75) ;    // valeur minimum de 0.75 pts pour TPCible afin de durcir les situations de faible TPCible.		// TPCible = TP_AUC avant entrée en position.		
				
				// On détermine DistanceMiniAuDanger
				if (FutureOuIndice == 0 )
				{
					// Détermination de DistanceMiniRecenteAuNiveau_Future
					if ( Position == 1 )  		DistanceMiniRecenteAuNiveau_Future = Lowest(LastIndex, LastIndex-sc.UpdateStartIndex+1, ptrsc) - NiveauFutureAjuste ;                          // float Lowest (int IndexDebut, int NbChandeliersEnArriere, s_sc* ptrsc)
					else if ( Position == -1 )	DistanceMiniRecenteAuNiveau_Future = NiveauFutureAjuste - Highest(LastIndex, LastIndex-sc.UpdateStartIndex+1, ptrsc) ;
				
					DistanceMiniAuDanger = DistanceMiniRecenteAuNiveau_Future / TPCibleMajore + 1 ; 
					DistanceMiniRecenteAuNiveau_Indice = 0 ;  // on met cette valeur à 0 pour l'affichage dans le log
				}
				else if (FutureOuIndice == 1 )
				{
					// Détermination de DistanceMiniRecenteAuNiveau_Future
					if ( Position == 1 )  		DistanceMiniRecenteAuNiveau_Future = Lowest(LastIndex, LastIndex-sc.UpdateStartIndex+1, ptrsc) - NiveauFutureAjuste ;                          																								// float Lowest (int IndexDebut, int NbChandeliersEnArriere, s_sc* ptrsc)
					else if ( Position == -1 )	DistanceMiniRecenteAuNiveau_Future = NiveauFutureAjuste - Highest(LastIndex, LastIndex-sc.UpdateStartIndex+1, ptrsc) ;
				
					// Détermination de DistanceMiniRecenteAuNiveau_Indice
					if ( Position == 1 )  		DistanceMiniRecenteAuNiveau_Indice = Lowest_Array(INQ100SansCorrection, LastIndex, LastIndex-sc.UpdateStartIndex+1) - NiveauIndice - Position*(AjustementChoisiEnPas*0.25+AjustementSubiPoints) ;   // float Lowest_Array (SCFloatArray& Array, int IndexDebut, int NbChandeliersEnArriere)
					else if ( Position == -1 )	DistanceMiniRecenteAuNiveau_Indice = NiveauIndice - Highest_Array(INQ100SansCorrection, LastIndex, LastIndex-sc.UpdateStartIndex+1) - Position*(AjustementChoisiEnPas*0.25+AjustementSubiPoints) ;
					
					DistanceMiniAuDanger = std::min(DistanceMiniRecenteAuNiveau_Future/TPCibleMajore+1, DistanceMiniRecenteAuNiveau_Indice/TPCibleMajore) ; 
				}
			}
			else { DistanceMiniAuDanger = 999999 ;  }

		
			// 1) // Récupération de ParentOrderDetails, Target1OrderDetails et Stop1OrderDetails, pour calculer PositionOuverte et OrdreOuvert   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			{
				if ( AutoTradeStatus == 20  or  AutoTradeStatus == 30  or  AutoTradeStatus == 60  or  AutoTradeStatus == 70 )          // en principe ParentOrderID, Target1OrderID et Stop1OrderID doivent exister (lors d'une sim locale, ils sont effacés dès leur annulation, et ça fait bugger Sierra)
				{
					s_SCTradeOrder ParentOrderDetails ; 
					int ResultatGetOrderByOrderID = sc.GetOrderByOrderID(ParentOrderID, ParentOrderDetails) ;
					if ( ResultatGetOrderByOrderID != 1  and  ParentOrderID_EchecGetOrder_MessageDansLog == 0 )
					{  SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | !! ERREUR !! sc.GetOrderByOrderID pour ParentOrderID = %d ; Code erreur = %d ; lancer un KILL ?", ParentOrderID, ResultatGetOrderByOrderID) ;  sc.AddMessageToLog(message,1) ;   ParentOrderID_EchecGetOrder_MessageDansLog = 1 ;  }             
				
					s_SCTradeOrder Target1OrderDetails ;
					ResultatGetOrderByOrderID = sc.GetOrderByOrderID(Target1OrderID, Target1OrderDetails) ;
					if ( ResultatGetOrderByOrderID != 1  and  Target1OrderID_EchecGetOrder_MessageDansLog == 0 )
					{  SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | !! ERREUR !! sc.GetOrderByOrderID pour Target1OrderID = %d ; Code erreur = %d ; lancer un KILL ?", Target1OrderID, ResultatGetOrderByOrderID) ;  sc.AddMessageToLog(message,1) ;  Target1OrderID_EchecGetOrder_MessageDansLog = 1 ;  }
				
					s_SCTradeOrder Stop1OrderDetails ;
					ResultatGetOrderByOrderID = sc.GetOrderByOrderID(Stop1OrderID, Stop1OrderDetails) ;
					if ( ResultatGetOrderByOrderID != 1  and  Stop1OrderID_EchecGetOrder_MessageDansLog == 0 )
					{  SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | !! ERREUR !! sc.GetOrderByOrderID pour Stop1OrderID = %d ; Code erreur = %d ; lancer un KILL ?", Stop1OrderID, ResultatGetOrderByOrderID) ;  sc.AddMessageToLog(message,1) ;  Stop1OrderID_EchecGetOrder_MessageDansLog = 1 ;  }
				
					// Détermination des variables OrderStatusCode qui seront reportées dans le bloc event "CTRL+S"
					ParentOrderDetails_OrderStatusCode = ParentOrderDetails.OrderStatusCode ;
					Target1OrderDetails_OrderStatusCode = Target1OrderDetails.OrderStatusCode ;
					Stop1OrderDetails_OrderStatusCode = Stop1OrderDetails.OrderStatusCode ;

				
					// Calcul de PositionOuverte //////
					{
						if ( ParentOrderDetails.OrderStatusCode == SCT_OSC_FILLED )           // l'ordre parent a été filled			
						{ 
							if ( Target1OrderDetails.OrderStatusCode == SCT_OSC_FILLED  or  Stop1OrderDetails.OrderStatusCode == SCT_OSC_FILLED  or  ( Target1OrderDetails.OrderStatusCode == SCT_OSC_CANCELED  and  Stop1OrderDetails.OrderStatusCode == SCT_OSC_CANCELED  ) )  
							{	PositionOuverte = 2 ;  }		// l'un des child orders a été filled, ou ils ont été annulés  (i.e. le robot est redevenu inactif)
							else    
							{	PositionOuverte = 1 ;  }		// Position ouverte, mais child order non filled, ni annulé
						}
						else  
						{ PositionOuverte = 0 ; }		// l'ordre parent n'a pas été filled
					}
					
					// Détermination de OrdreOuvert //////   on simplifie ce bloc en utilisant IsWorkingOrderStatus. Note : ça a supprimé les conditions SCT_OSC_UNSPECIFIED et SCT_OSC_ERROR
					{
						/* // OrdreOuvertParent			
						if ( ParentOrderDetails.OrderStatusCode == SCT_OSC_UNSPECIFIED or ParentOrderDetails.OrderStatusCode == SCT_OSC_ORDERSENT  or  ParentOrderDetails.OrderStatusCode == SCT_OSC_PENDINGOPEN  or  ParentOrderDetails.OrderStatusCode == SCT_OSC_PENDING_CHILD_CLIENT 
							or  ParentOrderDetails.OrderStatusCode == SCT_OSC_PENDING_CHILD_SERVER or  ParentOrderDetails.OrderStatusCode == SCT_OSC_OPEN or  ParentOrderDetails.OrderStatusCode == SCT_OSC_PENDINGMODIFY or  ParentOrderDetails.OrderStatusCode == SCT_OSC_PENDINGCANCEL
							or  ParentOrderDetails.OrderStatusCode == SCT_OSC_ERROR )  // statuts pertinents pour une demande d'annulation
						{ OrdreOuvertParent = 1 ;  }
						else { OrdreOuvertParent = 0 ; }
						// OrdreOuvertTarget1
						if ( Target1OrderDetails.OrderStatusCode == SCT_OSC_UNSPECIFIED or Target1OrderDetails.OrderStatusCode == SCT_OSC_ORDERSENT  or  Target1OrderDetails.OrderStatusCode == SCT_OSC_PENDINGOPEN  or  Target1OrderDetails.OrderStatusCode == SCT_OSC_PENDING_CHILD_CLIENT 
							or  Target1OrderDetails.OrderStatusCode == SCT_OSC_PENDING_CHILD_SERVER or  Target1OrderDetails.OrderStatusCode == SCT_OSC_OPEN or  Target1OrderDetails.OrderStatusCode == SCT_OSC_PENDINGMODIFY or  Target1OrderDetails.OrderStatusCode == SCT_OSC_PENDINGCANCEL
							or  Target1OrderDetails.OrderStatusCode == SCT_OSC_ERROR )  // statuts pertinents pour une demande d'annulation
						{ OrdreOuvertTarget1 = 1 ;  }
						else { OrdreOuvertTarget1 = 0 ; }
						// OrdreOuvertStop1
						if ( Stop1OrderDetails.OrderStatusCode == SCT_OSC_UNSPECIFIED or Stop1OrderDetails.OrderStatusCode == SCT_OSC_ORDERSENT  or  Stop1OrderDetails.OrderStatusCode == SCT_OSC_PENDINGOPEN  or  Stop1OrderDetails.OrderStatusCode == SCT_OSC_PENDING_CHILD_CLIENT 
							or  Stop1OrderDetails.OrderStatusCode == SCT_OSC_PENDING_CHILD_SERVER or  Stop1OrderDetails.OrderStatusCode == SCT_OSC_OPEN or  Stop1OrderDetails.OrderStatusCode == SCT_OSC_PENDINGMODIFY or  Stop1OrderDetails.OrderStatusCode == SCT_OSC_PENDINGCANCEL
							or  Stop1OrderDetails.OrderStatusCode == SCT_OSC_ERROR )  // statuts pertinents pour une demande d'annulation
						{ OrdreOuvertStop1 = 1 ;  }
						else { OrdreOuvertStop1 = 0 ; } */
						
						// OrdreOuvertParent			
						if ( IsWorkingOrderStatus(ParentOrderDetails.OrderStatusCode) == 1 )  // The IsWorkingOrderStatus function returns TRUE if the order is a working/open order. This means the order status code is: SCT_OSC_ORDERSENT, SCT_OSC_PENDINGOPEN, SCT_OSC_OPEN, SCT_OSC_PENDINGCANCEL, SCT_OSC_PENDINGMODIFY, SCT_OSC_PENDING_CHILD_CLIENT, SCT_OSC_PENDING_CHILD_SERVER. Otherwise, FALSE is returned.
						{ OrdreOuvertParent = 1 ;  }
						else { OrdreOuvertParent = 0 ; }
						
						// OrdreOuvertTarget1
						if ( IsWorkingOrderStatus(Target1OrderDetails.OrderStatusCode) == 1 ) 
						{ OrdreOuvertTarget1 = 1 ;  }
						else { OrdreOuvertTarget1 = 0 ; }
						
						// OrdreOuvertStop1
						if ( IsWorkingOrderStatus(Stop1OrderDetails.OrderStatusCode) == 1 ) 
						{ OrdreOuvertStop1 = 1 ;  }
						else { OrdreOuvertStop1 = 0 ; }
						
						// Si l'un des ordres (parent, target1, stop1) est ouvert, OrdreOuvert  vaut 1.
						if ( OrdreOuvertParent or OrdreOuvertTarget1 or OrdreOuvertStop1 )  { OrdreOuvert = 1 ;  }
						else { OrdreOuvert = 0 ;  }
					}
					
					// Modification de AutoTradeStatus : 6 cas possibles ; 3 valeurs (0, 1 et 2) pour PositionOuverte, multipliées par 2 valeurs (0 et 1) pour OrdreOuvert ; 3*2=6 cas)
					{
						if ( PositionOuverte == 0  and  OrdreOuvert == 1 )    // 1er cas
						{
							if ( AutoTradeStatus != 60 ) // aucune annulation d'ordre n'est en cours
							{
								AutoTradeStatus = 20 ;
								SetButton52And56Text (ptrsc) ;
							}
							else   // l'annulation d'ordres en cours traine
							{
								SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | L'annulation de l'ordre en cours traine (PositionOuverte == 0  and  OrdreOuvert == 1  and  AutoTradeStatus == 60)") ;    sc.AddMessageToLog(message,0) ;       
							}
						}
						else if ( PositionOuverte == 1 )  // cas 2 et 3 : OrdreOuvert == 0 ou 1 (OrdreOuvert vaut 1 en principe)
						{
							AutoTradeStatus = 30 ;
							SetButton52And56Text (ptrsc) ;
						}
						else if ( ( PositionOuverte == 0  or  PositionOuverte == 2 )  and  OrdreOuvert == 0 )  // cas 4 et 5
						{
							AutoTradeStatus = 50 ;			// ''OFF (ordres et position fermes)"
							SetButton52And56Text (ptrsc) ;
						}
						else if ( PositionOuverte == 2  and  OrdreOuvert == 1 )  // cas 6  // la position est fermée, mais il reste un ordre ouvert ; soit il faut attendre qques millisecondes pour annuler les ordres encore ouverts, soit ça se prolonge et c'est anormal (solution : coder l'annulation des ordres problématiques ou appeler FlattenAndCancelAllOrders)
						{
							AutoTradeStatus = 70 ;		// ''pb annulation ordres"
							SetButton52And56Text (ptrsc) ;
							SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | !! ERREUR !! PositionOuverte == 2  and  OrdreOuvert == 1 (la position est fermee, mais il reste un ordre ouvert). Lancer un KILL ?") ;    sc.AddMessageToLog(message,1) ;       
						}
					}
				} 
				else // PositionOuverte et OrdreOuvert n'ont pas été déterminés
				{ PositionOuverte = -1 ; OrdreOuvert = -1 ; }
			}
			
			
			// 2) // gestion des bugs   ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			{  
				// vide pour le moment
			} 
			
			
			// 3) // gestion des événements    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			{
			
				// gestion des événements : lorsque des ordres sont ouverts, mais pas encore d'entrée en position /////////////////////////
				if ( OrdreOuvert == 1  and  PositionOuverte == 0  and DistMiniFuture[LastIndex] >= 0  and  AutoTradeDemandeArret == 0 )    // " PositionOuverte == 0 and DistMiniFuture[LastIndex] >= 0 " n'est pas une redondance. Selon que l'OrderStatus d'IB ou le flux de ticks SC soit plus rapide, PositionOuverte et DistMiniFuture peuvent être contradictoires (contradiction veut dire qu'on est entré en position)
				{
					// RatioAUC est totalement saturé 
					if ( RatioAUC[LastIndex] > 6 and  DistanceMiniAuDanger < 5 )    
					{
						AutoTradeDemandeArret = 1 ;
						AutoTradeStatus = 60 ; 
						SetButton52And56Text (ptrsc) ;
						// Notification sur le graphique
						Texte.Format("!! RatioAUC depasse 600%% et DistanceMiniAuDanger < 4 (Fut) ou 5 (Ind) !!") ;   if ( SequenceONOFF_Input.GetIndex() == 0 )  { Texte.AppendFormat(" | Tr. %d", SequenceStatus) ;  }
						DrawTextAligne_UTAMAOA_UDD (Texte, LastIndex, NiveauFutureAjuste-4*Position*Interligne, 8, RGB(0,0,0), ptrsc, 10010+10000*ThisStudyID) ;
						DrawMarker_UTAMAOA_UDD (MARKER_X, LastIndex, NiveauFutureAjuste, 16, RGB(0,0,0), ptrsc, 10011+10000*ThisStudyID) ; 
						{  SCString message ; message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | RatioAUC depasse 600%% et DistanceMiniAuDanger < 4 (Fut) ou 5 (Ind) | DistanceMiniRecenteAuNiveau_Future = %.2f pts | DistanceMiniRecenteAuNiveau_Indice = %.2f pts | TPCibleMajore = %.2f pts", DistanceMiniRecenteAuNiveau_Future, DistanceMiniRecenteAuNiveau_Indice, TPCibleMajore) ;  sc.AddMessageToLog(message,0) ;  }          
					}
					
					// RatioAUC est saturé    
					else if ( RatioAUC[LastIndex] > 3  and  DistanceMiniAuDanger < 4 )    
					{
						AutoTradeDemandeArret = 1 ;
						AutoTradeStatus = 60 ;
						SetButton52And56Text (ptrsc) ;
						// Notification sur le graphique
						Texte.Format("!! RatioAUC depasse 300%% et DistanceMiniAuDanger < 3 (Fut) ou 4 (Ind) !!") ;   if ( SequenceONOFF_Input.GetIndex() == 0 )  { Texte.AppendFormat(" | Tr. %d", SequenceStatus) ;  }
						DrawTextAligne_UTAMAOA_UDD (Texte, LastIndex, NiveauFutureAjuste-4*Position*Interligne, 8, RGB(0,0,0), ptrsc, 10010+10000*ThisStudyID) ;
						DrawMarker_UTAMAOA_UDD (MARKER_X, LastIndex, NiveauFutureAjuste, 16, RGB(0,0,0), ptrsc, 10011+10000*ThisStudyID) ; 
						{  SCString message ;  message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | RatioAUC depasse 300%% et DistanceMiniAuDanger < 3 (Fut) ou 4 (Ind) | DistanceMiniRecenteAuNiveau_Future = %.2f pts | DistanceMiniRecenteAuNiveau_Indice = %.2f pts | TPCibleMajore = %.2f pts", DistanceMiniRecenteAuNiveau_Future, DistanceMiniRecenteAuNiveau_Indice, TPCibleMajore) ;  sc.AddMessageToLog(message,0) ;  }             
					}
					
					// SommeRatio est légèrement saturé    // On n'utilise pas SommeRatioAvecMalus ici car il fait de grosses amplitudes.
					else if ( ( ( RatioAUC[LastIndex] + RatioSigma[LastIndex] ) / 1.8 > 1.5 ) and  DistanceMiniAuDanger < 3 )    		//  SommeRatio[LastIndex] = ( RatioAUC[LastIndex] + RatioSigma[LastIndex] ) / 1.8
					{
						AutoTradeDemandeArret = 1 ;
						AutoTradeStatus = 60 ;
						SetButton52And56Text (ptrsc) ;
						// Notification sur le graphique
						Texte.Format("!! SommeRatio depasse 150%% et DistanceMiniAuDanger < 2 (Fut) ou 3 (Ind) !!") ;   if ( SequenceONOFF_Input.GetIndex() == 0 )  { Texte.AppendFormat(" | Tr. %d", SequenceStatus) ;  }
						DrawTextAligne_UTAMAOA_UDD (Texte, LastIndex, NiveauFutureAjuste-4*Position*Interligne, 8, RGB(0,0,0), ptrsc, 10010+10000*ThisStudyID) ;
						DrawMarker_UTAMAOA_UDD (MARKER_X, LastIndex, NiveauFutureAjuste, 16, RGB(0,0,0), ptrsc, 10011+10000*ThisStudyID) ; 
						{  SCString message ;  message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | SommeRatio depasse 150%% et DistanceMiniAuDanger < 2 (Fut) ou 3 (Ind) | DistanceMiniRecenteAuNiveau_Future = %.2f pts | DistanceMiniRecenteAuNiveau_Indice = %.2f pts | TPCibleMajore = %.2f pts", DistanceMiniRecenteAuNiveau_Future, DistanceMiniRecenteAuNiveau_Indice, TPCibleMajore) ;  sc.AddMessageToLog(message,0) ;  }             
					}
				
					else if ( SommeRatioAvecMalus[LastIndex] > 1  and  DistanceMiniAuDanger < 3  and  ( DistanceMiniAuDanger < SommeRatioAvecMalus[LastIndex] + 1 ) )    // ie DistanceMiniRecenteAuNiveau_Future = 1.0 correspond à SommeRatioAvecMalus > 100%, DistanceMiniRecenteAuNiveau_Future = 2.0 correspond à SommeRatioAvecMalus > 200%
					{
						AutoTradeDemandeArret = 1 ;
						AutoTradeStatus = 60 ;
						SetButton52And56Text (ptrsc) ;
						// Notification sur le graphique
						Texte.Format("!! SommeRatioAvecMalus depasse 100%% et DistanceMiniAuDanger faible !!") ;   if ( SequenceONOFF_Input.GetIndex() == 0 )  { Texte.AppendFormat(" | Tr. %d", SequenceStatus) ;  }
						DrawTextAligne_UTAMAOA_UDD (Texte, LastIndex, NiveauFutureAjuste-4*Position*Interligne, 8, RGB(0,0,0), ptrsc, 10010+10000*ThisStudyID) ;
						DrawMarker_UTAMAOA_UDD (MARKER_X, LastIndex, NiveauFutureAjuste, 16, RGB(0,0,0), ptrsc, 10011+10000*ThisStudyID) ; 
						{  SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | (SommeRatioAvecMalus depasse 100%%) AND (DistanceMiniAuDanger < 2 (Fut) ou 3 (Ind)) AND (SommeRatioAvecMalus>DistanceMiniAuDanger-1) | DistanceMiniRecenteAuNiveau_Future = %.2f pts | DistanceMiniRecenteAuNiveau_Indice = %.2f pts | TPCibleMajore = %.2f pts", DistanceMiniRecenteAuNiveau_Future, DistanceMiniRecenteAuNiveau_Indice, TPCibleMajore) ;  sc.AddMessageToLog(message,0) ;  }             
					}
					
					// TP est insuffisant
					if ( TPOptimumArrondi  < 0.75  and  DistanceMiniAuDanger < 3 )   
					{
						AutoTradeDemandeArret = 1 ;
						AutoTradeStatus = 60 ;
						SetButton52And56Text (ptrsc) ;
						// Notification sur le graphique
						Texte.Format("!! TP Optimum Arrondi < 0.75 et DistanceMiniAuDanger < 2 (Fut) ou 3 (Ind) !!") ;   if ( SequenceONOFF_Input.GetIndex() == 0 )  { Texte.AppendFormat(" | Tr. %d", SequenceStatus) ;  }
						DrawTextAligne_UTAMAOA_UDD (Texte, LastIndex, NiveauFutureAjuste-5*Position*Interligne, 8, RGB(0,0,0), ptrsc, 10012+10000*ThisStudyID) ;
						DrawMarker_UTAMAOA_UDD (MARKER_X, LastIndex, NiveauFutureAjuste, 16, RGB(0,0,0), ptrsc, 10013+10000*ThisStudyID) ; 
						{  SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | TP Optimum Arrondi < 0.75 et DistanceMiniAuDanger < 2 (Fut) ou 3 (Ind) | DistanceMiniRecenteAuNiveau_Future = %.2f pts | DistanceMiniRecenteAuNiveau_Indice = %.2f pts | TPCibleMajore = %.2f pts", DistanceMiniRecenteAuNiveau_Future, DistanceMiniRecenteAuNiveau_Indice, TPCibleMajore) ;  sc.AddMessageToLog(message,0) ;  }             
					}
					
					// BonusMoy est insuffisant
					if ( BonusMoy  < 4  and  DistanceMiniAuDanger < 2 )   
					{
						AutoTradeDemandeArret = 1 ;
						AutoTradeStatus = 60 ;
						SetButton52And56Text (ptrsc) ;
						// Notification sur le graphique
						Texte.Format("!! BonusMoy (%.0f%%) inferieur a 4%% et DistanceMiniAuDanger < 1 (Fut) ou 2 (Ind) !!") ;   if ( SequenceONOFF_Input.GetIndex() == 0 )  { Texte.AppendFormat(" | Tr. %d", SequenceStatus) ;  }
						DrawTextAligne_UTAMAOA_UDD (Texte, LastIndex, NiveauFutureAjuste-6*Position*Interligne, 8, RGB(0,0,0), ptrsc, 10048+10000*ThisStudyID) ;
						DrawMarker_UTAMAOA_UDD (MARKER_X, LastIndex, NiveauFutureAjuste, 16, RGB(0,0,0), ptrsc, 10049+10000*ThisStudyID) ; 
						{  SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | BonusMoy (%.0f%%)  inferieur a 4%% et DistanceMiniAuDanger < 1 (Fut) ou 2 (Ind) | DistanceMiniRecenteAuNiveau_Future = %.2f pts | DistanceMiniRecenteAuNiveau_Indice = %.2f pts | TPCibleMajore = %.2f pts", BonusMoy, DistanceMiniRecenteAuNiveau_Future, DistanceMiniRecenteAuNiveau_Indice, TPCibleMajore) ;  sc.AddMessageToLog(message,0) ;  }             
					}
					
					/* // PHPB est situé avant le SL   		// on a supprimé ce bloc car on gère cet état différemment :  on a rajouté un malus sur SommeRatio (on a divisé par DistanceAuPHPB_SurSL²)
					if ( DistanceAuPHPBSuffisant == 0  and  DistanceMiniAuDanger < 3 )     
					{
						AutoTradeDemandeArret = 1 ;
						AutoTradeStatus = 60 ;
						SetButton52And56Text (ptrsc) ;  // "AutoTrade : OFF car SL depasse PH/PB"
						// Notification sur le graphique
						Texte.Format("!! SL depasse PH/PB et DistanceMiniAuDanger < 2 (Fut) ou 3 (Ind) !!") ;   if ( SequenceONOFF_Input.GetIndex() == 0 )  { Texte.AppendFormat(" | Tr. %d", SequenceStatus) ;  }
						DrawTextAligne_UTAMAOA_UDD (Texte, LastIndex, NiveauFutureAjuste-3*Position*Interligne, 8, RGB(0,0,0), ptrsc, 10044+10000*ThisStudyID) ;
						DrawMarker_UTAMAOA_UDD (MARKER_X, LastIndex, NiveauFutureAjuste, 16, RGB(0,0,0), ptrsc, 10045+10000*ThisStudyID) ; 
						{  SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | SL depasse PH/PB et DistanceMiniAuDanger < 2 (Fut) ou 3 (Ind) | DistanceMiniRecenteAuNiveau_Future = %.2f pts | DistanceMiniRecenteAuNiveau_Indice = %.2f pts | TPCibleMajore = %.2f pts", DistanceMiniRecenteAuNiveau_Future, DistanceMiniRecenteAuNiveau_Indice, TPCibleMajore) ;  sc.AddMessageToLog(message,0) ;  }             
					} */
					
					// SommeRatio de AMR intègre directement Virginite et RatioDistanceAuPHPB => pas besoin de rajouter de condition au robot
					
					// Si on dépasse HoraireExpiration 
					if ( HoraireExpiration.GetFloat() != 0  and  sc.LatestDateTimeForLastBar >= SCDateHoraireExpiration )
					{
						AutoTradeDemandeArret = 1 ;
						AutoTradeStatus = 60 ;
						SequenceStatus = 0 ;
						SetButton52And56Text (ptrsc) ;
						// Notification sur le graphique
						int hh = floor ( HoraireExpiration.GetFloat() /100 ) ;
						int mm = floor ( HoraireExpiration.GetFloat() -100*hh ) ;
						Texte.Format("!! Horaire expiration : %dh%02d !!", hh, mm) ;
						DrawTextAligne_UTAMAOA_UDD (Texte, LastIndex, NiveauFutureAjuste-2*Position*Interligne, 8, RGB(0,0,0), ptrsc, 10025+10000*ThisStudyID) ;
						DrawMarker_UTAMAOA_UDD (MARKER_X, LastIndex, NiveauFutureAjuste, 16, RGB(0,0,0), ptrsc, 10026+10000*ThisStudyID) ; 				
						{  SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | On depasse HoraireExpiration | Demande d'arret") ;  sc.AddMessageToLog(message,0) ;  }      			
					}
					
					// Si on a récemment dépassé 15h45
					if ( sc.BaseDateTimeIn[LastIndex].GetTimeInSecondsWithoutMilliseconds() >= 56690  and  sc.BaseDateTimeIn[LastIndexPrecedent].GetTimeInSecondsWithoutMilliseconds() < 56820 )    // on vient de franchir 15h44m50s (15h44m50s = 15*3600+44*60+50 = 56690)   // ajout d'un tampon de 130s => en pratique on teste si on est entre 15h44m50s et 15h47
					{
						AutoTradeDemandeArret = 1 ;
						AutoTradeStatus = 60 ;
						SequenceStatus = 0 ;
						SetButton52And56Text (ptrsc) ;
						// Notification sur le graphique
						Texte.Format("!! 15h45 !!") ; 
						DrawTextAligne_UTAMAOA_UDD (Texte, LastIndex, NiveauFutureAjuste-1*Position*Interligne, 8, RGB(0,0,0), ptrsc, 10020+10000*ThisStudyID) ;
						DrawMarker_UTAMAOA_UDD (MARKER_X, LastIndex, NiveauFutureAjuste, 16, RGB(0,0,0), ptrsc, 10021+10000*ThisStudyID) ; 
						{  SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | On depasse 15h45 | Demande d'arret") ;  sc.AddMessageToLog(message,0) ;  }      			
					}

					// Si on est à moins de 5 min avant la news
					if ( sc.BaseDateTimeIn[LastIndexPrecedent] < SCDateHoraireNewsMoins5Min + SCDateTime::MINUTES(2)     and    sc.BaseDateTimeIn[LastIndex] >= SCDateHoraireNewsMoins5Min )    // on est entre t-5min et t-3min avant la news
					{
						AutoTradeDemandeArret = 1 ;
						AutoTradeStatus = 60 ;
						SequenceStatus = 0 ;
						SetButton52And56Text (ptrsc) ;
						// Notification sur le graphique
						Texte.Format("!! 5 min avant news !!") ; 
						DrawTextAligne_UTAMAOA_UDD (Texte, LastIndex, NiveauFutureAjuste-1*Position*Interligne, 8, RGB(0,0,0), ptrsc, 10020+10000*ThisStudyID) ;
						DrawMarker_UTAMAOA_UDD (MARKER_X, LastIndex, NiveauFutureAjuste, 16, RGB(0,0,0), ptrsc, 10021+10000*ThisStudyID) ; 
						{  SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | On est a moins de 5 min de la news | Demande d'arret") ;  sc.AddMessageToLog(message,0) ;  }      			
					}
					
					
					// Modification du TP/SL
					if ( DistanceMiniAuDanger < 5 )      // le TPOptimumArrondi a varié et on n'a pas encore de position ouverte => il faut modifier les ordres de TP et SL ; AutoTradeStatus n'est pas changé (en principe, AutoTradeStatus == 20 )
					{
						// Modification du TP
						float PrixDuTP_EnTempsReel = NiveauFutureOrdreEntree + Position * TPOptimumArrondi ;	
						if ( PrixDuTP != PrixDuTP_EnTempsReel )
						{
							s_SCNewOrder ModificationDuTP ;
							ModificationDuTP.InternalOrderID = Target1OrderID ;
							ModificationDuTP.Price1 =  PrixDuTP_EnTempsReel ;
							int ResultatSoumissionOrdreTP = sc.ModifyOrder(ModificationDuTP) ;      // sc.ModifyOrder returns 1 on a successful order modification.
							if ( ResultatSoumissionOrdreTP )  
							{
								PrixDuTP = PrixDuTP_EnTempsReel ;  // nouvelle valeur de PrixDuTP
								TP_de_la_precedente_modification_de_TP = TPOptimumArrondi ;
								// { SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | Nouveau Prix du TP = %.2f", PrixDuTP) ;  sc.AddMessageToLog(message,0) ;  }     
							}
							else {  SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | !! ERREUR !! L'ordre de Modification du TP n'a pas ete correctement soumis. Code erreur = %d", ResultatSoumissionOrdreTP) ;    sc.AddMessageToLog(message,1) ;  }     
						}
						
						// Modification du SL
						float PrixDuSL_EnTempsReel ;	
							// Calcul de PrixDuSL_EnTempsReel
							if ( FutureOuIndice == 0 )  // trading future
							{ PrixDuSL_EnTempsReel = NiveauFutureOrdreEntree - Position * SL ;	}
							else if ( FutureOuIndice == 1 )  // trading indice :  le SL est rallongé à SL'=SL*1.3+2 ; la vraie sortie a lieu lorsque l'indice dépasse SL ; c'est un filet de sécurité qui n'est pas sensé être atteint
							{ PrixDuSL_EnTempsReel = round (4* (NiveauFutureOrdreEntree - Position * (1.3*SL+2) ) ) /4 ;  }
						if ( PrixDuSL != PrixDuSL_EnTempsReel )
						{
							s_SCNewOrder ModificationDuSL ;
							ModificationDuSL.InternalOrderID = Stop1OrderID ;
							ModificationDuSL.Price1 = PrixDuSL_EnTempsReel ; 
							int ResultatSoumissionOrdreSL = sc.ModifyOrder(ModificationDuSL) ;      // sc.ModifyOrder returns 1 on a successful order modification.
							if ( ResultatSoumissionOrdreSL )  
							{
								PrixDuSL = PrixDuSL_EnTempsReel ;  // nouvelle valeur de PrixDuSL
								// { SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | Nouveau Prix du SL = %.2f", PrixDuSL) ;  sc.AddMessageToLog(message,0) ;  }     
							}
							else 
							{  SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | !! ERREUR !! L'ordre de Modification du SL n'a pas ete correctement soumis. Code erreur = %d", ResultatSoumissionOrdreSL) ;    sc.AddMessageToLog(message,1) ;  }
							
							// la valeur du SL utilisé pour le robot est affectée au SL_Prior du tracé de AMR ; lancer un TriggerRecalculate
							if ( SL_Prior != SL ) 
							{  SL_Prior = SL ;  sc.UpdateAlways = 1 ;  TriggerRecalculate = 1 ;  }
						}
					}  // fin de Modification du TP/SL
					

					// Refresh du prix de l'ordre limite d'entrée en position : bouton CTRL+A  : Bloc pour OrdreOuvert = 1 avant entrée en position
					if ( ( sc.MenuEventID == ACS_BUTTON_53  and  CetteInstanceControleLesBoutons.GetYesNo() )  or  External_Input_ACS_BUTTON_53.GetInt() != 0 )
					{
						External_Input_ACS_BUTTON_53.SetInt(0) ; // on remet à zéro	
						
						// Calcul du nouveau NiveauFutureOrdreEntree, initiation d'un TriggerRecalculate
						if ( FutureOuIndice == 1 )  // trading indice : il faut vérifier que le NiveauFutureOrdreEntree coïncide encore avec le niveau de prix de l'indice projeté sur le future, car ce niveau se balade en temps réel
						{
							NiveauFutureOrdreEntree = round ( (NiveauIndice+SpreadMoyen[LastIndex]) *4 ) /4 + AjustementChoisiEnPas*0.25 ;  // arrondi au tick
							if ( NiveauFutureOrdreEntree != NiveauFutureAjuste )   //  le niveau de l'ordre d'entrée est différent de celui analysé par la study AMR => on recalcule la study après avoir lancé le robot
							{
								NiveauFuture = round ( (NiveauIndice+SpreadMoyen[LastIndex]) *4 ) /4 ;  // arrondi au tick
								NiveauFutureTexte.SetFloat( NiveauFuture ) ;
								TriggerRecalculate = 1 ;  sc.UpdateAlways = 1 ;  
								
								// Modification du prix d'entrée
								s_SCNewOrder ModificationEntree ;
								ModificationEntree.InternalOrderID = ParentOrderID ;
								ModificationEntree.Price1 =  NiveauFutureOrdreEntree ;
								int ResultatModificationOrdreEntree = sc.ModifyOrder(ModificationEntree) ;      // sc.ModifyOrder returns 1 on a successful order modification.
								
								// Ecriture dans le log et modificaton de PrixDuTP
								if ( ResultatModificationOrdreEntree > 0 ) 
								{
									// Modificaton de PrixDuTP (la modification de NiveauFutureOrdreEntree entraine la modification automatique du prix du target (il est décalé de TP points) par Sierra, mais pas la modification du prix du SL)
									PrixDuTP = NiveauFutureOrdreEntree + Position * TP_de_la_precedente_modification_de_TP ;	
									MyEP = 999999 ;
									
									// Ecriture dans le log
									SCString message ;  message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | Refresh d'ordre (CTRL+A). ParentOrderID = %d", ParentOrderID) ;    // if ( FutureOuIndice )  { message.AppendFormat(" Ind | Ouverture d'ordre. ParentOrderID = %d", ParentOrderID) ;  }  else { message.AppendFormat(" Fut | Ouverture d'ordre. ParentOrderID = %d", ParentOrderID) ;  } 
									if ( AjustementChoisiEnPas == 0 )  {  message.AppendFormat(" | Prix Trade = %.2f (0 aj)", NiveauFutureOrdreEntree) ; }  else {  message.AppendFormat(" | Prix Trade = %.2f (%.2f aj %d t)", NiveauFutureOrdreEntree, NiveauFutureOrdreEntree-(float)AjustementChoisiEnPas/4, AjustementChoisiEnPas) ; }	
									message.AppendFormat(" | PrixDuTP = %.2f", PrixDuTP) ;
									sc.AddMessageToLog(message,0) ;   
								}
								else 
								{  SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | !! ERREUR !! L'ordre de Modification du prix d'entree n'a pas ete correctement soumis. Code erreur = %d", ResultatModificationOrdreEntree) ;    sc.AddMessageToLog(message,1) ;  }     
							}
							else { SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | Refresh d'ordre (CTRL+A) n'a pas ete execute car NiveauFutureAjuste n'aurait pas ete modifie") ;    sc.AddMessageToLog(message,0) ;  }     
						}		
					}	// fin de Modification lié à refresh CTRL+A
					
					
			
				}   // fin du bloc "gestion des événements : lorsque des ordres sont ouverts, mais pas encore d'entrée en position"   ///////////////////////////////////////////////


				// gestion des événements : lorsqu'une position est déjà ouverte     /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				if ( PositionOuverte == 1 )                        // Ca ne sert à rien de mettre " if ( PositionOuverte == 1  or  DistMiniFuture[LastIndex] < 0 ) ",  car tant que TWS et SC ne sont pas informés qu'une position est ouverte ( " PositionOuverte == 1 ") il n'y a pas moyen de soumettre une modification de TP/SL.
				{
					// Dépassement de BasculeTP0
					if ( BasculeTP0Depassee == 1  and  BasculeTP0Depassee_TriggerDuRobot == 0 )   // Il faut un Trigger spécifique pour le robot ; utiliser l'astuce BasculeTP0Depassee ==2 fait bugger le robot lorsque la study est recalculée car un recalcul conduit à BasculeTP0Depassee==1
					{
						BasculeTP0Depassee_TriggerDuRobot = 1 ;  // pour désactivation du bloc    
						
						// On déplace le TP du bracket OCO
						s_SCNewOrder ModificationDuTP ;
						ModificationDuTP.InternalOrderID = Target1OrderID ;
						ModificationDuTP.Price1 =  NiveauFutureOrdreEntree + Position * FutureOuIndice * 0.25 ;   // on bascule sur TP0 lors du trading sur future et on bascule sur TP0.25 lors du trading sur indice
						int ResultatSoumissionOrdreTP = sc.ModifyOrder(ModificationDuTP) ;      // sc.ModifyOrder returns 1 on a successful order modification.
						
						if ( ResultatSoumissionOrdreTP == 1 )   // l'ordre a bien été soumis
						{
							// Notification sur le graphique et dans le log
							SCString message ;
							if ( FutureOuIndice == 0 )  { Texte.Format("Tr %d. Le robot bascule sur TP0", SequenceStatus) ;  		message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | Le robot bascule sur TP0 | Le TP avant bascule etait de %.2f", TP_de_la_precedente_modification_de_TP) ;  }	
							else  								{ Texte.Format("Tr %d. Le robot bascule sur TP0.25", SequenceStatus) ;  	message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | Le robot bascule sur TP0.25 | Le TP avant bascule etait de %.2f", TP_de_la_precedente_modification_de_TP) ;  }
							DrawMarker_UTAMAOA_UDD (MARKER_X, LastIndex, NiveauFutureAjuste+1.8*Position*Interligne, 16, RGB(0,0,0), ptrsc, 10032+10000*ThisStudyID) ; 
							DrawTextAligne_UTAMAOA_UDD (Texte, LastIndex, NiveauFutureAjuste+3.6*Position*Interligne, 8, RGB(0,0,0), ptrsc, 10031+10000*ThisStudyID) ;
							sc.AddMessageToLog(message, 0) ;
							TP_de_la_precedente_modification_de_TP = FutureOuIndice * 0.25 ;
							PrixDuTP = NiveauFutureOrdreEntree + Position * FutureOuIndice * 0.25 ;	
						}  
						else    // problème de soumission d'ordre
						{ SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | !! ERREUR !! L'ordre de Modification du TP (Bascule vers TP0) n'a pas ete correctement soumis. Code erreur = %d", ResultatSoumissionOrdreTP) ;  sc.AddMessageToLog(message,1) ;  }
					}
					
					// Dépassement de StopLoss indice	// On exécute FlattenPosition, suivi d'un CancelOrder du Stop1OrderID et d'un CancelOrder du Target1OrderID  (bien que les odres Stop1OrderID et Target1OrderID soient liés en théorie, l'annulation du stop n'entraîne pas l'annulation du TP)   	 
					if ( StopLossIndiceDepasse == 1  and  StopLossIndiceDepasse_TriggerDuRobot == 0)   // Il faut un Trigger spécifique pour le robot ; utiliser l'astuce StopLossIndiceDepasse==2 fait bugger le robot lorsque la study est recalculée car un recalcul conduit à StopLossIndiceDepasse==1
					{
						StopLossIndiceDepasse_TriggerDuRobot = 1 ;
						SCString message ;
						
						// FlattenPosition
						int Resultat_FlattenPosition = sc.FlattenPosition() ;
						if ( Resultat_FlattenPosition == 1 )
						{
							Texte.Format("Tr %d. FlattenPosition : OK. ", SequenceStatus) ;
							message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | StopLoss indice depasse | FlattenPosition : OK. " ) ;
						}
						else
						{
							Texte.Format("Tr %d. FlattenPosition : ERREUR %d. ", SequenceStatus, Resultat_FlattenPosition) ;
							message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | StopLoss indice depasse | FlattenPosition : ERREUR %d. ", Resultat_FlattenPosition) ;
						}
						
						// CancelOrder du Stop1OrderID
						int Resultat_Cancel_StopOrder = sc.CancelOrder(Stop1OrderID);
						if ( Resultat_Cancel_StopOrder == 1 )
						{
							Texte.AppendFormat("Cancel StopOrder : OK. ") ;
							message.AppendFormat("Cancel StopOrder : OK. ") ;
						}
						else
						{
							Texte.AppendFormat("Cancel StopOrder : ERREUR %d. ", Resultat_Cancel_StopOrder) ;
							message.AppendFormat("Cancel StopOrder : ERREUR %d. ", Resultat_Cancel_StopOrder) ;
						}
						
						// CancelOrder du Target1OrderID
						int Resultat_Cancel_TargetOrder = sc.CancelOrder(Target1OrderID);	
						if ( Resultat_Cancel_TargetOrder == 1 )
						{
							Texte.AppendFormat("Cancel TargetOrder : OK.") ;
							message.AppendFormat("Cancel TargetOrder : OK.") ;
						}
						else
						{
							Texte.AppendFormat("Cancel TargetOrder : ERREUR %d.", Resultat_Cancel_TargetOrder) ;
							message.AppendFormat("Cancel TargetOrder : ERREUR %d.", Resultat_Cancel_TargetOrder) ;
						}
						
						// Notification sur le graphique et dans le log
						DrawMarker_UTAMAOA_UDD (MARKER_X, LastIndex, NiveauFutureAjuste+1.8*Position*Interligne, 16, RGB(0,0,0), ptrsc, 10028+10000*ThisStudyID) ; 
						DrawTextAligne_UTAMAOA_UDD (Texte, LastIndex, NiveauFutureAjuste+5.4*Position*Interligne, 8, RGB(0,0,0), ptrsc, 10027+10000*ThisStudyID) ;
						if ( Resultat_FlattenPosition and Resultat_Cancel_StopOrder and Resultat_Cancel_TargetOrder) sc.AddMessageToLog(message,0) ;  else sc.AddMessageToLog(message,1) ;
						
					}	// fin du bloc "Dépassement de StopLoss indice"
				}   // fin du bloc "gestion des événements : lorsqu'une position est déjà ouverte"   ///////////////////////////////////////////////

				
				// gestion des événements : appui sur le bouton KILL    ///////////////////////////////////////////////////////////////////////////////////
				if (sc.MenuEventID == ACS_BUTTON_90  or  External_Input_ACS_BUTTON_90.GetInt() != 0 )
				{
					External_Input_ACS_BUTTON_90.SetInt(0) ;
					AutoTradeFACAO = 1 ;
					AutoTradeFACAO_MessageDansLog = 0 ;   // On RAZ cet indicateur pour avoir un message dans le log en cas d'échec du FACAO (un précédent FACAO peut avoir échoué)
				}
			
			
				// gestion des événements : 15h54m40s (kill des positions ouvertes)  /////////////////////////////////////////////////////////////
				if ( sc.BaseDateTimeIn[LastIndexPrecedent].GetTimeInSecondsWithoutMilliseconds() < 57280+20  and  sc.BaseDateTimeIn[LastIndex].GetTimeInSecondsWithoutMilliseconds() >= 57280 )     // on vient de franchir 15h54m40s (15h54m40s = 15*3600+54*60+40 = 57280)   // on ajoute 20s de tampon au cas où un robot démarrerait
				{
					if ( PositionOuverte == 1  or  OrdreOuvert == 1)   // en principe il ne devrait pas y avoir d'ordre ouvert à 15h54m40s (mais par sécurité on met une redondance de fermeture)
					{
						AutoTradeFACAO = 1 ;
						// Notification sur le graphique
						Texte.Format("!! 15h54m40s !!\nKILL des positions ouvertes") ;
						DrawTextAligne_UTAMAOA_UDD (Texte, LastIndex, NiveauFutureAjuste-4.5*Position*TPOptimumArrondi, 8, RGB(0,0,0), ptrsc, 10022+10000*ThisStudyID) ;
						DrawMarker_UTAMAOA_UDD (MARKER_X, LastIndex, NiveauFutureAjuste-3.4*Position*TPOptimumArrondi, 16, RGB(0,0,0), ptrsc, 10023+10000*ThisStudyID) ; 
					}
				}


				// gestion des événements : 20s avant HoraireNews (kill des positions ouvertes)  /////////////////////////////////////////////////////////////
				if ( sc.BaseDateTimeIn[LastIndexPrecedent] < SCDateHoraireNewsMoins20Sec + SCDateTime::SECONDS(20)      and      sc.BaseDateTimeIn[LastIndex] >= SCDateHoraireNewsMoins20Sec )     // on vient de franchir HoraireNewsMoins20Sec   // on ajoute 20s de tampon au cas où un robot démarrerait
				{
					if ( PositionOuverte == 1  or  OrdreOuvert == 1)   // en principe il ne devrait pas y avoir d'ordre ouvert à 15h54m40s (mais par sécurité on met une redondance de fermeture)
					{
						AutoTradeFACAO = 1 ;
						// Notification sur le graphique
						Texte.Format("!! 20s avant news !!\nKILL des positions ouvertes") ;
						DrawTextAligne_UTAMAOA_UDD (Texte, LastIndex, NiveauFutureAjuste-4.5*Position*TPOptimumArrondi, 8, RGB(0,0,0), ptrsc, 10022+10000*ThisStudyID) ;
						DrawMarker_UTAMAOA_UDD (MARKER_X, LastIndex, NiveauFutureAjuste-3.4*Position*TPOptimumArrondi, 16, RGB(0,0,0), ptrsc, 10023+10000*ThisStudyID) ; 
					}
				}
				
				
				// gestion des événements : Last call to function ///////////////////////////////////////////////////////////////////////////////////////////
				if (sc.LastCallToFunction) 
				{
					if ( PositionOuverte == 1 )   // on met un pop up pour signaler le problème
					{	SCString message ;  message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | !!!!! WARNING CRITIQUE !!!!! Cette instance d'AMR a ete supprimee tandis qu'une position etait ouverte") ;  sc.AddMessageToLog(message,1) ;  }	
					else if ( OrdreOuvert == 1 )  // on fait une demande d'arrêt
					{	AutoTradeDemandeArret = 1 ;  }
				}
				
				// gestion des événements : refresh du prix de l'ordre limite d'entrée en position (bouton CTRL+A). Bloc pour le cas où le robot est en file d'attente  ///////////////////////////////////////////////
				if ( ( sc.MenuEventID == ACS_BUTTON_53  and  AutoTradeStatus == 11  and  CetteInstanceControleLesBoutons.GetYesNo() )  or  External_Input_ACS_BUTTON_53.GetInt() != 0 )
				{
					External_Input_ACS_BUTTON_53.SetInt(0) ; // on remet à zéro		
					
					// Calcul du nouveau NiveauFutureOrdreEntree, initiation d'un TriggerRecalculate
					if ( FutureOuIndice == 0 )  // trading future
					{ NiveauFutureOrdreEntree = NiveauFutureAjuste ;  }    // c'est le prix d'entrée qui sera demandé
					else if ( FutureOuIndice == 1 )  // trading indice : il faut vérifier que le NiveauFutureOrdreEntree coïncide encore avec le niveau de prix de l'indice projeté sur le future, car ce niveau se balade en temps réel
					{
						NiveauFutureOrdreEntree = round ( (NiveauIndice+SpreadMoyen[LastIndex]) *4 ) /4 + AjustementChoisiEnPas*0.25 ;  // arrondi au tick
						if ( NiveauFutureOrdreEntree != NiveauFutureAjuste )   //  le niveau de l'ordre d'entrée est différent de celui analysé par la study AMR => on recalcule la study après avoir lancé le robot
						{
							NiveauFuture = round ( (NiveauIndice+SpreadMoyen[LastIndex]) *4 ) /4 ;  // arrondi au tick
							NiveauFutureTexte.SetFloat( NiveauFuture ) ;
							TriggerRecalculate = 1 ;  sc.UpdateAlways = 1 ;  
						}
					}		
				}
			} // fin du bloc 3) gestion des événements
			
			
			// 4) // gestion de AutoTradeFACAO et AutoTradeDemandeArret      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			{
				// AutoTradeFACAO (sc.FlattenAndCancelAllOrders) 
				if ( AutoTradeFACAO == 1 )
				{ 
					AutoTradeFACAO = 0 ;   // désactivation du bloc
					AutoTradeDemandeArret = 0 ;  // car AutoTradeFACAO surpasse les effets de AutoTradeDemandeArret
					SequenceStatus = 0 ;  // sinon la séquence reprend après un FlattenAndCancelAllOrders
					int ResultatFACAO = sc.FlattenAndCancelAllOrders() ;       // returns 1 on success
					if ( ResultatFACAO == 1 )
					{
						AutoTradeStatus = 1000 ;
						SetButton52And56Text (ptrsc) ;
					}
					else
					{  
						AutoTradeStatus = 1010 ;
						SetButton52And56Text (ptrsc) ;   // "pb avec FlattenAndCancelAllOrders"
						
						if ( AutoTradeFACAO_MessageDansLog == 0 )
						{ SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | !! ERREUR !! La demande de FlattenAndCancelAllOrders a echoue") ;    sc.AddMessageToLog(message,1) ;  }
						
						AutoTradeFACAO_MessageDansLog = 1 ;  // pour éviter de saturer le log
					}
				}
			
				// AutoTradeDemandeArret
				if ( AutoTradeDemandeArret == 1 )          
				{
					if ( PositionOuverte == 0 )
					{	
						if ( OrdreOuvert == 1  and  DistMiniFuture[LastIndex] >= 0 )  // un ordre est ouvert et on n'a pas dépassé le prix demandé  =>  on fait une demande d'annulation de l'ordre parent     // le rajout de la condition sur DistMiniFuture permet de diminuer les annulations envoyées au moment où une ouverture de position a lieu
						{ 
							int ResultatAnnulation = sc.CancelOrder(ParentOrderID) ;       // sc.CancelOrder returns 1 on success
							if ( ResultatAnnulation == 1 )
							{
								AutoTradeDemandeArret = 0 ;
								AutoTradeStatus = 60 ;
								SetButton52And56Text (ptrsc) ;  // "AutoTrade : annulation d'ordres demandee"
							}
							else
							{  
								AutoTradeDemandeArret = 0 ;  // pour éviter de saturer le log
								AutoTradeStatus = 70 ;
								SequenceStatus = 0 ;
								SetButton52And56Text (ptrsc) ;  // "AutoTrade :  pb annulation ordres"
								SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | !! ERREUR !! La demande d'annulation de l'ordre parent (sc.CancelOrder) a echoue (AutoTradeDemandeArret==1). Lancer un KILL ?") ;    sc.AddMessageToLog(message,1) ; 
							}
						}
					}
					else if ( AutoTradeStatus == 11 )  // Le cas de figure typique est la demande d'arrêt envoyé par une study externe (l'appui sur le bouton AT est géré séparément).  // PositionOuverte==-1 lorsque AutoTradeStatus==11
					{
						AutoTradeDemandeArret = 0 ;
						AutoTradeStatus = 50 ; 
						SetButton52And56Text (ptrsc) ;
						EffacementDesUDD(ptrsc) ; 
						Texte.Format("") ; DrawText_RelativePosition_UTAMAOA (Texte, 0, 0, 12, 0, ptrsc, 10041+10000*ThisStudyID) ;  // suppression du DrawText_RelativePosition_UTAMAOA "AutoTrade : demarre"
					}
					else if ( PositionOuverte == 1 )  //  par exemple lorsqu'une demande d'arrêt a lieu sur SC et qu'on a reçu préalablement une info de " PositionOuverte == 1 "  (les infos de PositionOuverte sont reçues plus rapidement que les ticks)
					{
						AutoTradeDemandeArret = 0 ;   // on annule la demande d'arrêt et on laisse se poursuivre le trade qui vient de s'ouvrir  // AutoTradeStatus vaut 30 en principe
					}
					else  // autre cas : cas de figure non defini. 
					{ 
						SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | !! ERREUR !! Une demande d'annulation ( AutoTradeDemandeArret == 1 ) a ete declenchee. Cas de figure non defini. On met AutoTradeDemandeArret = 0") ;    sc.AddMessageToLog(message,1) ; 
						AutoTradeDemandeArret = 0 ;  // pour réinitialiser proprement
						AutoTradeStatus = 50 ; 
						SetButton52And56Text (ptrsc) ;
						EffacementDesUDD(ptrsc) ; 
						Texte.Format("") ; DrawText_RelativePosition_UTAMAOA (Texte, 0, 0, 12, 0, ptrsc, 10041+10000*ThisStudyID) ;  // suppression du DrawText_RelativePosition_UTAMAOA "AutoTrade : demarre"
					}   
				}			
			}
			
			
			// 5) // démarrage du robot   ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			{
				
				// Initialisations et tests de routine suite au démarrage du robot
				if ( AutoTradeStatus == 10 )   
				{
					CompteTicksONOFF = 1 ;  CompteTicksONOFFInput.SetCustomInputIndex(0) ;  sc.SetCustomStudyControlBarButtonText(2, " CT ON") ;    // Le compte-ticks doit être ON pour gérer BasculeTP0
					AutoTradeEstArreteManuellement = 0 ;
					
					if ( sc.IsAutoTradingEnabled and sc.IsAutoTradingOptionEnabledForChart )
					{
						AutoTradeStatus = 11 ;   // pour désactiver ce bloc
					}
					else
					{
						AutoTradeStatus = 12 ;   // pour désactiver ce bloc
						SetButton52And56Text (ptrsc) ;  // "AutoTrade : pb Auto Trading Enabled est OFF"
						SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | !! ERREUR !! Auto Trading Enabled (Global ou Chart) est OFF") ;    sc.AddMessageToLog(message,1) ; 
					}
				}
				
				
				// Y a-t-il déjà un robot en position ?
				bool UnRobotEstDejaEnPosition = 0 ;
				if ( Position == 1  and  GetNombreRobotsAcheteurs(ptrsc) != 0 )   UnRobotEstDejaEnPosition = 1 ;
				if ( Position == -1  and  GetNombreRobotsVendeurs(ptrsc) != 0 )   UnRobotEstDejaEnPosition = 1 ;
				
				
				// Si 'affichage est OFF (parce qu'on a choisi de le mettre OFF après avoir démarré le robot), on met l'affichage ON, dès que le précédent robot se termine (càd : UnRobotEstDejaEnPosition == 0 ) et on rappelle immédiatement le DLL par un "return"
				if ( AutoTradeStatus == 11  and  UnRobotEstDejaEnPosition == 0  and  AffichageONOFF == 0 )
				{
					AffichageONOFF_Input.SetCustomInputIndex(0);     // on inverse le booléen => la variable AffichageONOFF sera màj lors du prochain appel du DLL (TriggerRecalculate = 1)
					AffichageONOFF = 1 ; 
					sc.HideStudy=0;
					Void = 0 ;
					TriggerRecalculate = 1 ;  sc.UpdateAlways = 1 ;  return ;
				}
				
				
				
				// Tests pour voir si les conditions de démarrage du robot sont remplies
				if ( AutoTradeStatus == 11  and  AffichageONOFF == 1 )  
				{
					
					// RatioAUC est totalement saturé
					if ( RatioAUC[LastIndex] > 6  and  DistanceMiniAuDanger < 5 ) 
					{
						AutoTradeStatus = 100 ;
						SetButton52And56Text (ptrsc) ;  // "AutoTrade : OFF car SommeRatioAvecMalus sature"
						// Notification sur le graphique
						Texte.Format("!! RatioAUC depasse 600%% et DistanceMiniAuDanger < 4 (Fut) ou 5 (Ind) !!") ;   if ( SequenceONOFF_Input.GetIndex() == 0 )  { Texte.AppendFormat(" | Tr. %d", SequenceStatus) ;  }
						DrawTextAligne_UTAMAOA_UDD (Texte, LastIndex, NiveauFutureAjuste-4*Position*Interligne, 8, RGB(0,0,0), ptrsc, 10010+10000*ThisStudyID) ;
						DrawMarker_UTAMAOA_UDD (MARKER_X, LastIndex, NiveauFutureAjuste, 16, RGB(0,0,0), ptrsc, 10011+10000*ThisStudyID) ; 
						{  SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | RatioAUC depasse 600%% et DistanceMiniAuDanger < 4 (Fut) ou 5 (Ind) | DistanceMiniRecenteAuNiveau_Future = %.2f pts | DistanceMiniRecenteAuNiveau_Indice = %.2f pts | TPCibleMajore = %.2f pts", DistanceMiniRecenteAuNiveau_Future, DistanceMiniRecenteAuNiveau_Indice, TPCibleMajore) ;  sc.AddMessageToLog(message,0) ;  }          
					}
					// RatioAUC est très saturé
					else if ( RatioAUC[LastIndex] > 3  and  DistanceMiniAuDanger < 4 ) 
					{
						AutoTradeStatus = 100 ;
						SetButton52And56Text (ptrsc) ;  // "AutoTrade : OFF car SommeRatioAvecMalus sature"
						// Notification sur le graphique
						Texte.Format("!! RatioAUC depasse 300%% et DistanceMiniAuDanger < 3 (Fut) ou 4 (Ind) !!") ;   if ( SequenceONOFF_Input.GetIndex() == 0 )  { Texte.AppendFormat(" | Tr. %d", SequenceStatus) ;  }
						DrawTextAligne_UTAMAOA_UDD (Texte, LastIndex, NiveauFutureAjuste-4*Position*Interligne, 8, RGB(0,0,0), ptrsc, 10010+10000*ThisStudyID) ;
						DrawMarker_UTAMAOA_UDD (MARKER_X, LastIndex, NiveauFutureAjuste, 16, RGB(0,0,0), ptrsc, 10011+10000*ThisStudyID) ; 
						{  SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | RatioAUC depasse 300%% et DistanceMiniAuDanger < 3 (Fut) ou 4 (Ind) | DistanceMiniRecenteAuNiveau_Future = %.2f pts | DistanceMiniRecenteAuNiveau_Indice = %.2f pts | TPCibleMajore = %.2f pts", DistanceMiniRecenteAuNiveau_Future, DistanceMiniRecenteAuNiveau_Indice, TPCibleMajore) ;  sc.AddMessageToLog(message,0) ;  }             
					}
					// SommeRatio est bien saturé    // On n'utilise pas SommeRatioAvecMalus ici car il fait de grosses amplitudes.
					else if ( ( ( RatioAUC[LastIndex] + RatioSigma[LastIndex] ) / 1.8 > 1.5 ) and  DistanceMiniAuDanger < 3 )    		//  SommeRatio[LastIndex] = ( RatioAUC[LastIndex] + RatioSigma[LastIndex] ) / 1.8
					{
						AutoTradeStatus = 100 ;
						SetButton52And56Text (ptrsc) ;  // "AutoTrade : OFF car SommeRatioAvecMalus sature"
						// Notification sur le graphique
						Texte.Format("!! SommeRatio depasse 150%% et DistanceMiniAuDanger < 2 (Fut) ou 3 (Ind) !!") ;   if ( SequenceONOFF_Input.GetIndex() == 0 )  { Texte.AppendFormat(" | Tr. %d", SequenceStatus) ;  }
						DrawTextAligne_UTAMAOA_UDD (Texte, LastIndex, NiveauFutureAjuste-4*Position*Interligne, 8, RGB(0,0,0), ptrsc, 10010+10000*ThisStudyID) ;
						DrawMarker_UTAMAOA_UDD (MARKER_X, LastIndex, NiveauFutureAjuste, 16, RGB(0,0,0), ptrsc, 10011+10000*ThisStudyID) ; 
						{  SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | SommeRatio depasse 150%% et DistanceMiniAuDanger < 2 (Fut) ou 3 (Ind) | DistanceMiniRecenteAuNiveau_Future = %.2f pts | DistanceMiniRecenteAuNiveau_Indice = %.2f pts | TPCibleMajore = %.2f pts", DistanceMiniRecenteAuNiveau_Future, DistanceMiniRecenteAuNiveau_Indice, TPCibleMajore) ;  sc.AddMessageToLog(message,0) ;  }             
					}
					// SommeRatioAvecMalus est légèrement saturé
					else if ( SommeRatioAvecMalus[LastIndex] > 1  and  DistanceMiniAuDanger < 2 ) 
					{
						AutoTradeStatus = 100 ;
						SetButton52And56Text (ptrsc) ;  // "AutoTrade : OFF car SommeRatioAvecMalus sature"
						// Notification sur le graphique
						Texte.Format("!! SommeRatioAvecMalus depasse 100%% et DistanceMiniAuDanger < 1 (Fut) ou 2 (Ind) !!") ;   if ( SequenceONOFF_Input.GetIndex() == 0 )  { Texte.AppendFormat(" | Tr. %d", SequenceStatus) ;  }
						DrawTextAligne_UTAMAOA_UDD (Texte, LastIndex, NiveauFutureAjuste-4*Position*Interligne, 8, RGB(0,0,0), ptrsc, 10010+10000*ThisStudyID) ;
						DrawMarker_UTAMAOA_UDD (MARKER_X, LastIndex, NiveauFutureAjuste, 16, RGB(0,0,0), ptrsc, 10011+10000*ThisStudyID) ; 
						{  SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | SommeRatioAvecMalus depasse 100%% et DistanceMiniAuDanger < 1 (Fut) ou 2 (Ind) | DistanceMiniRecenteAuNiveau_Future = %.2f pts | DistanceMiniRecenteAuNiveau_Indice = %.2f pts | TPCibleMajore = %.2f pts", DistanceMiniRecenteAuNiveau_Future, DistanceMiniRecenteAuNiveau_Indice, TPCibleMajore) ;  sc.AddMessageToLog(message,0) ;  }             
					}
					
					// TP est insuffisant
					if ( TPOptimumArrondi  < 0.75  and  DistanceMiniAuDanger < 3 )   
					{
						AutoTradeStatus = 120 ;
						SetButton52And56Text (ptrsc) ;  // "AutoTrade : OFF car TP insuffisant"
						// Notification sur le graphique
						Texte.Format("!! TP Optimum Arrondi < 0.75 et DistanceMiniAuDanger < 2 (Fut) ou 3 (Ind) !!") ;   if ( SequenceONOFF_Input.GetIndex() == 0 )  { Texte.AppendFormat(" | Tr. %d", SequenceStatus) ;  }
						DrawTextAligne_UTAMAOA_UDD (Texte, LastIndex, NiveauFutureAjuste-5*Position*Interligne, 8, RGB(0,0,0), ptrsc, 10012+10000*ThisStudyID) ;
						DrawMarker_UTAMAOA_UDD (MARKER_X, LastIndex, NiveauFutureAjuste, 16, RGB(0,0,0), ptrsc, 10013+10000*ThisStudyID) ; 
						{  SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | TP Optimum Arrondi < 0.75 et DistanceMiniAuDanger < 2 (Fut) ou 3 (Ind) | DistanceMiniRecenteAuNiveau_Future = %.2f pts | DistanceMiniRecenteAuNiveau_Indice = %.2f pts | TPCibleMajore = %.2f pts", DistanceMiniRecenteAuNiveau_Future, DistanceMiniRecenteAuNiveau_Indice, TPCibleMajore) ;  sc.AddMessageToLog(message,0) ;  }             
					}
					
					// BonusMoy est insuffisant
					if ( BonusMoy  < 4  and  DistanceMiniAuDanger < 2 )   
					{
						AutoTradeStatus = 170 ;
						SetButton52And56Text (ptrsc) ;
						// Notification sur le graphique
						Texte.Format("!! BonusMoy (%.0f%%) inferieur a 4%% et DistanceMiniAuDanger < 1 (Fut) ou 2 (Ind) !!") ;   if ( SequenceONOFF_Input.GetIndex() == 0 )  { Texte.AppendFormat(" | Tr. %d", SequenceStatus) ;  }
						DrawTextAligne_UTAMAOA_UDD (Texte, LastIndex, NiveauFutureAjuste-6*Position*Interligne, 8, RGB(0,0,0), ptrsc, 10048+10000*ThisStudyID) ;
						DrawMarker_UTAMAOA_UDD (MARKER_X, LastIndex, NiveauFutureAjuste, 16, RGB(0,0,0), ptrsc, 10049+10000*ThisStudyID) ; 
						{  SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | BonusMoy (%.0f%%)  inferieur a 4%% et DistanceMiniAuDanger < 1 (Fut) ou 2 (Ind) | DistanceMiniRecenteAuNiveau_Future = %.2f pts | DistanceMiniRecenteAuNiveau_Indice = %.2f pts | TPCibleMajore = %.2f pts", BonusMoy, DistanceMiniRecenteAuNiveau_Future, DistanceMiniRecenteAuNiveau_Indice, TPCibleMajore) ;  sc.AddMessageToLog(message,0) ;  }             
					}
					
					/* // PHPB est situé avant le SL			// on a supprimé ce bloc car on gère cet état différemment :  on a rajouté un malus sur SommeRatio (on a divisé par DistanceAuPHPB_SurSL²)
					if ( DistanceAuPHPBSuffisant == 0  and  DistanceMiniAuDanger < 3 )     
					{
						AutoTradeStatus = 160 ;
						SetButton52And56Text (ptrsc) ;  // "AutoTrade : OFF car SL depasse PH/PB"
						// Notification sur le graphique
						Texte.Format("!! SL depasse PH/PB et DistanceMiniAuDanger < 2 (Fut) ou 3 (Ind) !!") ;   if ( SequenceONOFF_Input.GetIndex() == 0 )  { Texte.AppendFormat(" | Tr. %d", SequenceStatus) ;  }
						DrawTextAligne_UTAMAOA_UDD (Texte, LastIndex, NiveauFutureAjuste-3*Position*Interligne, 8, RGB(0,0,0), ptrsc, 10044+10000*ThisStudyID) ;
						DrawMarker_UTAMAOA_UDD (MARKER_X, LastIndex, NiveauFutureAjuste, 16, RGB(0,0,0), ptrsc, 10045+10000*ThisStudyID) ; 
						{  SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | SL depasse PH/PB et DistanceMiniAuDanger < 2 (Fut) ou 3 (Ind) | DistanceMiniRecenteAuNiveau_Future = %.2f pts | DistanceMiniRecenteAuNiveau_Indice = %.2f pts | TPCibleMajore = %.2f pts", DistanceMiniRecenteAuNiveau_Future, DistanceMiniRecenteAuNiveau_Indice, TPCibleMajore) ;  sc.AddMessageToLog(message,0) ;  }             
					} */
					
					// Si on dépasse HoraireExpiration 
					if ( HoraireExpiration.GetFloat() != 0  and  sc.LatestDateTimeForLastBar >= SCDateHoraireExpiration )
					{
						AutoTradeStatus = 140 ;
						SequenceStatus = 0 ;
						SetButton52And56Text (ptrsc) ;  // "AutoTrade : OFF car HoraireExpiration est depasse"
						// Notification sur le graphique
						int hh = floor ( HoraireExpiration.GetFloat() /100 ) ;
						int mm = floor ( HoraireExpiration.GetFloat() -100*hh ) ;
						Texte.Format("!! Horaire expiration : %dh%02d !!", hh, mm) ; 
						DrawTextAligne_UTAMAOA_UDD (Texte, LastIndex, NiveauFutureAjuste-2*Position*Interligne, 8, RGB(0,0,0), ptrsc, 10025+10000*ThisStudyID) ;
						DrawMarker_UTAMAOA_UDD (MARKER_X, LastIndex, NiveauFutureAjuste, 16, RGB(0,0,0), ptrsc, 10026+10000*ThisStudyID) ; 		
						{  SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | On depasse HoraireExpiration") ;  sc.AddMessageToLog(message,0) ;  }      				
					}
					
					// Si on a récemment dépassé 15h45
					if ( sc.BaseDateTimeIn[LastIndex].GetTimeInSecondsWithoutMilliseconds() >= 56690  and  sc.BaseDateTimeIn[LastIndexPrecedent].GetTimeInSecondsWithoutMilliseconds() < 56820 )  
					{
						AutoTradeStatus = 150 ;
						SequenceStatus = 0 ;
						SetButton52And56Text (ptrsc) ;  // "AutoTrade : OFF car il est 15h45"
						// Notification sur le graphique
						Texte.Format("!! 15h45 !!") ;
						DrawTextAligne_UTAMAOA_UDD (Texte, LastIndex, NiveauFutureAjuste-1*Position*Interligne, 8, RGB(0,0,0), ptrsc, 10020+10000*ThisStudyID) ;
						DrawMarker_UTAMAOA_UDD (MARKER_X, LastIndex, NiveauFutureAjuste, 16, RGB(0,0,0), ptrsc, 10021+10000*ThisStudyID) ; 
						{  SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | On depasse 15h45") ;  sc.AddMessageToLog(message,0) ;  }      			
					}

					// Si on est à moins de 5 min avant la news
					if ( sc.BaseDateTimeIn[LastIndexPrecedent] < SCDateHoraireNewsMoins5Min + SCDateTime::MINUTES(2)     and    sc.BaseDateTimeIn[LastIndex] >= SCDateHoraireNewsMoins5Min )    // on on est à moins de 5 min avant la news    // ajout d'un tampon de 120s 
					{
						AutoTradeStatus = 200 ; 
						SequenceStatus = 0 ;
						SetButton52And56Text (ptrsc) ;  // "AutoTrade : OFF car News dans 5min"
						// Notification sur le graphique
						Texte.Format("!! 5 min avant la news !!") ;
						DrawTextAligne_UTAMAOA_UDD (Texte, LastIndex, NiveauFutureAjuste-1*Position*Interligne, 8, RGB(0,0,0), ptrsc, 10020+10000*ThisStudyID) ;
						DrawMarker_UTAMAOA_UDD (MARKER_X, LastIndex, NiveauFutureAjuste, 16, RGB(0,0,0), ptrsc, 10021+10000*ThisStudyID) ; 
						{  SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | On est a moins de 5 min de la news") ;  sc.AddMessageToLog(message,0) ;  }      			
					}
					
					// DistMiniFuture doit être >0 pour pouvoir ouvrir une position
					if ( DistMiniFuture[LastIndex] <= 0 )
					{
						AutoTradeStatus = 190 ;
						SetButton52And56Text (ptrsc) ;  // "AutoTrade : OFF car DistMiniFuture <= 0"
						// Notification sur le graphique
						Texte.Format("!! DistMiniFuture <= 0 !!") ;   if ( SequenceONOFF_Input.GetIndex() == 0 )  { Texte.AppendFormat(" | Tr. %d", SequenceStatus) ;  }
						DrawTextAligne_UTAMAOA_UDD (Texte, LastIndex, NiveauFutureAjuste-1*Position*Interligne, 8, RGB(0,0,0), ptrsc, 10057+10000*ThisStudyID) ;
						DrawMarker_UTAMAOA_UDD (MARKER_X, LastIndex, NiveauFutureAjuste, 16, RGB(0,0,0), ptrsc, 10058+10000*ThisStudyID) ; 
						{  SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | DistMiniFuture <= 0") ;  sc.AddMessageToLog(message,0) ;  }             
					}

					
					
					// Si aucun des tests ci-dessus n'a invalidé "AutoTradeStatus == 11" et qu'il n'y a pas de robot concurrent, on peut tester si les conditions sont réunies pour soumettre l'ordre triple   // On surpasse la condition "UnRobotEstDejaEnPosition" lorsqu'une séquence est déjà démarrée
					if ( AutoTradeStatus == 11  and  ( UnRobotEstDejaEnPosition == 0  or  SequenceStatus >= 2 ) )
					{
						
						// Calcul de NiveauFutureOrdreEntree
						if ( FutureOuIndice == 0 )  // trading future
						{ NiveauFutureOrdreEntree = NiveauFutureAjuste ;  }    // c'est le prix d'entrée qui sera demandé
						else if ( FutureOuIndice == 1 )  // trading indice : il faut vérifier que le NiveauFutureOrdreEntree coïncide encore avec le niveau de prix de l'indice projeté sur le future, car ce niveau se balade en temps réel
						{
							NiveauFutureOrdreEntree = round ( (NiveauIndice+SpreadMoyen[LastIndex]) *4 ) /4 + AjustementChoisiEnPas*0.25 ;  // arrondi au tick
							if ( NiveauFutureOrdreEntree != NiveauFutureAjuste )   //  le niveau de l'ordre d'entrée est différent de celui analysé par la study AMR => on recalcule la study après avoir lancé le robot
							{
								NiveauFuture = round ( (NiveauIndice+SpreadMoyen[LastIndex]) *4 ) /4 ;  // arrondi au tick
								NiveauFutureTexte.SetFloat( NiveauFuture ) ;
								TriggerRecalculate = 1 ;
								sc.UpdateAlways = 1 ;  
							}
						}
						
						// Create an s_SCNewOrder object. 
						s_SCNewOrder PremierOrdre ;
						PremierOrdre.TimeInForce = SCT_TIF_DAY ;
						PremierOrdre.OrderQuantity = 1 ;
						PremierOrdre.OCOGroup1Quantity = 1 ;    // If this is left at the default of 0, then it will be automatically set.
						PremierOrdre.OrderType = SCT_ORDERTYPE_LIMIT ;
						PremierOrdre.Price1 = NiveauFutureOrdreEntree ;
						
						// Calcul du Target1Price (TP : ordre limit)    		 // By default when you specify an Attached Order Target Price or Offset, the order type is Limit ( s_SCNewOrder::AttachedOrderTarget1Type = SCT_ORDERTYPE_LIMIT )
						TP_de_la_precedente_modification_de_TP = std::max (0.75f, TPOptimumArrondi) ;  // valeur initiale de TP_de_la_precedente_modification_de_TP
						PrixDuTP = NiveauFutureOrdreEntree + Position * TP_de_la_precedente_modification_de_TP ;		// valeur initiale de PrixDuTP (servira de point de comparaison pour savoir s'il faut modifier le TP)
						PremierOrdre.Target1Price =  PrixDuTP ;     // Note : Although you specify an actual Target price with these variables, this price is converted to an offset based upon the parent order price or based upon the expected fill price of a parent Market order. Additionally, when there is a fill of the parent order, the Target orders will be adjusted to maintain the original specified offset relative to the fill price of the parent. Therefore, the actual price may change slightly.
						
						
						// Calcul du Stop1Price	(SL : ordre stop-limit)				// By default when you specify an Attached Order Stop Price or Stop Offset, the order type is Stop ( s_SCNewOrder::AttachedOrderStop1Type = SCT_ORDERTYPE_STOP ), or je veux un SCT_ORDERTYPE_STOP_LIMIT
						PremierOrdre.AttachedOrderStop1Type = SCT_ORDERTYPE_STOP_LIMIT ;
						if ( FutureOuIndice == 0 )  // trading future
						{ PrixDuSL = NiveauFutureOrdreEntree - Position * SL ;  PremierOrdre.Stop1Price = PrixDuSL ;  }
						else if ( FutureOuIndice == 1 )  // trading indice :  le SL sur future est rallongé à SL'=SL*1.3+2 ; la vraie sortie a lieu lorsque l'indice dépasse SL ; SL' est un filet de sécurité qui n'est pas sensé être atteint
						{ PrixDuSL = round (4* (NiveauFutureOrdreEntree - Position * (1.3*SL+2) ) ) /4 ;  PremierOrdre.Stop1Price = PrixDuSL ;  }		// Note : Although you specify an actual Stop price with these variables, this price is converted to an offset based upon the parent order price or based upon the expected fill price of a parent Market order. Additionally, when there is a fill of the parent order, the Stop orders will be adjusted to maintain the original specified offset relative to the fill price of the parent. Therefore, the actual price may change slightly.
						// PremierOrdre.StopLimitOrderLimitOffset = Offset_StopLimit ;             //  utilisation de StopLimitOrderLimitOffset au lieu de Stop1Offset => ne fonctionne pas, car ça semble s'appliquer à l'ordre parent (offset entre stop et limit lorsque l'ordre parent est un ordre stop-limit) => pour l'instant, je laisse tomber l'offset entre stop et limit lors de la soumission de l'ordre triple, car l'essentiel est de positionner un ordre stop-limit qui pourra être manipulé ultérieurement.
						
						
						// la valeur du SL utilisé pour le robot est affectée au SL_Prior du tracé de AMR ; lancer un TriggerRecalculate après avoir lancé le robot
						if ( SL_Prior != SL ) 
						{  SL_Prior = SL ;  sc.UpdateAlways = 1 ;  TriggerRecalculate = 1 ;  }
					
						// TextTag
						if (Position == 1 )  {	Texte.Format("Achat ") ;  }  else if (Position == -1 )   {	Texte.Format("Vente ") ;  }
						Texte.AppendFormat( NomDuNiveauTrade.GetString() ) ;  
						Texte.AppendFormat("| BonusIntr = %d%% | Aj %dt | ", BonusIntrinseque, AjustementChoisiEnPas) ; 
						// Tag de Sequence
						if ( SequenceONOFF_Input.GetIndex() == 0 )   // 0 = ON, 1 = OFF
						{
							if ( SequenceStatus == 1 )
							{ Texte.AppendFormat("Seq : Tr 1") ;  }
							else if ( SequenceStatus == 2 )
							{ Texte.AppendFormat("Seq : Tr 2") ;  }
							else if ( SequenceStatus == 3 )
							{ Texte.AppendFormat("Seq : Tr 3") ;  }
							else
							{ Texte.AppendFormat("Seq : erreur") ;  }
						}
						else
						{ Texte.AppendFormat("Seq : OFF") ;  }
						PremierOrdre.TextTag = Texte ;
						
						// Soumission de l'ordre
						int ResultatSoumissionOrdre ;
						if (Position == 1 ) 
						{ ResultatSoumissionOrdre = sc.BuyEntry(PremierOrdre,LastIndex) ;  }
						else if (Position == -1 ) 
						{ ResultatSoumissionOrdre = sc.SellEntry(PremierOrdre,LastIndex) ;  }

						// Résultat de la soumission de l'ordre
						if (ResultatSoumissionOrdre > 0)				// SC a bien soumis l'ordre
						{
							// Modifications des variables
							AutoTradeStatus = 20 ;
							Texte.Format("") ; DrawText_RelativePosition_UTAMAOA (Texte, 0, 0, 12, 0, ptrsc, 10041+10000*ThisStudyID) ;  // suppression du DrawText_RelativePosition_UTAMAOA "AutoTrade : demarre"
							SetButton52And56Text (ptrsc) ;  // "AutoTrade : ordres ouverts"
							BasculeTP0Depassee_TriggerDuRobot = 0 ; 
							StopLossIndiceDepasse_TriggerDuRobot == 0 ;
							RobotPosition.SetInt(Position) ;
							External_Input_ACS_BUTTON_53.SetInt(0) ;
							MyEP = 999999 ;
							// Remember the order IDs for subsequent modification and cancellation
							ParentOrderID = PremierOrdre.InternalOrderID;
							Target1OrderID = PremierOrdre.Target1InternalOrderID;
							Stop1OrderID   = PremierOrdre.Stop1InternalOrderID;
							// on écrit ParentOrderID et NiveauFutureAjuste dans le log
							SCString message ; 
							message.Format("ID:%d | ", ThisStudyID) ;
							message.AppendFormat( NomDuNiveauTrade.GetString() ) ;
							message.AppendFormat(" | Ouverture d'ordre. ParentOrderID = %d", ParentOrderID) ;    // if ( FutureOuIndice )  { message.AppendFormat(" Ind | Ouverture d'ordre. ParentOrderID = %d", ParentOrderID) ;  }  else { message.AppendFormat(" Fut | Ouverture d'ordre. ParentOrderID = %d", ParentOrderID) ;  } 
							if ( AjustementChoisiEnPas == 0 )  {  message.AppendFormat(" | Prix Trade = %.2f (0 aj)", NiveauFutureOrdreEntree) ; }  else {  message.AppendFormat(" | Prix Trade = %.2f (%.2f aj %d t)", NiveauFutureOrdreEntree, NiveauFutureOrdreEntree-(float)AjustementChoisiEnPas/4, AjustementChoisiEnPas) ; }	
							sc.AddMessageToLog(message,0) ;   
							// Il faudrait modifier l'input RobotAcheteurActif_Input / RobotVendeurActif_Input  de la study "prix typique" pour qu'il n'y ait pas un autre robot qui démarre en même temps
						}
						else  // l'ordre n'a pas été correctement soumis
						{ 
							AutoTradeStatus = 15 ;
							Texte.Format("") ; DrawText_RelativePosition_UTAMAOA (Texte, 0, 0, 12, 0, ptrsc, 10041+10000*ThisStudyID) ;  // suppression du DrawText_RelativePosition_UTAMAOA "AutoTrade : demarre"
							SetButton52And56Text (ptrsc) ;  // "AutoTrade : pb soumission premier ordre"
							SequenceStatus = 0 ; 	// pour que la séquence reprenne sur le Trade 1 lorsqu'on relance AutoTrade
						
							// Ecriture dans le log
							SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | !! ERREUR !! Le Premier Ordre n'a pas ete correctement soumis. Code erreur = %d", ResultatSoumissionOrdre) ;    sc.AddMessageToLog(message,1) ;  
							if ( ResultatSoumissionOrdre == -1 )
							{	message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | Code erreur = -1. ''Auto Trading Enabled'' ?    ''Trade Simulation Mode On'' ?  ''sc.SendOrdersToTradeService=%d'' ?", sc.SendOrdersToTradeService) ;    sc.AddMessageToLog(message,1) ;  }
							if ( ResultatSoumissionOrdre == -8997 )   
							{	message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | Code erreur = -8997. ''Ordre soumis pendant que le replay du chart était en pause") ;    sc.AddMessageToLog(message,1) ;  }	
							if ( ResultatSoumissionOrdre == -8998 )   
							{	message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | Code erreur = -8998. ''Ordre soumis grace a une tres grosse augmentation du bonus ?") ;    sc.AddMessageToLog(message,1) ;  }	
						}   
					}  // fin du "bloc de soumission d'ordre"
					
					if ( AutoTradeStatus == 11 ) // le robot est démarré mais il n'y a pas d'ordre soumis.
					{
						Texte.Format("                AutoTrade : demarre\nOrdre non soumis car autre robot en position") ; 
						DrawText_RelativePosition_UTAMAOA (Texte, Position_3eme_Colonne+10, NiveauFutureAjuste-4.0*Position*Interligne, 9, CouleurDrawText, ptrsc, 10041+10000*ThisStudyID) ;  
					}
					
					// Ajout d'un Popup/Warning si HoraireRAZ < 9h30  ET  LastIndex > 9h30
					if ( HoraireRAZ < 930  and  sc.BaseDateTimeIn[LastIndex].GetTimeInSecondsWithoutMilliseconds() > 34200 )	// HoraireRAZ au format hhmm.ss 
					{  SCString message ;   message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | !! WARNING !! HoraireRAZ < 9h30 (cette operation est autorisee pour un test a blanc mais interdite pour du live trading)") ;  sc.AddMessageToLog(message,1) ;  }         
					
				}   // fin du bloc " Tests pour voir si les conditions de démarrage du robot sont remplies"
		
			}    // fin du bloc " 5) démarrage du robot "
		
		}
		
		else   // RobotActif==0, car AutoTradeStatus vaut 0 ou 50, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200
		{	PositionOuverte = -2 ; OrdreOuvert = -2 ; }
		
		
		QueryPerformanceCounter(&ElapsedTicks) ;  
		Clicks_BlocRobotDeTrading = ( ElapsedTicks.QuadPart - ElapsedTicks_BlocRobotDeTrading.QuadPart ) *1000.0 / Frequency.QuadPart ;     // calculé en millisecondes
	}
	// fin du bloc du ROBOT DE TRADING ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	// Données issues du market depth : détermination de Quantity_NiveauFutureAjuste et MyEP
	{
		if ( AffichageONOFF == 1)
		{
			s_MarketDepthEntry DepthEntry;
			float Price ;
			int Level = 0 ; 	// sur le ask (resp. le bid) on parcourt les niveaux en partant du best ask (resp. best bid) qui vaut Level=0.
			int NbLevels ;
			
			if ( Position == -1 )  // vente, on est au ask
			{
				NbLevels = sc.GetAskMarketDepthNumberOfLevels() ;
				do  // on recherche la DepthEntry de NiveauFutureAjuste
				{
					sc.GetAskMarketDepthEntryAtLevel(DepthEntry, Level);     // on parcourt le CO du Level = 0 jusqu'à Level = NbLevels-1.
					Price = DepthEntry.Price;
					Level++ ;
				}
				while ( Price != NiveauFutureAjuste  and  Level < NbLevels ) ;
			}
			else if ( Position == 1 )  // achat, on est au bid
			{
				NbLevels = sc.GetBidMarketDepthNumberOfLevels() ;
				do  // on recherche la DepthEntry de NiveauFutureAjuste
				{
					sc.GetBidMarketDepthEntryAtLevel(DepthEntry, Level);
					Price = DepthEntry.Price;
					Level++ ;
				}
				while ( Price != NiveauFutureAjuste  and  Level < NbLevels ) ;
			}
			Level_NiveauFutureAjuste = Level - 1 ;		
			
			//	si la qté de lot est de 0 sur le prix NiveauFutureAjuste, alors Level_NiveauFutureAjuste=NbLevels-1  => on le met arbitrairement à 99999 pour signifier que le nb de lots est de 0
			if ( Level_NiveauFutureAjuste == NbLevels - 1 )  Level_NiveauFutureAjuste = 99999 ;
			
			// affichage à supprimer :
			// { SCString message ;  message.Format("Level_NiveauFutureAjuste = %d | NbLevels_Ask = %d | NbLevels_Bid = %d", Level_NiveauFutureAjuste, sc.GetAskMarketDepthNumberOfLevels(), sc.GetBidMarketDepthNumberOfLevels()) ;  sc.AddMessageToLog(message,0) ;    } 

			// Détermination de MyEP,  pour un ordre dans le CO
			if ( OrdreOuvert == 1  and  PositionOuverte == 0 )   // Un ordre (parent, target1 ou stop1) est ouvert  ET  l'ordre parent n'a pas été filled  
			{
				Quantity_NiveauFutureAjuste = DepthEntry.Quantity;
				//int NumOrders = DepthEntry.NumOrders;
				
				// Calcul de MyEP
				if ( MyEP == 999999 ) { MyEP = 999998 ;  }  // au premier appel du bloc, on n'attribue pas " MyEP = Quantity_NiveauFutureAjuste " car l'information " OrdreOuvert == 1 " arrive plus vite (via flux de routage) que le flux de data
				else if ( Quantity_NiveauFutureAjuste < MyEP )  { MyEP = Quantity_NiveauFutureAjuste ;  SCString message ;  message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | MyEP = %d", MyEP) ; sc.AddMessageToLog(message,0) ;    } 
			}
		}
	}
	
	
	// AFFICHAGE TEMPS REEL DES PARAMETRES IMPORTANTS //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	{
		LARGE_INTEGER ElapsedTicks_BlocAffichage ;  QueryPerformanceCounter(&ElapsedTicks_BlocAffichage) ;  


		// Booléen : la fenêtre de AMR est-elle visible ?
		bool FenetreDeAMR_EstVisible = 0 ;
		if ( sc.ChartWindowIsActive  or  NbMonitors == 2 )  FenetreDeAMR_EstVisible = 1 ;

		
		// Booléen : le NiveauFutureAjuste est-il compris dans le range d'affichage ? Ou la study est-elle la study active ? 
		float TopCoordinate = sc.YPixelCoordinateToGraphValue(sc.StudyRegionTopCoordinate) ;
		float BottomCoordinate = sc.YPixelCoordinateToGraphValue(sc.StudyRegionBottomCoordinate) ;
		bool NiveauFutureAjuste_EstVisible = 0 ;  sc.HideStudy=1 ;  
		if (  /* AffichageONOFF  and  */ ( ( NiveauFutureAjuste > BottomCoordinate  and  NiveauFutureAjuste < TopCoordinate )  or  CetteInstanceControleLesBoutons.GetYesNo() )  ) 
		{ NiveauFutureAjuste_EstVisible = 1 ;  sc.HideStudy=0 ;  }  
		// Si une position est ouverte, la study DOIT être affichée  //  AutoTradeStatus == 11 permet de mettre l'affichage OFF ))  
		if (  RobotActif and  AutoTradeStatus != 11 )  
		{ NiveauFutureAjuste_EstVisible = 1 ;  sc.HideStudy=0 ;  }  
	
	
		// les useTool sans LineNumber doivent être effacés à chaque appel du DLL pour éviter une fuite de mémoire : ce sont les useTool Rectangle2 pour la construction des zones grisées de Niveau1TP-Niveau2TP-Niveau3TP-Niveau4TP, et les useTool SegmentPointille
		if ( FenetreDeAMR_EstVisible )
		sc.DeleteACSChartDrawing(sc.ChartNumber, TOOL_DELETE_ALL, 0) ; 		// sc.DeleteACSChartDrawing : if Tool is set to TOOL_DELETE_ALL, then all non user drawn Chart Drawings on the specified ChartNumber will be deleted. The LineNumber parameter in this case is ignored and should be 0.

		
		// si la study est la study active, le range s'adapte à elle pour qu'elle soit visible (les autres studies ont SCALE_SAMEASREGION)
		if ( CetteInstanceControleLesBoutons.GetYesNo() ) sc.ScaleRangeType = SCALE_AUTO ;
		else sc.ScaleRangeType = SCALE_SAMEASREGION ;   
		
		// Le triangle vert/rouge sur NiveauFutureAjuste et le texte générée par DrawText_ID_NomDuNiveauTrade_Ajustement sont toujours affichés (si Void = 0, alors NiveauFutureAjuste=0 et c'est tracé sur la ligne 0)
		if ( FenetreDeAMR_EstVisible  and  NiveauFutureAjuste_EstVisible )
		{	
			// Calcul des positions relatives pour le texte et le marker
			float PositionRelative_Texte, PositionRelative_Marker ;
				    if ( Config_tailleTexte == 1 )  { if ( AffichageONOFF ) { PositionRelative_Texte = Position_1ere_Colonne; PositionRelative_Marker = Position_1ere_Colonne-11 ;  }  else { PositionRelative_Texte = Position_1ere_Colonne-19 ;  PositionRelative_Marker = Position_1ere_Colonne-30 ;  }  }
			else if ( Config_tailleTexte == 2 )  { if ( AffichageONOFF ) { PositionRelative_Texte = Position_1ere_Colonne; PositionRelative_Marker = 5 ;  }   									else { PositionRelative_Texte = 16 ;  PositionRelative_Marker = 5 ;  }  }

			// affichage du texte permanent sur le graphe en 16-ticks
			DrawText_ID_NomDuNiveauTrade_Ajustement (PositionRelative_Texte, NiveauFutureAjuste-AffichageONOFF*2.7*Position*Interligne, ptrsc, 10016+10000*ThisStudyID) ;    
			
			// affichage du triangle sur le graphe en 16-ticks
			if ( Position == 1 )  DrawMarker_RelativePosition_UTAMAOA (MARKER_TRIANGLEUP, PositionRelative_Marker, NiveauFutureAjuste, 17, CouleurPosition, ptrsc, 10030+10000*ThisStudyID) ;
			else if ( Position == -1 )  DrawMarker_RelativePosition_UTAMAOA (MARKER_TRIANGLEDOWN, PositionRelative_Marker, NiveauFutureAjuste, 17, CouleurPosition, ptrsc, 10030+10000*ThisStudyID) ;
			else 
			{ 
				DrawMarker_RelativePosition_UTAMAOA (MARKER_TRIANGLERIGHT, IndexHoraireRAZ, NiveauFutureAjuste, 17, CouleurPosition, ptrsc, 10030+10000*ThisStudyID) ;
				Texte.Format("Impossible de determiner si la Position est acheteuse ou vendeuse ") ;														DrawText_RelativePosition_UTAMAOA (Texte, Position_3eme_Colonne, NiveauFutureAjuste, 14, CouleurDrawText, ptrsc,10051+10000*ThisStudyID) ;  
			}
		}
		
		// Tracé de MARKER_TRIANGLERIGHT et de MARKER_POINT sur les graphes externes :  indice (INQ100 UT 20s et INQ100 UT 2min) et future (200 et 2000 ticks)		
		if ( (! sc.ChartWindowIsActive)  or  NbMonitors == 2 )
		{
			const int TailleMarkerPoint = 5 + AffichageONOFF*3 ;
			if ( Void == 0 ) 	// Void est inactif
			{
				if ( FutureOuIndice == 1 )
				{
					// Tracés des MARKER_TRIANGLERIGHT à HoraireRAZ 
					DrawMarker_UTAMAOA_UDD_SurGrapheExterne (MARKER_TRIANGLERIGHT, ExternalChartIndex_UT20s, NiveauIndice, 10+3*AffichageONOFF, CouleurPosition, ptrsc, 10042+10000*ThisStudyID, 15, 0) ; 
					DrawMarker_UTAMAOA_UDD_SurGrapheExterne (MARKER_TRIANGLERIGHT, ExternalChartIndex_UT2min, NiveauIndice, 10+3*AffichageONOFF, CouleurPosition, ptrsc, 10042+10000*ThisStudyID, 19, 0) ; 
					// Tracés des MARKER_POINT à HoraireRAZ 
					if ( CetteInstanceControleLesBoutons.GetYesNo() )
					{
						DrawMarker_UTAMAOA_UDD_SurGrapheExterne (MARKER_POINT, ExternalChartIndex_UT20s, NiveauIndice, TailleMarkerPoint, RGB(0,127,255), ptrsc, 10055+10000*ThisStudyID, 15, 0) ;  
						DrawMarker_UTAMAOA_UDD_SurGrapheExterne (MARKER_POINT, ExternalChartIndex_UT2min, NiveauIndice, TailleMarkerPoint, RGB(0,127,255), ptrsc, 10055+10000*ThisStudyID, 19, 0) ; 
					}
				}		
				else if ( FutureOuIndice == 0 )
				{
					// Tracés des MARKER_TRIANGLERIGHT à HoraireRAZ
					DrawMarker_UTAMAOA_UDD_SurGrapheExterne (MARKER_TRIANGLERIGHT, ExternalChartIndex_UT200t, NiveauFutureAjuste, 10+3*AffichageONOFF, CouleurPosition, ptrsc, 10042+10000*ThisStudyID, 11, 0) ; 
					DrawMarker_UTAMAOA_UDD_SurGrapheExterne (MARKER_TRIANGLERIGHT, ExternalChartIndex_UT2000t, NiveauFutureAjuste, 10+3*AffichageONOFF, CouleurPosition, ptrsc, 10042+10000*ThisStudyID, 12, 1) ;
					// Tracés des MARKER_POINT à HoraireRAZ 
					if ( CetteInstanceControleLesBoutons.GetYesNo() )
					{
						DrawMarker_UTAMAOA_UDD_SurGrapheExterne (MARKER_POINT, ExternalChartIndex_UT200t, NiveauFutureAjuste, TailleMarkerPoint, RGB(0,127,255), ptrsc, 10055+10000*ThisStudyID, 11, 0) ;  
						DrawMarker_UTAMAOA_UDD_SurGrapheExterne (MARKER_POINT, ExternalChartIndex_UT2000t, NiveauFutureAjuste, TailleMarkerPoint, RGB(0,127,255), ptrsc, 10055+10000*ThisStudyID, 12, 1) ;  
					}
				}
			}
			else if ( CetteInstanceControleLesBoutons.GetYesNo() )   // les ronds bleus sont affichés même si la study est Void 
			{
				if ( FutureOuIndice == 1 )
				{
					// Tracés des MARKER_POINT à HoraireRAZ
					DrawMarker_UTAMAOA_UDD_SurGrapheExterne (MARKER_POINT, ExternalChartIndex_UT20s, NiveauIndice, TailleMarkerPoint, RGB(0,127,255), ptrsc, 10055+10000*ThisStudyID, 15, 0) ;  
					DrawMarker_UTAMAOA_UDD_SurGrapheExterne (MARKER_POINT, ExternalChartIndex_UT2min, NiveauIndice, TailleMarkerPoint, RGB(0,127,255), ptrsc, 10055+10000*ThisStudyID, 19, 0) ; 
				}		
				else if ( FutureOuIndice == 0 )
				{
					// Tracés des MARKER_POINT à HoraireRAZ
					DrawMarker_UTAMAOA_UDD_SurGrapheExterne (MARKER_POINT, ExternalChartIndex_UT200t, NiveauFutureAjuste, TailleMarkerPoint, RGB(0,127,255), ptrsc, 10055+10000*ThisStudyID, 11, 0) ;  
					DrawMarker_UTAMAOA_UDD_SurGrapheExterne (MARKER_POINT, ExternalChartIndex_UT2000t, NiveauFutureAjuste, TailleMarkerPoint, RGB(0,127,255), ptrsc, 10055+10000*ThisStudyID, 12, 1) ;  
				}
			}
		}
		
		
		// Si RobotActif pour la study, tracer une ligne horizontale pointillé rose sur NiveauFutureAjuste sur les graphes #11 et #12, et sur NiveauIndiceAjusté des graphes #15 et #19
		if ( NbMonitors == 2  or  ! FenetreDeAMR_EstVisible )		// pour le VPS et le portable, on restreint le calcul au cas où le chart 16-tick n'est pas visible
		{
			if ( RobotActif  or  AutoTradeStatus == 11 )	// trouver moyen de ne pas re-re-re-tracer ces objets à chaque appel du DLL
			{
				// Epaisseur ligne pointillé
				int Epaisseur = 5 ;  if ( AutoTradeStatus == 11 ) Epaisseur = 3 ;
				// Affichage de Sequence
				if ( SequenceONOFF_Input.GetIndex() == 1 )  // Sequence = OFF
				{ Texte.Format("Tr. 0   ") ; }	 
				else if ( SequenceStatus == 1)
				{  Texte.Format("Tr. 1   ") ; } 																			
				else if ( SequenceStatus == 2)
				{  Texte.Format("Tr. 2   ") ; }
				else if ( SequenceStatus == 3)
				{  Texte.Format("Tr. 3   ") ; }
				else if ( SequenceStatus == 0)
				{  Texte.Format("Tr. erreur   ") ; }								
				// Future
				DrawHorizontalRay_UTAMAOA_UDD_SurGrapheExterne (ExternalChartIndex_UT200t, NiveauFutureAjuste, Epaisseur, RGB(255,127,255), ptrsc, 10056+10000*ThisStudyID, 11, 0) ;      // void DrawHorizontalRay_UTAMAOA_UDD_SurGrapheExterne (int index, float prix, int epaisseur, COLORREF couleur, s_sc* ptrsc, int Line_Number, int chartNumber, int graphRegion)
				DrawHorizontalRay_UTAMAOA_UDD_SurGrapheExterne (ExternalChartIndex_UT2000t, NiveauFutureAjuste, Epaisseur, RGB(255,127,255), ptrsc, 10056+10000*ThisStudyID, 12, 1) ;     
				// Indice
				float Niveau = NiveauIndice + (float)AjustementChoisiEnPas/4 + AjustementSubiPoints ;
				DrawHorizontalRay_UTAMAOA_UDD_SurGrapheExterne (ExternalChartIndex_UT20s, Niveau, Epaisseur, RGB(255,127,255), ptrsc, 10056+10000*ThisStudyID, 15, 0) ;    
				DrawHorizontalRay_UTAMAOA_UDD_SurGrapheExterne (ExternalChartIndex_UT2min, Niveau, Epaisseur, RGB(255,127,255), ptrsc, 10056+10000*ThisStudyID, 19, 0) ;  
				DrawTextAligne_UTAMAOA_UDD_SurGrapheExterne (Texte, ExternalChartIndex_UT20s, Niveau, Epaisseur*3, CouleurPosition, ptrsc, 10120+10000*ThisStudyID, 15, 0) ;	 		// void DrawTextAligne_UTAMAOA_UDD_SurGrapheExterne (SCString texte, int index, float prix, int tailleTexte, COLORREF couleur, s_sc* ptrsc, int Line_Number, int chartNumber, int GraphRegion)
				DrawTextAligne_UTAMAOA_UDD_SurGrapheExterne (Texte, ExternalChartIndex_UT2min, Niveau, Epaisseur*3, CouleurPosition, ptrsc, 10120+10000*ThisStudyID, 19, 0) ;	 	
			}
			else
			{
				sc.DeleteUserDrawnACSDrawing(11, 10056+10000*ThisStudyID) ;  sc.DeleteUserDrawnACSDrawing(12, 10056+10000*ThisStudyID) ;  sc.DeleteUserDrawnACSDrawing(15, 10056+10000*ThisStudyID) ;  sc.DeleteUserDrawnACSDrawing(19, 10056+10000*ThisStudyID) ;    
																																																						sc.DeleteUserDrawnACSDrawing(15, 10120+10000*ThisStudyID) ;  sc.DeleteUserDrawnACSDrawing(19, 10120+10000*ThisStudyID) ; 
			}
		}
		
		
		// Affichages sur le graphe 16 ticks (et 1-tick) : niveau tradé / prix demandé / première colonne / deuxième colonne / troisième colonne / CO / mvts verticaux / zones grisées (objets rectangles)
		if ( AffichageONOFF == 1  and  Position != 0 )
		{

			// Tracé du prix d'entrée sur le chart #13 (chart 1-tick) ; il faut le réaliser dans ce bloc car CouleurDrawText est dépendant du statut "actif" (bleu turquoise) de la study
			DrawHorizontalRay_UTAMAOA_UDD_SurGrapheExterne (0, NiveauFutureAjuste, 1, CouleurDrawText, ptrsc, 10127+10000*ThisStudyID, 13, 0) ;      // void DrawHorizontalRay_UTAMAOA_UDD_SurGrapheExterne (int index, float prix, int epaisseur, COLORREF couleur, s_sc* ptrsc, int Line_Number, int chartNumber, int graphRegion)

			// Tracé du prix du TP et du SL sur les chart #15/#19 
			// DrawHorizontalRay_UTAMAOA_UDD_SurGrapheExterne (0, NiveauIndiceAjuste_Prior+Position*TPOptimumArrondi_Prior, 1, CouleurPositionInverse, ptrsc, 10138+10000*ThisStudyID, 15, 0) ;     
			// DrawHorizontalRay_UTAMAOA_UDD_SurGrapheExterne (0, NiveauIndiceAjuste_Prior+Position*TPOptimumArrondi_Prior, 1, CouleurPositionInverse, ptrsc, 10139+10000*ThisStudyID, 19, 0) ;     
			// DrawHorizontalRay_UTAMAOA_UDD_SurGrapheExterne (0, NiveauIndiceAjuste_Prior-Position*SL_Prior, 1, CouleurPositionInverse, ptrsc, 10140+10000*ThisStudyID, 15, 0) ;     
			// DrawHorizontalRay_UTAMAOA_UDD_SurGrapheExterne (0, NiveauIndiceAjuste_Prior-Position*SL_Prior, 1, CouleurPositionInverse, ptrsc, 10141+10000*ThisStudyID, 19, 0) ;     


			// Bloc des tracés sur le chart #6
			if ( FenetreDeAMR_EstVisible  and  NiveauFutureAjuste_EstVisible )
			{
				// Tracés du niveau tradé et du prix demandé sur le chart #6  
				DrawSegmentPointille (IndexHoraireRAZ, NiveauFuture, sc.IndexOfLastVisibleBar, NiveauFuture, 2, CouleurDrawText, ptrsc) ;  // trait fin sur le niveau tradé     							//   void DrawSegmentPointille (int index1, float prix1, int index2, float prix2, COLORREF couleur, s_sc* ptrsc)
				DrawSegmentPointille (IndexHoraireRAZ, NiveauFutureAjuste, sc.IndexOfLastVisibleBar, NiveauFutureAjuste, 4, CouleurDrawText, ptrsc) ;  // trait épais sur le prix demandé
				
				
				if ( PremiereToucheFaite == 0 )      // Paramètres tracés avant la Premiere Touche 
				{
					// Affichages sur la première colonne 
					{
						// Affichage du Prix tradé
						Texte.Format("Prix Trade = %.2f", NiveauFutureAjuste) ;																							DrawText_RelativePosition_UTAMAOA (Texte, Position_1ere_Colonne, NiveauFutureAjuste-4.4*Position*Interligne, 9, CouleurDrawText, ptrsc, 10017+10000*ThisStudyID) ;
						
						// Affichage de BonusIntrinseque
						Texte.Format("Bonus intrinseque = %.0f%%", BonusIntrinseque) ;  																			DrawText_RelativePosition_UTAMAOA (Texte, Position_1ere_Colonne, NiveauFutureAjuste-5.8*Position*Interligne, 10, CouleurDrawText, ptrsc, 10018+10000*ThisStudyID) ;  

						// Affichage de Sequence
						if ( SequenceONOFF_Input.GetIndex() == 1 )  // Sequence = OFF
						{ Texte.Format("Sequence OFF") ; }	 
						else if ( SequenceStatus == 1)
						{  Texte.Format("Sequence ON : trade 1") ; } 																			
						else if ( SequenceStatus == 2)
						{  Texte.Format("Sequence ON : trade 2") ; }
						else if ( SequenceStatus == 3)
						{  Texte.Format("Sequence ON : trade 3") ; }
						else if ( SequenceStatus == 0)
						{  Texte.Format("Sequence ON : ready") ; }																												DrawText_RelativePosition_UTAMAOA (Texte, Position_1ere_Colonne, NiveauFutureAjuste-7.2*Position*Interligne, 9, CouleurDrawText, ptrsc, 10050+10000*ThisStudyID) ;  

						// compteur du nb de lignes d'affichages
						int CompteurLignes = 0 ;
						float NbInterlignes ; 			// vaut par exemple 8.6 pour HoraireExpiration
						
						// Affichage de HoraireExpiration le cas échéant
						if ( HoraireExpiration.GetFloat() != 0 )
						{
							int hh = floor ( HoraireExpiration.GetFloat() /100 ) ;
							int mm = floor ( HoraireExpiration.GetFloat() -100*hh ) ;
							Texte.Format("HoraireExpiration = %dh%02dm", hh, mm) ;  																					
							CompteurLignes++ ;		NbInterlignes = 7.2+CompteurLignes*1.4 ;																
						}
						else
						{ Texte.Format("") ;  }																																					DrawText_RelativePosition_UTAMAOA (Texte, Position_1ere_Colonne, NiveauFutureAjuste-NbInterlignes*Position*Interligne, 9, CouleurDrawText, ptrsc, 10019+10000*ThisStudyID) ; 
					
						// Taux virginité
						if ( TauxVirginite < 1 ) 
						{	Texte.Format("Virginite=%.0f%%", TauxVirginite*100) ;  CompteurLignes++ ;  NbInterlignes = 7.2+CompteurLignes*1.4 ;  } 																				
						else { Texte.Format("") ;  }																																			DrawText_RelativePosition_UTAMAOA (Texte, Position_1ere_Colonne, NiveauFutureAjuste-NbInterlignes*Position*Interligne, 9, RGB(255,0,0), ptrsc, 10006+10000*ThisStudyID) ;
						
						// DistanceAuPHPB_SurSL
						if ( DistanceAuPHPB_SurSL < 1 )
						{ Texte.Format("DistanceAuPHPB/SL=%.0f%%", DistanceAuPHPB_SurSL*100) ;  CompteurLignes++ ;  NbInterlignes = 7.2+CompteurLignes*1.4 ;}
						else { Texte.Format("") ;  }																																				DrawText_RelativePosition_UTAMAOA (Texte, Position_1ere_Colonne, NiveauFutureAjuste-NbInterlignes*Position*Interligne, 9, RGB(255,0,0), ptrsc, 10128+10000*ThisStudyID) ; 

						// Malus_SpreadIndex
						if ( NatureNiveau_Input.GetInt() == 0  and  SpreadIndex > 1.3 )
						{ Texte.Format("Malus_SpreadIndex = %.0f%%", Malus_SpreadIndex*100) ;  CompteurLignes++ ;  NbInterlignes = 7.2+CompteurLignes*1.4 ;}
						else { Texte.Format("") ;  }																																			DrawText_RelativePosition_UTAMAOA (Texte, Position_1ere_Colonne, NiveauFutureAjuste-NbInterlignes*Position*Interligne, 9, RGB(255,0,0), ptrsc, 10136+10000*ThisStudyID) ; 
						
						// MalusExcedentaire
						if ( BonusExcedentaireSuffisant < 0 )
						{
							Texte.Format("Malus Excedentaire = +%.0f%%", MalusExcedentaire) ; 
							CompteurLignes++ ;		NbInterlignes = 7.2+CompteurLignes*1.4 ;		
						}
						else { Texte.Format("") ;  }						DrawText_RelativePosition_UTAMAOA (Texte, Position_1ere_Colonne, NiveauFutureAjuste-NbInterlignes*Position*Interligne, 9, RGB(200,0,0), ptrsc, 10137+10000*ThisStudyID) ;
						
					}
					
					// Affichages sur la deuxième colonne 
					if ( IndexArretManualLooping != -1 )   // on n'affiche que si entrée en position avant 16h00
					{
						// Affichage de TPNu  // Affichage de MyEP si 
						if ( OrdreOuvert == 1  and  PositionOuverte == 0 )
						{	Texte.Format("TP nu = %.2f t  | MyEP = %d/%d", TPNu, MyEP, Quantity_NiveauFutureAjuste) ;  }
						else  { Texte.Format("TP nu = %.2f p", TPNu) ;  } 																										DrawText_RelativePosition_UTAMAOA (Texte, Position_2eme_Colonne, NiveauFutureAjuste+5.0*Position*Interligne, 10, CouleurDrawText, ptrsc, 10024+10000*ThisStudyID) ;

						// Affichages de TP / SL 
						Texte.Format("TP = %.2f (%.2f) | SL = %.2f", TPOptimumArrondi, TPOptimum, SL) ;												DrawText_RelativePosition_UTAMAOA (Texte, Position_2eme_Colonne, NiveauFutureAjuste+3.0*Position*Interligne, 14, CouleurDrawText, ptrsc,10001+10000*ThisStudyID) ; 

						// Affichage de BonusExcedentaire
						Texte.Format("Bonus excedentaire = %.0f%%", BonusExcedentaire) ;  																	DrawText_RelativePosition_UTAMAOA (Texte, Position_2eme_Colonne, NiveauFutureAjuste-2.7*Position*Interligne, 12, CouleurDrawText, ptrsc, 10004+10000*ThisStudyID) ; 

						// Affichage de BonusMoy
						if ( BonusMoy < 4 )  { Texte.Format("BonusMoy = %.0f%% est INSUFFISANT !", BonusMoy) ;  								DrawText_RelativePosition_UTAMAOA (Texte, Position_2eme_Colonne, NiveauFutureAjuste-4.4*Position*Interligne, 14, CouleurDrawText, ptrsc, 10005+10000*ThisStudyID) ;  }  
						else  { Texte.Format("BonusMoy = %.0f%%", BonusMoy) ;  																						DrawText_RelativePosition_UTAMAOA (Texte, Position_2eme_Colonne, NiveauFutureAjuste-4.4*Position*Interligne, 13, CouleurDrawText, ptrsc, 10005+10000*ThisStudyID) ;  }
						
						// Affichage de MalusDeNRV, MalusDePremierPassageJournalier
						if ( MalusNRV_Manuel_Input.GetFloat() == 0 )
						{
							Texte.Format("Malus : NRV=%.0f%%", MalusDeNRV) ;
							if ( MalusDeNRV < -0.5 )  { SCString StringAjoute ;  StringAjoute.Format(" (Bonus)") ; Texte.Append(StringAjoute) ;  } 
							if ( NewDailyHighLow_Input.GetYesNo() )  { SCString StringAjoute ;  StringAjoute.Format(" | NewDHiLo=%.0f%%", MalusDePremierPassageJournalier) ; Texte.Append(StringAjoute) ;  } 
						}
						else 
						{	Texte.Format("Malus : NRV=%.0f%% (INPUT MANUEL)", MalusDeNRV) ;  }
						;																																													DrawText_RelativePosition_UTAMAOA (Texte, Position_2eme_Colonne, NiveauFutureAjuste-6.0*Position*Interligne, 9, CouleurDrawText, ptrsc, 10003+10000*ThisStudyID) ;  
						
						// Affichage des malus d'ajustements (choisi et subi)
						if ( MalusAjustementParabolique_Input.GetIndex() == 0 )   // Modèle linéaire
						{
							if ( FutureOuIndice == 0 )
							{	Texte.Format( "Malus ajustement (abs) = %.0f%% (Linear)", fabs(AjustementChoisiEnPas*CoeffMalusAjustement/4) ) ;  }
							else if ( FutureOuIndice == 1 )
							{	Texte.Format( "Malus aj : Choisi(%.1fpt)=%.0f%% | Subi(%.1fpt)=%.0f%% | SommeAbs=%.0f%% (Linear)", AjustementChoisiEnPas*0.25, AjustementChoisiEnPas*CoeffMalusAjustement*0.25, AjustementSubiPoints, AjustementSubiPoints*CoeffMalusAjustement, MalusAjustementAbsolu)  ;  }
						}
						else if ( MalusAjustementParabolique_Input.GetIndex() == 1 )   // Modèle Parabolique           
						{
							if ( FutureOuIndice == 0 )
							{	Texte.Format( "Malus ajustement (abs) = %.0f%% (Parabolic)", fabs(AjustementChoisiEnPas*CoeffMalusAjustement*0.25) ) ;  }
							else if ( FutureOuIndice == 1 )
							{	Texte.Format( "Malus aj : Choisi(%.1fpt) | Subi(%.1fpt) | SommeAbs=%.0f%% (Parabolic)", AjustementChoisiEnPas*0.25, AjustementSubiPoints, MalusAjustementAbsolu ) ;  }
						}
						;																																													DrawText_RelativePosition_UTAMAOA (Texte, Position_2eme_Colonne, NiveauFutureAjuste-7.6*Position*Interligne, 9, CouleurDrawText, ptrsc, 10002+10000*ThisStudyID) ;

						// BonusExcedentaireSuffisant
						if ( FutureOuIndice == 0 and AjustementChoisiEnPas == 0 )  // non affiché si trading de future et ajustement=0
						{ Texte.Format("") ; }
						else
						{
							if ( BonusExcedentaireSuffisant < 0 )  {	Texte.Format("!! BonusExcedentaireSuffisant = %.0f%% !!", BonusExcedentaireSuffisant) ;  }
							else { Texte.Format("BonusExcedentaireSuffisant = %.0f%%", BonusExcedentaireSuffisant) ;  }
						}
						;																																													DrawText_RelativePosition_UTAMAOA (Texte, Position_2eme_Colonne, NiveauFutureAjuste-9.6*Position*Interligne, 12, CouleurDrawText, ptrsc, 10007+10000*ThisStudyID) ;
					}
					else    // IndexArretManualLooping == -1   // on n'affiche pas la deuxième colonne
					{
						Texte.Format("Pas d'entree avant la fin de la seance") ;																							DrawText_RelativePosition_UTAMAOA (Texte, Position_2eme_Colonne, NiveauFutureAjuste-0.0*Position*Interligne, 14, CouleurDrawText, ptrsc,10001+10000*ThisStudyID) ; 
					}
					
					// Affichage du CO
					if ( sc.ServerConnectionState == SCS_CONNECTED )    // pour éviter affichage lors des backtests 
					{
						SCString Texte_Quantity_NiveauFutureAjuste;
						if ( Level_NiveauFutureAjuste == 99999 )		// il y a zéro lot sur le prix NiveauFutureAjuste => on cherche le niveau peuplé le plus proche en se dirigeant vers le best-bid-ask
						{
							Texte_Quantity_NiveauFutureAjuste.Format("0") ; 
							s_MarketDepthEntry DepthEntry;
							float Price ;
							int Level = 0 ; 	// sur le ask (resp. le bid) on parcourt les niveaux en partant du best ask (resp. best bid) qui vaut Level=0.
							
							if ( Position == -1 )  // vente, on est au ask
							{
								int NbLevels = sc.GetAskMarketDepthNumberOfLevels() ;
								do  // on parcourt le CO du Level = 0 jusqu'à arriver à un prix qui dépasse NiveauFutureAjuste
								{
									sc.GetAskMarketDepthEntryAtLevel(DepthEntry, Level);     
									Price = DepthEntry.Price;
									Level++ ;
								}
								while ( Price < NiveauFutureAjuste  and  Level < NbLevels ) ;
							}
							else if ( Position == 1 )  // achat, on est au bid
							{
								int NbLevels = sc.GetBidMarketDepthNumberOfLevels() ;
								do  // on parcourt le CO du Level = 0 jusqu'à arriver à un prix qui dépasse NiveauFutureAjuste
								{
									sc.GetBidMarketDepthEntryAtLevel(DepthEntry, Level);
									Price = DepthEntry.Price;
									Level++ ;
								}
								while ( Price > NiveauFutureAjuste  and  Level < NbLevels ) ;
							}
							Level_NiveauFutureAjuste = Level - 1 ;		
						}
						
						// on balaye de -10 levels à +10 levels autour de Level_NiveauFutureAjuste, et on affiche le nb de lots sur ces niveaux
						for ( int Index = -10 ; Index <= 10 ; Index ++ )  
						{
							if ( Level_NiveauFutureAjuste + Index < 0 ) continue ;
							
							s_MarketDepthEntry DepthEntry;
							if ( Position == 1 )  { sc.GetBidMarketDepthEntryAtLevel(DepthEntry, Level_NiveauFutureAjuste + Index) ;  }
							else if ( Position == -1 )  { sc.GetAskMarketDepthEntryAtLevel(DepthEntry, Level_NiveauFutureAjuste + Index) ;  }
							
							float Price = DepthEntry.Price;
							float PositionRelative = 148 + int(Price*4) %2 ;   // PositionRelative vaut 148 (prix en xyz.00 ou en xyz.50) ou 149 (prix en xyz.25 ou xyz.75)
							
							// Marqueur sur prix rond
							int PrixRond = 1 - ceil ( Price - int(Price) ) ;     // c'est un booléen qui retourne 0 si le prix finit par 0.25/0.50/0.75, et retourne 1 si le prix finit par .00
							if ( PrixRond )  DrawMarker_RelativePosition_UTAMAOA (MARKER_POINT, PositionRelative, Price, 9, RGB(195,195,195), ptrsc, 0) ;           
							
							
							// tracé du texte ; le tracé du MARKER_POINT doit avoir lieu avant celui du texte pour que le texte soit affiché par-dessus
							int Quantity = DepthEntry.Quantity ;
							if ( Price != NiveauFutureAjuste )
							{
								Texte.Format("%d", Quantity) ;																															DrawText_RelativePosition_UTAMAOA (Texte, PositionRelative, Price, 8, CouleurDrawText, ptrsc, 0) ;    // on réserve les LineNumber 10093/10094/10095/10096/10097/10098/10099/10100/10101/10102/10103/10104/10105/10106/10107/10108/10109/10110/10111/10112/10113
							}
							else
							{
								Texte_Quantity_NiveauFutureAjuste.Format("%d", Quantity) ;
							}
						}
						
						// on marque spécifiquement la qté de NiveauFutureAjuste avec un disque rose ; on le trace à la fin pour qu'il soit par-dessus les autres disques
						float PositionRelative = 148 + int(NiveauFutureAjuste*4) %2 ;	
						DrawMarker_RelativePosition_UTAMAOA (MARKER_POINT, PositionRelative, NiveauFutureAjuste, 9, RGB(255,210,255), ptrsc, 0) ;		// void DrawMarker_RelativePosition_UTAMAOA (int marqueur, float RelativePositionAbcsisse, float prix, int tailleObjet, COLORREF couleur, s_sc* ptrsc, int Line_Number)  
						;																																													DrawText_RelativePosition_UTAMAOA (Texte_Quantity_NiveauFutureAjuste, PositionRelative, NiveauFutureAjuste, 8, CouleurDrawText, ptrsc, 0) ; 
					} // fin affichage du CO
					
				}
				
				else // Paramètres tracés après la Premiere Touche 
				{
					// Affichages sur la première colonne 
					{
						// Affichage du Prix tradé
						Texte.Format("Prix Trade = %.2f", NiveauFutureAjuste) ;																							DrawText_RelativePosition_UTAMAOA (Texte, Position_1ere_Colonne, NiveauFutureAjuste-4.4*Position*Interligne, 9, CouleurDrawText, ptrsc, 10017+10000*ThisStudyID) ;
						
						// Affichage de BonusIntrinseque
						Texte.Format("Bonus intrinseque = %.0f%%", BonusIntrinseque) ;  																			DrawText_RelativePosition_UTAMAOA (Texte, Position_1ere_Colonne, NiveauFutureAjuste-5.8*Position*Interligne, 10, CouleurDrawText, ptrsc, 10018+10000*ThisStudyID) ;  
						
						// Affichage de Sequence
						if ( SequenceONOFF_Input.GetIndex() == 1 )  // Sequence = OFF
						{ Texte.Format("Sequence OFF") ; }	 
						else if ( SequenceStatus == 1)
						{  Texte.Format("Sequence ON : trade 1") ; } 																			
						else if ( SequenceStatus == 2)
						{  Texte.Format("Sequence ON : trade 2") ; }
						else if ( SequenceStatus == 3)
						{  Texte.Format("Sequence ON : trade 3") ; }
						else if ( SequenceStatus == 0)
						{  Texte.Format("Sequence ON : ready") ; }																												DrawText_RelativePosition_UTAMAOA (Texte, Position_1ere_Colonne, NiveauFutureAjuste-7.2*Position*Interligne, 9, CouleurDrawText, ptrsc, 10050+10000*ThisStudyID) ;  

						// compteur du nb de lignes d'affichages
						int CompteurLignes = 0 ;
						float NbInterlignes ; 			// vaut par exemple 8.6 pour HoraireExpiration
						
						// Affichage de HoraireExpiration le cas échéant
						if ( HoraireExpiration.GetFloat() != 0 )
						{
							int hh = floor ( HoraireExpiration.GetFloat() /100 ) ;
							int mm = floor ( HoraireExpiration.GetFloat() -100*hh ) ;
							Texte.Format("HoraireExpiration = %dh%02dm", hh, mm) ;
							CompteurLignes++ ;		NbInterlignes = 7.2+CompteurLignes*1.4 ;																		DrawText_RelativePosition_UTAMAOA (Texte, Position_1ere_Colonne, NiveauFutureAjuste-NbInterlignes*Position*Interligne, 9, CouleurDrawText, ptrsc, 10019+10000*ThisStudyID) ;   }

						// Taux virginité
						if ( TauxVirginite_Prior < 1 )  // %NBV n'est affiché que s'il est inférieur à 100%
						{	Texte.Format("Virginite=%.0f%%", TauxVirginite_Prior*100) ;  																		
						CompteurLignes++ ;		NbInterlignes = 7.2+CompteurLignes*1.4 ;																			DrawText_RelativePosition_UTAMAOA (Texte, Position_1ere_Colonne, NiveauFutureAjuste-NbInterlignes*Position*Interligne, 9, RGB(255,0,0), ptrsc, 10006+10000*ThisStudyID) ;  }
						
						// DistanceAuPHPB_SurSL
						if ( DistanceAuPHPB_SurSL_Prior < 1 )
						{ Texte.Format("DistanceAuPHPB/SL=%.0f%%", DistanceAuPHPB_SurSL_Prior*100) ;  										
						CompteurLignes++ ;		NbInterlignes = 7.2+CompteurLignes*1.4 ;																			DrawText_RelativePosition_UTAMAOA (Texte, Position_1ere_Colonne, NiveauFutureAjuste-NbInterlignes*Position*Interligne, 9, RGB(255,0,0), ptrsc, 10119+10000*ThisStudyID) ;   }
						
						// Malus_SpreadIndex
						if ( NatureNiveau_Input.GetInt() == 0  and  SpreadIndex_Prior > 1.3 )
						{ Texte.Format("Malus_SpreadIndex = %.0f%%", Malus_SpreadIndex_Prior*100) ; 
						CompteurLignes++ ;		NbInterlignes = 7.2+CompteurLignes*1.4 ;																			DrawText_RelativePosition_UTAMAOA (Texte, Position_1ere_Colonne, NiveauFutureAjuste-NbInterlignes*Position*Interligne, 9, RGB(255,0,0), ptrsc, 10136+10000*ThisStudyID) ;  }

						// MalusExcedentaire
						if ( MalusExcedentaire_Prior != 0 )
						{ Texte.Format("Malus Excedentaire = +%.0f%%", MalusExcedentaire_Prior) ;
						CompteurLignes++ ;		NbInterlignes = 7.2+CompteurLignes*1.4 ;																			DrawText_RelativePosition_UTAMAOA (Texte, Position_1ere_Colonne, NiveauFutureAjuste-NbInterlignes*Position*Interligne, 9, RGB(200,0,0), ptrsc, 10137+10000*ThisStudyID) ;  }
						
					}
					
					// Affichages sur la deuxième colonne 
					{
						// Affichage de TP_AUC
						Texte.Format("TP_AUC = %.2f pt, RatioTailleBougies 4000t = %.2f", TP_AUC_Prior, TailleBougie_4000Ticks(SpreadIndex_Prior, ptrsc) / TailleBougie_4000Ticks(1, ptrsc)) ;
						;																																													DrawText_RelativePosition_UTAMAOA (Texte, Position_2eme_Colonne, NiveauFutureAjuste+6.8*Position*Interligne, 9, CouleurDrawText, ptrsc, 10130+10000*ThisStudyID) ; 
						
						// Affichage de TPNu
						Texte.Format("TP nu = %.2f p", TPNu_Prior) ; 
						int RedTPNu = 0 ;  int GreenTPNu = 0 ;  int BlueTPNu = 0 ; 
						if  ( TPNu_Prior <= 2.5 )  // violet
						{ RedTPNu = 178 ;  GreenTPNu = 102 ;  BlueTPNu = 255 ; }
						else if ( TPNu_Prior <= 3.5 )  // vert
						{ GreenTPNu = 220;  }
						else if ( TPNu_Prior <= 4.5 )   // jaune
						{ RedTPNu = 200 ; GreenTPNu = 200 ; }
						else if ( TPNu_Prior <= 5.5 )   // orange
						{ RedTPNu = 250 ; GreenTPNu = 150 ; }
						else if ( TPNu_Prior <= 6.5 )   // rouge
						{ RedTPNu = 250 ; }
						else if ( TPNu_Prior <= 7.5 )   // brun
						{ RedTPNu = 168 ; GreenTPNu = 84 ; }
						else if ( TPNu_Prior <= 8.5 )   // gris clair
						{ RedTPNu = 171 ; GreenTPNu = 171 ; BlueTPNu = 171 ; }
						else if ( TPNu_Prior <= 9.5 )   // gris foncé
						{ RedTPNu = 100 ; GreenTPNu = 100 ; BlueTPNu = 100 ; }
						else  // noir
						{ RedTPNu = 0 ; GreenTPNu = 0 ; BlueTPNu = 0 ; }																										DrawText_RelativePosition_UTAMAOA (Texte, Position_2eme_Colonne, NiveauFutureAjuste+5.0*Position*Interligne, 12, RGB(RedTPNu, GreenTPNu, BlueTPNu), ptrsc, 10024+10000*ThisStudyID) ; 

						// Affichages de TP / SL 
						Texte.Format("TP = %.2f (%.2f) | SL = %.2f", TPOptimumArrondi_Prior, TPOptimum_Prior, SL_Prior) ;  					DrawText_RelativePosition_UTAMAOA (Texte, Position_2eme_Colonne, NiveauFutureAjuste+3.0*Position*Interligne, 14, CouleurDrawText, ptrsc,10001+10000*ThisStudyID) ; 
						
						// Affichage de BonusExcedentaire_Prior
						if ( BonusExcedentaire_Prior < 0 )
						{
							Texte.Format("!! Bonus excedentaire = %.0f%% !!", BonusExcedentaire_Prior) ; 													DrawText_RelativePosition_UTAMAOA (Texte, Position_2eme_Colonne, NiveauFutureAjuste-2.7*Position*Interligne, 13, CouleurDrawText, ptrsc, 10004+10000*ThisStudyID) ; 
						}
						else
						{
							Texte.Format("Bonus excedentaire = %.0f%%", BonusExcedentaire_Prior) ; 															DrawText_RelativePosition_UTAMAOA (Texte, Position_2eme_Colonne, NiveauFutureAjuste-2.7*Position*Interligne, 12, CouleurDrawText, ptrsc, 10004+10000*ThisStudyID) ; 
						}

						// Affichage de BonusMoy_Prior 
						if ( BonusMoy_Prior < 4 )  { Texte.Format("!! BonusMoy = %.0f%% est INSUFFISANT !!", BonusMoy_Prior) ;  			DrawText_RelativePosition_UTAMAOA (Texte, Position_2eme_Colonne, NiveauFutureAjuste-4.4*Position*Interligne, 14, CouleurDrawText, ptrsc, 10005+10000*ThisStudyID) ;  }  
						else  {  Texte.Format("BonusMoy = %.0f%%", BonusMoy_Prior) ;  																			DrawText_RelativePosition_UTAMAOA (Texte, Position_2eme_Colonne, NiveauFutureAjuste-4.4*Position*Interligne, 13, CouleurDrawText, ptrsc, 10005+10000*ThisStudyID) ;  }

						// Affichage de MalusDeNRV, MalusDePremierPassageJournalier
						if ( MalusNRV_Manuel_Input.GetFloat() == 0 )
						{
							Texte.Format("Malus : NRV=%.0f%%", MalusDeNRV_Prior) ;
							if ( MalusDeNRV_Prior < -0.5 )  { SCString StringAjoute ;  StringAjoute.Format(" (Bonus)") ; Texte.Append(StringAjoute) ;  } 
							if ( NewDailyHighLow_Input.GetYesNo() )  { SCString StringAjoute ;  StringAjoute.Format(" | NewDHiLo=%.0f%%", MalusDePremierPassageJournalier_Prior) ; Texte.Append(StringAjoute) ;  } 
						}
						else 
						{	Texte.Format("Malus : NRV=%.0f%% (INPUT MANUEL)", MalusDeNRV_Prior) ;  }
						;																																											DrawText_RelativePosition_UTAMAOA (Texte, Position_2eme_Colonne, NiveauFutureAjuste-6.0*Position*Interligne, 9, CouleurDrawText, ptrsc, 10003+10000*ThisStudyID) ;  

						// Affichage des malus d'ajustements (choisi et subi)
						if ( MalusAjustementParabolique_Input.GetIndex() == 0 )   // Modèle linéaire
						{
							if ( FutureOuIndice == 0 )
							{	Texte.Format( "Malus ajustement (abs) = %.0f%% (Linear)", fabs(AjustementChoisiEnPas*CoeffMalusAjustement_Prior/4) ) ;  }
							else if ( FutureOuIndice == 1 )
							{	Texte.Format( "Malus aj : Choisi(%.1fpt)=%.0f%% | Subi(%.1fpt)=%.0f%% | SommeAbs=%.0f%% (Linear)", AjustementChoisiEnPas*0.25, AjustementChoisiEnPas*0.25*CoeffMalusAjustement_Prior, AjustementSubiPoints_Prior, AjustementSubiPoints_Prior*CoeffMalusAjustement_Prior, MalusAjustementAbsolu_Prior ) ;  }
							;																																										DrawText_RelativePosition_UTAMAOA (Texte, Position_2eme_Colonne, NiveauFutureAjuste-7.6*Position*Interligne, 9, CouleurDrawText, ptrsc, 10002+10000*ThisStudyID) ;
						}
						else if ( MalusAjustementParabolique_Input.GetIndex() == 1 )   // Modèle Parabolique
						{
							if ( FutureOuIndice == 0 )
							{	Texte.Format( "Malus ajustement (abs) = %.0f%% (Parabolic)", MalusAjustementAbsolu_Prior) ;  }
							else if ( FutureOuIndice == 1 )
							{	Texte.Format( "Malus aj : Choisi(%.1fpt) | Subi(%.1fpt) | SommeAbs=%.0f%% (Parabolic)", AjustementChoisiEnPas*0.25, AjustementSubiPoints_Prior, MalusAjustementAbsolu_Prior ) ;  }
							;																																										DrawText_RelativePosition_UTAMAOA (Texte, Position_2eme_Colonne, NiveauFutureAjuste-7.6*Position*Interligne, 9, CouleurDrawText, ptrsc, 10002+10000*ThisStudyID) ;
						}

						// Affichage de BonusExcedentaireSuffisant_Prior
						if ( FutureOuIndice == 0 and AjustementChoisiEnPas == 0 )  // non affiché si trading de future et ajustement=0
						{
							Texte.Format("") ; 																																				
						}
						else
						{
							if ( BonusExcedentaireSuffisant_Prior < 0 )   
							{	Texte.Format("!! BonusExcedentaireSuffisant = %.0f%% !!", BonusExcedentaireSuffisant_Prior) ;  }
							else
							{	Texte.Format("BonusExcedentaireSuffisant = %.0f%%", BonusExcedentaireSuffisant_Prior) ;  }
						}
						;																																											DrawText_RelativePosition_UTAMAOA (Texte, Position_2eme_Colonne, NiveauFutureAjuste-9.6*Position*Interligne, 12, CouleurDrawText, ptrsc, 10007+10000*ThisStudyID) ; 

						// affichage du MODEL
						/* if 		  	( ModelePourAUC_Input.GetIndex() == 0 )  Texte.Format("MODEL 7") ;  
						else if	( ModelePourAUC_Input.GetIndex() == 1 )  Texte.Format("MODEL 9") ;  
						else if	( ModelePourAUC_Input.GetIndex() == 2 )  Texte.Format("MODEL 10") ;  
						else if	( ModelePourAUC_Input.GetIndex() == 3 )  Texte.Format("MODEL 11") ;   */
						Texte = ModelePourAUC_Input.GetSelectedCustomString() ;
						;																																											DrawText_RelativePosition_UTAMAOA (Texte, Position_2eme_Colonne, NiveauFutureAjuste-11.8*Position*Interligne, 14, CouleurDrawText, ptrsc, 10131+10000*ThisStudyID) ;   
						
						// affichage de NewDailyHighLow_InputManuel
						if ( NatureNiveau_Input.GetInt() == 6 )  // le niveau est PHJ ou PBJ
						{
									if ( NewDailyHighLow_InputManuel.GetIndex() == 0 )  Texte.Format("PHJ/PBJ trade sans malus NewDHiLo") ;  
							else if ( NewDailyHighLow_InputManuel.GetIndex() == 1 )  Texte.Format("PHJ/PBJ trade avec malus NewDHiLo") ;  
						;																																											DrawText_RelativePosition_UTAMAOA (Texte, Position_2eme_Colonne, NiveauFutureAjuste-14.0*Position*Interligne, 12, CouleurDrawText, ptrsc, 10133+10000*ThisStudyID) ;   
						}
					}
					
					// Affichages sur la troisième colonne
					float NbInterligneMax_3emeColonne = 0 ; 
					{
						
						// affichage de la distance à ISP500 et le ratio de distanciation
						{
							float Distance, RatioDistance ;
							if (Position == 1)
							{
								// on récupère l'array contenant la distance au PBJ du SP500
								SCFloatArray SP500_DistanceBas ;
								sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_DistancePHJPBJ_SP500, 1, 4, SP500_DistanceBas);   //  int GetStudyExtraArrayFromChartUsingID(int ChartNumber, int StudyID, int SubgraphIndex, int ExtraArrayIndex, SCFloatArrayRef ExtraArrayRef);
								Distance = SP500_DistanceBas[IndexPremiereTouche] ;
								RatioDistance = 3*Distance/SL_Prior*100 ;
								/* if ( RatioDistance < 10 )  RatioDistance = round (RatioDistance) ;  else */  RatioDistance = round (RatioDistance/10)*10 ;
								Texte.Format("Distance du SP500 a son PBJ = %.1f pts (%.0f%%)", Distance, RatioDistance) ; 
							}
							else if (Position == -1)
							{
								// on récupère l'array contenant la distance au PHJ du SP500
								SCFloatArray SP500_DistanceHaut ;
								sc.GetStudyExtraArrayFromChartUsingID(sc.ChartNumber, StudyID_DistancePHJPBJ_SP500, 1, 3, SP500_DistanceHaut);   //  int GetStudyExtraArrayFromChartUsingID(int ChartNumber, int StudyID, int SubgraphIndex, int ExtraArrayIndex, SCFloatArrayRef ExtraArrayRef);
								Distance = SP500_DistanceHaut[IndexPremiereTouche] ;
								RatioDistance = 3*Distance/SL_Prior*100 ;
								/* if ( RatioDistance < 10 )  RatioDistance = round (RatioDistance) ;  else */  RatioDistance = round (RatioDistance/10)*10 ;
								Texte.Format("Distance du SP500 a son PHJ = %.1f pts (%.0f%%)", Distance, RatioDistance) ; 
							}
							;																																												DrawText_RelativePosition_UTAMAOA (Texte, Position_3eme_Colonne, NiveauFutureAjuste+5.9*Position*Interligne, 10, CouleurDrawText, ptrsc, 10134+10000*ThisStudyID) ;    
						}
						
						// affichage de SpreadIndex à la première touche :
						Texte.Format("SpreadIndex = %.2f", SpreadIndex_Prior) ;  																						
						;																																													DrawText_RelativePosition_UTAMAOA (Texte, Position_3eme_Colonne, NiveauFutureAjuste+4.1*Position*Interligne, 10, CouleurDrawText, ptrsc, 10040+10000*ThisStudyID) ;   
						
						// affichage de CoeffMalusAjustement à la première touche :
						Texte.Format("CoeffMalusAjustement = %.1f", CoeffMalusAjustement_Prior) ; 														DrawText_RelativePosition_UTAMAOA (Texte, Position_3eme_Colonne, NiveauFutureAjuste+2.3*Position*Interligne, 8, CouleurDrawText, ptrsc, 10123+10000*ThisStudyID) ;   

						// affichage de "SommeRatio & Malus" à la première touche   
						if ( ( MalusExcedentaire_Prior != 0 )  or  ( TauxVirginite_Prior < 1 )  or  ( DistanceAuPHPB_SurSL_Prior < 1 )  or  ( SpreadIndex_Prior > 1.3  and  NatureNiveau_Input.GetInt() == 0 ) )
						Texte.Format("Somme Ratio & Malus = %.0f%%", SommeRatioAvecMalus_Prior) ;  			
						// else  Texte.Format("Somme Ratio = %.0f%%", (RatioAUC_Prior+RatioSigma_Prior)/1.8 ) ;  			
						else  Texte.Format("Somme Ratio = %.0f%%", SommeRatioAvecMalus_Prior ) ;  			
						;																																													DrawText_RelativePosition_UTAMAOA (Texte, Position_3eme_Colonne, NiveauFutureAjuste-2.4*Position*Interligne, 13, RGB(255,0,0), ptrsc, 10052+10000*ThisStudyID) ;  

						// affichage de RatioAUC à la première touche
						if (FutureOuIndice == 0 )
						{	Texte.Format("RatioAUC = %.0f%%", RatioAUC_Prior) ;  }
						else if (FutureOuIndice == 1 )
						{	
							if ( RatioAUC_Future_Prior < 1000 ) 	Texte.Format("RatioAUC = %.0f%% (Fut : %.0f%%)", RatioAUC_Prior, RatioAUC_Future_Prior) ;
							else														Texte.Format("RatioAUC = %.0f%% (!!! FUT : %.0f%% !!!)", RatioAUC_Prior, RatioAUC_Future_Prior) ;
						} 
						;																																													DrawText_RelativePosition_UTAMAOA (Texte, Position_3eme_Colonne, NiveauFutureAjuste-5.4*Position*Interligne, 11, RGB(120,120,255), ptrsc, 10035+10000*ThisStudyID) ; 

						// affichage de SeuilAUC à la première touche 
						Texte.Format("SeuilAUC = %.0f", SeuilAUC_Prior) ;  		
						{
							SCString StringAjoute ;
							StringAjoute.Format("  (20*TPNu^2)") ; 
							Texte.Append(StringAjoute) ;
						}
						;																																													DrawText_RelativePosition_UTAMAOA (Texte, Position_3eme_Colonne, NiveauFutureAjuste-6.8*Position*Interligne, 9, RGB(120,120,255), ptrsc, 10034+10000*ThisStudyID) ;   

						// affichage de AUC à la première touche
						Texte.Format("AUC = %.0f", AUC_Prior) ;  																												DrawText_RelativePosition_UTAMAOA (Texte, Position_3eme_Colonne, NiveauFutureAjuste-8.0*Position*Interligne, 9, RGB(120,120,255), ptrsc, 10033+10000*ThisStudyID) ;   

						// valeur de Sigma/Seuil à la première touche :
						Texte.Format("NewRatioSigma= %.0f%%", RatioSigma_Prior) ;  																				DrawText_RelativePosition_UTAMAOA (Texte, Position_3eme_Colonne, NiveauFutureAjuste-9.5*Position*Interligne, 11, RGB(255,128,0), ptrsc, 10038+10000*ThisStudyID) ; 

						// affichage de Sigma à la première touche :
						Texte.Format("NewSigma = %.0f%%", Sigma_Prior) ;  																								DrawText_RelativePosition_UTAMAOA (Texte, Position_3eme_Colonne, NiveauFutureAjuste-11.5*Position*Interligne, 9, RGB(255,128,0), ptrsc, 10036+10000*ThisStudyID) ;  
						
						// compteur du nb de lignes d'affichages de corrections de SommeRatio
						int CompteurLignes = 0 ;
						float NbInterlignes ; 		// vaut par exemple 14.5 pour Sigma_Prior
						
						// affichage de SommeRatio, avant les autres malus
						if ( ( MalusExcedentaire_Prior != 0 )  or  ( TauxVirginite_Prior < 1 )  or  ( DistanceAuPHPB_SurSL_Prior < 1 )  or  ( SpreadIndex_Prior > 1.3  and  NatureNiveau_Input.GetInt() == 0 ) )
						{	Texte.Format("Somme Ratio = %.0f%% (avant les autres malus)", ( RatioAUC_Prior + RatioSigma_Prior ) /1.8 ) ;		
						CompteurLignes++ ;		NbInterlignes = 11.5+CompteurLignes*2 ;																			DrawText_RelativePosition_UTAMAOA (Texte, Position_3eme_Colonne, NiveauFutureAjuste-NbInterlignes*Position*Interligne, 11, RGB(255,0,0), ptrsc, 10135+10000*ThisStudyID) ;  }
						
						// NbInterligneMax_3emeColonne 
						NbInterligneMax_3emeColonne = 11.5+CompteurLignes*2 ;
					}
					
					// Affichage des marqueurs sur Niveau du TPOptimumArrondi_Prior et du TPBooste
					{
						if (Position == 1 ) 	{ DrawMarker_UTAMAOA (MARKER_TRIANGLERIGHT, IndexPremiereTouche, NiveauFutureAjuste+Position*TPOptimumArrondi_Prior, 12, RGB(250,0,0), ptrsc, 10060+10000*ThisStudyID) ; }            // void DrawMarker_UTAMAOA (int marqueur, int IndexPremiereTouche, float prix, int tailleObjet, COLORREF couleur, s_sc* ptrsc, int Line_Number)  
						else							{ DrawMarker_UTAMAOA (MARKER_TRIANGLERIGHT, IndexPremiereTouche, NiveauFutureAjuste+Position*TPOptimumArrondi_Prior, 12, RGB(0,200,0), ptrsc, 10060+10000*ThisStudyID) ; }

					}
					
					// Affichages des mvts verticaux            
					{
						// Position du tracé de Texte_x128 versus affichages de la 3ème colonne
						float NiveauTextesMvtsVerticaux ;
						if ( (NiveauFutureAjuste-NbInterligneMax_3emeColonne*Position*Interligne)*Position > (BaseDuTrace+Position*1.0*Interligne)*Position )		// cas habituel (faibles amplitudes des échelles verticales)
						{ NiveauTextesMvtsVerticaux = BaseDuTrace-Position*1.4*Interligne ;  }    
						else		// forte amplitude de l'échelle verticale
						{ NiveauTextesMvtsVerticaux = NiveauFutureAjuste-(NbInterligneMax_3emeColonne+2.5)*Position*Interligne ;  }
						
						// 128uple :
						DrawText_UTAMAOA (Texte_x128, IndexPremiereTouche-128*NbBarresVirginite_Prior, NiveauTextesMvtsVerticaux, 9, RGB(255,128,0), ptrsc, 10062+10000*ThisStudyID) ;		// void DrawText_UTAMAOA (SCString texte, int index, float prix, int tailleTexte, COLORREF couleur, s_sc* ptrsc, int Line_Number)  	
						// 64uple :
						DrawText_UTAMAOA (Texte_x64, IndexPremiereTouche-64*NbBarresVirginite_Prior, NiveauTextesMvtsVerticaux, 9, RGB(255,128,0), ptrsc, 10063+10000*ThisStudyID) ; 
						// 32uple :
						DrawText_UTAMAOA (Texte_x32, IndexPremiereTouche-32*NbBarresVirginite_Prior, NiveauTextesMvtsVerticaux, 9, RGB(255,128,0), ptrsc, 10064+10000*ThisStudyID) ;
						// 16uple :
						DrawText_UTAMAOA (Texte_x16, IndexPremiereTouche-16*NbBarresVirginite_Prior, NiveauTextesMvtsVerticaux, 9, RGB(255,128,0), ptrsc, 10065+10000*ThisStudyID) ;
						// octuple :
						DrawText_UTAMAOA (Texte_x8, IndexPremiereTouche-8*NbBarresVirginite_Prior, NiveauTextesMvtsVerticaux, 9, RGB(255,128,0), ptrsc, 10066+10000*ThisStudyID) ;  
						// quadruple :
						DrawText_UTAMAOA (Texte_x4, IndexPremiereTouche-4*NbBarresVirginite_Prior, NiveauTextesMvtsVerticaux, 9, RGB(255,128,0), ptrsc, 10067+10000*ThisStudyID) ;  	
						// double :
						DrawText_UTAMAOA (Texte_x2, IndexPremiereTouche-2*NbBarresVirginite_Prior, NiveauTextesMvtsVerticaux, 9, RGB(255,128,0), ptrsc, 10068+10000*ThisStudyID) ;  	
						// full :
						DrawText_UTAMAOA (Texte_x1, IndexPremiereTouche-1*NbBarresVirginite_Prior, NiveauTextesMvtsVerticaux, 9, RGB(255,128,0), ptrsc, 10069+10000*ThisStudyID) ;  	
						// moitié :
						DrawText_UTAMAOA (Texte_x05, IndexPremiereTouche-0.5*NbBarresVirginite_Prior, NiveauTextesMvtsVerticaux, 9, RGB(255,128,0), ptrsc, 10070+10000*ThisStudyID) ;  	
						// quart :
						DrawText_UTAMAOA (Texte_x025,  IndexPremiereTouche-0.25*NbBarresVirginite_Prior, NiveauTextesMvtsVerticaux, 9, RGB(255,128,0), ptrsc, 10071+10000*ThisStudyID) ;
						// récapitulatifs x128, x64, x32, x16, x8, x4, x2, x1, x0.5, x0.25 sous entrée :
						DrawTextAligne_UTAMAOA (Texte_Recapitulatif, IndexPremiereTouche, NiveauTextesMvtsVerticaux-1.6*Position*Interligne, 9, RGB(255,128,0), ptrsc, 10072+10000*ThisStudyID) ;
						
						// Affichage des Triangles : 
						if ( Position == 1 ) 
						{
							DrawMarker_UTAMAOA (MARKER_TRIANGLEDOWN, IndexPremiereTouche-0.25*NbBarresVirginite_Prior, BaseDuTrace, 8, RGB(255,128,0), ptrsc, 10073+10000*ThisStudyID) ;
							DrawMarker_UTAMAOA (MARKER_TRIANGLEDOWN, IndexPremiereTouche-0.5*NbBarresVirginite_Prior, BaseDuTrace, 8, RGB(255,128,0), ptrsc, 10074+10000*ThisStudyID) ;
							DrawMarker_UTAMAOA (MARKER_TRIANGLEDOWN, IndexPremiereTouche-1*NbBarresVirginite_Prior, BaseDuTrace, 8, RGB(255,128,0), ptrsc, 10075+10000*ThisStudyID) ;
							DrawMarker_UTAMAOA (MARKER_TRIANGLEDOWN, IndexPremiereTouche-2*NbBarresVirginite_Prior, BaseDuTrace, 8, RGB(255,128,0), ptrsc, 10076+10000*ThisStudyID) ;
							DrawMarker_UTAMAOA (MARKER_TRIANGLEDOWN, IndexPremiereTouche-4*NbBarresVirginite_Prior, BaseDuTrace, 8, RGB(255,128,0), ptrsc, 10077+10000*ThisStudyID) ;
							DrawMarker_UTAMAOA (MARKER_TRIANGLEDOWN, IndexPremiereTouche-8*NbBarresVirginite_Prior, BaseDuTrace, 8, RGB(255,128,0), ptrsc, 10078+10000*ThisStudyID) ;
							/* if ( fabs(MouvementVertical16upleNorme) < 100 ) */ 	DrawMarker_UTAMAOA (MARKER_TRIANGLEDOWN, IndexPremiereTouche-16*NbBarresVirginite_Prior, BaseDuTrace, 8, RGB(255,128,0), ptrsc, 10079+10000*ThisStudyID) ;
							/* if ( fabs(MouvementVertical32upleNorme) < 100 ) */ 	DrawMarker_UTAMAOA (MARKER_TRIANGLEDOWN, IndexPremiereTouche-32*NbBarresVirginite_Prior, BaseDuTrace, 8, RGB(255,128,0), ptrsc, 10080+10000*ThisStudyID) ;
							/* if ( fabs(MouvementVertical64upleNorme) < 100 ) */ 	DrawMarker_UTAMAOA (MARKER_TRIANGLEDOWN, IndexPremiereTouche-64*NbBarresVirginite_Prior, BaseDuTrace, 8, RGB(255,128,0), ptrsc, 10081+10000*ThisStudyID) ;
							/* if ( fabs(MouvementVertical128upleNorme) < 100 ) */ 	DrawMarker_UTAMAOA (MARKER_TRIANGLEDOWN, IndexPremiereTouche-128*NbBarresVirginite_Prior, BaseDuTrace, 8, RGB(255,128,0), ptrsc, 10082+10000*ThisStudyID) ;					
						}
						else if ( Position == -1 ) 
						{
							DrawMarker_UTAMAOA (MARKER_TRIANGLEUP, IndexPremiereTouche-0.25*NbBarresVirginite_Prior, BaseDuTrace, 8, RGB(255,128,0), ptrsc, 10073+10000*ThisStudyID) ;
							DrawMarker_UTAMAOA (MARKER_TRIANGLEUP, IndexPremiereTouche-0.5*NbBarresVirginite_Prior, BaseDuTrace, 8, RGB(255,128,0), ptrsc, 10074+10000*ThisStudyID) ;
							DrawMarker_UTAMAOA (MARKER_TRIANGLEUP, IndexPremiereTouche-1*NbBarresVirginite_Prior, BaseDuTrace, 8, RGB(255,128,0), ptrsc, 10075+10000*ThisStudyID) ;
							DrawMarker_UTAMAOA (MARKER_TRIANGLEUP, IndexPremiereTouche-2*NbBarresVirginite_Prior, BaseDuTrace, 8, RGB(255,128,0), ptrsc, 10076+10000*ThisStudyID) ;
							DrawMarker_UTAMAOA (MARKER_TRIANGLEUP, IndexPremiereTouche-4*NbBarresVirginite_Prior, BaseDuTrace, 8, RGB(255,128,0), ptrsc, 10077+10000*ThisStudyID) ;
							DrawMarker_UTAMAOA (MARKER_TRIANGLEUP, IndexPremiereTouche-8*NbBarresVirginite_Prior, BaseDuTrace, 8, RGB(255,128,0), ptrsc, 10078+10000*ThisStudyID) ;
							/* if ( fabs(MouvementVertical16upleNorme) < 100 ) */ 	DrawMarker_UTAMAOA (MARKER_TRIANGLEUP, IndexPremiereTouche-16*NbBarresVirginite_Prior, BaseDuTrace, 8, RGB(255,128,0), ptrsc, 10079+10000*ThisStudyID) ;
							/* if ( fabs(MouvementVertical32upleNorme) < 100 ) */ 	DrawMarker_UTAMAOA (MARKER_TRIANGLEUP, IndexPremiereTouche-32*NbBarresVirginite_Prior, BaseDuTrace, 8, RGB(255,128,0), ptrsc, 10080+10000*ThisStudyID) ;
							/* if ( fabs(MouvementVertical64upleNorme) < 100 ) */ 	DrawMarker_UTAMAOA (MARKER_TRIANGLEUP, IndexPremiereTouche-64*NbBarresVirginite_Prior, BaseDuTrace, 8, RGB(255,128,0), ptrsc, 10081+10000*ThisStudyID) ;
							/* if ( fabs(MouvementVertical128upleNorme) < 100 ) */ 	DrawMarker_UTAMAOA (MARKER_TRIANGLEUP, IndexPremiereTouche-128*NbBarresVirginite_Prior, BaseDuTrace, 8, RGB(255,128,0), ptrsc, 10082+10000*ThisStudyID) ;
						}
					}
					
					// Tracé sur le graphe de Sigma d'un trait vertical entre IndexPremiereTouche-1 et IndexPremiereTouche 
					{
						Rectangle2 objetTraitVertical(IndexPremiereTouche-1, IndexPremiereTouche+1, -1000, 1000, RGB (180,180,180), 0, ptrsc);
						sc.UseTool(objetTraitVertical); 
					}
					
					// Valeur du SL (si dépassé) sur le graphe
					if ( StopLossIndiceDepasse  and  ! RobotActif )
					{
						// DrawText_UTAMAOA (Texte_SL, IndexArretManualLooping,  PrixSL-Position*2, 9, CouleurDrawText, ptrsc, 10124+10000*ThisStudyID) ;			// void DrawText_UTAMAOA (SCString texte, int index, float prix, int tailleTexte, COLORREF couleur, s_sc* ptrsc, int Line_Number)  	
						Texte_UTAMAOA objetTexte(Texte_SL, IndexArretManualLooping, PrixSL-Position*2, 9, CouleurDrawText, 1, ptrsc, 10124+10000*ThisStudyID, DT_VCENTER | DT_RIGHT) ;			// Texte_UTAMAOA (SCString texte, int index, float prix, int tailleTexte,  COLORREF couleur, int graphRegion, s_sc* ptrsc, int Line_Number, int Alignement)
						sc.UseTool(objetTexte); 
					}
				}
				
				// Tracé vectoriel des sc.Subgraph remplis : StopLossProjete-NiveauFutureAjuste-Niveau1TP-Niveau2TP-Niveau3TP-Niveau4TP      // Le tracé débute à "index = IndexWarpJump+1" et s'achève à "index = IndexFinTrace"
				{
					COLORREF GrisFonce = RGB (180,180,180) ;
					
					// Détermination de IndexFinTrace          !!  simplifier ce bloc en recyclant IndexArretManualLooping
					int IndexFinTrace = sc.ArraySize-1 ; // initialisation par défaut
					for ( int index = IndexWarpJump+1 ; index < sc.ArraySize ; index ++ )
					{
						if ( StopLossProjete[index] == 0 )
						{
							IndexFinTrace = index ;
							break ;
						}
					}
					
					// Tracé vectoriel entre NiveauFutureAjuste et StopLossProjete
					{
						int beginIndex = IndexWarpJump ;
						float StopLossProjete_IndexMoinsUn = StopLossProjete[beginIndex] ;
						float StopLossProjete_Index ;
						for ( int index = IndexWarpJump+1 ; index < IndexFinTrace+1 ; index++ )
						{
							StopLossProjete_Index = StopLossProjete[index] ; 
							// on teste si on fait un nouveau rectangle. Si oui, on trace un rectangle entre beginIndex et index-1. Etc jusqu'au prochain changement de valeur de StopLossProjete
							if ( StopLossProjete_Index != StopLossProjete_IndexMoinsUn)
							{
								creationRectangle (beginIndex, index, StopLossProjete_IndexMoinsUn, NiveauFutureAjuste, GrisFonce, ptrsc)    ;      
								beginIndex = index ;
								StopLossProjete_IndexMoinsUn = StopLossProjete_Index ;
							}
						}
						//Dernier rectangle
						creationRectangle (beginIndex, IndexFinTrace, StopLossProjete_Index, NiveauFutureAjuste, GrisFonce, ptrsc)    ;    
					}
					
					
					// Tracé vectoriel entre Niveau1TP et Niveau2TP, et entre Niveau3TP et Niveau4TP
					{
						int NbRectangles = 0 ; 
						int beginIndex = IndexWarpJump ;       
						float Niveau4TP_IndexMoinsUn = Niveau4TP[beginIndex] ;
						float Niveau4TP_Index ;
						float Niveau4TP_Previous ;
						for ( int index = IndexWarpJump+1 ; index < IndexFinTrace+1 ; index++ )
						{
							Niveau4TP_Index = Niveau4TP[index] ; 
							// on teste si on fait un nouveau rectangle, sur la base de la variation de Niveau4TP. Si oui, on trace deux rectangles entre beginIndex et index-1 (un rectangle entre Niveau1TP et Niveau2TP, et un rectangle entre Niveau3TP et Niveau4TP)
							if ( fabs( Niveau4TP_Index - Niveau4TP_Previous ) > delta )  // Niveau4TP a varié de plus de delta pts depuis la précédente variation de delta pts
							{
								Niveau4TP_Previous = Niveau4TP_Index ;
								creationRectangle (beginIndex, index, Niveau3TP[beginIndex], Niveau4TP_IndexMoinsUn, GrisFonce, ptrsc) ;      NbRectangles ++ ;
								creationRectangle (beginIndex, index, Niveau1TP[index-1], Niveau2TP[index-1], GrisFonce, ptrsc) ;      
								beginIndex = index ;
								Niveau4TP_IndexMoinsUn = Niveau4TP_Index ;
							}
						}
						//Dernier rectangle
						creationRectangle (beginIndex, IndexFinTrace, Niveau3TP[IndexFinTrace], Niveau4TP_Index, GrisFonce, ptrsc) ;    
						creationRectangle (beginIndex, IndexFinTrace, Niveau1TP[IndexFinTrace], Niveau2TP[IndexFinTrace], GrisFonce, ptrsc) ;

						// Débuggage du clignotement des rectangles
						if ( NbRectangles == 0  and  IndexArretManualLooping != -1)
						{SCString message ;  message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | Niveau4TP[beginIndex] = %.2f | Niveau4TP_Previous = %.2f | fabs( Niveau4TP_Index - Niveau4TP_Previous ) = %.2f", Niveau4TP[beginIndex], Niveau4TP_Previous, fabs( Niveau4TP_Index - Niveau4TP_Previous )) ; sc.AddMessageToLog(message,0) ;  }

						
						// delta est la variation minimum de Niveau4TP_Previous pour lancer la création de nouveaux rectangles.
						if ( NbRectangles > 90 )  { delta = delta + 0.01 ;  }
						else if ( NbRectangles < 60 )  { delta = std::max(0.01, delta - 0.01) ;  }
					}
					
				}
				
			}
			
		}
		else // effacement des UDD
		{
			// chart #13 (chart 1-tick) 
			sc.DeleteUserDrawnACSDrawing(13, 10127+10000*ThisStudyID) ;  
			sc.DeleteUserDrawnACSDrawing(13, 10132+10000*ThisStudyID) ;  
			sc.DeleteUserDrawnACSDrawing(15, 10138+10000*ThisStudyID) ;  
			sc.DeleteUserDrawnACSDrawing(19, 10138+10000*ThisStudyID) ;  
			sc.DeleteUserDrawnACSDrawing(15, 10139+10000*ThisStudyID) ;  
			sc.DeleteUserDrawnACSDrawing(19, 10140+10000*ThisStudyID) ;  
		}
		
		QueryPerformanceCounter(&ElapsedTicks) ;  
		Clicks_BlocAffichage = ( ElapsedTicks.QuadPart - ElapsedTicks_BlocAffichage.QuadPart ) *1000.0 / Frequency.QuadPart ;     // calculé en millisecondes
	} // fin du bloc AFFICHAGE TEMPS REEL DES PARAMETRES IMPORTANTS
	
	
	// Last call to function ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (sc.LastCallToFunction) 
	{
		// effacement des s_UseTool::AddAsUserDrawnDrawing
		EffacementDesUDD(ptrsc) ;    // effacement des UDD sur le graphe 16 ticks
		sc.DeleteUserDrawnACSDrawing(sc.ChartNumber, 10014+10000*ThisStudyID) ;  	sc.DeleteUserDrawnACSDrawing(sc.ChartNumber, 10015+10000*ThisStudyID) ;  // trait fin sur le niveau tradé + trait épais sur le prix demandé
		EffacementDesUDD_GraphesExternes(ptrsc);
		
		// Effacement des DrawText "quantieme" sur les graphe 15 et 19
		if (  CetteInstanceControleLesBoutons.GetYesNo() == 1  ) 
		{
			AffichageONOFF = 0 ;
			Set_StudyQuantiemeMvt(ptrsc) ;
		}
		
		// RAZ des boutons gérés par la study active 
		if (  CetteInstanceControleLesBoutons.GetYesNo() == 1  ) 
		{
			// sc.SetCustomStudyControlBarButtonText(60, "No active study") ;   => inutile car le bouton est géré par le DLL "Prix typique"
			sc.SetCustomStudyControlBarButtonText(2, "      ") ; 
			sc.SetCustomStudyControlBarButtonText(17, "      ") ; 
			sc.SetCustomStudyControlBarButtonText(49, "      ") ; 
			sc.SetCustomStudyControlBarButtonText(52, "      ") ; 
			sc.SetCustomStudyControlBarButtonText(56, "      ") ; 
			sc.SetCustomStudyControlBarButtonText(61, "      ") ; 
		}

		// Transmission aux studies "Padding et boutons ACS" du basculement sur SCALE_AUTO
		// if (  CetteInstanceControleLesBoutons.GetYesNo() == 1  ) 
		{
			sc.SetChartStudyInputInt(15, StudyID_Padding_Chart15, 0, 2) ; 	// int SetChartStudyInputInt(int ChartNumber, int StudyID, int InputIndex, int IntegerValue);
			sc.SetChartStudyInputInt(19, StudyID_Padding_Chart19, 0, 2) ; 			
		}

		// mettre un popup si on efface par erreur la study alors qu'un robot a des ordres en attente => OK, c'est fait dans le bloc du robot
	}

	
	// Les changements de AutoTradeStatus sont reportés dans le log   /////////////////////////////////////////////////////////////////////////////
	if ( AutoTradeStatus != AutoTradeStatus_Prior )                                              
	{	
		// première partie du message : ID | Niveau Fut/Ind
		SCString message ;
		message.Format("ID:%d | ", ThisStudyID) ;
		message.AppendFormat( NomDuNiveauTrade.GetString() ) ;
		message.AppendFormat(" | ") ; 
		
		
		// deuxième partie du message
		SCString StringAjoute ;
		if        ( AutoTradeStatus_Prior ==       0 )  { StringAjoute.Format("AutoTradeStatus : 0 ''OFF (ready)'' vers ") ;  }
		else if ( AutoTradeStatus_Prior ==     10 )  { StringAjoute.Format("AutoTradeStatus : 10 ''demarre'' vers ") ;  }
		else if ( AutoTradeStatus_Prior ==     11 )  { StringAjoute.Format("AutoTradeStatus : 11 ''demarre (aucun ordre soumis)'' vers ") ;  }
		else if ( AutoTradeStatus_Prior ==     12 )  { StringAjoute.Format("AutoTradeStatus : 12 ''pb Auto Trading Enabled est OFF'' vers ") ;  }
		else if ( AutoTradeStatus_Prior ==     15 )  { StringAjoute.Format("AutoTradeStatus : 15 ''pb soumission premier ordre'' vers ") ;  }
		else if ( AutoTradeStatus_Prior ==     20 )  { StringAjoute.Format("AutoTradeStatus : 20 ''ordre triple ouvert'' vers ") ;  }
		else if ( AutoTradeStatus_Prior ==     30 )  { StringAjoute.Format("AutoTradeStatus : 30 ''position ouverte'' vers ") ;  }
		else if ( AutoTradeStatus_Prior ==     50 )  { StringAjoute.Format("AutoTradeStatus : 50 ''OFF (ordres et position fermes)'' vers ") ;  }
		else if ( AutoTradeStatus_Prior ==     60 )  { StringAjoute.Format("AutoTradeStatus : 60 ''annulation ordres demandee'' vers ") ;  }
		else if ( AutoTradeStatus_Prior ==     70 )  { StringAjoute.Format("AutoTradeStatus : 70 ''pb annulation ordres'' vers ") ;  }
		else if ( AutoTradeStatus_Prior ==   100 )  { StringAjoute.Format("AutoTradeStatus : 100 ''OFF car SommeRatioAvecMalus sature'' vers ") ;  }
		else if ( AutoTradeStatus_Prior ==   110 )  { StringAjoute.Format("AutoTradeStatus : 110 ''OFF car prix depasse'' vers ") ;  }
		else if ( AutoTradeStatus_Prior ==   120 )  { StringAjoute.Format("AutoTradeStatus : 120 ''OFF car TP insuffisant'' vers ") ;  }
		else if ( AutoTradeStatus_Prior ==   130 )  { StringAjoute.Format("AutoTradeStatus : 130 ''OFF car BonusExcedentaire insuffisant'' vers ") ;  }
		else if ( AutoTradeStatus_Prior ==   140 )  { StringAjoute.Format("AutoTradeStatus : 140 ''OFF car HoraireExpiration est depasse'' vers ") ;  }
		else if ( AutoTradeStatus_Prior ==   150 )  { StringAjoute.Format("AutoTradeStatus : 150 ''OFF car il est 15h45'' vers ") ;  }
		else if ( AutoTradeStatus_Prior ==   160 )  { StringAjoute.Format("AutoTradeStatus : 160 ''OFF car SL depasse PH/PB'' vers ") ;  }
		else if ( AutoTradeStatus_Prior ==   170 )  { StringAjoute.Format("AutoTradeStatus : 170 ''OFF car BonusMoy insuffisant'' vers ") ;  }
		else if ( AutoTradeStatus_Prior ==   180 )  { StringAjoute.Format("AutoTradeStatus : 180 ''OFF car Sequence terminee'' vers ") ;  }
		else if ( AutoTradeStatus_Prior ==   190 )  { StringAjoute.Format("AutoTradeStatus : 190 ''OFF car DistMiniFuture <= 0'' vers ") ;  }
		else if ( AutoTradeStatus_Prior ==   200 )  { StringAjoute.Format("AutoTradeStatus : 200 ''OFF car 5 min avant News'' vers ") ;  } 
		else if ( AutoTradeStatus_Prior == 1000 )  { StringAjoute.Format("AutoTradeStatus : 1000 ''FlattenAndCancelAllOrders'' vers ") ;  }
		else if ( AutoTradeStatus_Prior == 1010 )  { StringAjoute.Format("AutoTradeStatus : 1010 ''pb avec FlattenAndCancelAllOrders'' vers ") ;  }
		else   															{ StringAjoute.Format("AutoTradeStatus : %d ''Statut non documente'' vers ", AutoTradeStatus_Prior) ;  }
		message.Append(StringAjoute) ;
		
		
		// troisième partie du message
		if        ( AutoTradeStatus ==       0 )  { StringAjoute.Format("0 ''OFF (ready)''") ;  }
		else if ( AutoTradeStatus ==     10 )  { StringAjoute.Format("10 ''demarre''") ;  }
		else if ( AutoTradeStatus ==     11 )  { StringAjoute.Format("11 ''demarre (aucun ordre soumis)''") ;  }
		else if ( AutoTradeStatus ==     12 )  { StringAjoute.Format("12 ''pb Auto Trading Enabled est OFF''") ;  }
		else if ( AutoTradeStatus ==     15 )  { StringAjoute.Format("15 ''pb soumission premier ordre''") ;  }
		else if ( AutoTradeStatus ==     20 )  { StringAjoute.Format("20 ''ordre triple ouvert''") ;  }
		else if ( AutoTradeStatus ==     30 )  { StringAjoute.Format("30 ''position ouverte'' | BonusIntr = %.0f%% | TP = %.2f", BonusIntrinseque, TP_de_la_precedente_modification_de_TP) ;  }
		else if ( AutoTradeStatus ==     50 )  { StringAjoute.Format("50 ''OFF (ordres et position fermes)''") ;  }
		else if ( AutoTradeStatus ==     60 )  { StringAjoute.Format("60 ''annulation ordres demandee''") ;  }
		else if ( AutoTradeStatus ==     70 )  { StringAjoute.Format("70 ''pb annulation ordres''") ;  }
		else if ( AutoTradeStatus ==   100 )  { StringAjoute.Format("100 ''OFF car SommeRatio&Co saturent''") ;  }
		else if ( AutoTradeStatus ==   110 )  { StringAjoute.Format("110 ''OFF car prix depasse''") ;  }
		else if ( AutoTradeStatus ==   120 )  { StringAjoute.Format("120 ''OFF car TP insuffisant''") ;  }
		else if ( AutoTradeStatus ==   130 )  { StringAjoute.Format("130 ''OFF car BonusExcedentaire insuffisant''") ;  }
		else if ( AutoTradeStatus ==   140 )  { StringAjoute.Format("140 ''OFF car HoraireExpiration est depasse''") ;  }
		else if ( AutoTradeStatus ==   150 )  { StringAjoute.Format("150 ''OFF car il est 15h45''") ;  }
		else if ( AutoTradeStatus ==   160 )  { StringAjoute.Format("160 ''OFF car SL depasse PH/PB''") ;  }
		else if ( AutoTradeStatus ==   170 )  { StringAjoute.Format("170 ''OFF car BonusMoy insuffisant''") ;  }
		else if ( AutoTradeStatus ==   180 )  { StringAjoute.Format("180 ''OFF car Sequence terminee''") ;  }
		else if ( AutoTradeStatus ==   190 )  { StringAjoute.Format("190 ''OFF car DistMiniFuture <= 0''") ;  }
		else if ( AutoTradeStatus ==   200 )  { StringAjoute.Format("200 ''OFF car 5 min avant News''") ;  }
		else if ( AutoTradeStatus == 1000 )  { StringAjoute.Format("1000 ''FlattenAndCancelAllOrders''") ;  }
		else if ( AutoTradeStatus == 1010 )  { StringAjoute.Format("1010 ''pb avec FlattenAndCancelAllOrders''") ;  }
		else													  { StringAjoute.Format("%d ''Statut non documente''", AutoTradeStatus) ;  }
		message.Append(StringAjoute) ;
		message.AppendFormat(" | ") ; 
		
		
		// quatrième partie du message
				if ( PositionOuverte == -2 ) 	{ StringAjoute.Format("PositionOuverte = -2 ''Robot OFF'' ;  ") ;  }
		else if ( PositionOuverte == -1 )  { StringAjoute.Format("PositionOuverte = -1 ''Non determine'' ;  ") ;  }
		else if ( PositionOuverte ==  0 )  { StringAjoute.Format("PositionOuverte = 0 ''Pas de position ouverte'' ;  ") ;  }
		else if ( PositionOuverte ==  1 )  { StringAjoute.Format("PositionOuverte = 1 ''Position ouverte'' ;  ") ;  }
		else if ( PositionOuverte ==  2 )  { StringAjoute.Format("PositionOuverte = 2 ''Position refermee'' ;  ") ;  }
		else 											{ StringAjoute.Format("PositionOuverte = %d ''bug!'' ;  ", PositionOuverte) ;  }
		message.Append(StringAjoute) ;
		
		
		// cinquième partie du message
				if ( OrdreOuvert == -2 )  { StringAjoute.Format("OrdreOuvert = -2 ''Robot OFF''") ;  }
		else	if ( OrdreOuvert == -1 )  { StringAjoute.Format("OrdreOuvert = -1 ''Non determine''") ;  }
		else if ( OrdreOuvert ==  0 )  { StringAjoute.Format("OrdreOuvert = 0 ''Pas d'ordre ouvert''") ;  }
		else if ( OrdreOuvert ==  1 )  { StringAjoute.Format("OrdreOuvert = 1 ''Ordre ouvert''") ;  }
		// else if ( OrdreOuvert ==  2 )  { StringAjoute.Format("OrdreOuvert = 2 ''Ordre referme''") ;  }
		else 									  { StringAjoute.Format("OrdreOuvert = %d ''bug!'' ;  ", OrdreOuvert) ;  }
		message.Append(StringAjoute) ;
		
		
		// sixième partie du message
		if ( SequenceONOFF_Input.GetIndex() == 1 )  { StringAjoute.Format(" | Sequence est OFF ") ;  }
		else { StringAjoute.Format(" | SequenceStatus = %d ", SequenceStatus ) ;  }
		message.Append(StringAjoute) ;
		
		
		// septième partie du message (à l'ouverture de position)
		// if ( AutoTradeStatus == 30 )  { StringAjoute.Format("30 ''position ouverte''") ;  }

		
		// écriture dans le log
		sc.AddMessageToLog(message,0) ;

		// PlaySound si le robot devient inactif après une période d'activité
		if  ( (  AutoTradeEstArreteManuellement == 0  and  SequenceStatus == 0  )  and   ( AutoTradeStatus == 50  or  AutoTradeStatus == 100  or  AutoTradeStatus == 120  or  AutoTradeStatus == 130  or  AutoTradeStatus == 140  or  AutoTradeStatus == 150 or  AutoTradeStatus == 160  or  AutoTradeStatus == 170  or  AutoTradeStatus == 180  or  AutoTradeStatus == 190  or  AutoTradeStatus == 200  ) ) 
		{	
			// SCString AlertPathAndFileName = "C:\\SierraChart\\AlertSounds\\blong2.wav" ;  sc.PlaySound(AlertPathAndFileName,1) ;    // ne fonctionne pas sur le VPS
			SCString AlertPathAndFileName = "C:\\SierraChart\\AlertSounds\\AlertSound2.wav" ;  sc.PlaySound(AlertPathAndFileName,1) ;
			SCString message ;  message.Format("ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | PlaySound") ; sc.AddMessageToLog(message,0) ;  
		}
	}	
	
	
	// Variables persistantes appelées lors du prochain appel du DLL
	{
		AutoTradeStatus_Prior = AutoTradeStatus ;  
		LastIndexPrecedent = LastIndex ;
	}
	
	
	// lorsque je suis déconnecté du serveur, pour les backtests :
	if ( sc.ServerConnectionState != SCS_CONNECTED )  sc.UpdateAlways = 1 ; 
	
	
	// Status Report (CTRL+S)
	if (sc.MenuEventID == ACS_BUTTON_3  and CetteInstanceControleLesBoutons.GetYesNo() )      
	{
		SCString message ;
		
		// Première ligne
		message.Format("SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS | STATUS REPORT | ID:%d | ", ThisStudyID) ;  message.AppendFormat( NomDuNiveauTrade.GetString() ) ;  message.AppendFormat(" | SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS") ;  sc.AddMessageToLog(message,0) ;

		// ParentOrderID
		message.Format("ParentOrderID = %d                    ", ParentOrderID) ;  sc.AddMessageToLog(message,0) ;
		
		// AutoTradeStatus
		if        ( AutoTradeStatus ==       0 )  { message.Format("AutoTradeStatus = 0 ''OFF (ready)''                    ") ;  }
		else if ( AutoTradeStatus ==     10 )  { message.Format("AutoTradeStatus = 10 ''demarre''                    ") ;  }
		else if ( AutoTradeStatus ==     11 )  { message.Format("AutoTradeStatus = 11 ''demarre (aucun ordre soumis)''                    ") ;  }
		else if ( AutoTradeStatus ==     12 )  { message.Format("AutoTradeStatus = 12 ''pb Auto Trading Enabled est OFF''                    ") ;  }
		else if ( AutoTradeStatus ==     15 )  { message.Format("AutoTradeStatus = 15 ''pb soumission premier ordre''                    ") ;  }
		else if ( AutoTradeStatus ==     20 )  { message.Format("AutoTradeStatus = 20 ''ordre triple ouvert''                    ") ;  }
		else if ( AutoTradeStatus ==     30 )  { message.Format("AutoTradeStatus = 30 ''position ouverte'' | BonusIntr = %.0f%% | TP = %.2f                    ", BonusIntrinseque, TP_de_la_precedente_modification_de_TP) ;  }
		else if ( AutoTradeStatus ==     50 )  { message.Format("AutoTradeStatus = 50 ''OFF (ordres et position fermes)''                    ") ;  }
		else if ( AutoTradeStatus ==     60 )  { message.Format("AutoTradeStatus = 60 ''annulation ordres demandee''                    ") ;  }
		else if ( AutoTradeStatus ==     70 )  { message.Format("AutoTradeStatus = 70 ''pb annulation ordres''                    ") ;  }
		else if ( AutoTradeStatus ==   100 )  { message.Format("AutoTradeStatus = 100 ''OFF car SommeRatio&Co saturent''                    ") ;  }
		else if ( AutoTradeStatus ==   110 )  { message.Format("AutoTradeStatus = 110 ''OFF car prix depasse''                    ") ;  }
		else if ( AutoTradeStatus ==   120 )  { message.Format("AutoTradeStatus = 120 ''OFF car TP insuffisant''                    ") ;  }
		else if ( AutoTradeStatus ==   130 )  { message.Format("AutoTradeStatus = 130 ''OFF car BonusExcedentaire insuffisant''                    ") ;  }
		else if ( AutoTradeStatus ==   140 )  { message.Format("AutoTradeStatus = 140 ''OFF car HoraireExpiration est depasse''                    ") ;  }
		else if ( AutoTradeStatus ==   150 )  { message.Format("AutoTradeStatus = 150 ''OFF car il est 15h45''                    ") ;  }
		else if ( AutoTradeStatus ==   160 )  { message.Format("AutoTradeStatus = 160 ''OFF car SL depasse PH/PB''                    ") ;  }
		else if ( AutoTradeStatus ==   170 )  { message.Format("AutoTradeStatus = 170 ''OFF car BonusMoy insuffisant''                    ") ;  }
		else if ( AutoTradeStatus ==   180 )  { message.Format("AutoTradeStatus = 180 ''OFF car Sequence terminee''                    ") ;  }
		else if ( AutoTradeStatus ==   190 )  { message.Format("AutoTradeStatus = 190 ''OFF car DistMiniFuture <= 0''                    ") ;  }
		else if ( AutoTradeStatus ==   200 )  { message.Format("AutoTradeStatus = 200 ''OFF car 5 min avant News''                    ") ;  }
		else if ( AutoTradeStatus == 1000 )  { message.Format("AutoTradeStatus = 1000 ''FlattenAndCancelAllOrders''                    ") ;  }
		else if ( AutoTradeStatus == 1010 )  { message.Format("AutoTradeStatus = 1010 ''pb avec FlattenAndCancelAllOrders''                    ") ;  }
		else													  { message.Format("AutoTradeStatus = %d ''Statut non documente''                    ", AutoTradeStatus) ;  }
		sc.AddMessageToLog(message,0) ;
		
		// RobotActif
		message.Format("RobotActif = %d (0 = OFF ; 1 = ON)                    ", RobotActif) ;  sc.AddMessageToLog(message,0) ;
		
		// OrderStatusCode
		message.Format("PARENT Order StatusCode = %d, TARGET Order StatusCode = %d, STOP Order StatusCode = %d                    ", ParentOrderDetails_OrderStatusCode, Target1OrderDetails_OrderStatusCode, Stop1OrderDetails_OrderStatusCode) ;  sc.AddMessageToLog(message,0) ;
		
		// Aide-mémoire : liste des OrderStatusCode
		message.Format("SCT_OSC = -3. Lorsque le robot est inactif OSC n'est pas determine.                    ") ;  sc.AddMessageToLog(message,0) ;
		message.Format("SCT_OSC_UNSPECIFIED = 0. OrderStatusCode is undefined. You will never get this. It is internally used.                    ") ;  sc.AddMessageToLog(message,0) ;
		message.Format("SCT_OSC_ORDERSENT = 1") ;  sc.AddMessageToLog(message,0) ;
		message.Format("SCT_OSC_PENDINGOPEN = 2. This is only used for external trading services, and it should be used when the service indicates the order is pending and it is clear that the order cannot be modified or it is unclear if the order can be modified in the pending state. Sierra Chart will queue and delay an order modification if the state is Pending Open. This order status must not be used in the case of child orders or when an order like a Market If Touched order is being held until it is triggered.                    ") ;  sc.AddMessageToLog(message,0) ;
		message.Format("SCT_OSC_PENDING_CHILD_CLIENT = 3. Indicates a pending child order that resides on the client-side within Sierra Chart.                    ") ;  sc.AddMessageToLog(message,0) ;
		message.Format("SCT_OSC_PENDING_CHILD_SERVER = 4. Indicates a pending child order that resides on the server.                    ") ;  sc.AddMessageToLog(message,0) ;
		message.Format("SCT_OSC_OPEN = 5                    ") ;  sc.AddMessageToLog(message,0) ;
		message.Format("SCT_OSC_PENDINGMODIFY = 6                    ") ;  sc.AddMessageToLog(message,0) ;
		message.Format("SCT_OSC_PENDINGCANCEL = 7                    ") ;  sc.AddMessageToLog(message,0) ;
		message.Format("SCT_OSC_FILLED = 8. Order has been completely filled or partially filled, and is no longer considered working.                    ") ;  sc.AddMessageToLog(message,0) ;
		message.Format("SCT_OSC_CANCELED = 9. Order has been canceled. The order could have been partially filled before it was canceled.                    ") ;  sc.AddMessageToLog(message,0) ;
		message.Format("SCT_OSC_ERROR = 10. Error with order.                    ") ;  sc.AddMessageToLog(message,0) ;
		message.Format("SCT_OSC_PENDING_CANCEL_FOR_REPLACE = 11. To be used with trading services that do not support modifying orders.                    ") ;  sc.AddMessageToLog(message,0) ;
		message.Format("SCT_OSC_HELD = 12. This is used internally by Sierra Chart to indicate an order is being held before it sent to the trading service.                    ") ;  sc.AddMessageToLog(message,0) ;
		
		// OrdreOuvert
				if ( OrdreOuvert == -2 )  	{ message.Format("OrdreOuvert = -2 ''n.d. car Robot OFF''") ;  }
		else if ( OrdreOuvert == -1 )  	{ message.Format("OrdreOuvert = -1 ''Non determine (Robot ON)''") ;  }
		else if ( OrdreOuvert ==  0 )  	{ message.Format("OrdreOuvert = 0 ''Pas d'ordre ouvert (Robot ON)''") ;  }
		else if ( OrdreOuvert ==  1 )  	{ message.Format("OrdreOuvert = 1 ''Il y a au moins un ordre ouvert (Robot ON)''") ;  }
		// else if ( OrdreOuvert ==  2 )  	{ message.Format("OrdreOuvert = 2 ''Ordres refermes (Robot ON)''") ;  }
		else 										{ message.Format("OrdreOuvert = %d ''bug!'' ;  ", OrdreOuvert) ;  }
		SCString StringAjoute ;
		StringAjoute.Format(" | OrdreOuvertParent = %d, OrdreOuvertTarget1 = %d, OrdreOuvertStop1 = %d                    ", OrdreOuvertParent, OrdreOuvertTarget1, OrdreOuvertStop1) ;
		message.Append(StringAjoute) ;
		sc.AddMessageToLog(message,0) ;

		// PositionOuverte
				if ( PositionOuverte == -2 ) 	{ message.Format("PositionOuverte = -2 ''n.d. car Robot OFF''                    ") ;  }
		else if ( PositionOuverte == -1 )  { message.Format("PositionOuverte = -1 ''Non determine (Robot ON)''                    ") ;  }
		else if ( PositionOuverte ==  0 )  { message.Format("PositionOuverte = 0 ''Pas de position ouverte (Robot ON)''                    ") ;  }
		else if ( PositionOuverte ==  1 )  { message.Format("PositionOuverte = 1 ''Position ouverte (Robot ON)''                    ") ;  }
		else if ( PositionOuverte ==  2 )  { message.Format("PositionOuverte = 2 ''Position refermee (Robot ON)''                    ") ;  }
		else 											{ message.Format("PositionOuverte = %d ''bug!''                    ", PositionOuverte) ;  }
		sc.AddMessageToLog(message,0) ;
		

		// SequenceStatus
		message.Format("SequenceStatus = %d                    ", SequenceStatus) ;  sc.AddMessageToLog(message,0) ;
		
		// Prix tradé (et ajustement)
		if ( AjustementChoisiEnPas == 0 )  {  message.Format("Prix Trade = %.2f (0 aj)                    ", NiveauFutureOrdreEntree) ; }  else {  message.Format("Prix Trade = %.2f (%.2f aj %d t)                    ", NiveauFutureOrdreEntree, NiveauFutureOrdreEntree-(float)AjustementChoisiEnPas/4, AjustementChoisiEnPas) ; }	   sc.AddMessageToLog(message,0) ;

		// PrixDuTP
		message.Format("PrixDuTP = %.2f (TP = %.2f)                    ", PrixDuTP, TP_de_la_precedente_modification_de_TP) ;  sc.AddMessageToLog(message,0) ;
		
		// Dernière ligne
		message.Format("SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS | Fin du Status Report | SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS") ;  sc.AddMessageToLog(message,0) ;
	}
	
	
	// DrawTempsDeCalculs, EnvergureMoyenne4000Ticks et VIX
	if ( CetteInstanceControleLesBoutons.GetYesNo() )
	{ 
		// Affichage de EnvergureMoyenne4000Ticks utilisée par Sigma
		{
			// Création du texte
			Texte.Format("EvergureMoyenne4000T Ref = %.1f",  ExternalArray_EnvergureMoyenne4000Ticks[sc.IndexOfLastVisibleBar]) ;
			
			// Création d'instance de TexteBasAligneGauche_UTAMAOA sur le présent chart et appel de sc.UseTool
			TexteBasAligneGauche_UTAMAOA objetTexte(Texte, 8, 0, 0, ptrsc, 10126+10000*ThisStudyID); 	
			sc.UseTool(objetTexte);  
		}

		// Affichage du VIX sur graphe de Sigma
		{
			// on récupère le VIX
			SCFloatArray VIX_Array;
			sc.GetStudyArrayUsingID(StudyID_VIX, 3, VIX_Array) ;
	
			// Création du texte
			float VIX = VIX_Array[sc.IndexOfLastVisibleBar] ;
			if ( VIX < 50 )  Texte.Format("VIX = %.1f", VIX) ;  else  Texte.Format("VIX = %.0f", VIX) ;
			
			// Création d'instance de TexteBasAligneDroite_UTAMAOA sur le présent chart et appel de sc.UseTool
			TexteBasAligneDroite_UTAMAOA objetTexte(Texte, 8, 0, 0, ptrsc, 10129+10000*ThisStudyID); 	
			sc.UseTool(objetTexte);  
		}		

		// Affichage des temps de calcul
		{
			// Calcul de la durée du dernier appel du DLL            
			QueryPerformanceCounter(&ElapsedTicks) ;  
			float DureeCalculSimpleAMR_QPC = ( ElapsedTicks.QuadPart - ElapsedTicks_t0.QuadPart ) *1000.0 / Frequency.QuadPart ;     // calculé en millisecondes
			// DureeCalculCompletAMR
			if ( RecalculComplet ) 
			{	DureeCalculCompletAMR = DureeCalculSimpleAMR_QPC ;  }    // DureeCalculCompletAMR est une variable persistante
			
			// Calcul de DureeCycleCompletAffichage :
			float DureeCycleCompletAffichage = ( ElapsedTicks.QuadPart - ElapsedTicks_CycleMoins4 ) *1000.0 / Frequency.QuadPart / 4 ;     // calculé en millisecondes  // divisé par 4 car moyenne sur 4 cycles
			
			// Calcul de DureeCycleComplet_FullRecalculation
			if ( sc.IsFullRecalculation == 1 )  
			{	
				DureeCycleComplet_FullRecalculation = ( ElapsedTicks.QuadPart - ElapsedTicks_PrecedentCycle ) *1000.0 / Frequency.QuadPart ;  
				if ( DureeCycleComplet_FullRecalculation > 999999 ) DureeCycleComplet_FullRecalculation = 0 ;		// le premier cycle (après addition de la study) est tjs faux
			}
			
			// Couleur Affichage
			COLORREF couleur = RGB(0,127,255) ;
			if ( DureeCycleCompletAffichage > 200 ) { NombreCyclesConsecutifsTropLong ++ ;  }  else { NombreCyclesConsecutifsTropLong = 0 ;  }
			if ( NombreCyclesConsecutifsTropLong > 1 ) { couleur = RGB(255,0,0) ;}
			
			// Création du texte
			/* if ( DureeCalculSimpleAMR_QPC > 1  and  Clicks_BlocAffichage > 1)
			Texte.Format("%.2f ms | %.02d  ms | %.02d  ms | %.0f ms | %03d ms | %.0f ms",  Clicks_BlocRobotDeTrading, int(round(Clicks_BlocAffichage)), int(round(DureeCalculSimpleAMR_QPC)), round(DureeCycleCompletAffichage/10.0)*10, DureeCalculCompletAMR, round(DureeCycleComplet_FullRecalculation/10)*10) ;
			else if ( DureeCalculSimpleAMR_QPC > 1 )
			Texte.Format("%.2f ms | %.1f ms | %.02d  ms | %.0f ms | %03d ms | %.0f ms",  Clicks_BlocRobotDeTrading, Clicks_BlocAffichage, int(round(DureeCalculSimpleAMR_QPC)), round(DureeCycleCompletAffichage/10.0)*10, DureeCalculCompletAMR, round(DureeCycleComplet_FullRecalculation/10)*10) ;
			else
			Texte.Format("%.2f ms | %.1f ms | %.1f ms | %.0f ms | %03d ms | %.0f ms",  Clicks_BlocRobotDeTrading, Clicks_BlocAffichage, DureeCalculSimpleAMR_QPC, round(DureeCycleCompletAffichage/10.0)*10, DureeCalculCompletAMR, round(DureeCycleComplet_FullRecalculation/10)*10) ; */
			// Texte.Format("%.2f ms | %.02d ms | %.02d ms | %.0f ms | %03d ms | %.0f ms",  Clicks_BlocRobotDeTrading, int(round(Clicks_BlocAffichage)), int(round(DureeCalculSimpleAMR_QPC)), round(DureeCycleCompletAffichage/10.0)*10, DureeCalculCompletAMR, round(DureeCycleComplet_FullRecalculation/10)*10) ;
			Texte.Format("%.02d ms | %.0f ms | %03d ms | %.0f ms", int(round(DureeCalculSimpleAMR_QPC)), round(DureeCycleCompletAffichage/10.0)*10, DureeCalculCompletAMR, round(DureeCycleComplet_FullRecalculation/10)*10) ;
			
			
			// Création d'instance de TexteBasAligneGauche_UTAMAOA sur le présent chart et appel de sc.UseTool
			TexteBasAligneGauche_UTAMAOA objetTexte(Texte, 8, couleur, 2, ptrsc, 10046); 	
			sc.UseTool(objetTexte);  

			// Création d'instance de TexteBasCentre_UTAMAOA_UDD sur le chart #15 et appel de sc.UseTool
			if ( sc.IsChartDataLoadingCompleteForAllCharts() )
			{
				TexteBasCentre_UTAMAOA_UDD objetTexteExterne(Texte, 8, couleur, 15, 0, ptrsc, 10121); 	
				sc.UseTool(objetTexteExterne);  
			}
			
			// Report des clicks des cycles précédents
			ElapsedTicks_CycleMoins4 = ElapsedTicks_CycleMoins3 ;
			ElapsedTicks_CycleMoins3 = ElapsedTicks_CycleMoins2 ;
			ElapsedTicks_CycleMoins2 = ElapsedTicks_PrecedentCycle ;
			ElapsedTicks_PrecedentCycle = ElapsedTicks.QuadPart  ;
		}
	}
}



