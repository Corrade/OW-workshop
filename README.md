// All nested arrays unavoidably(?) have Null at index 0 and therefore essentially begin at index 1.
// Card values are from 2-14 and suits are from 0-3.

// gA = starting cash
// gB = small blind
// gC = pot
// gD = array of community cards {cards{value, suit}}
// gE = stage: resting value 0, preflop 1, flop 2, turn 3, river 4
// gF = player order for button rotation {button, small blind, big blind}
// gG = player order for betting in the ongoing round
// gH = index of current player in gG
// gI = temporary variable for updating gG in disconnect cases
// gJ = current stage leading bet
// gK = physical game object info {pot{center, radius}, community card positions[5], card sphere radius, chips/score units above eye level, current bet distance from feet, deal card speed}
// gL = modify bet increment
// gM = deck{value, suit, center, visible to, ...}. Hence each card has four adjacent values in gM. This unintuitive representation is necessary because it's impossible to change one specific element in a 2D array. A card's 0th index in gM = 4*(4*(value - 2) + suit).
// gN = interface for deal card{trigger, visible to, position, random card index in gM, temporary memory}
// gO = nth community card
// gP = buttons{buttons{center, radius, ID}}
// gQ = iterator vars and trigger for initialising gM

// pA = hand {cards{value, suit}}
// pB = player current stage bet
// pC = player turn or not
// pD = trigger for turn actions. resting value 0, fold bet call raise check = {1, 2, 3, 4, 5} respectively.
// pE = trigger for end turn module
// pF = acted this stage
// pG = proposed bet
// pZ = player score for testing purposes



