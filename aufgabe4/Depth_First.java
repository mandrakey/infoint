import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Date;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.Stack;
import java.util.Vector;

public class Depth_First {

	private static Map<String, String> CM = new HashMap<String, String>();
	private static boolean debug = false;

	public Depth_First(String input_file_path, String output_file_path,
			boolean debug) {
		this.debug = debug;
		try {
			long start = System.currentTimeMillis();
			FileInputStream input_file = new FileInputStream(input_file_path);
			BufferedReader input = new BufferedReader(new InputStreamReader(
					input_file));

			FileOutputStream output_file = new FileOutputStream(
					output_file_path);
			BufferedWriter output = new BufferedWriter(new OutputStreamWriter(
					output_file));

			String statement = null;
			Query a = null;
			Query b = null;

			int count_input_lines = 1;
			while ((statement = input.readLine()) != null) {

				if (statement.startsWith("//")) {
					count_input_lines = 1;
					if (this.debug) {
						System.out.println(statement);
						output.write(statement);
						output.newLine();
					}
				} else {
					count_input_lines++;
					switch (count_input_lines) {
					case 2:
						a = new Query(statement);
						if (this.debug) {
							System.out.println(statement);
						}
						break;
					case 3:
						b = new Query(statement);
						if (this.debug) {
							System.out.println(statement);
						}
						break;
					case 4:
						String result = "" + tree_search(a, b);
						output.write(result);
						if (this.debug) {
							System.out.println(result + "(" + statement + ")");
							output.write("(" + statement + ")");
						}
						output.newLine();
						break;
					}
				}
			}
			System.out.println("[INFO] RUNTIME: "
					+ (System.currentTimeMillis() - start) + "ms");
			output.flush();
			input.close();
			output.close();
		} catch (Exception e) {
			System.out.println("[FEHLER]\t" + e);
		}

	}

	/**
	 * returns TRUE if Query A contains Query B | B is a part of A else returns
	 * FALSE uses depth first tree search algorithm
	 * */
	public static boolean tree_search(Query a, Query b) {

		if (a.outputs.size() != b.outputs.size()) {
			return false;
		}

		Stack<Literal> literals = new Stack<Literal>();
		literals.addAll(b.literals);
		Stack<Triple> checks = new Stack<Triple>();
		List<Vector<Literal>> CM = new ArrayList<Vector<Literal>>();

		Triple initial_triple = new Triple(new ArrayList<String>(),
				get_candidates(literals.peek(), a), literals);
		checks.push(initial_triple);

		while (!checks.isEmpty()) {

			Triple my_triple = checks.pop();
			Stack<Literal> candidates_left = my_triple.get_H();
			Stack<Literal> literals_left = my_triple.get_L();

			if (!candidates_left.isEmpty()) {
				Literal candidate = candidates_left.pop();
				checks.push(new Triple(my_triple.get_h(), candidates_left,
						my_triple.get_L()));
				Literal literal = literals_left.peek();

				String local_CM = get_local_CM(literal, candidate); // get_local_CM -> createMapping
				List<String> global_CM = my_triple.get_h();

				if (check_contain(my_triple.get_h(), local_CM, a, b, candidate,
						literal)) { // check_contain -> mappingCompatible
					literals_left.pop();
					if (literals_left.isEmpty()) {

						if (debug) {
							System.out.println(my_triple.get_h());
						}

						return true;
					} else {
						List<String> new_CM = my_triple.get_h();
						new_CM.add(local_CM);
						Stack<Literal> new_candidates = get_candidates(
								literals_left.peek(), a);

						checks.push(new Triple(new_CM, new_candidates,
								literals_left));
					}
				}
			}
		}
		return false;

	}

	private static String get_local_CM(Literal literal, Literal candidate) {
		List<String> local_CM = new ArrayList<String>();
		for (int i = 0; i < literal.variables.size(); i++) {
			local_CM.add(literal.variables.get(i) + "->"
					+ candidate.variables.get(i));
		}
		return local_CM.toString().replace("[", "").replace(" ", "")
				.replace("]", "");
	}

