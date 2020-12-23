//Pombo usando threads e semaforos

#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<pthread.h>
#include <unistd.h>

#define N 20

int contPostit=0;
sem_t mutex;
sem_t chute;
int mensagem=0;

void leva_mochila_ate_B_e_volta(){
    
    sem_wait(&mutex);
    printf("em viagem\n");

    sleep( 1 + rand() % 3);

}
void ColaPostIt(){

    int valor;
    valor= (contPostit % 20)+1;
    printf("Post It: %d\n", valor);
    contPostit++;

}
void dorme_aleatorio(){

    sleep( 1 + rand() % 3);

}
void* func_usuario() {
    printf("usuarios\n");

    while(1){
        dorme_aleatorio();

        sem_wait(&mutex);

        ColaPostIt();
        //contPostit++;

        sem_post(&mutex);

        if (contPostit == 20)
        {   
            printf("mochila cheia\n");

            sem_post(&chute);

        }
    }
}
void* func_pombo() {
    printf("pombo\n");

    while(1){
        
        sem_wait(&chute);
        printf("Acordar\n");
        //sem_wait(&mutex);
        
        leva_mochila_ate_B_e_volta();
        
        contPostit = 0;
        
        for (int i=0; i<N; i++){
           
           
            printf("entregar mensagem %d\n", i+1);
            
            
        }

        sem_post(&mutex);
        printf("chegada em A\n");
        
    }
} 
int main() 
{ 
    int numUsu;

    printf("Informe o número de usuarios: ");
    scanf("%d", &numUsu);
    printf("\n");


    sem_init(&mutex, 0, 1);
    sem_init(&chute, 0, 0);

    pthread_t usuario[numUsu]; 
    pthread_t pombo;
    
    for (int i=0; i<numUsu; i++)
	{
		pthread_create(&usuario[i], NULL, &func_usuario, NULL);
	}
    pthread_create(&pombo, NULL, &func_pombo, NULL);

    pthread_join(pombo,0);
    for (int i=0; i<numUsu; i++)
	{
		pthread_join(usuario[i],0);
	}

    sem_destroy(&mutex);
    sem_destroy(&chute);

    exit(0); 
}

