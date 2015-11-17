#include "ClueReasoner.h"
using namespace std;

int ClueReasoner::GetPlayerNum(string player)
{
	if (player == case_file)
		return num_players;
	
	for (int i = 0; i < num_players; i++)
		if (player == players[i])
			return i;
			
	cout<<"Illegal player: "<<player<<endl;
	return -1;
}
int ClueReasoner::GetCardNum(string card)
{
	for (int i = 0; i < num_cards; i++)
		if (card == cards[i])
			return i;
			
	cout<<"Illegal card: "<<card<<endl;
	return -1;
}

string ClueReasoner::QueryString(int return_code)
{
	if (return_code == kFalse)
		return "n";
	else if (return_code == kTrue)
		return "Y";
	else
		return "-";
}

void ClueReasoner::PrintNotepad()
{
	for (int i = 0; i < num_players; i++)
		cout<<"\t"<<players[i];
	cout<<"\t"<<case_file<<endl;
	
	for (int i = 0; i < num_cards; i++)
	{
		cout<<cards[i]<<"\t";
		for (int j = 0; j < num_players; j++)
			cout<<QueryString(Query(players[j], cards[i]))<<"\t";
		
		cout<<QueryString(Query(case_file, cards[i]))<<endl;
	}
}
	
void ClueReasoner::AddInitialClauses()
{
	/* The following code is given as an example to show how to create Clauses and post them to the solver. SatSolver.h uses the following typedefs:
		typedef int Literal;
		typedef std::vector<Literal> Clause;
		
	That is, a Literal is defined as a positive or a negative integer (meaning that it is the negated form, as in -p or -q), and a Clause is defined as a vector of Literals (integers).
	
	The function GetPairNum(p, c) returns the literal that means that player p has card c (note that p can be case_file as well). 
	See ClueReasoner.h, lines 7-29 for a definition of the arrays and variables that you can use in your implementation. 
	*/

	// Each card is in at least one place (including case file).
	for (int c = 0; c < num_cards; c++)	// Iterate over all cards.
	{
		Clause clause;
		for (int p = 0; p <= num_players; p++)	// Iterate over all players, including the case file (as a possible place for a card).
			clause.push_back(GetPairNum(p, c));
		
		solver->AddClause(clause);
	}
        
	// If a card is in one place, it cannot be in another place.
	// TO BE IMPLEMENTED AS AN EXERCISE

		for (int c = 0; c < num_cards; c++)	// Iterate over all cards.
	{
		
		for (int p = 0; p <= num_players; p++)	// Iterate over all players, including the case file (as a possible place for a card).
			{
				for(int p_temp = 0; p_temp <= num_players; p_temp++)
				{
					Clause clause_1;
					if(p!=p_temp){
						clause_1.push_back(-GetPairNum(p, c));
						clause_1.push_back(-GetPairNum(p_temp,c));
						solver->AddClause(clause_1);
					}
					
				}
			}		
	}

	
	// At least one card of each category is in the case file.
	// TO BE IMPLEMENTED AS AN EXERCISE

    Clause clause_2;
    Clause clause_3;
    Clause clause_4;

		for (int c = 0; c < num_cards; c++)	// Iterate over all cards.
	{
		if(c<6){
			clause_2.push_back(GetPairNum(6, c));
			clause_3.push_back(GetPairNum(6, c+6));
		}
		if(c<9){
			clause_4.push_back(GetPairNum(6, c+12));
		}
	}

	solver->AddClause(clause_2);
	solver->AddClause(clause_3);
	solver->AddClause(clause_4);

	// No two cards in each category can both be in the case file.
	// TO BE IMPLEMENTED AS AN EXERCISE

    //for no repeat in suspect
	for (int c = 0; c < 6; c++)	// Iterate over all cards.
	{
		for(int c_temp=0 ; c_temp<6 ; c_temp++){
			Clause clause_5;
			if(c!=c_temp){
				clause_5.push_back(-GetPairNum(6,c));
				clause_5.push_back(-GetPairNum(6,c_temp));
			}
			solver->AddClause(clause_5);
		}	
	}

	//for no repeat in weapons
	for (int c = 6; c < 12; c++)	// Iterate over all cards.
	{
		for(int c_temp=6 ; c_temp<12 ; c_temp++){
			Clause clause_6;
			if(c!=c_temp){
				clause_6.push_back(-GetPairNum(6,c));
				clause_6.push_back(-GetPairNum(6,c_temp));
			}
			solver->AddClause(clause_6);
		}	
	}

	//for no repeat in rooms
		//for no repeat in weapons
	for (int c = 12; c < 21; c++)	// Iterate over all cards.
	{
		for(int c_temp=12 ; c_temp<21 ; c_temp++){
			Clause clause_7;
			if(c!=c_temp){
				clause_7.push_back(-GetPairNum(6,c));
				clause_7.push_back(-GetPairNum(6,c_temp));
			}
			solver->AddClause(clause_7);
		}	
	}

}
void ClueReasoner::Hand(string player, string cards[3])
{
	// GetPlayerNum returns the index of the player in the players array (not the suspects array). Remember that the players array is sorted wrt the order that the players play. Also note that, player_num (not to be confused with num_players) is a private variable of the ClueReasoner class that is initialized when this function is called.
	player_num = GetPlayerNum(player);

    int *cards_value = new int[3];

    for(int a = 0; a < 3 ; a++){
    	Clause clause_8;
    	cards_value[a]=GetCardNum(cards[a]);
    	clause_8.push_back(GetPairNum(player_num,cards_value[a]));
    	solver->AddClause(clause_8);
    }

	// TO BE IMPLEMENTED AS AN EXERCISE
}
void ClueReasoner::Suggest(string suggester, string card1, string card2, string card3, string refuter, string card_shown)
{
	// Note that in the Java implementation, the refuter and the card_shown can be NULL. 
	// In this C++ implementation, NULL is translated to be the empty string "".
	// To check if refuter is NULL or card_shown is NULL, you should use if(refuter == "") or if(card_shown == ""), respectively.
	// TO BE IMPLEMENTED AS AN EXERCISE

	int *cards_value_1 = new int[3];

	cards_value_1[0]=GetCardNum(card1);
	cards_value_1[1]=GetCardNum(card2);
	cards_value_1[2]=GetCardNum(card3);


    int suggester_id = GetPlayerNum(suggester);
    int refuter_id = GetPlayerNum(refuter);
	
	if(refuter==""){
		//if refuter=="", everyone else doesn't have those three cards;
		for(int b = 0; b <num_players ; b++){
			if(b!=suggester_id){
				for(int e = 0; e<3 ;e++){
					Clause clause_9;
					clause_9.push_back(-GetPairNum(b,cards_value_1[e]));
					solver->AddClause(clause_9);
				}
			}
		}
    }else{
    	//if refuter!="", everyone between doesn't have those three cards and there are two conditions

    	if(refuter_id > suggester_id+1){
    		for(int q = suggester_id+1 ; q < refuter_id ; q++){
    			for(int e = 0; e<3 ;e++){
					Clause clause_10;
					clause_10.push_back(-GetPairNum(q,cards_value_1[e]));
					solver->AddClause(clause_10);
				}
    		}
    	}else if(refuter_id < suggester_id ){
	        if(suggester_id+1<num_players){
	        	for(int p = suggester_id+1 ; p < num_players ; p++){
    				for(int e = 0; e<3 ;e++){
					Clause clause_11;
					clause_11.push_back(-GetPairNum(p,cards_value_1[e]));
					solver->AddClause(clause_11);
					}
    			}
    		}
    		
    		if(refuter_id>0){
    			for(int t = 0 ; t < refuter_id ; t++){
    				for(int e = 0; e<3 ;e++){
					Clause clause_12;
					clause_12.push_back(-GetPairNum(t,cards_value_1[e]));
					solver->AddClause(clause_12);
					}
    			}
   			}
    		
    	}
    	

    	//first,if refuter is "sc", then suggester doesn't have that card
    	if(refuter==players[player_num]){
    		Clause clause_13;
    		clause_13.push_back(-GetPairNum(suggester_id,GetCardNum(card_shown)));
    		solver->AddClause(clause_13);
    	}
    	//else if refuture isn't "sc"
    	else if(refuter != players[player_num]){
    		if(card_shown!=""){
	       		//if card_shown isn't "", sc is suggester and refuter has card_shown. 
	       		Clause clause_14;
	       		clause_14.push_back(GetPairNum(refuter_id,GetCardNum(card_shown)));
	       		solver->AddClause(clause_14);
    		}else if(card_shown==""){
    			//if card_shown is ""
    			//the refuter has at least one of the card
    			Clause clause_19;
	       		clause_19.push_back(GetPairNum(refuter_id,GetCardNum(card1)));
	       		clause_19.push_back(GetPairNum(refuter_id,GetCardNum(card2)));
	       		clause_19.push_back(GetPairNum(refuter_id,GetCardNum(card3)));
	       		solver->AddClause(clause_19);

    			//the cf doesn't have at least one of the card
    			Clause clause_20;
	       		clause_20.push_back(-GetPairNum(6,GetCardNum(card1)));
	       		clause_20.push_back(-GetPairNum(6,GetCardNum(card2)));
	       		clause_20.push_back(-GetPairNum(6,GetCardNum(card3)));
	       		solver->AddClause(clause_20);

    		}
    	}

    }
	

	   
        
	       


}
void ClueReasoner::Accuse(string suggester, string card1, string card2, string card3, bool is_correct)
{
	//If the is_correct is true, the cf have those three cards
	if(is_correct){
			Clause clause_15;
			clause_15.push_back(GetPairNum(6,GetCardNum(card1)));
			solver->AddClause(clause_15);

			Clause clause_16;
			clause_16.push_back(GetPairNum(6,GetCardNum(card2)));
			solver->AddClause(clause_16);

			Clause clause_17;
			clause_17.push_back(GetPairNum(6,GetCardNum(card3)));
			solver->AddClause(clause_17);

	}else{
		//else if the is_correct is false, at least one of the three cards are not in the case file.
			Clause clause_18;
			clause_18.push_back(-GetPairNum(6,GetCardNum(card1)));
			clause_18.push_back(-GetPairNum(6,GetCardNum(card2)));
			clause_18.push_back(-GetPairNum(6,GetCardNum(card3)));
			solver->AddClause(clause_18);
	}

	
	// TO BE IMPLEMENTED AS AN EXERCISE
}
