TEMPLATE = subdirs
CONFIG += qt warn_on debug



CONFIG += samosa mosa

mosa {
	SUBDIRS += mosa
}

samosa {
	SUBDIRS += samosa
}

nampark {
	SUBDIRS += nampark
}

fixedstate {
	SUBDIRS += fixedstate
}

test {
	SUBDIRS += test
}