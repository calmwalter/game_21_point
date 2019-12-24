#include<stdio.h>
#include<stdlib.h>
#include<time.h>

//swap the card
void swap(int* cards, int i, int j){
  int temp = cards[i];
  cards[i] = cards[j];
  cards[j] = temp;
}

//shuffle the card
void shuffle(int* cards){
  for(int i=0;i<156;i++){
    swap(cards,i,rand()%156);
  }
}

int add_card_to_player(int* cards, int* cards_offset, int** player_cards, int* player_card_number, int player_id){
  int* ptr = (int*)realloc(player_cards[player_id],sizeof(int)*(player_card_number[player_id]+1));
  if(ptr){
    player_cards[player_id] = ptr;
  }else{
    printf("ERROR\n");
    return 0;
  }
  player_cards[player_id][player_card_number[player_id]++] = cards[(*cards_offset)++];
  return 1;
}
int calculate_point(int* cards, int card_number){
  int* results= (int*)malloc(sizeof(int));
  int result_number=1;
  results[0]=0;
  for(int i=0;i<card_number;i++){
    if(cards[i]==1){
      int* ptr = (int*)realloc(results,sizeof(int)*result_number*2);
      if(ptr){
        results = ptr;
      }
      else{
        printf("ERROR.\n");
        return -1;
      }
      for(int j=result_number;j<result_number*2;j++){
        results[j] = results[j-result_number]+11;
      }
      for(int j=0;j<result_number;j++){
        results[j] += cards[i]>10 ? 10:cards[i];
      }
      result_number*=2;
    }else{
      for(int j=0;j<result_number;j++){
        results[j] += cards[i]>10 ? 10:cards[i];
      }
    }
  }
  int best_result = results[0];
  for(int i=0;i<result_number;i++){
    if(best_result<results[i] && results[i]<=21){
      best_result = results[i];
    }
  }
  return best_result;
}
void  print_player_card(int** player_cards,int* player_card_number, int player_number){
  printf("\n%-10s%-10s\n","player","cards");
  for(int i=0;i<player_number;i++){
    printf("%-10d",i);
    for(int j=0;j<player_card_number[i];j++){
      printf("%-5d",player_cards[i][j]);
    }
    printf("\n");
  }

}
void  print_bookmaker_card(int* cards,int card_number,int show_hide){
  if(!show_hide){
    printf("\n%-15s%-5d*\n\n","bookmaker cards:",cards[0]);
    return;
  }
    printf("\n%-15s","bookmaker cards:");
  for(int i=0;i<card_number;i++){
    printf("%-5d",cards[i]);
  }
  printf("\n\n");
}
void print_player_money(int* player_money,int player_number){
    printf("\n%-10s%-20s\n","player","money");
    for(int i=0;i<player_number;i++){
      printf("%-10d",i);
      printf("%-20d\n",player_money[i]);
    }
}
int main(){
  printf("\n\n**21 pointer game**\n\n");
  //initialize the game
  srand((unsigned)time(NULL));

  int player_number;
  printf("input player number:");
  scanf("%d",&player_number);

  int bookmaker_money,player_money[player_number],player_bet_amount[player_number];
  for(int i=0;i<player_number;i++){
    player_bet_amount[i] = 0;
  }
  printf("input bookmaker money:");
  scanf("%d",&bookmaker_money);
  for(int i=0;i<player_number;i++){
    printf("input player%d money:",i);
    scanf("%d",player_money+i);
  }
  while(1){
    int* bookmaker_cards = (int*)malloc(sizeof(int));
    int bookmaker_card_number=0;

    int** player_cards = (int**)malloc(sizeof(int*)*player_number);
    for(int i=0;i<player_number;i++){
      player_cards[i] = (int*)malloc(sizeof(int));
    }
    int player_card_number[player_number];
    for(int i=0;i<player_number;i++){
      player_card_number[i]=0;
    }
    //0 surrender or burst, 1 normal, 2 suspension
    int player_state[player_number];
    for(int i=0;i<player_number;i++){
      player_state[i] = 1;
    }

    int cards[156];
    for(int k=0;k<3;k++){
      for(int i=0;i<13;i++){
        for(int j=0;j<4;j++){
          cards[k*52+i*4+j] = i+1;
        }
      }
    }
  
    shuffle(cards);
    int cards_offset=0;
    //start the game

    //--------------------------------------
    // TODO this position code is redundancy, improve it after develop
    int* ptr;
    // TODO send one card to each player
    for(int i=0;i<player_number;i++){
      ptr = (int*)realloc(player_cards[i],sizeof(int)*(player_card_number[i]+1));
      if(ptr){
        player_cards[i] = ptr;
      }else{
        printf("ERROR\n");
        return 0;
      }
      player_cards[i][player_card_number[i]++] = cards[cards_offset++];
    }
    // TODO bookmaker get one card
    ptr = (int*)realloc(bookmaker_cards,sizeof(int)*(bookmaker_card_number+1));
    if(ptr){
      bookmaker_cards = ptr;
      bookmaker_cards[bookmaker_card_number++] = cards[cards_offset++];
    }
    // TODO send one card to each player
    for(int i=0;i<player_number;i++){
      ptr = (int*)realloc(player_cards[i],sizeof(int)*(player_card_number[i]+1));
      if(ptr){
        player_cards[i] = ptr;
      }else{
        printf("ERROR\n");
        return 0;
      }
      player_cards[i][player_card_number[i]++] = cards[cards_offset++];
    }
    // TODO bookmaker get one card but hide
    ptr = (int*)realloc(bookmaker_cards,sizeof(int)*(bookmaker_card_number+1));
    if(ptr){
      bookmaker_cards = ptr;
      bookmaker_cards[bookmaker_card_number++] = cards[cards_offset++];
    }
    //--------------------------------------

    // TODO show the player card and the first card of the bookmaker
    print_player_card(player_cards,player_card_number,player_number);
    print_bookmaker_card(bookmaker_cards,bookmaker_card_number,0);

    //input player intial bet
    for(int i=0;i<player_number;i++){
      printf("player%d bet:",i);
      int money;
      scanf("%d",&money);
      if(money>player_money[i]){
        printf("NO ENOUGH MONEY.\n");
        i--;
        continue;
      }
      player_bet_amount[i] += money;
      player_money[i] -= money;
    }

    while(1){
      // TODO player select 0 raise or 1 surrender or 2 suspension or 3 do nothing or 4 show bookmaker card or 5 show player money
      for(int i=0;i<player_number;i++){
        printf("player%d's turn\nselect 0 raise or 1 surrender or 2 suspension or 3 do nothing or 4 show bookmaker card or 5 show player money\ninput:",i);
        if(!player_state[i]){
          continue;
        }
        int select;
        scanf("%d",&select);
        if(select==0){
          int money;
          printf("input bet money:");
          scanf("%d",&money);
          if(money>player_money[i]){
            printf("NO ENOUGH MONEY.\n");
            i--;
            continue;
          }
          player_bet_amount[i] += money;
          player_money[i] -= money;
        }else if(select==1){
          for(int j=0;j<player_number;j++){
            if(player_card_number[j]>2){
              i--;
              printf("CAN'T SURRENDER FOR SOME PLAYERS HAVE ADD CARD\n");
              continue;
            }
          }
          player_state[i] = 0;
          player_money[i] += player_bet_amount[i]/2;
          bookmaker_money += player_bet_amount[i]/2;
        }else if(select==2){
          if(player_state[i]==2){
            printf("ALREADY SUSPENSION.\n");
            i--;
            continue;
          }
          player_state[i] = 2;
        }else if(select==3){
          continue;
        }else if(select==4){
          print_bookmaker_card(bookmaker_cards,bookmaker_card_number,0);
          i--;
          continue;
        }else if(select==5){
          printf("card:");
          for(int j=0;j<player_card_number[i];j++){
            printf("%-4d",player_cards[i][j]);
          }
          printf("total point:%d\n",calculate_point(player_cards[i],player_card_number[i]));
          i--;
          continue;
        }
        else{
          printf("INPUT ERROR\n");
          i--;
          continue;
        }
      }
      // TODO bookmaker ask if add card
      for(int i=0;i<player_number;i++){
        if(player_state[i]==0 || player_state[i]==2){
            continue;
        }
        int select;
        printf("player%d's turn\n",i);
        printf("card:");
        for(int j=0;j<player_card_number[i];j++){
          printf("%-4d",player_cards[i][j]);
        }
        printf("total point:%d\n",calculate_point(player_cards[i],player_card_number[i]));
        printf("player %d add card? 0-->NO, 1-->YES\ninput your choice:",i);
        scanf("%d",&select);
        if(select==0){
          continue;
        }else if(select==1){
          add_card_to_player(cards,&cards_offset,player_cards,player_card_number,i);
          int point = calculate_point(player_cards[i],player_card_number[i]);
          if(point>21){
            //burst
            player_state[i] = 0;
            bookmaker_money += player_bet_amount[i];
          }
        }else{
          printf("INPUT ERROR\n");
          i--;
          continue;
        }
      }
      int flag=0;
      // TODO judge if every player stop
      for(int i=0;i<player_number;i++){
        if(player_state[i]==1){
          flag=1;
          break;
        }
      }
      if(flag){
        continue;
      }
      // TODO bookmaker show the hidden card
      print_bookmaker_card(bookmaker_cards,bookmaker_card_number,1);
      // TODO bookmaker add card until the sum of card larger than 17
      int bookmaker_point = calculate_point(bookmaker_cards,bookmaker_card_number);
      while(bookmaker_point<17){
        int* ptr = (int*)realloc(bookmaker_cards,sizeof(int)*(bookmaker_card_number+1));
        if(ptr){
          bookmaker_cards = ptr;
        }else{
          continue;
        }
        bookmaker_cards[bookmaker_card_number++] = cards[cards_offset++];
        bookmaker_point = calculate_point(bookmaker_cards,bookmaker_card_number);
      }
      printf("after add card\n");
      print_bookmaker_card(bookmaker_cards,bookmaker_card_number,1);
      // TODO settlement
      if(bookmaker_point>21){
        // bookmaker burst
        for (int i = 0; i < player_number; i++){
          if(player_state[i]==2){
            player_money[i] += player_bet_amount[i]*2;
            bookmaker_money -= player_bet_amount[i];
            printf("player%d win money: %d\n",i,player_bet_amount[i]);
          }else{
            continue;
          }
        }
        
      }else{
        for (int i = 0; i < player_number; i++){
          int player_point = calculate_point(player_cards[i],player_card_number[i]);
          if(bookmaker_point< player_point){
            printf("player%d win money: %d\n",i,player_bet_amount[i]);
            player_money[i] += player_bet_amount[i]*2;
            bookmaker_money -= player_bet_amount[i];
          }else if(bookmaker_point==player_point){
            printf("player%d draw\n",i);
            player_money[i] += player_bet_amount[i];
          }
          else{
            printf("player%d loses money: %d\n",i,player_bet_amount[i]);
            bookmaker_money += player_bet_amount[i];
          }
        }
      }
      print_player_money(player_money,player_number);
      printf("bookemaker money:%d\n",bookmaker_money);
      break;
    }
    //release pointer
    free(bookmaker_cards);
    for(int i=0;i<player_number;i++){
      free(player_cards[i]);
    }
    free(player_cards);
    
    if(bookmaker_money<=0){
      printf("GAME END.\n");
      break;  
    }
    printf("GAME END.\nstart a new round? 0-->YES, 1-->NO\ninput:");
    int run;
    scanf("%d",&run);
    if(run){
      break;
    }
  }
  return 0;
}
