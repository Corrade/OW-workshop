// Card values are from 2-14 and suits are from 0-3.

// gA = starting cash
// gB = small blind
// gC = pot
// gD = array of community cards {card{value, suit}}
// gE = stage: resting value 0, preflop 1, flop 2, turn 3, river 4
// gF = player order for button rotation {button, small blind, big blind}
// gG = player order for betting in the ongoing round
// gH = index of current player in gG
// gI = public temporary variable
// gJ = current stage leading bet
// gK = physical game object info {pot{center, radius}, community card positions[5], card sphere radius, chips/score units above eye level, current bet distance from feet, deal card speed}
// gL = modify bet increment
// gM = deck{card{value, suit, center, visible to}}. A card's index in gM = (4*(value - 2) + suit).
// gN = interface for deal card{trigger, position, visible to, random card index in gM}
// gO = nth community card
// gP = buttons{buttons{center, radius, ID}}
// gQ = iterator vars and trigger for initialising gM

// pA = hand {card{value, suit}}
// pB = player current stage bet
// pC = player turn or not
// pD = trigger for turn actions. resting value 0, fold bet call raise check = {1, 2, 3, 4, 5} respectively.
// pE = trigger for end turn module
// pF = acted this stage
// pG = proposed bet
// pZ = player score for testing purposes



// UI and housekeeping_________________________________________________________
rule("Init global vars")
{
	event
	{
		Ongoing - Global;
	}
    
    actions
    {
        // It's not necessary to initialise all of these, but I do it for peace of mind
        Set Global Variable(A, 200);
        Set Global Variable(B, 5);
        Set Global Variable(C, 0);
        Set Global Variable(D, Empty Array);
        Set Global Variable(E, 0);
        Set Global Variable(F, Empty Array);
        Set Global Variable(G, Empty Array);
        Set Global Variable(H, 0);
        Set Global Variable(I, 0);
        Set Global Variable(J, 0);
        Set Global Variable(K, Empty Array);
            Set Global Variable(I, Empty Array);
                Modify Global Variable(I, Append to Array, Vector(-210, 0, -18));
                Modify Global Variable(I, Append to Array, 1);
                Set Global Variable At Index(K, 0, Global Variable(I));
            Set Global Variable(I, Empty Array);
                Modify Global Variable(I, Append to Array, Add(Value In Array(Value In Array(Global Variable(K), 0), 0), Vector(1, 0, 0)));
                Modify Global Variable(I, Append to Array, Add(Value In Array(Value In Array(Global Variable(K), 0), 0), Vector(2, 0, 0)));
                Modify Global Variable(I, Append to Array, Add(Value In Array(Value In Array(Global Variable(K), 0), 0), Vector(3, 0, 0)));
                Modify Global Variable(I, Append to Array, Add(Value In Array(Value In Array(Global Variable(K), 0), 0), Vector(4, 0, 0)));
                Modify Global Variable(I, Append to Array, Add(Value In Array(Value In Array(Global Variable(K), 0), 0), Vector(5, 0, 0)));
                Set Global Variable At Index(K, 1, Global Variable(I));
            Modify Global Variable(K, Append To Array, 0.175);
            Modify Global Variable(K, Append To Array, 0.5);
            Modify Global Variable(K, Append To Array, 0.75);
            Modify Global Variable(K, Append To Array, 6);
        Set Global Variable(L, 10);
        Set Global Variable(M, Empty Array);
        Set Global Variable(N, Empty Array);
            Modify Global Variable(N, Append To Array, False);
            Modify Global Variable(N, Append To Array, Null);
            Modify Global Variable(N, Append To Array, Null);
            Modify Global Variable(N, Append To Array, 0);
        Set Global Variable(O, 0);
        Set Global Variable(P, Empty Array);
            Set Global Variable(I, Empty Array);
                Modify Global Variable(I, Append to Array, Add(Value In Array(Value In Array(Global Variable(K), 0), 0), Vector(0, 2, 0)));
                Modify Global Variable(I, Append to Array, 0.5);
                Modify Global Variable(I, Append to Array, 1);
                Set Global Variable At Index(P, 0, Global Variable(I));
            Set Global Variable(I, Empty Array);
                Modify Global Variable(I, Append to Array, Add(Value In Array(Value In Array(Global Variable(K), 0), 0), Vector(0, 3.1, 0)));
                Modify Global Variable(I, Append to Array, 0.5);
                Modify Global Variable(I, Append to Array, 2);
                Set Global Variable At Index(P, 1, Global Variable(I));
            Set Global Variable(I, Empty Array);
                Modify Global Variable(I, Append to Array, Add(Value In Array(Value In Array(Global Variable(K), 0), 0), Vector(0, 4.2, 0)));
                Modify Global Variable(I, Append to Array, 0.5);
                Modify Global Variable(I, Append to Array, 3);
                Set Global Variable At Index(P, 2, Global Variable(I));
            Set Global Variable(I, Empty Array);
                Modify Global Variable(I, Append to Array, Add(Value In Array(Value In Array(Global Variable(K), 0), 0), Vector(0, 5.3, 0)));
                Modify Global Variable(I, Append to Array, 0.5);
                Modify Global Variable(I, Append to Array, 4);
                Set Global Variable At Index(P, 3, Global Variable(I));
            Set Global Variable(I, Empty Array);
                Modify Global Variable(I, Append to Array, Add(Value In Array(Value In Array(Global Variable(K), 0), 0), Vector(0, 6.4, 0)));
                Modify Global Variable(I, Append to Array, 0.5);
                Modify Global Variable(I, Append to Array, 5);
                Set Global Variable At Index(P, 4, Global Variable(I));
        Set Global Variable(Q, Empty Array);
    }
}