    // todo: Look here
	private static boolean check_contain(List<String> global_CM,
			String local_CM, Query container, Query mapper, Literal candidate,
			Literal literal) {

		List<String> global_map = new ArrayList<String>();
		List<String> local_map = new ArrayList<String>();

		if (local_CM.contains(",")) {
			local_map = Arrays.asList(local_CM.split(","));
		} else {
			local_map.add(local_CM);
		}
        
        // =====================================================================
        // Map header
        
		//~ if (!mapper.outputs.get(0).equals("null")) {
			//~ Map<Character, Character> header_constants = new HashMap<Character, Character>();
			//~ for (int i = 0; i < container.outputs.size(); i++) {
				//~ for (String header_map : local_map) {
					//~ String header_key = header_map.split("->")[0];
					//~ String header_value = header_map.split("->")[1];
//~ 
					//~ if (mapper.outputs.get(i).equals(header_key)) {
						//~ if (!container.outputs.contains(header_value)) {
							//~ if (debug) {
								//~ System.out.println("HEADER HAS WRONG VARIABLES:\n\t"
										//~ + header_key + "->" + header_value + "\n\tCM: "
										//~ + global_CM + " " + local_CM + "\n\tLiteral:"
										//~ + literal + "Candidate:" + candidate
										//~ + "\n\tQuery A: " + container.toString()
										//~ + " Query B: " + mapper.toString());
							//~ }
							//~ return false;
						//~ }
					//~ }
//~ 
					//~ if (Character.isUpperCase(container.outputs.get(i)
							//~ .charAt(0))) {
						//~ if (header_constants.containsKey(mapper.outputs.get(i)
								//~ .charAt(0))) {
							//~ if ((!header_constants.get(
									//~ mapper.outputs.get(i).charAt(0)).equals(
									//~ container.outputs.get(i).charAt(0)))) {
								//~ if (debug) {
									//~ System.out.println("CONSTANT NOT UNIQUE:\n\t"
											//~ + header_constants.get(	mapper.outputs.get(i).charAt(0)) + "->" + container.outputs.get(i) + "\n\tCM: "
											//~ + global_CM + " " + local_CM + "\n\tLiteral:"
											//~ + literal + "Candidate:" + candidate
											//~ + "\n\tQuery A: " + container.toString()
											//~ + " Query B: " + mapper.toString());
								//~ }
								//~ return false;
							//~ }
						//~ } else {
							//~ header_constants.put(mapper.outputs.get(i)
									//~ .charAt(0), container.outputs.get(i)
									//~ .charAt(0));
						//~ }
//~ 
						//~ if (Character.isLowerCase(mapper.outputs.get(i).charAt(
								//~ 0))) {
							//~ if (debug) {
								//~ System.out.println("VARIABLE MAPPED ON CONSTANT:\n\t"
										//~ + mapper.outputs.get(i) + "->" + container.outputs.get(i) + "\n\tCM: "
										//~ + global_CM + " " + local_CM + "\n\tLiteral:"
										//~ + literal + "Candidate:" + candidate
										//~ + "\n\tQuery A: " + container.toString()
										//~ + " Query B: " + mapper.toString());
							//~ }
							//~ return false;
						//~ }
					//~ }
				//~ }
			//~ }
		//~ }
        
        //======================================================================
        // Variables / Constants
        
        // Check C -> C and not C -> A
		Character local_key, global_key, local_value, global_value;
		Map<Character, Character> my_mapping = new HashMap();
		for (String local_mapping : local_map) {
			local_key = local_mapping.split("->")[0].charAt(0);
			local_value = local_mapping.split("->")[1].charAt(0);

			if (my_mapping.containsKey(local_key)) {
				if (!my_mapping.get(local_key).equals(local_value)) {
					if (debug) {
						System.out.println("CONSTANT NOT UNIQUE:\n\t"
								+ local_key + "->" + local_value + "\n\tCM: "
								+ global_CM + " " + local_CM + "\n\tLiteral:"
								+ literal + "Candidate:" + candidate
								+ "\n\tQuery A: " + container.toString()
								+ " Query B: " + mapper.toString());
					}
					return false;
				}
			} else {
				my_mapping.put(local_key, local_value);
			}
		}
        
        // Das gleiche nochmal?!
		if (global_CM.size() == 0) {
			for (String local_mapping : local_map) {
				local_key = local_mapping.split("->")[0].charAt(0);
				local_value = local_mapping.split("->")[1].charAt(0);

				// Konstante muss auf selbe Konstante abgebildet Werden
				if (Character.isUpperCase(local_value)
						|| Character.isUpperCase(local_key)) {

					if (!local_key.equals(local_value)) {
						if (debug) {
							System.out.println("CONSTANT NOT UNIQUE:\n\t"
									+ local_key + "->" + local_value
									+ "\n\tCM: " + global_CM + " " + local_CM
									+ "\n\tLiteral:" + literal + "Candidate:"
									+ candidate + "\n\tQuery A: "
									+ container.toString() + " Query B: "
									+ mapper.toString());
						}
						return false;
					}
				}
			}
		}

		for (String global_maps : global_CM) {
			global_map = Arrays.asList(global_maps.split(","));
            
            // Und nochmal?!
			for (String local_mapping : local_map) {
				local_key = local_mapping.split("->")[0].charAt(0);
				local_value = local_mapping.split("->")[1].charAt(0);

				// Konstante muss auf selbe Konstante abgebildet Werden
				if (Character.isUpperCase(local_value)
						|| Character.isUpperCase(local_key)) {

					if (!local_key.equals(local_value)) {
						if (debug) {
							System.out.println("CONSTANT MAPPING WRONG:\n\t"
									+ local_key + "->" + local_value
									+ "\n\tCM: " + global_CM + " " + local_CM
									+ "\n\tLiteral:" + literal + "Candidate:"
									+ candidate + "\n\tQuery A: "
									+ container.toString() + " Query B: "
									+ mapper.toString());
						}
						return false;
					}
				}
                
                // Teste eindeutigkeit Variablenmappings
				for (String global_mapping : global_map) {
					global_key = global_mapping.split("->")[0].charAt(0);
					global_value = global_mapping.split("->")[1].charAt(0);

					// key->value muss eindeutig sein
					if (local_key.equals(global_key)) {
						if (!local_value.equals(global_value)) {
							if (debug) {
								System.out.println("MAPPING NOT UNIQUE:\n\t"
										+ local_key + "->" + local_value + "|"
										+ global_value + "\n\tCM: " + global_CM
										+ " " + local_CM + "\n\tLiteral:"
										+ literal + "Candidate:" + candidate
										+ "\n\tQuery A: "
										+ container.toString() + " Query B: "
										+ mapper.toString());
							}
							return false;
						}
					}
				}
                
                // Sind Variablen auch im Header exportiert?
				//~ if (!mapper.outputs.get(0).equals("null")) {
					//~ if (mapper.outputs.contains(local_key)) {
						//~ // Variablen muessen exportiert werden
						//~ boolean exported = false;
						//~ for (String variable : candidate.variables) {
							//~ if (container.outputs.contains(variable)) {
								//~ exported = true;
							//~ }
						//~ }
						//~ if (!exported) {
							//~ if (debug) {
								//~ System.out.println("VARIABLE NOT EXPORTED:\n\t"
										//~ + local_key + "->" + local_value
										//~ + "\n\tCM: " + global_CM + " "
										//~ + local_CM + "\n\tLiteral:" + literal
										//~ + "Candidate:" + candidate
										//~ + "\n\tQuery A: "
										//~ + container.toString() + " Query B: "
										//~ + mapper.toString());
							//~ }
							//~ return false;
						//~ }
					//~ }
				//~ }
			}
		}
		if (debug) {
			System.out.println("MAPPING FOUND:\n\tCM: " + global_CM + " "
					+ local_CM + "\n\tLiteral:" + literal + "Candidate:"
					+ candidate + "\n\tQuery A: " + container.toString()
					+ " Query B: " + mapper.toString());
		}
		return true;
	}

	private static Stack<Literal> get_candidates(Literal literal, Query query) {
		Stack<Literal> candidates = new Stack<Literal>();

		for (Literal potential_candidate : query.literals) {
			if (potential_candidate.name.equals(literal.name)
					&& potential_candidate.variables.size() == literal.variables
							.size()) {
				candidates.add(potential_candidate);
			}
		}
		return candidates;
	}
}