//UI and housekeeping___________________________________________________________
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
            Modify Global Variable(K, Append To Array, Empty Array);
                Modify Global Variable At Index(K, 0, Append To Array, Vector(-210, 0, -18));
                Modify Global Variable At Index(K, 0, Append To Array, 1);
            Modify Global Variable(K, Append To Array, Empty Array);
                Modify Global Variable At Index(K, 1, Append To Array, Add(Value In Array(Global Variable(K), 0), Vector(1, 0, 0)));
                Modify Global Variable At Index(K, 1, Append To Array, Add(Value In Array(Global Variable(K), 0), Vector(2, 0, 0)));
                Modify Global Variable At Index(K, 1, Append To Array, Add(Value In Array(Global Variable(K), 0), Vector(3, 0, 0)));
                Modify Global Variable At Index(K, 1, Append To Array, Add(Value In Array(Global Variable(K), 0), Vector(4, 0, 0)));
                Modify Global Variable At Index(K, 1, Append To Array, Add(Value In Array(Global Variable(K), 0), Vector(5, 0, 0)));
            Modify Global Variable(K, Append To Array, 0.175);
            Modify Global Variable(K, Append To Array, 0.5);
            Modify Global Variable(K, Append To Array, 1.5);
            Modify Global Variable(K, Append To Array, 6);
        Set Global Variable(L, 10);
        Set Global Variable(M, Empty Array);
        Set Global Variable(N, Empty Array);
            Modify Global Variable(N, Append To Array, False);
            Modify Global Variable(N, Append To Array, Null);
            Modify Global Variable(N, Append To Array, Null);
            Modify Global Variable(N, Append To Array, 0);
            Modify Global Variable(N, Append To Array, Empty Array);
        Set Global Variable(O, 0);
        Set Global Variable(P, Empty Array);
            Modify Global Variable(P, Append To Array, Empty Array);
                Modify Global Variable At Index(P, 0, Append To Array, Add(Value In Array(Value In Array(Global Variable(K), 0), 1), Vector(0, 2, 0)));
                Modify Global Variable At Index(P, 0, Append To Array, 0.5);
                Modify Global Variable At Index(P, 0, Append To Array, 1);
            Modify Global Variable(P, Append To Array, Empty Array);
                Modify Global Variable At Index(P, 1, Append To Array, Add(Value In Array(Value In Array(Global Variable(K), 0), 1), Vector(0, 3.1, 0)));
                Modify Global Variable At Index(P, 1, Append To Array, 0.5);
                Modify Global Variable At Index(P, 1, Append To Array, 2);
            Modify Global Variable(P, Append To Array, Empty Array);
                Modify Global Variable At Index(P, 2, Append To Array, Add(Value In Array(Value In Array(Global Variable(K), 0), 1), Vector(0, 4.2, 0)));
                Modify Global Variable At Index(P, 2, Append To Array, 0.5);
                Modify Global Variable At Index(P, 2, Append To Array, 3);
            Modify Global Variable(P, Append To Array, Empty Array);
                Modify Global Variable At Index(P, 3, Append To Array, Add(Value In Array(Value In Array(Global Variable(K), 0), 1), Vector(0, 5.3, 0)));
                Modify Global Variable At Index(P, 3, Append To Array, 0.5);
                Modify Global Variable At Index(P, 3, Append To Array, 4);
            Modify Global Variable(P, Append To Array, Empty Array);
                Modify Global Variable At Index(P, 4, Append To Array, Add(Value In Array(Value In Array(Global Variable(K), 0), 1), Vector(0, 6.4, 0)));
                Modify Global Variable At Index(P, 4, Append To Array, 0.5);
                Modify Global Variable At Index(P, 4, Append To Array, 5);
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
	    Create Effect(All Players(All Teams), Cloud, Yellow, Value In Array(Value In Array(Global Variable(K), 0), 1), Value In Array(Value In Array(Global Variable(K), 0), 2), Visible To Position and Radius);
        Create In-World Text(All Players(All Teams), Global Variable(C), Value In Array(Value In Array(Global Variable(K), 0), 1), 2,
		Do Not Clip, Visible To and String);
        
        // Buttons - fold, bet, call, raise, check
        Create Effect(All Players(All Teams), Sphere, White, Value In Array(Value In Array(Global Variable(P), 0), 1), Value In Array(Value In Array(Global Variable(P), 0), 2), Visible To Position and Radius);
        Create Effect(All Players(All Teams), Sphere, Green, Value In Array(Value In Array(Global Variable(P), 1), 1), Value In Array(Value In Array(Global Variable(P), 1), 2), Visible To Position and Radius);
        Create Effect(All Players(All Teams), Sphere, Blue, Value In Array(Value In Array(Global Variable(P), 2), 1), Value In Array(Value In Array(Global Variable(P), 2), 2), Visible To Position and Radius);
        Create Effect(All Players(All Teams), Sphere, Red, Value In Array(Value In Array(Global Variable(P), 3), 1), Value In Array(Value In Array(Global Variable(P), 3), 2), Visible To Position and Radius);
        Create Effect(All Players(All Teams), Sphere, Purple, Value In Array(Value In Array(Global Variable(P), 4), 1), Value In Array(Value In Array(Global Variable(P), 4), 2), Visible To Position and Radius);
        
        Create In-World Text(All Players(All Teams), String("Exit", Null, Null, Null), Value In Array(Value In Array(Global Variable(P), 0), 1), 1, Do Not Clip, Visible To and String);
        Create In-World Text(All Players(All Teams), String("Better", Null, Null, Null), Value In Array(Value In Array(Global Variable(P), 1), 1), 1, Do Not Clip, Visible To and String);
        Create In-World Text(All Players(All Teams), String("Fall", Null, Null, Null), Value In Array(Value In Array(Global Variable(P), 2), 1), 1, Do Not Clip, Visible To and String);
        Create In-World Text(All Players(All Teams), String("Raise", Null, Null, Null), Value In Array(Value In Array(Global Variable(P), 3), 1), 1, Do Not Clip, Visible To and String);
        Create In-World Text(All Players(All Teams), String("Checkpoint", Null, Null, Null), Value In Array(Value In Array(Global Variable(P), 4), 1), 1, Do Not Clip, Visible To and String);
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
        Create In-World Text(All Players(All Teams), Player Variable(Event Player, B), Add(Position Of(Event Player), Multiply(Value In Array(Global Variable(K), 4), Normalize(Multiply(Vector(1, 0, 1), Vector Towards(Position Of(Event Player), Value In Array(Value In Array(Global Variable(K), 0), 1)))))), 1, Do Not Clip, Visible To Position and String);

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
                // Append gQ[0], gQ[1], gK[0][1], Null to gM
                Modify Global Variable(M, Append To Array, Value In Array(Global Variable(Q), 0));
                Modify Global Variable(M, Append To Array, Value In Array(Global Variable(Q), 1));
                Modify Global Variable(M, Append To Array, Value In Array(Value In Array(Global Variable(K), 0), 1));
                Modify Global Variable(M, Append To Array, Null);
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
        // See deck_gen.c
        Create Effect(
            Value In Array(Global Variable(M), 3),
            Sphere, Red,
            Value In Array(Global Variable(M), 2),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 3),
            2,
            Value In Array(Global Variable(M), 2),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 7),
            Sphere, Blue,
            Value In Array(Global Variable(M), 6),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 7),
            2,
            Value In Array(Global Variable(M), 6),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 11),
            Sphere, Yellow,
            Value In Array(Global Variable(M), 10),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 11),
            2,
            Value In Array(Global Variable(M), 10),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 15),
            Sphere, Green,
            Value In Array(Global Variable(M), 14),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 15),
            2,
            Value In Array(Global Variable(M), 14),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 19),
            Sphere, Red,
            Value In Array(Global Variable(M), 18),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 19),
            3,
            Value In Array(Global Variable(M), 18),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 23),
            Sphere, Blue,
            Value In Array(Global Variable(M), 22),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 23),
            3,
            Value In Array(Global Variable(M), 22),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 27),
            Sphere, Yellow,
            Value In Array(Global Variable(M), 26),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 27),
            3,
            Value In Array(Global Variable(M), 26),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 31),
            Sphere, Green,
            Value In Array(Global Variable(M), 30),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 31),
            3,
            Value In Array(Global Variable(M), 30),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 35),
            Sphere, Red,
            Value In Array(Global Variable(M), 34),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 35),
            4,
            Value In Array(Global Variable(M), 34),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 39),
            Sphere, Blue,
            Value In Array(Global Variable(M), 38),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 39),
            4,
            Value In Array(Global Variable(M), 38),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 43),
            Sphere, Yellow,
            Value In Array(Global Variable(M), 42),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 43),
            4,
            Value In Array(Global Variable(M), 42),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 47),
            Sphere, Green,
            Value In Array(Global Variable(M), 46),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 47),
            4,
            Value In Array(Global Variable(M), 46),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 51),
            Sphere, Red,
            Value In Array(Global Variable(M), 50),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 51),
            5,
            Value In Array(Global Variable(M), 50),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 55),
            Sphere, Blue,
            Value In Array(Global Variable(M), 54),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 55),
            5,
            Value In Array(Global Variable(M), 54),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 59),
            Sphere, Yellow,
            Value In Array(Global Variable(M), 58),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 59),
            5,
            Value In Array(Global Variable(M), 58),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 63),
            Sphere, Green,
            Value In Array(Global Variable(M), 62),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 63),
            5,
            Value In Array(Global Variable(M), 62),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 67),
            Sphere, Red,
            Value In Array(Global Variable(M), 66),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 67),
            6,
            Value In Array(Global Variable(M), 66),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 71),
            Sphere, Blue,
            Value In Array(Global Variable(M), 70),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 71),
            6,
            Value In Array(Global Variable(M), 70),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 75),
            Sphere, Yellow,
            Value In Array(Global Variable(M), 74),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 75),
            6,
            Value In Array(Global Variable(M), 74),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 79),
            Sphere, Green,
            Value In Array(Global Variable(M), 78),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 79),
            6,
            Value In Array(Global Variable(M), 78),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 83),
            Sphere, Red,
            Value In Array(Global Variable(M), 82),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 83),
            7,
            Value In Array(Global Variable(M), 82),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 87),
            Sphere, Blue,
            Value In Array(Global Variable(M), 86),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 87),
            7,
            Value In Array(Global Variable(M), 86),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 91),
            Sphere, Yellow,
            Value In Array(Global Variable(M), 90),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 91),
            7,
            Value In Array(Global Variable(M), 90),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 95),
            Sphere, Green,
            Value In Array(Global Variable(M), 94),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 95),
            7,
            Value In Array(Global Variable(M), 94),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 99),
            Sphere, Red,
            Value In Array(Global Variable(M), 98),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 99),
            8,
            Value In Array(Global Variable(M), 98),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 103),
            Sphere, Blue,
            Value In Array(Global Variable(M), 102),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 103),
            8,
            Value In Array(Global Variable(M), 102),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 107),
            Sphere, Yellow,
            Value In Array(Global Variable(M), 106),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 107),
            8,
            Value In Array(Global Variable(M), 106),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 111),
            Sphere, Green,
            Value In Array(Global Variable(M), 110),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 111),
            8,
            Value In Array(Global Variable(M), 110),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 115),
            Sphere, Red,
            Value In Array(Global Variable(M), 114),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 115),
            9,
            Value In Array(Global Variable(M), 114),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 119),
            Sphere, Blue,
            Value In Array(Global Variable(M), 118),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 119),
            9,
            Value In Array(Global Variable(M), 118),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 123),
            Sphere, Yellow,
            Value In Array(Global Variable(M), 122),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 123),
            9,
            Value In Array(Global Variable(M), 122),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 127),
            Sphere, Green,
            Value In Array(Global Variable(M), 126),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 127),
            9,
            Value In Array(Global Variable(M), 126),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 131),
            Sphere, Red,
            Value In Array(Global Variable(M), 130),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 131),
            10,
            Value In Array(Global Variable(M), 130),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 135),
            Sphere, Blue,
            Value In Array(Global Variable(M), 134),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 135),
            10,
            Value In Array(Global Variable(M), 134),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 139),
            Sphere, Yellow,
            Value In Array(Global Variable(M), 138),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 139),
            10,
            Value In Array(Global Variable(M), 138),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 143),
            Sphere, Green,
            Value In Array(Global Variable(M), 142),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 143),
            10,
            Value In Array(Global Variable(M), 142),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 147),
            Sphere, Red,
            Value In Array(Global Variable(M), 146),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 147),
            Hero Icon String(Hero(Junkrat)),
            Value In Array(Global Variable(M), 146),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 151),
            Sphere, Blue,
            Value In Array(Global Variable(M), 150),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 151),
            Hero Icon String(Hero(Junkrat)),
            Value In Array(Global Variable(M), 150),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 155),
            Sphere, Yellow,
            Value In Array(Global Variable(M), 154),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 155),
            Hero Icon String(Hero(Junkrat)),
            Value In Array(Global Variable(M), 154),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 159),
            Sphere, Green,
            Value In Array(Global Variable(M), 158),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 159),
            Hero Icon String(Hero(Junkrat)),
            Value In Array(Global Variable(M), 158),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 163),
            Sphere, Red,
            Value In Array(Global Variable(M), 162),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 163),
            Hero Icon String(Hero(Mercy)),
            Value In Array(Global Variable(M), 162),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 167),
            Sphere, Blue,
            Value In Array(Global Variable(M), 166),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 167),
            Hero Icon String(Hero(Mercy)),
            Value In Array(Global Variable(M), 166),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 171),
            Sphere, Yellow,
            Value In Array(Global Variable(M), 170),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 171),
            Hero Icon String(Hero(Mercy)),
            Value In Array(Global Variable(M), 170),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 175),
            Sphere, Green,
            Value In Array(Global Variable(M), 174),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 175),
            Hero Icon String(Hero(Mercy)),
            Value In Array(Global Variable(M), 174),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 179),
            Sphere, Red,
            Value In Array(Global Variable(M), 178),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 179),
            Hero Icon String(Hero(Reinhardt)),
            Value In Array(Global Variable(M), 178),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 183),
            Sphere, Blue,
            Value In Array(Global Variable(M), 182),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 183),
            Hero Icon String(Hero(Reinhardt)),
            Value In Array(Global Variable(M), 182),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 187),
            Sphere, Yellow,
            Value In Array(Global Variable(M), 186),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 187),
            Hero Icon String(Hero(Reinhardt)),
            Value In Array(Global Variable(M), 186),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 191),
            Sphere, Green,
            Value In Array(Global Variable(M), 190),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 191),
            Hero Icon String(Hero(Reinhardt)),
            Value In Array(Global Variable(M), 190),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 195),
            Sphere, Red,
            Value In Array(Global Variable(M), 194),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 195),
            Hero Icon String(Hero(Tracer)),
            Value In Array(Global Variable(M), 194),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 199),
            Sphere, Blue,
            Value In Array(Global Variable(M), 198),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 199),
            Hero Icon String(Hero(Tracer)),
            Value In Array(Global Variable(M), 198),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 203),
            Sphere, Yellow,
            Value In Array(Global Variable(M), 202),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 203),
            Hero Icon String(Hero(Tracer)),
            Value In Array(Global Variable(M), 202),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Global Variable(M), 207),
            Sphere, Green,
            Value In Array(Global Variable(M), 206),
            Value In Array(Global Variable(K), 2),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Global Variable(M), 207),
            Hero Icon String(Hero(Tracer)),
            Value In Array(Global Variable(M), 206),
            1, Do Not Clip, Visible To Position and String
        );
    }
}

//card dealing__________________________________________________________________
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
        skip 2 if gN[3] != Null

        gN[3] = 4*rand int(0, 51);
        gM[gN[3] + 3] = gN[1]

        // Loop until nearby
            gM[gN[3] + 2] += Vector Towards(gM[gN[3] + 2], gN[2]) / gK[5]
            wait 0.016, ignore conditions
        loop if Distance Between(gM[gN[3] + 2], gN[2]) < 0.1

        gM[gN[3] + 2] = gN[2]
        
        gN[4] = Empty Array
        append gM[gN[3] + 0] to gN[4]
        append gM[gN[3] + 1] to gN[4]

        if gN[1] == All Players
            append gN[4] to gD
        else
            append gN[4] to pA(gn[1])
        
        gN[3] = Null
        gN[0] = false
    }
}

//stages________________________________________________________________________

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

//turns________________________________________________________________________

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