rule("Init player vars")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	actions
	{
		Set Player Variable(Event Player, A, Empty Array);
        Set Player Variable(Event Player, B, 0);
        Set Player Variable(Event Player, C, False);
        Set Player Variable(Event Player, D, 0);
        Set Player Variable(Event Player, E, False);
        Set Player Variable(Event Player, F, False);
        Set Player Variable(Event Player, G, 0);
        Set Player Variable(Event Player, Z, Global Variable(A));
	}
}

rule("Display global UI")
{
	event
	{
		Ongoing - Global;
	}

	actions
	{
        // Current player
	    Create Effect(All Players(All Teams), Ring, White, Position Of(First Of(Filtered Array(All Players(All Teams), Player Variable(Current Array Element, C)))), 5, Visible To Position and Radius);
        
        // Pot
	    Create Effect(All Players(All Teams), Cloud, Yellow, Value In Array(Value In Array(Global Variable(K), 0), 0), Value In Array(Value In Array(Global Variable(K), 0), 1), Visible To Position and Radius);
        Create In-World Text(All Players(All Teams), Global Variable(C), Value In Array(Value In Array(Global Variable(K), 0), 0), 2,
		Do Not Clip, Visible To and String);
        
        // Buttons - fold, bet, call, raise, check
        Create Effect(All Players(All Teams), Sphere, White, Value In Array(Value In Array(Global Variable(P), 0), 0), Value In Array(Value In Array(Global Variable(P), 0), 1), Visible To Position and Radius);
        Create Effect(All Players(All Teams), Sphere, Green, Value In Array(Value In Array(Global Variable(P), 1), 0), Value In Array(Value In Array(Global Variable(P), 1), 1), Visible To Position and Radius);
        Create Effect(All Players(All Teams), Sphere, Blue, Value In Array(Value In Array(Global Variable(P), 2), 0), Value In Array(Value In Array(Global Variable(P), 2), 1), Visible To Position and Radius);
        Create Effect(All Players(All Teams), Sphere, Red, Value In Array(Value In Array(Global Variable(P), 3), 0), Value In Array(Value In Array(Global Variable(P), 3), 1), Visible To Position and Radius);
        Create Effect(All Players(All Teams), Sphere, Purple, Value In Array(Value In Array(Global Variable(P), 4), 0), Value In Array(Value In Array(Global Variable(P), 4), 1), Visible To Position and Radius);
        
        Create In-World Text(All Players(All Teams), String("Exit", Null, Null, Null), Value In Array(Value In Array(Global Variable(P), 0), 0), 1, Do Not Clip, Visible To and String);
        Create In-World Text(All Players(All Teams), String("Better", Null, Null, Null), Value In Array(Value In Array(Global Variable(P), 1), 0), 1, Do Not Clip, Visible To and String);
        Create In-World Text(All Players(All Teams), String("Fall", Null, Null, Null), Value In Array(Value In Array(Global Variable(P), 2), 0), 1, Do Not Clip, Visible To and String);
        Create In-World Text(All Players(All Teams), String("Raise", Null, Null, Null), Value In Array(Value In Array(Global Variable(P), 3), 0), 1, Do Not Clip, Visible To and String);
        Create In-World Text(All Players(All Teams), String("Checkpoint", Null, Null, Null), Value In Array(Value In Array(Global Variable(P), 4), 0), 1, Do Not Clip, Visible To and String);
	}
}

