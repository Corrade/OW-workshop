gA = starting cash
gB = small blind (big blind *2)
gC = pot
gD = array of community cards
gE = stage: resting value 0, preflop 1, flop 2, turn 3, river 4
gF = player order for button rotation. gF[0] = button, gF[1] = small blind, gF[2] = big blind.
gG = player order for betting in the ongoing round.
gH = index of current player in gG
gI = temporary variable for updating gG in disconnect cases
gJ = current stage leading bet
gK = physical game object info {pot{center, radius}, community card positions[5], card sphere radius, chips/score units above eye level, current bet distance from feet}
gL = modify bet increment
gM = deck{cards{value from 2-14, suit from 0-3, center, visible to, effect ID, text ID}}
gN = deal card interface
    gN[0] = trigger
    gN[1] = visible to
    gN[2] = position
    gN[3] = random value in gM
    gN[4] = temporary memory for module
gO = nth community card
gP = buttons{buttons{center, radius, ID}}
gQ = iterator vars and trigger for initialising gM

pA = array, hand
pB = player current stage bet
pC = player turn or not
pD = trigger for turn actions. resting value 0, fold bet call raise check = {1, 2, 3, 4, 5} respectively.
pE = trigger for end turn module
pF = acted this stage
pG = proposed bet
pZ = player score for testing purposes

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
        // Note: all 2D arrays unavoidably(?) have Null at index 0
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
                Modify Global Variable At Index(K, 0, Append To Array, 2);
            Modify Global Variable(K, Append To Array, Empty Array);
                Modify Global Variable At Index(K, 1, Append To Array, Add(Value In Array(Global Variable(K), 0), Vector(1, 0, 0)));
                Modify Global Variable At Index(K, 1, Append To Array, Add(Value In Array(Global Variable(K), 0), Vector(2, 0, 0)));
                Modify Global Variable At Index(K, 1, Append To Array, Add(Value In Array(Global Variable(K), 0), Vector(3, 0, 0)));
                Modify Global Variable At Index(K, 1, Append To Array, Add(Value In Array(Global Variable(K), 0), Vector(4, 0, 0)));
                Modify Global Variable At Index(K, 1, Append To Array, Add(Value In Array(Global Variable(K), 0), Vector(5, 0, 0)));
            Modify Global Variable(K, Append To Array, 0.25);
            Modify Global Variable(K, Append To Array, 0.5);
            Modify Global Variable(K, Append To Array, 1.5);
        Set Global Variable(L, 10);
        Set Global Variable(M, Empty Array);
        Set Global Variable(N, Empty Array);
            Modify Global Variable(N, Append To Array, False);
            Modify Global Variable(N, Append To Array, Null);
            Modify Global Variable(N, Append To Array, Null);
            Modify Global Variable(N, Append To Array, Null);
            Modify Global Variable(N, Append To Array, Null);
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

rule("Deck generation: init gM")
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
        // gM = deck{cards{value from 2-14, suit from 0-3, center, visible to}}
        Wait(0.016, Ignore Condition);

        Skip If(Compare(Global Variable(Q), !=, Empty Array), 2);
        Modify Global Variable(Q, Append To Array, 2);
        Modify Global Variable(Q, Append To Array, 0);
        
        // Loop from gQ[0] = 2-14
            // Loop from gQ[1] = 0-3
                // Append to gM gQ[0], gQ[1], gK[0][1], Null
                Modify Global Variable(M, Append To Array, Empty Array);
                    Modify Global Variable At Index(M, Add(Multiply(4, Subtract(Value In Array(Global Variable(Q), 0), 2)), Value In Array(Global Variable(Q), 1)), Append To Array, Value In Array(Global Variable(Q), 0));
                    Modify Global Variable At Index(M, Add(Multiply(4, Subtract(Value In Array(Global Variable(Q), 0), 2)), Value In Array(Global Variable(Q), 1)), Append To Array, Value In Array(Global Variable(Q), 1));
                    Modify Global Variable At Index(M, Add(Multiply(4, Subtract(Value In Array(Global Variable(Q), 0), 2)), Value In Array(Global Variable(Q), 1)), Append To Array, Value In Array(Value In Array(Global Variable(K), 0), 1));
                    Modify Global Variable At Index(M, Add(Multiply(4, Subtract(Value In Array(Global Variable(Q), 0), 2)), Value In Array(Global Variable(Q), 1)), Append To Array, Null);
            Modify Global Variable At Index(Q, 1, Add, 1);
            Loop If(Compare(Value In Array(Global Variable(Q), 1), <=, 3));
            Set Global Variable At Index(Q, 1, 0);
        Modify Global Variable At Index(Q, 0, Add, 1);
	    Loop If(Compare(Value In Array(Global Variable(Q), 0), <=, 14));

        Set Global Variable(Q, 999);
    }
}

