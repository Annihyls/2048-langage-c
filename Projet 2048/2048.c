/*#################################################   2048 en C, GIMBERT Vincent, PHILIPIAK Geoffrey   #################################################*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
int **creatab2D(int ligne){			//créer le Tableau 4*4
	int **M=NULL;
	M=(int**)calloc(ligne,sizeof(int*));
	if(M==NULL){
		return NULL;
	}
	int i,j;
	for(i=0;i<ligne;i++){
		M[i]=(int*)calloc(ligne,sizeof(int));
		if(M[i]==NULL){
			for(j=0;j<i;j++){
				free(M[j]);
			}
			free(M);
			return NULL;
		}
	}
	return M;
}
void LibereTab2D(int **T, int n){	//pour libérer le tableau 4*4 quand le jeu est fini
	int i;
	if(T!=NULL){
		for (i=0;i<n;i++){
			if(T[i]!=NULL){
				free(T[i]);
			}
		}
	}
	free(T);
}
void afficheTab(int **T, int n){	//affiche le Tableau
	int i,j;
	for(i=0;i<n;i++){
		printf("#\n#####################\n"); //ça c'est pour les traits horizontaux du tableau
		for(j=0;j<n;j++){
			if(T[i][j]==0){		//les cases 0 sont en réalité des cases vide, on les remplace par des x comme prévu dans les consignes
				printf("#    ");
			}
			else{
				printf("#   %d", T[i][j]);
			}
		}
	}
	printf("#\n#####################\n");
}
void InitTab(int **T, int n){	//pour initier la partie avec une cases du tableau prenant la valeur 2
	int i,j;
	do{					//do...while car il faut initialiser i et j au moins une fois
		i=rand()%4;
		j=rand()%4;
	}
	while(T[i][j]!=0); //il faut que les 2 cases initiées soient différentes, d'où cette condition
	T[i][j]=2;
}
void RandomTab(int **T, int n){	//cette fonction permet de générer une case 2 ou 4 aléatoirement dans le tableau (à faire à chaque tour)
	int i,j,k;
	do{					
		i=rand()%4;	//génére aléatoirement des nombre compris entre 0 et 3
		j=rand()%4;
		k=rand()%2;	//génére aléatoirement des nombre compris entre 0 et 1
	}
	while(T[i][j]!=0); 
	if(k==0){
		T[i][j]=2;
	}
	else{
		T[i][j]=4;
	}
}
void transpose(int **T,int n){		//transpose sert a transposer une matrice (les valeurs des lignes deviennent les valeurs de colonnes et vice-versa
	int i,j,temp;
	for(i=0;i<n;i++){
        for(j=0;j<n;j++){
			if(i>j){
				temp=T[j][i];
				T[j][i]=T[i][j];
				T[i][j]=temp;
			}
        }
    }
}
int gauche(int **T, int n, int score){ //retourne le score du coup joué
	int i,j;
	for(i=0;i<n;i++){		//boucle qui gère les lignes du tableau. En effet, on va gérer ligne par ligne
		for(j=0;j<n;j++){	//boucle qui gère les colonnes du tableau. On fait un déplacement de tableau de gauche à droite
			if(T[i][j]!=0){		
				if(T[i][j+1]==T[i][j]){		
					T[i][j]=T[i][j]+T[i][j+1]; 	// on additione la case actuelle avec la case de droite
					T[i][j+1]=0;					//la case de droite prends la valeur 0
					score=score+T[i][j];		//calcul du score sur la case actuelle (celle qui contient la fusion)
				}
				while(j!=0){			/*cette boucle while permet de déplacer la case actuelle vers la gauche si et 
										seulement si cette case est différente de 0 et que j est différent de 0 (le bord gauche du tableau)*/
					if(T[i][j-1]==0){   //si la case de gauche est égale à 0, alors on déplace la case actuelle à gauche
						T[i][j-1]=T[i][j];
						T[i][j]=0;
					}
					else if(T[i][j-1]==T[i][j]){	//mais si la case de gauche est égale a la case actuelle, une fusion s'impose.
						T[i][j-1]=T[i][j]+T[i][j-1]; // La case de gauche prendra la valeur de la fusion
						T[i][j]=0;					
						score=score+T[i][j-1];	//le score est calculé
					}
					else{		/*si la case de gauche n'est ni égale à 0, ni égale a la case actuelle, alors on sort
									de la boucle avec break*/
						break;
					}
					j--;	//j est décrémenté pour ne pas faire une boucle infini avec le while
				}
			}
		}
	}
	return score;	// le score est retourné
}
int droite(int **T, int n, int score){ //retourne le score du coup joué
	int i,j;
	for(i=0;i<n;i++){		//boucle qui gère les lignes du tableau. En effet, on va gérer ligne par ligne
		for(j=n;j>=0;j--){	//boucle qui gère les colonnes du tableau. On fait un déplacement de tableau de droite à gauche
			if(T[i][j]!=0){		
				if(T[i][j-1]==T[i][j]){		
					T[i][j]=T[i][j]+T[i][j-1]; 	// on additione la case actuelle avec la case de droite
					T[i][j-1]=0;					//la case de droite prends la valeur 0
					score=score+T[i][j];		//calcul du score sur la case actuelle (celle qui contient la fusion)
				}
				while(j!=3){			/*cette boucle while permet de déplacer la case actuelle vers la droite si et 
										seulement si cette case est différente de 0 et que j est différent de 3 (le bord droit du tableau)*/
					if(T[i][j+1]==0){   //si la case de droite est égale à 0, alors on déplace la case actuelle à droite
						T[i][j+1]=T[i][j];
						T[i][j]=0;
					}
					else if(T[i][j+1]==T[i][j]){	//mais si la case de droite est égale a la case actuelle, une fusion s'impose.
						T[i][j+1]=T[i][j]+T[i][j+1]; // La case de droite prendra la valeur de la fusion
						T[i][j]=0;					
						score=score+T[i][j+1];	//le score est calculé
					}
					else{		/*si la case de droite n'est ni égale à 0, ni égale a la case actuelle, alors on sort
									de la boucle avec break*/
						break;
					}
					j++;	//j est décrémenté pour ne pas faire une boucle infini avec le while
				}
			}
		}
	}
	return score;	// le score est retourné
}
int deplaceDroite(int **T, int n){
	int i,j;
	int compteur=0;
	for(i=0;i<n;i++){		//boucle permet de savoir si un déplacement vers la droite est possible
		for(j=n;j>=0;j--){
			if((T[i][j-1]!=T[i][j])&&(T[i][j]!=0)){	
				compteur++;
			}
		}
	}
	if(compteur==16){
		return 1;
	}
	return 0;
}
int deplaceGauche(int **T, int n){
	int i,j;
	int compteur=0;
	for(i=0;i<n;i++){		//boucle permet de savoir si un déplacement vers la gauche est possible
		for(j=0;j<n;j++){
			if((T[i][j+1]!=T[i][j])&&(T[i][j]!=0)){	
				compteur++;
			}
		}
	}
	if(compteur==16){
		return 1;
	}
	return 0;
}
void sauvegarde(int **T, int n, int score){
    int i,j;
    FILE *fichier=fopen("sauvegarde.txt", "w+");		//on créer, ouvre un fichier sauvegarde.txt en mode écriture
	if(fichier==NULL){	//s'il y a un problème, perror !
		perror("Echec de la sauvegarde ! Impossible de sauvegarder\n");
		LibereTab2D(T,n);
		exit(-1);
	}
    for (i=0;i<n;i++){	//on écrit dans le fichier le contenu de chaques cases, séparés par un ";"
		for(j=0;j<n;j++){
			fprintf(fichier,";%d",T[i][j]);	
		}
	}
	fprintf(fichier,";%d",score);
	printf("Sauvegarde réussi !\n");	
	fclose(fichier);	//sauvegarde réussi ! On ferme le fichier
}
int chargement(int **T, int n, int score){
    FILE *fichier=fopen("sauvegarde.txt", "r");		//on ouvre "sauvegarde.txt" en mode lecture
    if(fichier==NULL)			//si aucun fichier de ce nom n'existe, alors on commence une nouvelle partie
	{
		printf("Pas de sauvegarde ! Nouvelle partie !\n");
        return score;
	}
    else				//sinon, on reprends notre partie là où on l'avait laissé
	{
		int i,j;
		int taille=0;
		char c;
		while((c=fgetc(fichier))!=EOF){	//fgetc permet d'écrire le caractère du fichier dans un flux
			taille++;		//on détermine le nombre de caractères présent dans le fichier afin d'avoir une taille de tableau la plus petite possible
		}
		rewind(fichier);		//on remet le curseur au début du fichier
		char *ch=NULL;		//on créer un tableau de caractère 
		ch=malloc(taille*sizeof(char));
		if(ch==NULL){
			printf("Erreur ! Allocation échoué !\n");
			LibereTab2D(T,n);
			exit(-1);
		}
		while((c=fgetc(fichier))!=EOF){		
			fscanf(fichier, "%s", ch);		//on utilise fscanf pour écrire la contenu du fichier dans la chaine de caractère ch
		}
		const char *separator=";";		//permet de définir un séparateur (ici, ";" est notre séparateur)
		char *p=strtok(ch,separator);	//défini un pointeur p et découpe à chaque séparateurs
		for (i=0;i<n;i++){
			for(j=0;j<n;j++){
				if(p!=NULL){
					T[i][j]=atoi(p);	//ASCII TO INTEGER
					p=strtok(NULL,separator);
				}
			}
		}
		while(p!=NULL){
			score=atoi(p);
			p=strtok(NULL,separator);
		}
		fclose(fichier);		//on peut fermer le fichier
		printf("Sauvegarde trouvée ! On reprends la partie !\n");
		free(ch);
		remove("sauvegarde.txt"); //efface la sauvegarde pour éviter que le joueur la modifie pendant l'exécution de la fonction
		return score;
	}
}
int GAMEOVER(int **T, int taille, int gameover){
	int somme=0;
	//ci dessous on vérifie que le déplacement vers le haut est possible
	transpose(T,taille);
	somme=somme+deplaceGauche(T,taille);
	transpose(T,taille);
	//ci dessous on vérifie que le déplacement vers le bas est possible
	transpose(T,taille);
	somme=somme+deplaceDroite(T,taille);
	transpose(T,taille);
	//ci dessous on vérifie que le déplacement vers la gauche est possible
	somme=somme+deplaceGauche(T,taille);
	//ci dessous on vérifie que le déplacement vers la droite est possible
	somme=somme+deplaceDroite(T,taille);
	if(somme==4){ //si le déplacement dans les 4 direction est impossible, alors GAMEOVER
		gameover=1;
	}
	return gameover;
}
//#################################################################################################################################
int main(){
	srand(time(NULL));
	int score=-1;
	int taille=4;
	int **T=creatab2D(taille);
	if(T==NULL){		//si le tableau n'a pas pu être créer, alors on quitte le programme avec un message d'erreur
		printf("ERREUR : Création du tableau de jeu impossible !");
		exit(-1);
	}
	score=chargement(T,taille,score);
	if(score==-1){
		InitTab(T,taille);
		InitTab(T,taille);
		score=0;
	}
	printf("SCORE : %d\n", score);
	afficheTab(T,taille);
	int gameover=0;
	int turn=1; //permet de savoir si un tour a été joué ou non
	char choix;
	while(gameover!=1){
		printf("d=droite g=gauche h=haut b=bas s=save&quit k=quitter et recommencer\n");
		scanf("%c",&choix);
		switch(choix){
			case 'h':
			transpose(T,taille);
			turn=deplaceGauche(T,taille);
			score=gauche(T,taille,score); //faire un mouvement vers le haut reviens à transposer la matrice et faire un mouvement vers la droite
			transpose(T,taille); //on retranspose pour revenir à la matrice de base
			break;
			case 'b':
			transpose(T,taille);
			turn=deplaceDroite(T,taille); 
			score=droite(T,taille,score); //faire un mouvement vers le bas reviens à transposer la matrice et faire un mouvement vers la gauche
			transpose(T,taille); //on retranspose pour revenir à la matrice de base
			break;
			case 'g':	//touche de gauche
			turn=deplaceGauche(T,taille);
			score=gauche(T,taille,score);
			break;
			case 'd':	//touche de droite
			turn=deplaceDroite(T,taille);
			score=droite(T,taille,score);
			break;
			case 's':	//sauvegarder et quitter
			sauvegarde(T,taille,score);
			LibereTab2D(T,taille);
			exit(-1);
			break;
			case 'k':	//quitter sans sauvegarder
			LibereTab2D(T,taille);
			exit(-1);
			break;
			default:
			if (turn==0){	//cette boucle permet de remplir les cases avec RandomTab si le joueur a tapé ces touches : h,b,g,d.
				RandomTab(T, taille);
				turn=1;
			}
			else{
				printf("\nMouvement impossible ! Essayez un autre mouvement !\n");
			}
			printf("SCORE : %d\n", score);
			afficheTab(T,taille);
			gameover=GAMEOVER(T,taille,gameover);
		}
	}
	printf("GAME OVER !\nFIN DE LA PARTIE ! FERMETURE DU PROGRAMME !\n");
	LibereTab2D(T,taille);
	return 0;		
}