rule("Display player UI and settings")
{
	event
	{
		Ongoing - Each Player;
		All;
		All;
	}

	actions
	{
        // Physical properties
        Set Status(Event Player, Null, Invincible, 9999999);
        disabled Set Move Speed(Event Player, 0);

		// Chips/score
        Create In-World Text(All Players(All Teams), Player Variable(Event Player, Z), Add(Eye Position(Event Player), Vector(0, Value In Array(Global Variable(K), 3), 0)), 1, Do Not Clip, Visible To Position and String);

        // Current bet: add effect or something as a label
        Create In-World Text(All Players(All Teams), Player Variable(Event Player, B), Add(Position Of(Event Player), Multiply(Value In Array(Global Variable(K), 4), Normalize(Multiply(Vector(1, 0, 1), Vector Towards(Position Of(Event Player), Value In Array(Value In Array(Global Variable(K), 0), 0)))))), 1, Do Not Clip, Visible To Position and String);

        // Proposed bet
        Create HUD Text(All Players(All Teams), Player Variable(Event Player, G), Null, Null, Left, 0, White, White, White, Visible To and String);
	}
}

rule("Deck generation / init")
{
    event
	{
		Ongoing - Global;
	}

    conditions
    {
        Compare(Global Variable(Q), ==, Empty Array) == True;
    }

    actions
    {
        Wait(0.016, Ignore Condition);

        Skip If(Compare(Global Variable(Q), !=, Empty Array), 2);
        Modify Global Variable(Q, Append To Array, 2);
        Modify Global Variable(Q, Append To Array, 0);
        
        // Loop from gQ[0] = 2-14
            // Loop from gQ[1] = 0-3
                Set Global Variable(I, Empty Array);
                    Modify Global Variable(I, Append to Array, Value In Array(Global Variable(Q), 0));
                    Modify Global Variable(I, Append to Array, Value In Array(Global Variable(Q), 1));
                    Modify Global Variable(I, Append to Array, Value In Array(Value In Array(Global Variable(K), 0), 0));
                    Modify Global Variable(I, Append to Array, Null);
                    Set Global Variable At Index(M, Add(Multiply(4, Subtract(Value In Array(Global Variable(Q), 0), 2)), Value In Array(Global Variable(Q), 1)), Global Variable(I));
            Modify Global Variable At Index(Q, 1, Add, 1);
            Loop If(Compare(Value In Array(Global Variable(Q), 1), <=, 3));
            Set Global Variable At Index(Q, 1, 0);
        Modify Global Variable At Index(Q, 0, Add, 1);
	    Loop If(Compare(Value In Array(Global Variable(Q), 0), <=, 14));

        Set Global Variable(Q, 999);
    }
}