rule("Deck generation: create and link objects to their corresponding elements in gM")
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
            Value In Array(Value In Array(Global Variable(M), 1)), 4),
            Sphere, Red,
            Value In Array(Value In Array(Global Variable(M), 1)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 1)), 4),
            2,
            Value In Array(Value In Array(Global Variable(M), 1)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 2)), 4),
            Sphere, Blue,
            Value In Array(Value In Array(Global Variable(M), 2)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 2)), 4),
            2,
            Value In Array(Value In Array(Global Variable(M), 2)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 3)), 4),
            Sphere, Yellow,
            Value In Array(Value In Array(Global Variable(M), 3)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 3)), 4),
            2,
            Value In Array(Value In Array(Global Variable(M), 3)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 4)), 4),
            Sphere, Green,
            Value In Array(Value In Array(Global Variable(M), 4)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 4)), 4),
            2,
            Value In Array(Value In Array(Global Variable(M), 4)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 5)), 4),
            Sphere, Red,
            Value In Array(Value In Array(Global Variable(M), 5)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 5)), 4),
            3,
            Value In Array(Value In Array(Global Variable(M), 5)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 6)), 4),
            Sphere, Blue,
            Value In Array(Value In Array(Global Variable(M), 6)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 6)), 4),
            3,
            Value In Array(Value In Array(Global Variable(M), 6)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 7)), 4),
            Sphere, Yellow,
            Value In Array(Value In Array(Global Variable(M), 7)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 7)), 4),
            3,
            Value In Array(Value In Array(Global Variable(M), 7)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 8)), 4),
            Sphere, Green,
            Value In Array(Value In Array(Global Variable(M), 8)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 8)), 4),
            3,
            Value In Array(Value In Array(Global Variable(M), 8)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 9)), 4),
            Sphere, Red,
            Value In Array(Value In Array(Global Variable(M), 9)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 9)), 4),
            4,
            Value In Array(Value In Array(Global Variable(M), 9)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 10)), 4),
            Sphere, Blue,
            Value In Array(Value In Array(Global Variable(M), 10)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 10)), 4),
            4,
            Value In Array(Value In Array(Global Variable(M), 10)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 11)), 4),
            Sphere, Yellow,
            Value In Array(Value In Array(Global Variable(M), 11)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 11)), 4),
            4,
            Value In Array(Value In Array(Global Variable(M), 11)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 12)), 4),
            Sphere, Green,
            Value In Array(Value In Array(Global Variable(M), 12)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 12)), 4),
            4,
            Value In Array(Value In Array(Global Variable(M), 12)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 13)), 4),
            Sphere, Red,
            Value In Array(Value In Array(Global Variable(M), 13)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 13)), 4),
            5,
            Value In Array(Value In Array(Global Variable(M), 13)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 14)), 4),
            Sphere, Blue,
            Value In Array(Value In Array(Global Variable(M), 14)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 14)), 4),
            5,
            Value In Array(Value In Array(Global Variable(M), 14)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 15)), 4),
            Sphere, Yellow,
            Value In Array(Value In Array(Global Variable(M), 15)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 15)), 4),
            5,
            Value In Array(Value In Array(Global Variable(M), 15)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 16)), 4),
            Sphere, Green,
            Value In Array(Value In Array(Global Variable(M), 16)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 16)), 4),
            5,
            Value In Array(Value In Array(Global Variable(M), 16)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 17)), 4),
            Sphere, Red,
            Value In Array(Value In Array(Global Variable(M), 17)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 17)), 4),
            6,
            Value In Array(Value In Array(Global Variable(M), 17)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 18)), 4),
            Sphere, Blue,
            Value In Array(Value In Array(Global Variable(M), 18)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 18)), 4),
            6,
            Value In Array(Value In Array(Global Variable(M), 18)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 19)), 4),
            Sphere, Yellow,
            Value In Array(Value In Array(Global Variable(M), 19)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 19)), 4),
            6,
            Value In Array(Value In Array(Global Variable(M), 19)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 20)), 4),
            Sphere, Green,
            Value In Array(Value In Array(Global Variable(M), 20)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 20)), 4),
            6,
            Value In Array(Value In Array(Global Variable(M), 20)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 21)), 4),
            Sphere, Red,
            Value In Array(Value In Array(Global Variable(M), 21)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 21)), 4),
            7,
            Value In Array(Value In Array(Global Variable(M), 21)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 22)), 4),
            Sphere, Blue,
            Value In Array(Value In Array(Global Variable(M), 22)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 22)), 4),
            7,
            Value In Array(Value In Array(Global Variable(M), 22)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 23)), 4),
            Sphere, Yellow,
            Value In Array(Value In Array(Global Variable(M), 23)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 23)), 4),
            7,
            Value In Array(Value In Array(Global Variable(M), 23)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 24)), 4),
            Sphere, Green,
            Value In Array(Value In Array(Global Variable(M), 24)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 24)), 4),
            7,
            Value In Array(Value In Array(Global Variable(M), 24)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 25)), 4),
            Sphere, Red,
            Value In Array(Value In Array(Global Variable(M), 25)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 25)), 4),
            8,
            Value In Array(Value In Array(Global Variable(M), 25)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 26)), 4),
            Sphere, Blue,
            Value In Array(Value In Array(Global Variable(M), 26)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 26)), 4),
            8,
            Value In Array(Value In Array(Global Variable(M), 26)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 27)), 4),
            Sphere, Yellow,
            Value In Array(Value In Array(Global Variable(M), 27)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 27)), 4),
            8,
            Value In Array(Value In Array(Global Variable(M), 27)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 28)), 4),
            Sphere, Green,
            Value In Array(Value In Array(Global Variable(M), 28)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 28)), 4),
            8,
            Value In Array(Value In Array(Global Variable(M), 28)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 29)), 4),
            Sphere, Red,
            Value In Array(Value In Array(Global Variable(M), 29)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 29)), 4),
            9,
            Value In Array(Value In Array(Global Variable(M), 29)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 30)), 4),
            Sphere, Blue,
            Value In Array(Value In Array(Global Variable(M), 30)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 30)), 4),
            9,
            Value In Array(Value In Array(Global Variable(M), 30)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 31)), 4),
            Sphere, Yellow,
            Value In Array(Value In Array(Global Variable(M), 31)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 31)), 4),
            9,
            Value In Array(Value In Array(Global Variable(M), 31)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 32)), 4),
            Sphere, Green,
            Value In Array(Value In Array(Global Variable(M), 32)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 32)), 4),
            9,
            Value In Array(Value In Array(Global Variable(M), 32)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 33)), 4),
            Sphere, Red,
            Value In Array(Value In Array(Global Variable(M), 33)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 33)), 4),
            10,
            Value In Array(Value In Array(Global Variable(M), 33)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 34)), 4),
            Sphere, Blue,
            Value In Array(Value In Array(Global Variable(M), 34)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 34)), 4),
            10,
            Value In Array(Value In Array(Global Variable(M), 34)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 35)), 4),
            Sphere, Yellow,
            Value In Array(Value In Array(Global Variable(M), 35)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 35)), 4),
            10,
            Value In Array(Value In Array(Global Variable(M), 35)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 36)), 4),
            Sphere, Green,
            Value In Array(Value In Array(Global Variable(M), 36)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 36)), 4),
            10,
            Value In Array(Value In Array(Global Variable(M), 36)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 37)), 4),
            Sphere, Red,
            Value In Array(Value In Array(Global Variable(M), 37)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 37)), 4),
            Hero Icon String(Hero(Junkrat)),
            Value In Array(Value In Array(Global Variable(M), 37)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 38)), 4),
            Sphere, Blue,
            Value In Array(Value In Array(Global Variable(M), 38)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 38)), 4),
            Hero Icon String(Hero(Junkrat)),
            Value In Array(Value In Array(Global Variable(M), 38)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 39)), 4),
            Sphere, Yellow,
            Value In Array(Value In Array(Global Variable(M), 39)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 39)), 4),
            Hero Icon String(Hero(Junkrat)),
            Value In Array(Value In Array(Global Variable(M), 39)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 40)), 4),
            Sphere, Green,
            Value In Array(Value In Array(Global Variable(M), 40)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 40)), 4),
            Hero Icon String(Hero(Junkrat)),
            Value In Array(Value In Array(Global Variable(M), 40)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 41)), 4),
            Sphere, Red,
            Value In Array(Value In Array(Global Variable(M), 41)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 41)), 4),
            Hero Icon String(Hero(Mercy)),
            Value In Array(Value In Array(Global Variable(M), 41)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 42)), 4),
            Sphere, Blue,
            Value In Array(Value In Array(Global Variable(M), 42)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 42)), 4),
            Hero Icon String(Hero(Mercy)),
            Value In Array(Value In Array(Global Variable(M), 42)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 43)), 4),
            Sphere, Yellow,
            Value In Array(Value In Array(Global Variable(M), 43)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 43)), 4),
            Hero Icon String(Hero(Mercy)),
            Value In Array(Value In Array(Global Variable(M), 43)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 44)), 4),
            Sphere, Green,
            Value In Array(Value In Array(Global Variable(M), 44)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 44)), 4),
            Hero Icon String(Hero(Mercy)),
            Value In Array(Value In Array(Global Variable(M), 44)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 45)), 4),
            Sphere, Red,
            Value In Array(Value In Array(Global Variable(M), 45)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 45)), 4),
            Hero Icon String(Hero(Reinhardt)),
            Value In Array(Value In Array(Global Variable(M), 45)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 46)), 4),
            Sphere, Blue,
            Value In Array(Value In Array(Global Variable(M), 46)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 46)), 4),
            Hero Icon String(Hero(Reinhardt)),
            Value In Array(Value In Array(Global Variable(M), 46)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 47)), 4),
            Sphere, Yellow,
            Value In Array(Value In Array(Global Variable(M), 47)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 47)), 4),
            Hero Icon String(Hero(Reinhardt)),
            Value In Array(Value In Array(Global Variable(M), 47)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 48)), 4),
            Sphere, Green,
            Value In Array(Value In Array(Global Variable(M), 48)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 48)), 4),
            Hero Icon String(Hero(Reinhardt)),
            Value In Array(Value In Array(Global Variable(M), 48)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 49)), 4),
            Sphere, Red,
            Value In Array(Value In Array(Global Variable(M), 49)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 49)), 4),
            Hero Icon String(Hero(Tracer)),
            Value In Array(Value In Array(Global Variable(M), 49)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 50)), 4),
            Sphere, Blue,
            Value In Array(Value In Array(Global Variable(M), 50)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 50)), 4),
            Hero Icon String(Hero(Tracer)),
            Value In Array(Value In Array(Global Variable(M), 50)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 51)), 4),
            Sphere, Yellow,
            Value In Array(Value In Array(Global Variable(M), 51)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 51)), 4),
            Hero Icon String(Hero(Tracer)),
            Value In Array(Value In Array(Global Variable(M), 51)), 3),
            1, Do Not Clip, Visible To Position and String
        );

        Create Effect(
            Value In Array(Value In Array(Global Variable(M), 52)), 4),
            Sphere, Green,
            Value In Array(Value In Array(Global Variable(M), 52)), 3),
            Value In Array(Global Variable(K), 3),
            Visible To Position and Radius
        );
        Create In-World Text(
            Value In Array(Value In Array(Global Variable(M), 52)), 4),
            Hero Icon String(Hero(Tracer)),
            Value In Array(Value In Array(Global Variable(M), 52)), 3),
            1, Do Not Clip, Visible To Position and String
        );
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
	    Create Effect(All Players(All Teams), Orb, Yellow, Value In Array(Value In Array(Global Variable(K), 0), 1), Value In Array(Value In Array(Global Variable(K), 0), 2), Visible To);
        Create In-World Text(All Players(All Teams), Global Variable(C), Value In Array(Value In Array(Global Variable(K), 0), 1), 2,
		Do Not Clip, Visible To and String);
        
        // Buttons - fold, bet, call, raise, check
        Create Effect(All Players(All Teams), Sphere, White, Value In Array(Value In Array(Global Variable(P), 0), 1), Value In Array(Value In Array(Global Variable(P), 0), 2), Visible To);
        Create Effect(All Players(All Teams), Sphere, Green, Value In Array(Value In Array(Global Variable(P), 1), 1), Value In Array(Value In Array(Global Variable(P), 1), 2), Visible To);
        Create Effect(All Players(All Teams), Sphere, Blue, Value In Array(Value In Array(Global Variable(P), 2), 1), Value In Array(Value In Array(Global Variable(P), 2), 2), Visible To);
        Create Effect(All Players(All Teams), Sphere, Red, Value In Array(Value In Array(Global Variable(P), 3), 1), Value In Array(Value In Array(Global Variable(P), 3), 2), Visible To);
        Create Effect(All Players(All Teams), Sphere, Purple, Value In Array(Value In Array(Global Variable(P), 4), 1), Value In Array(Value In Array(Global Variable(P), 4), 2), Visible To);
        
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

