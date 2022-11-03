all: generate-data_dictionary clean

venv: venv/touchfile

venv/touchfile: requirements.txt
	test -d venv || virtualenv venv
	(. venv/bin/activate; pip install -Ur requirements.txt)
	touch venv/touchfile

generate-data_dictionary: venv
	(. venv/bin/activate; python3 generate_data_dictionary.py)

clean:
	rm -rf venv