rule("Deck generation / create and link objects to their corresponding elements")
{
    event
	{
		Ongoing - Global;
	}

    conditions
    {
        Compare(Global Variable(Q), ==, 999) == True;
    }

    actions
    {
        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 0), 3),
            Sphere, Red,
            Value In Array(Value In Array(Global Variable(M), 0), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 0), 3),
            2,
            Value In Array(Value In Array(Global Variable(M), 0), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 1), 3),
            Sphere, Blue,
            Value In Array(Value In Array(Global Variable(M), 1), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 1), 3),
            2,
            Value In Array(Value In Array(Global Variable(M), 1), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 2), 3),
            Sphere, Yellow,
            Value In Array(Value In Array(Global Variable(M), 2), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 2), 3),
            2,
            Value In Array(Value In Array(Global Variable(M), 2), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 3), 3),
            Sphere, Green,
            Value In Array(Value In Array(Global Variable(M), 3), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 3), 3),
            2,
            Value In Array(Value In Array(Global Variable(M), 3), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 4), 3),
            Sphere, Red,
            Value In Array(Value In Array(Global Variable(M), 4), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 4), 3),
            3,
            Value In Array(Value In Array(Global Variable(M), 4), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 5), 3),
            Sphere, Blue,
            Value In Array(Value In Array(Global Variable(M), 5), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 5), 3),
            3,
            Value In Array(Value In Array(Global Variable(M), 5), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 6), 3),
            Sphere, Yellow,
            Value In Array(Value In Array(Global Variable(M), 6), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 6), 3),
            3,
            Value In Array(Value In Array(Global Variable(M), 6), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 7), 3),
            Sphere, Green,
            Value In Array(Value In Array(Global Variable(M), 7), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 7), 3),
            3,
            Value In Array(Value In Array(Global Variable(M), 7), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 8), 3),
            Sphere, Red,
            Value In Array(Value In Array(Global Variable(M), 8), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 8), 3),
            4,
            Value In Array(Value In Array(Global Variable(M), 8), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 9), 3),
            Sphere, Blue,
            Value In Array(Value In Array(Global Variable(M), 9), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 9), 3),
            4,
            Value In Array(Value In Array(Global Variable(M), 9), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 10), 3),
            Sphere, Yellow,
            Value In Array(Value In Array(Global Variable(M), 10), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 10), 3),
            4,
            Value In Array(Value In Array(Global Variable(M), 10), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 11), 3),
            Sphere, Green,
            Value In Array(Value In Array(Global Variable(M), 11), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 11), 3),
            4,
            Value In Array(Value In Array(Global Variable(M), 11), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 12), 3),
            Sphere, Red,
            Value In Array(Value In Array(Global Variable(M), 12), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 12), 3),
            5,
            Value In Array(Value In Array(Global Variable(M), 12), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 13), 3),
            Sphere, Blue,
            Value In Array(Value In Array(Global Variable(M), 13), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 13), 3),
            5,
            Value In Array(Value In Array(Global Variable(M), 13), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 14), 3),
            Sphere, Yellow,
            Value In Array(Value In Array(Global Variable(M), 14), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 14), 3),
            5,
            Value In Array(Value In Array(Global Variable(M), 14), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 15), 3),
            Sphere, Green,
            Value In Array(Value In Array(Global Variable(M), 15), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 15), 3),
            5,
            Value In Array(Value In Array(Global Variable(M), 15), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 16), 3),
            Sphere, Red,
            Value In Array(Value In Array(Global Variable(M), 16), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 16), 3),
            6,
            Value In Array(Value In Array(Global Variable(M), 16), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 17), 3),
            Sphere, Blue,
            Value In Array(Value In Array(Global Variable(M), 17), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 17), 3),
            6,
            Value In Array(Value In Array(Global Variable(M), 17), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 18), 3),
            Sphere, Yellow,
            Value In Array(Value In Array(Global Variable(M), 18), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 18), 3),
            6,
            Value In Array(Value In Array(Global Variable(M), 18), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 19), 3),
            Sphere, Green,
            Value In Array(Value In Array(Global Variable(M), 19), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 19), 3),
            6,
            Value In Array(Value In Array(Global Variable(M), 19), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 20), 3),
            Sphere, Red,
            Value In Array(Value In Array(Global Variable(M), 20), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 20), 3),
            7,
            Value In Array(Value In Array(Global Variable(M), 20), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 21), 3),
            Sphere, Blue,
            Value In Array(Value In Array(Global Variable(M), 21), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 21), 3),
            7,
            Value In Array(Value In Array(Global Variable(M), 21), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 22), 3),
            Sphere, Yellow,
            Value In Array(Value In Array(Global Variable(M), 22), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 22), 3),
            7,
            Value In Array(Value In Array(Global Variable(M), 22), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 23), 3),
            Sphere, Green,
            Value In Array(Value In Array(Global Variable(M), 23), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 23), 3),
            7,
            Value In Array(Value In Array(Global Variable(M), 23), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 24), 3),
            Sphere, Red,
            Value In Array(Value In Array(Global Variable(M), 24), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 24), 3),
            8,
            Value In Array(Value In Array(Global Variable(M), 24), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 25), 3),
            Sphere, Blue,
            Value In Array(Value In Array(Global Variable(M), 25), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 25), 3),
            8,
            Value In Array(Value In Array(Global Variable(M), 25), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 26), 3),
            Sphere, Yellow,
            Value In Array(Value In Array(Global Variable(M), 26), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 26), 3),
            8,
            Value In Array(Value In Array(Global Variable(M), 26), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 27), 3),
            Sphere, Green,
            Value In Array(Value In Array(Global Variable(M), 27), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 27), 3),
            8,
            Value In Array(Value In Array(Global Variable(M), 27), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 28), 3),
            Sphere, Red,
            Value In Array(Value In Array(Global Variable(M), 28), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 28), 3),
            9,
            Value In Array(Value In Array(Global Variable(M), 28), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 29), 3),
            Sphere, Blue,
            Value In Array(Value In Array(Global Variable(M), 29), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 29), 3),
            9,
            Value In Array(Value In Array(Global Variable(M), 29), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 30), 3),
            Sphere, Yellow,
            Value In Array(Value In Array(Global Variable(M), 30), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 30), 3),
            9,
            Value In Array(Value In Array(Global Variable(M), 30), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 31), 3),
            Sphere, Green,
            Value In Array(Value In Array(Global Variable(M), 31), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 31), 3),
            9,
            Value In Array(Value In Array(Global Variable(M), 31), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 32), 3),
            Sphere, Red,
            Value In Array(Value In Array(Global Variable(M), 32), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 32), 3),
            10,
            Value In Array(Value In Array(Global Variable(M), 32), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 33), 3),
            Sphere, Blue,
            Value In Array(Value In Array(Global Variable(M), 33), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 33), 3),
            10,
            Value In Array(Value In Array(Global Variable(M), 33), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 34), 3),
            Sphere, Yellow,
            Value In Array(Value In Array(Global Variable(M), 34), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 34), 3),
            10,
            Value In Array(Value In Array(Global Variable(M), 34), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 35), 3),
            Sphere, Green,
            Value In Array(Value In Array(Global Variable(M), 35), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 35), 3),
            10,
            Value In Array(Value In Array(Global Variable(M), 35), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 36), 3),
            Sphere, Red,
            Value In Array(Value In Array(Global Variable(M), 36), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 36), 3),
            Hero Icon String(Hero(Junkrat)),
            Value In Array(Value In Array(Global Variable(M), 36), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 37), 3),
            Sphere, Blue,
            Value In Array(Value In Array(Global Variable(M), 37), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 37), 3),
            Hero Icon String(Hero(Junkrat)),
            Value In Array(Value In Array(Global Variable(M), 37), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 38), 3),
            Sphere, Yellow,
            Value In Array(Value In Array(Global Variable(M), 38), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 38), 3),
            Hero Icon String(Hero(Junkrat)),
            Value In Array(Value In Array(Global Variable(M), 38), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 39), 3),
            Sphere, Green,
            Value In Array(Value In Array(Global Variable(M), 39), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 39), 3),
            Hero Icon String(Hero(Junkrat)),
            Value In Array(Value In Array(Global Variable(M), 39), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 40), 3),
            Sphere, Red,
            Value In Array(Value In Array(Global Variable(M), 40), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 40), 3),
            Hero Icon String(Hero(Mercy)),
            Value In Array(Value In Array(Global Variable(M), 40), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 41), 3),
            Sphere, Blue,
            Value In Array(Value In Array(Global Variable(M), 41), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 41), 3),
            Hero Icon String(Hero(Mercy)),
            Value In Array(Value In Array(Global Variable(M), 41), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 42), 3),
            Sphere, Yellow,
            Value In Array(Value In Array(Global Variable(M), 42), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 42), 3),
            Hero Icon String(Hero(Mercy)),
            Value In Array(Value In Array(Global Variable(M), 42), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 43), 3),
            Sphere, Green,
            Value In Array(Value In Array(Global Variable(M), 43), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 43), 3),
            Hero Icon String(Hero(Mercy)),
            Value In Array(Value In Array(Global Variable(M), 43), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 44), 3),
            Sphere, Red,
            Value In Array(Value In Array(Global Variable(M), 44), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 44), 3),
            Hero Icon String(Hero(Reinhardt)),
            Value In Array(Value In Array(Global Variable(M), 44), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 45), 3),
            Sphere, Blue,
            Value In Array(Value In Array(Global Variable(M), 45), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 45), 3),
            Hero Icon String(Hero(Reinhardt)),
            Value In Array(Value In Array(Global Variable(M), 45), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 46), 3),
            Sphere, Yellow,
            Value In Array(Value In Array(Global Variable(M), 46), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 46), 3),
            Hero Icon String(Hero(Reinhardt)),
            Value In Array(Value In Array(Global Variable(M), 46), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 47), 3),
            Sphere, Green,
            Value In Array(Value In Array(Global Variable(M), 47), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 47), 3),
            Hero Icon String(Hero(Reinhardt)),
            Value In Array(Value In Array(Global Variable(M), 47), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 48), 3),
            Sphere, Red,
            Value In Array(Value In Array(Global Variable(M), 48), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 48), 3),
            Hero Icon String(Hero(Tracer)),
            Value In Array(Value In Array(Global Variable(M), 48), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 49), 3),
            Sphere, Blue,
            Value In Array(Value In Array(Global Variable(M), 49), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 49), 3),
            Hero Icon String(Hero(Tracer)),
            Value In Array(Value In Array(Global Variable(M), 49), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 50), 3),
            Sphere, Yellow,
            Value In Array(Value In Array(Global Variable(M), 50), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 50), 3),
            Hero Icon String(Hero(Tracer)),
            Value In Array(Value In Array(Global Variable(M), 50), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 51), 3),
            Sphere, Green,
            Value In Array(Value In Array(Global Variable(M), 51), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 51), 3),
            Hero Icon String(Hero(Tracer)),
            Value In Array(Value In Array(Global Variable(M), 51), 2),
            1, Do Not Clip, Visible To Position and String
        );
    }
}

