domains
	gender=male; female.

predicates
	nondeterm person(symbol,gender)
	nondeterm parent(symbol,symbol)
	nondeterm happy(symbol)
	nondeterm sister(symbol,symbol)
	nondeterm brother(symbol,symbol)
	nondeterm hasASibling(symbol)
	nondeterm hasAtLeastTwoChildren(symbol)
	nondeterm grandchild(symbol,symbol)
	nondeterm aunt(symbol,symbol)
clauses
	happy(Person):-
	parent(Person,_).
	
	sister(Person1,Person2):-
	parent(X,Person1) and
	parent(X,Person2) and
	person(Person1,female) and
	Person1 <> Person2.
	
	brother(Person1,Person2):-
	parent(X,Person1) and
	parent(X,Person2) and
	person(Person1,male) and
	Person1 <> Person2.
	
	hasASibling(Person):-
	sister(Person, _) or brother(Person, _).
	
	hasAtLeastTwoChildren(Person):-
	parent(Person, Child) and hasASibling(Child).
	
	grandchild(GChild, GParent):-
	parent(GParent,X) and parent(X, GChild).
	
	aunt(Aunt, Nephew):-
	sister(Aunt,X) and
	parent(X,Nephew).
	
	person("Alexander",male).
	person("Vladimir",male).
	person("Pavel",male).
	person("Yurii",male).
	person("Vyacheslav",male).
	person("Roman",male).
	person("Evgenii",male).
	person("Natalia",female).
	person("Antonina",female).
	person("Svetlana",female).
	person("Radmila",female).
	person("Yulia",female).
	person("Maria",female).
	
	person("Aleksey", male).
	parent("Aleksey", "Vlad").
	parent("Maria", "Vlad").
	
	parent("Alexander","Pavel").
	parent("Alexander","Yurii").
	parent("Natalia","Pavel").
	parent("Natalia","Yurii").
	parent("Antonina","Radmila").
	parent("Vladimir","Radmila").
	parent("Antonina","Yulia").
	parent("Vladimir","Yulia").
	parent("Antonina","Vyacheslav").
	parent("Vladimir","Vyacheslav").
	parent("Svetlana","Roman").
	parent("Svetlana","Maria").
	parent("Pavel","Roman").
	parent("Pavel","Maria").
	parent("Yurii","Evgenii").
	parent("Radmila","Evgenii").
goal
	aunt(_,HasAnAunt).