//card dealing__________________________________________________________________

// deal random card
/*
    gN[0] = trigger
    gN[1] = visible to
    gN[2] = position
    gN[3] = random value in gM
    gN[4] = temporary memory for module
*/

if gN[0] = true
{
    // update only visible to for all

    gN[3] = random value in gM
    remove gN[3] in gM

    if visible to 1 person:
    switch gN[3][1]
        = 0
            create effect sphere r = gK[3] at gN[2], colour = red, visible to gN[1]
        = 1
            create effect sphere r = gK[3] at gN[2], colour = blue, visible to gN[1]
        = 2
            create effect sphere r = gK[3] at gN[2], colour = yellow, visible to gN[1]
        = 3
            create effect sphere r = gK[3] at gN[2, colour = green, visible to gN[1]
    
    append ID of last created effect to gN[4]

    switch gN[3][0]
        = 2-10
            create in-world text at gN[2], value = gN[3][0], visible to gN[1]
        = 11
            create in-world text at gN[2], junkrat icon, visible to gN[1]
        = 12
            create in-world text at gN[2], mercy icon, visible to gN[1]
        = 13
            create in-world text at gN[2], reinhardt icon, visible to gN[1]
        = 14
            create in-world text at gN[2], tracer icon, visible to gN[1]
    
    append ID of last created text to gN[4]

    if gN[1] is a player, append gN[4] to pA of that player
    else append gN[4] to gD

    gN[4] = empty array
    gN[0] = false
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