// Card manipulation___________________________________________________________
rule("Deal card")
{
    event
	{
		Ongoing - Global;
	}

    conditions
    {
        Compare(Value In Array(Global Variable(N), 0), ==, True) == True;
    }

    actions
    {
        // gN = interface for deal card{trigger, position, visible to, random card index in gM}
        Skip 5 if gN[3] != Null

        gN[3] = rand int(0, 51);

        // Update visible to
        // Note gI represents an element in gM to allow for 2D array manipulation.
        // Hence gI = {value, suit, center, visible to}.
        gI = gM[gN[3]]
        gI[3] = gN[2]
        gM[gN[3]] = gI

        // Update position
        // Loop until nearby
            gI[2] += Divide(Vector Towards(gI[2], gN[1]), gK[5])
            gM[gN[3]] = gI
        Wait 0.016, ignore conditions
        Loop if Compare(Distance Between(gI[2], gN[1]), <, 0.1)
        gI[2] = gN[1]
        gM[gN[3]] = gI
        
        // Add card to relevant array
        if gI[3] == All Players
            append gI to gD
        else
            append gI to pA(gI[3])
        
        gN[3] = Null
        gN[0] = false
    }
}

// Stages______________________________________________________________________

// begin round
{
    update gF by removing inactive players, appending new players and then moving the first element to the end (queue)

    wait until sizeof(gF) >= 2
        append remove any inactive players and append new players

    gG = gF

    // delete all cards
    for all players,
        remove all effects and texts in pA
        pA = empty array
    remove all effects and texts in gD

    // refresh deck


    // start off the preflop with blinds
    gC = gB*3
    gJ = 2*gB

    gH = 0
    gH++
    // handle bankruptcy - can't call or raise, dies
    if gH = sizeof(gG), set gH = 0

    pB at gG[gH] = gB
    pZ  at gG[gH] -= gB

    gH++
    if gH = sizeof(gG), set gH = 0

    pB at gG[gH] = 2*gB
    pZ at gG[gH] -= 2*gB

    // deal 2 cards to each player
    for (int gH = 1; pA for gG[gH] == empty array; gH++, if gH = sizeof(gG), set gH = 0)
        gN[1] = gG[gH]
        gN[2] = [position of gG[gH] + normalize(vector from (position(player), gK[0])) * (1,0,1)] * 2 + normalize(1, 0, -x component of previous vector / z component of previous vector)
        gN[0] = true
        
        wait until gN[0] = false

        gN[1] = gG[gH]
        gN[2] = [position of gG[gH] + normalize(vector from (position(player), gK[0])) * (1,0,1)] * 2 + normalize(-1, 0, x component of previous vector / z component of previous vector)
        gN[0] = true

    set gE to 1
}

// preflop
if gE == 1
{
    gH = 1
    gH++
    if gH = sizeof(gG), set gH = 0
    gH++
    if gH = sizeof(gG), set gH = 0
    pC at gG[gH] to true
}

// flop
if gE == 2
{
    // burn 1 card
    remove random element in gM

    // deal the first 3 community cards
    gO = 0
    gN[1] = all players
    gN[2] = gK[2][gO]
    gN[0] = true
    
    wait until gN[0] = false
    gO++
    gN[1] = all players
    gN[2] = gK[2][gO]
    gN[0] = true

    wait until gN[0] = false
    gO++
    gN[1] = all players
    gN[2] = gK[2][gO]
    gN[0] = true

    gH = 1
    pC at gG[gH] to true
}

// turn
if gE == 3
{
    // burn 1 card
    remove random element in gM

    // deal the 4th community card
    gO++
    gN[1] = all players
    gN[2] = gK[2][gO]
    gN[0] = true

    gH = 1
    pC at gG[gH] to true
}

// river
if gE == 4
{
    // burn 1 card
    remove random element in gM

    // deal the 5th community card
    gO++
    gN[1] = all players
    gN[2] = gK[2][gO]
    gN[0] = true

    gH = 1
    pC at gG[gH] to true
}

// showdown
if gE == 5
{
    // player vars have just been cleared
    // clear pot, clear player vars, etc.
    // get best 5-card hand from any amount of hole/community cards for each player, determine winner
    // gE = 0 etc.
    // the remaining players show their hands, with the bettor or last raiser showing first. The highest five-card hand wins the pot. (In case of a tie, the pot is evenly split among the winning hands.)
}

// end stage
if  - gE != 0
    - (sizeof(gG) == 1) OR (for all players in gG, pB are equal AND pF = true)
{
    set all pF = false
    set all pB = 0
    set all pG = 0
    gJ = 0
    
    if sizeof(gG == 1)
        end round

    gE++
}

// handle disconnect for gG
if gG contains a player that has disconnected
{
    gI = gG[gH]
    remove all disconnected players from gG
    gH = index of player gI in gG
}

// enforce minimum proposed raise/bet
if  - (pG < 2*gB) OR (pG < gJ) {
    pG = [(2*gB >= gJ) * 2*gB] + [(gJ > 2*gB) * gJ]
}

// change proposed bet/raise
if  - gE != 0
    - jumping
{
    pG += gL
}
if  - gE != 0
    - crouching
{
    pG -= gL
}

// auto fold

// Turns_______________________________________________________________________

// register turn
if pC == true and primary fire {
    set pD = [2] of first element of filtered array(hit detect; sort gP; center = current array element[0], radius = cae[1])
    pD = 0
}

    // fold
    if  - pC == true
        - pD == 1
    {
        remove player from gG
        remove all effects and texts in pA
        pA = empty array
        sleep and change camera
        gH--

        pE = true
    }

    // bet
    if  - pC == true
        - pD == 2
        - gJ == 0
        - pZ >= pG
    {
        gC += pG
        gJ = pG
        pB = pG
        pZ -= pG

        pG = 0
        pE = true
    }

    // call
    if  - pC == true
        - pD == 3
        - pB < gJ
        - pZ >= gJ - pB
    {
        gC += gJ - pB
        pB = gJ
        pZe -= (gJ - pB)

        pE = true
    }

    // raise
    if  - pC == true
        - pD == 4
        - gJ != 0
        - pZ >= pG
    {
        gC += pG
        gJ = pG
        pB = pG
        pZ -= pG

        pG = 0
        pE = true
    }

    // check
    if  - pC == true
        - pD == 5
        - pB == gJ
    {
        pE = true
    }

// next turn
if pE == true {
    pC = false
    pE == false
    pF = true

    abort if (sizeof(gG) == 1) OR (for all players in gG, pB are equal AND pF = true)

    gH++
    if gH = sizeof(gG), set gH = 0
    set pC of gG[gH] to true
}