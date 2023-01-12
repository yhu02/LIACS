import json
import math
import random


def apply_auto_layout(nodes, connections):
    raw_data = {"nodes": nodes, "connections": connections}
    json_data_string = json.dumps(raw_data)

    json_data = JsonData()
    json_data.read_json(json_data_string)

    network_master = NetworkMaster(json_data)
    network_master.import_nodes()
    network_master.import_connections()
    network_master.fill_connected_nodes()

    root_set = NodeSet(network_master.get_nodes())
    root_set.split()
    network_master.set_root_set(root_set)

    heuristic_rules = HeuristicRules(network_master)
    springs_algorithm = SpringsAlgorithm(network_master)
    # nr_springs_iterations, settings_string = randomize_parameters(heuristic_rules, springs_algorithm, chosen_network)
    # nr_springs_iterations, settings_string = import_generation_settings(
    #     heuristic_rules,
    #     springs_algorithm,
    #     [153, 47, 200, 75, 441, 261, 1, 35, 200, 0, 1, 22, 17],
    # )  # use user feedback on computer generated data
    nr_springs_iterations, settings_string = import_generation_settings(
        heuristic_rules, springs_algorithm, [59, 54, 198, 53, 258, 360, 1, 26, 159, 0, 6, 87, 21]
    )  # USe only USer Feedback data

    heuristic_rules.fill_sets()
    heuristic_rules.generate_layout()
    heuristic_rules.fill_set_connections()
    heuristic_rules.layout_sets()

    for i in range(nr_springs_iterations):
        springs_algorithm.apply_springs()

    heuristic_rules.align_nodes(100, 100)

    return json_data.return_json_data(network_master)

    # THIS IS FOR GATHERING TESTING DATA
    #
    # if False: # Manually Ask For Feedback
    #     current_time = datetime.now().strftime("%_h:%_m:%_s")
    #     f = open("./TestResultaten/" + str(currentTime) + "OnlyOverallScore.txt", "x")
    #     # f = open("./TestResultaten/" + str(currentTime) + ".txt", "x")
    #     f.write(json.dumps(settingsString) + "\n")
    #     f.write("Distance between nodes (1-5):\n")
    #     f.write("Allignment of elements (1-5):\n")
    #     f.write("Lines crossing elements (1-5):\n")
    #     f.write("Relational position of elements (1-5):\n")
    #     f.write("Time taken to load (1-5):\n")
    #     f.write("Overal score (1-10):\n")
    #     f.close

    # if False: # Manually Ask For Feedback ON IMPROVED SHOW UFandCG
    #     currentTime = datetime.now().strftime("%H:%M:%S")
    #     f = open("./TestResultaten/UFandCG" + str(currentTime) + "OnlyOverallScore.txt", "x")
    #     # f = open("./TestResultaten/" + str(currentTime) + ".txt", "x")
    #     f.write(json.dumps(settingsString) + "\n")
    #     f.write("Distance between nodes (1-5):\n")
    #     f.write("Allignment of elements (1-5):\n")
    #     f.write("Lines crossing elements (1-5):\n")
    #     f.write("Relational position of elements (1-5):\n")
    #     f.write("Time taken to load (1-5):\n")
    #     f.write("Overal score (1-10):\n")
    #     f.close

    # if True: # Manually Ask For Feedback ON IMPROVED SHOW UF
    #     currentTime = datetime.now().strftime("%H:%M:%S")
    #     f = open("./TestResultaten/UF" + str(currentTime) + "OnlyOverallScore.txt", "x")
    #     # f = open("./TestResultaten/" + str(currentTime) + ".txt", "x")
    #     f.write(json.dumps(settingsString) + "\n")
    #     f.write("Distance between nodes (1-5):\n")
    #     f.write("Allignment of elements (1-5):\n")
    #     f.write("Lines crossing elements (1-5):\n")
    #     f.write("Relational position of elements (1-5):\n")
    #     f.write("Time taken to load (1-5):\n")
    #     f.write("Overal score (1-10):\n")
    #     f.close

    # if False: # _generate _feedback
    #     averageConnectedNodeDistance, averageUnconnectedNodeDistance, overlappingFraction, allignedness, correctRelationPositionFraction, layoutTime = calculate_cgfb(new_json_data_string, network_master, springs_algorithm, timer_end- timer_start)
    #     currentTime = datetime.now().strftime("%H:%M:%S")
    #     # f = open("./TestResultaten/" + str(currentTime) + "OnlyOverallScore.txt", "x")
    #     f = open("./TestResultaten/CGFB" + str(currentTime) + ".txt", "x")
    #     f.write(json.dumps(settingsString) + "\n")
    #     f.write("averageConnectedNodeDistance:"+ str(round(averageConnectedNodeDistance, 0)) + "\n")
    #     f.write("averageUnconnectedNodeDistance:"+ str(round(averageUnconnectedNodeDistance, 0)) + "\n")
    #     f.write("overlappingFraction:"+ str(round(overlappingFraction, 3)) + "\n")
    #     f.write("allignedness:"+ str(round(allignedness, 0)) + "\n")
    #     f.write("correctRelationPositionFraction:"+ str(round(correctRelationPositionFraction, 3)) + "\n")
    #     f.write("layoutTime:"+ str(round(layoutTime, 1)) + "\n")
    #     f.close


# def import_generation_settings(heuristic_rules, springs_algorithm, chosen_network, settings):
def import_generation_settings(heuristic_rules, springs_algorithm, settings):
    target_distance = settings[0]

    heuristic_rules.set_parameters(target_distance)

    connection_force = settings[1]
    connection_target_distance = settings[2]
    non_connected_force = settings[3]
    non_connected_target_distance = settings[4]
    alignment_ignore_distance = settings[5]

    do_alignment = settings[6]
    if do_alignment == 1:
        do_alignment = True
    else:
        do_alignment = False

    overlapping_check_interval = settings[7]
    overlapping_min_distance = settings[8]

    checking_overlapping_connection = settings[9]
    if checking_overlapping_connection == 1:
        checking_overlapping_connection = True
    else:
        checking_overlapping_connection = False

    random_factor = settings[10]
    random_magnitude = settings[11]

    springs_algorithm.set_parameters(
        connection_force,
        connection_target_distance,
        non_connected_force,
        non_connected_target_distance,
        alignment_ignore_distance,
        do_alignment,
        overlapping_check_interval,
        overlapping_min_distance,
        checking_overlapping_connection,
        random_factor,
        random_magnitude,
    )

    springs_algorithm_iterations = settings[12]

    return springs_algorithm_iterations, {
        "target_distance": target_distance,
        "connection_force": connection_force,
        "connection_target_distance": connection_target_distance,
        "non_connected_force": non_connected_force,
        "non_connected_target_distance": non_connected_target_distance,
        "alignment_ignore_distance": alignment_ignore_distance,
        "do_alignment": do_alignment,
        "overlapping_check_interval": overlapping_check_interval,
        "overlapping_min_distance": overlapping_min_distance,
        "checking_overlapping_connection": checking_overlapping_connection,
        "random_factor": random_factor,
        "random_magnitude": random_magnitude,
        "springs_algorithm_iterations": springs_algorithm_iterations,
    }


def calculate_cgfb(network_master, springs_algorithm, layout_time):
    # _calculate average _connected _node _distance
    total_connected_node_distance = 0
    nr_connections = 0
    for connection in network_master.connections:
        node_from = network_master.get_node(connection.id_from)
        node_to = network_master.get_node(connection.id_to)

        a = node_from.x - node_to.x
        b = node_from.y - node_to.y
        c = math.sqrt(a * a + b * b)

        total_connected_node_distance += c
        nr_connections += 1
    average_connected_node_distance = total_connected_node_distance / nr_connections

    # _calculate non connected _node _distance
    total_unconnected_node_distance = 0
    nr_node_pairs = 0
    for i in range(len(network_master.nodes) - 1):
        for j in range(i + 1, len(network_master.nodes)):
            node_from = network_master.nodes[i]
            node_to = network_master.nodes[j]

            a = node_from.x - node_to.x
            b = node_from.y - node_to.y
            c = math.sqrt(a * a + b * b)

            total_unconnected_node_distance += c
            nr_node_pairs += 1
    total_unconnected_node_distance -= total_connected_node_distance
    nr_node_pairs -= nr_connections
    average_unconnected_node_distance = total_unconnected_node_distance / nr_node_pairs

    # _calculate nr _overlapping _connections
    nr_overlapping_connections = 0
    for node in network_master.nodes:
        nr_overlapping_connections += springs_algorithm.get_nr_overlapping_connections_of_node_data_generation(node)
    nr_overlapping_connections = nr_overlapping_connections / 2
    overlapping_fraction = nr_overlapping_connections / nr_connections

    # _calculate _allignment
    allignedness = 0
    for connection in network_master.connections:
        node_from = network_master.get_node(connection.id_from)
        node_to = network_master.get_node(connection.id_to)

        x_difference = abs(node_from.x - node_to.x)
        y_difference = abs(node_from.y - node_to.y)

        if x_difference > y_difference:
            allignedness += y_difference
        else:
            allignedness += x_difference

    allignedness = allignedness / nr_connections

    # _calculate relational position
    nr_correct_relational_position = 0
    nr_generalizations = 0
    for connection in network_master.connections:
        if connection.type == "generalization":
            node_from = network_master.get_node(connection.id_from)
            node_to = network_master.get_node(connection.id_to)

            if node_to.y <= node_from.y:
                nr_correct_relational_position += 1
            nr_generalizations += 1
    correct_relation_position_fraction = nr_correct_relational_position / nr_generalizations

    return (
        average_connected_node_distance,
        average_unconnected_node_distance,
        overlapping_fraction,
        allignedness,
        correct_relation_position_fraction,
        layout_time,
    )


def randomize_parameters(heuristic_rules, springs_algorithm):
    target_distance = random.randint(50, 300)

    heuristic_rules.set_parameters(target_distance)

    connection_force = random.randint(5, 75)
    connection_target_distance = random.randint(25, 350)
    non_connected_force = random.randint(0, 75)
    non_connected_target_distance = random.randint(50, 500)
    alignment_ignore_distance = random.randint(5, 500)

    do_alignment = random.randint(0, 1)
    if do_alignment == 1:
        do_alignment = True
    else:
        do_alignment = False

    overlapping_check_interval = random.randint(5, 50)
    overlapping_min_distance = random.randint(0, 200)

    checking_overlapping_connection = random.randint(0, 1)
    if checking_overlapping_connection == 1:
        checking_overlapping_connection = True
    else:
        checking_overlapping_connection = False

    random_factor = random.randint(0, 10)
    random_magnitude = random.randint(0, 100)

    springs_algorithm.set_parameters(
        connection_force,
        connection_target_distance,
        non_connected_force,
        non_connected_target_distance,
        alignment_ignore_distance,
        do_alignment,
        overlapping_check_interval,
        overlapping_min_distance,
        checking_overlapping_connection,
        random_factor,
        random_magnitude,
    )

    springs_algorithm_iterations = random.randint(0, 25)

    return springs_algorithm_iterations, {
        "target_distance": target_distance,
        "connection_force": connection_force,
        "connection_target_distance": connection_target_distance,
        "non_connected_force": non_connected_force,
        "non_connected_target_distance": non_connected_target_distance,
        "alignment_ignore_distance": alignment_ignore_distance,
        "do_alignment": do_alignment,
        "overlapping_check_interval": overlapping_check_interval,
        "overlapping_min_distance": overlapping_min_distance,
        "checking_overlapping_connection": checking_overlapping_connection,
        "random_factor": random_factor,
        "random_magnitude": random_magnitude,
        "springs_algorithm_iterations": springs_algorithm_iterations,
    }


class JsonData:
    def __init__(self):
        self.read_location = None
        self.write_location = None
        self.data = {}

    def read_json(self, json_data):
        self.data = json.loads(json_data)
        for node in self.data["nodes"]:
            node["position"] = {"x": 0, "y": 0}

    def return_json_data(self, network_master):
        for node in network_master.nodes:
            index = next(
                (index for (index, element) in enumerate(self.data["nodes"]) if element["id"] == node.get_node_id()),
                None,
            )
            self.data["nodes"][index]["position"]["x"] = node.get_x() + 500
            self.data["nodes"][index]["position"]["y"] = node.get_y() + 500

        for connection in network_master.connections:
            index = next(
                (
                    index
                    for (index, element) in enumerate(self.data["connections"])
                    if element["id"] == connection.get_connection_id()
                ),
                None,
            )
            self.data["connections"][index]["label"] = ""

        return self.data

    # _return the json data about the nodes
    def get_node_data(self):
        return self.data["nodes"]

    # _return the json data about the connections
    def get_connection_data(self):
        return self.data["connections"]

    # _get the json data
    def get_data(self):
        return self.data

    def get_node_name(self, node_id):
        for node in self.data["nodes"]:
            if node["id"] == node_id:
                return node["name"]
        return node_id


class NetworkMaster:
    def __init__(self, json_data):
        self.nodes = []
        self.connections = []
        self.json_data = json_data
        self.root_set = None

    # _generate _nodes based on json data
    def import_nodes(self):
        node_data = self.json_data.get_node_data()
        for node in node_data:
            new_node = Node(
                node["id"],
                node["name"],
                node["type"],
                node["position"]["x"],
                node["position"]["y"],
                node["instances"],
            )
            self.nodes.append(new_node)

    # _generate _connections based on json data
    def import_connections(self):
        connection_data = self.json_data.get_connection_data()
        for connection in connection_data:
            new_connection = Connection(
                connection["id"],
                connection["type"],
                connection["from"],
                connection["to"],
                connection["label"],
                connection["labelFrom"],
                connection["labelTo"],
            )
            self.connections.append(new_connection)

    def fill_connected_nodes(self):
        for connection in self.connections:
            node_from = self.get_node(connection.get_id_from())
            node_to = self.get_node(connection.get_id_to())
            if connection.type == "generalization":
                node_from.add_connected_node(node_to, "below")
                node_to.add_connected_node(node_from, "above")
            elif connection.type == "composition":
                node_from.add_connected_node(node_to, "above")
                node_to.add_connected_node(node_from, "below")
            else:
                node_from.add_connected_node(node_to, "None")
                node_to.add_connected_node(node_from, "None")

    # _set root_set
    def set_root_set(self, root_set):
        self.root_set = root_set

    # _get nodes based on node id
    def get_node(self, node_id):
        for node in self.nodes:
            if node.get_node_id() == node_id:
                return node
        return None

    # _get all nodes
    def get_nodes(self):
        return self.nodes


class Node:
    def __init__(self, node_id, name, node_type, x, y, instances):
        self.node_id = node_id
        self.name = name
        self.type = node_type
        self.x = x
        self.y = y
        self.instances = instances
        self.dimensions = [50, 100]

        self.connected_nodes = []
        self.preferred_position_connected_nodes = []

    def add_connected_node(self, node, preferred_position):
        self.connected_nodes.append(node)
        self.preferred_position_connected_nodes.append(preferred_position)

    def move(self, step):
        self.x += step[0]
        self.y += step[1]

    def set_x(self, x):
        self.x = x

    def set_y(self, y):
        self.y = y

    def set_position(self, position):
        if len(position) <= 1:
            return
        self.x = position[0]
        self.y = position[1]

    def get_node_id(self):
        return self.node_id

    def get_name(self):
        return self.name

    def get_x(self):
        return self.x

    def get_y(self):
        return self.y

    def get_position(self):
        return [self.x, self.y]

    def get_dimensions(self):
        return self.dimensions

    def get_connected_nodes(self):
        return self.connected_nodes


class Connection:
    def __init__(self, connection_id, connection_type, id_from, id_to, label, label_from, label_to):
        self.connection_id = connection_id
        self.type = connection_type
        self.id_from = id_from
        self.id_to = id_to
        self.label = label
        self.label_from = label_from
        self.label_to = label_to

    def get_connected_node(self, node_id):
        if self.id_from == node_id:
            return self.id_to
        if self.id_to == node_id:
            return self.id_from
        return None

    def get_connection_id(self):
        return self.connection_id

    def get_id_from(self):
        return self.id_from

    def get_id_to(self):
        return self.id_to


class NodeSet:
    def __init__(self, nodes):
        self.nodes = nodes
        self.sub_sets = []
        self.set_type = "Initial"
        self.dimensions = [0, 0]

        self.connected_sets = []
        self.preferred_position_connected_sets = []

    def split(self):
        infinite_loop_preventer = 5
        while infinite_loop_preventer > 0:
            if self.set_type == "Initial":
                # If set was split on connectivity
                if self.split_on_connectivity():
                    self.split_subsets()
                    return True

            if self.set_type == "InitialJoint":
                # If set was split on connectivity
                if self.split_on_in_cycle():
                    self.split_subsets()
                    return True

            if self.set_type == "OutCycles":
                # _if set was split on connectivity
                if self.split_on_connectivity():
                    self.split_subsets()
                    return True

            if self.set_type == "InCycles":
                # _if set was split on connectivity
                if self.split_on_cycles():
                    self.split_subsets()
                    return True

            if self.set_type == "RestOfInCycles":
                # _if set was split on connectivity
                if self.split_on_connectivity():
                    self.split_subsets()
                    return True

            if self.set_type == "RestOfInCyclesJoint":
                # _if set was split on connectivity
                if self.split_on_in_cycle():
                    self.split_subsets()
                    return True

            if self.set_type == "RestOfInCyclesCycles":
                # _if set was split on connectivity
                if self.split_on_cycles():
                    self.split_subsets()
                    return True
            infinite_loop_preventer -= 1
        return False

    def split_subsets(self):
        for sub_set in self.sub_sets:
            sub_set.split()

    def split_on_in_cycle(self):
        if len(self.nodes) == 0:
            return False

        cycles = get_cycles(self.nodes[0], self.nodes)
        nodes_in_cycles = get_nodes_of_list_of_cycles(cycles)

        # _if all nodes in this set are cycles
        if len(nodes_in_cycles) == len(self.nodes):
            if self.set_type == "InitialJoint":
                self.set_set_type("InCycles")
            if self.set_type == "RestOfInCyclesJoint":
                self.set_set_type("RestOfInCyclesCycles")
            return False

        nodes_not_in_cycles = []
        for node in self.nodes:
            if not node_in_list(node, nodes_in_cycles):
                nodes_not_in_cycles.append(node)

        set_with_cycles = NodeSet(nodes_in_cycles)
        set_without_cycles = NodeSet(nodes_not_in_cycles)

        if self.set_type == "InitialJoint":
            set_with_cycles.set_set_type("InCycles")
            set_without_cycles.set_set_type("OutCycles")
        if self.set_type == "RestOfInCyclesJoint":
            set_with_cycles.set_set_type("RestOfInCyclesCycles")
            set_without_cycles.set_set_type("CycleFragment")
        self.sub_sets.append(set_with_cycles)
        self.sub_sets.append(set_without_cycles)
        self.nodes = None
        return True

    def split_on_connectivity(self):
        if len(self.nodes) == 0:
            return False

        nr_added_subsets = 0
        while len(self.nodes) > 0:
            reachable_nodes = get_reachable_nodes(self.nodes[0], self.nodes)
            if nr_added_subsets == 0 and len(reachable_nodes) == len(self.nodes):
                # _if all the nodes can be reached and no nodes have been removed yet it means that it is all connected
                if self.set_type == "Initial":
                    self.set_set_type("InitialJoint")
                if self.set_type == "OutCycles":
                    self.set_set_type("DeadEnd")
                if self.set_type == "RestOfInCycles":
                    self.set_set_type("RestOfInCyclesJoint")
                return False
            nr_added_subsets += 1

            new_set = NodeSet(reachable_nodes)
            if self.set_type == "Initial":
                new_set.set_set_type("InitialJoint")
            if self.set_type == "OutCycles":
                new_set.set_set_type("DeadEnd")
            if self.set_type == "RestOfInCycles":
                new_set.set_set_type("RestOfInCyclesJoint")
            self.sub_sets.append(new_set)
            self.nodes = remove_nodes_from_list(reachable_nodes, self.nodes)
        return True

    def split_on_cycles(self):
        if len(self.nodes) == 0:
            return False

        cycles = get_cycles(self.nodes[0], self.nodes)
        if len(cycles) == 1:
            # _if there is only one cycle
            self.set_set_type("Cycles")
            return False

        smallest_cycle = cycles[0]
        for cycle in cycles:
            if len(cycle) < len(smallest_cycle):
                smallest_cycle = cycle

        rest_of_nodes = remove_nodes_from_list(smallest_cycle, self.nodes)
        smallest_cycle_set = NodeSet(smallest_cycle)
        smallest_cycle_set.set_set_type("Cycles")
        rest_of_nodes_set = NodeSet(rest_of_nodes)
        rest_of_nodes_set.set_set_type("RestOfInCycles")
        self.sub_sets.append(smallest_cycle_set)
        self.sub_sets.append(rest_of_nodes_set)
        self.nodes = None
        return True

    def move_location(self, step):
        for node in self.nodes:
            position = node.get_position()
            position[0] += step[0]
            position[1] += step[1]
            node.set_position(position)

    def set_set_type(self, set_type):
        self.set_type = set_type

    def set_dimensions(self, dimensions):
        self.dimensions = dimensions

    def get_set_type(self):
        return self.set_type

    def get_dimensions(self):
        return self.dimensions


class HeuristicRules:
    def __init__(self, network_master):
        self.network_master = network_master
        self.sets = []
        self.target_distance = 100

    def set_parameters(self, target_distance):
        self.target_distance = target_distance

    def fill_sets(self):
        self.fill_sets_recursive(self.network_master.root_set)

    def fill_sets_recursive(self, node_set):
        for subset in node_set.sub_sets:
            if subset.nodes is not None and len(subset.nodes):
                self.sets.append(subset)
            self.fill_sets_recursive(subset)

    def align_nodes(self, horizontal_distance, vertical_distance):
        nodes = self.network_master.nodes
        for i in range(len(nodes) - 1):
            pos1 = nodes[i].get_position()
            for j in range(i + 1, len(nodes)):
                pos2 = nodes[j].get_position()
                x_diff = abs(pos1[0] - pos2[0])
                y_diff = abs(pos1[1] - pos2[1])
                new_pos = [pos2[0], pos2[1]]
                if x_diff < horizontal_distance:
                    new_pos[0] = pos1[0]
                if y_diff < vertical_distance:
                    new_pos[1] = pos1[1]
                nodes[j].set_position(new_pos)

    def fill_set_connections(self):
        for node_set in self.sets:
            for node in node_set.nodes:
                connected_node_index = -1
                for connected_node in node.connected_nodes:
                    connected_node_index += 1
                    if not node_in_list(connected_node, node_set.nodes):
                        set_that_contains_connected_node = self.get_set_that_contains_node(connected_node)
                        preferred_position = node.preferred_position_connected_nodes[connected_node_index]
                        node_set.connected_sets.append(set_that_contains_connected_node)
                        node_set.preferred_position_connected_sets.append(preferred_position)

        for node_set in self.sets:
            index = -1
            for connected_set in node_set.connected_sets:
                index += 1

    def get_set_that_contains_node(self, node):
        for node_set in self.sets:
            if node_in_list(node, node_set.nodes):
                return node_set
        return None

    def generate_layout(self):
        for node_set in self.sets:
            self.generate_layout_nodes(node_set)

    def move_sets_away(self):
        current_x = 0
        for node_set in self.sets:
            node_set.move_location([current_x, 0])
            current_x += node_set.get_dimensions()[0]

    def generate_layout_nodes(self, node_set):
        nodes_with_outside_connections = self.get_nodes_with_outside_connections(node_set)

        if len(nodes_with_outside_connections) == 1:
            entrance_node = nodes_with_outside_connections[0]
        else:
            entrance_node = node_set.nodes[0]

        layers = [[entrance_node]]
        checked_nodes = [entrance_node]
        queue = [entrance_node]

        while len(queue) > 0:
            connected_nodes_index = 0
            for connected_node in queue[0].connected_nodes:
                if (
                    node_in_list(connected_node, node_set.nodes)
                    and not node_in_list(connected_node, queue)
                    and not node_in_list(connected_node, checked_nodes)
                ):
                    layer_index_temp = -1
                    layer_index = -1
                    for layer in layers:
                        layer_index_temp += 1
                        for node in layer:
                            if node == queue[0]:
                                layer_index = layer_index_temp

                    preferred_position = queue[0].preferred_position_connected_nodes[connected_nodes_index]
                    if preferred_position == "above":
                        if layer_index == 0:
                            layers.insert(0, [connected_node])
                        else:
                            layers[layer_index - 1].append(connected_node)
                    elif preferred_position == "below":
                        if layer_index == len(layers) - 1:
                            layers.append([connected_node])
                        else:
                            layers[layer_index + 1].append(connected_node)
                    else:
                        layers[layer_index].append(connected_node)
                    queue.append(connected_node)
                connected_nodes_index += 1
            checked_nodes.append(queue[0])
            queue.pop(0)

        min_y = 0
        max_x = 0

        layer_step = -1 * self.target_distance
        neighbour_step = self.target_distance
        current_y = 0
        for layer in layers:
            current_x = 0
            for node in layer:
                node.set_position([current_x, current_y])
                current_x += neighbour_step
                if current_x > max_x:
                    max_x = current_x
            if current_y < min_y:
                min_y = current_y
            current_y += layer_step
        node_set.set_dimensions([max_x, -1 * min_y])

    def layout_sets(self):
        layers = [[self.sets[0]]]
        checked_sets = []
        queue = [self.sets[0]]

        while len(queue) > 0:
            connected_sets_index = 0
            for connected_set in queue[0].connected_sets:
                if not node_in_list(connected_set, queue) and not node_in_list(connected_set, checked_sets):
                    layer_index_temp = -1
                    layer_index = -1
                    for layer in layers:
                        layer_index_temp += 1
                        for node_set in layer:
                            if node_set == queue[0]:
                                layer_index = layer_index_temp

                    preferred_position = queue[0].preferred_position_connected_sets[connected_sets_index]
                    if preferred_position == "above":
                        if layer_index <= 0:
                            layers.insert(0, [connected_set])
                        else:
                            layers[layer_index - 1].append(connected_set)
                    elif preferred_position == "below":
                        if layer_index == len(layers) - 1:
                            layers.append([connected_set])
                        else:
                            layers[layer_index + 1].append(connected_set)
                    queue.append(connected_set)
                connected_sets_index += 1
            checked_sets.append(queue[0])
            queue.pop(0)

        layer_step = -1 * self.target_distance
        neighbour_step = self.target_distance

        current_y = 0
        for layer in layers:
            current_x = 0
            big_gets_y_in_row = 0
            for node_set in layer:
                node_set.move_location([current_x, current_y])
                current_x += neighbour_step
                current_x += node_set.get_dimensions()[0]
                if node_set.get_dimensions()[1] > big_gets_y_in_row:
                    big_gets_y_in_row = node_set.get_dimensions()[1]
            current_y += layer_step
            current_y -= big_gets_y_in_row

    def get_nodes_with_outside_connections(self, node_set):
        nodes_with_outside_connections = []
        for node in node_set.nodes:
            for connected_node in node.connected_nodes:
                if not node_in_list(connected_node, node_set.nodes):
                    nodes_with_outside_connections.append(node)
                    break
        return nodes_with_outside_connections


class SpringsAlgorithm:
    def __init__(self, network_master):
        self.network_master = network_master
        self.connection_force = 25
        self.connection_target_distance = 250
        self.non_connected_force = 10
        self.non_connected_target_distance = 350
        self.alignment_ignore_distance = 300
        self.do_alignment = True
        self.overlapping_check_interval = 10
        self.overlapping_min_distance = 30
        self.checking_overlapping_connection = True
        self.random_factor = 2
        self.random_magnitude = 25

    def set_parameters(
        self,
        connection_force,
        connection_target_distance,
        non_connected_force,
        non_connected_target_distance,
        alignment_ignore_distance,
        do_alignment,
        overlapping_check_interval,
        overlapping_min_distance,
        checking_overlapping_connection,
        random_factor,
        random_magnitude,
    ):
        self.connection_force = connection_force
        self.connection_target_distance = connection_target_distance
        self.non_connected_force = non_connected_force
        self.non_connected_target_distance = non_connected_target_distance
        self.alignment_ignore_distance = alignment_ignore_distance
        self.do_alignment = do_alignment
        self.overlapping_check_interval = overlapping_check_interval
        self.overlapping_min_distance = overlapping_min_distance
        self.checking_overlapping_connection = checking_overlapping_connection
        self.random_factor = random_factor
        self.random_magnitude = random_magnitude

    def apply_springs(self):
        for node in self.network_master.nodes:
            node_center = self.get_center_of_node(node)
            radius1 = self.get_node_radius(node)
            move = [0, 0]
            aligned_axis = [False, False]
            for connected_node in node.connected_nodes:
                connected_node_center = self.get_center_of_node(connected_node)
                distance, direction = self.get_dist_and_dir_between_points(node_center, connected_node_center)
                radius2 = self.get_node_radius(connected_node)
                distance -= radius1 + radius2
                move_step_force = self.force_function(self.connection_force, distance, self.connection_target_distance)
                move[0] += direction[0] * move_step_force
                move[1] += direction[1] * move_step_force

                if direction[0] == 0 and distance < self.alignment_ignore_distance:
                    aligned_axis[1] = True
                if direction[1] == 0 and distance < self.alignment_ignore_distance:
                    aligned_axis[0] = True

            for other_node in self.network_master.nodes:
                if node != other_node and node_in_list(other_node, node.connected_nodes):
                    other_node_center = self.get_center_of_node(other_node)
                    distance, direction = self.get_dist_and_dir_between_points(node_center, other_node_center)
                    radius2 = self.get_node_radius(other_node)
                    distance -= radius1 + radius2
                    move_step_force = self.force_function(
                        self.non_connected_force,
                        distance,
                        self.non_connected_target_distance,
                    )
                    if move_step_force > 0:
                        move_step_force = 0
                    move[0] += direction[0] * move_step_force
                    move[1] += direction[1] * move_step_force

            if self.do_alignment:
                if aligned_axis[0]:
                    move[1] = 0
                if aligned_axis[1]:
                    move[0] = 0

            randomizer = random.randint(0, 10)
            if randomizer < self.random_factor:
                random_x = random.randint(-1 * self.random_magnitude, self.random_magnitude)
                random_y = random.randint(-1 * self.random_magnitude, self.random_magnitude)
                move[0] += random_x
                move[1] += random_y

            nr_overlapping_connection_before = 0
            if self.checking_overlapping_connection:
                nr_overlapping_connection_before = self.get_nr_overlapping_connections_of_node(node)

            node.move(move)

            if self.checking_overlapping_connection:
                nr_overlapping_connection_after = self.get_nr_overlapping_connections_of_node(node)
                if nr_overlapping_connection_after > nr_overlapping_connection_before:
                    move[0] = move[0] * -1
                    move[1] = move[1] * -1
                    node.move(move)

    def get_nr_overlapping_connections_of_node(self, node):
        nr_overlapping_connection = 0
        for connection in self.network_master.connections:
            if connection.get_id_from() == node.get_node_id() or connection.get_id_to() == node.get_node_id():

                for node_to_check in self.network_master.nodes:
                    node_from = self.network_master.get_node(connection.get_id_from())
                    node_to = self.network_master.get_node(connection.get_id_to())

                    if node_to_check != node_from and node_to_check != node_to:
                        center_of_node_to_check = node_to_check.get_position()
                        center_of_node_to_check[0] += 25
                        center_of_node_to_check[1] += 30

                        center_of_node_from = node_from.get_position()
                        center_of_node_from[0] += 25
                        center_of_node_from[1] += 30
                        center_of_node_to = node_to.get_position()
                        center_of_node_to[0] += 25
                        center_of_node_to[1] += 30

                        distance, direction = self.get_dist_and_dir_between_points(
                            center_of_node_from, center_of_node_to
                        )
                        direction[0] = direction[0] * self.overlapping_check_interval
                        direction[1] = direction[1] * self.overlapping_check_interval

                        line_check_interval = center_of_node_from

                        while distance > self.overlapping_check_interval:
                            line_check_interval[0] += direction[0]
                            line_check_interval[1] += direction[1]
                            distance, direction = self.get_dist_and_dir_between_points(
                                line_check_interval, center_of_node_to
                            )

                            (
                                node_to_line_distance,
                                node_to_line_direction,
                            ) = self.get_dist_and_dir_between_points(center_of_node_to_check, line_check_interval)
                            if node_to_line_distance < self.overlapping_min_distance:
                                nr_overlapping_connection += 1
                                break

        return nr_overlapping_connection

    def get_nr_overlapping_connections_of_node_data_generation(self, node):
        nr_overlapping_connection = 0
        for connection in self.network_master.connections:
            if connection.get_id_from() == node.get_node_id() or connection.get_id_to() == node.get_node_id():

                for node_to_check in self.network_master.nodes:
                    node_from = self.network_master.get_node(connection.get_id_from())
                    node_to = self.network_master.get_node(connection.get_id_to())

                    if node_to_check != node_from and node_to_check != node_to:
                        center_of_node_to_check = node_to_check.get_position()
                        center_of_node_to_check[0] += 25
                        center_of_node_to_check[1] += 30

                        center_of_node_from = node_from.get_position()
                        center_of_node_from[0] += 25
                        center_of_node_from[1] += 30
                        center_of_node_to = node_to.get_position()
                        center_of_node_to[0] += 25
                        center_of_node_to[1] += 30

                        distance, direction = self.get_dist_and_dir_between_points(
                            center_of_node_from, center_of_node_to
                        )
                        direction[0] = direction[0] * 5
                        direction[1] = direction[1] * 5

                        line_check_interval = center_of_node_from

                        while distance > 5:
                            line_check_interval[0] += direction[0]
                            line_check_interval[1] += direction[1]
                            distance, direction = self.get_dist_and_dir_between_points(
                                line_check_interval, center_of_node_to
                            )

                            (
                                node_to_line_distance,
                                node_to_line_direction,
                            ) = self.get_dist_and_dir_between_points(center_of_node_to_check, line_check_interval)
                            if node_to_line_distance < 40:
                                nr_overlapping_connection += 1
                                break

        return nr_overlapping_connection

    def force_function(self, force, distance, target_distance):
        x = distance / target_distance
        if x >= 1:
            y = math.pow((x - 1), 0.3)
        else:
            y = math.pow((-1 * x + 1), 0.3)
            y *= -1
        return force * y

    def get_dist_and_dir_between_points(self, point_from, point_to):
        a = point_to[0] - point_from[0]
        b = point_to[1] - point_from[1]
        distance = self.pythagoras(a, b)
        if distance == 0:
            return 0, [0, 0]
        a_normalized = a / distance
        b_normalized = b / distance
        direction = [a_normalized, b_normalized]
        return distance, direction

    def get_center_of_node(self, node):
        left_upper_corner = node.get_position()
        dimensions = node.get_dimensions()
        return [
            left_upper_corner[0] + dimensions[0] / 2,
            left_upper_corner[1] - dimensions[1] / 2,
        ]

    def get_node_radius(self, node):
        dimensions = node.get_dimensions()
        a = dimensions[0] / 2
        b = dimensions[1] / 2
        return self.pythagoras(a, b)

    def pythagoras(self, a, b):
        return math.sqrt(a * a + b * b)


def get_reachable_nodes(base_node, node_mask):
    reached_nodes = []
    node_queue = [base_node]
    while len(node_queue) > 0:
        connected_nodes = node_queue[0].get_connected_nodes()
        for node in connected_nodes:
            if (
                node_in_list(node, node_mask)
                and not node_in_list(node, node_queue)
                and not node_in_list(node, reached_nodes)
            ):
                node_queue.append(node)
        reached_nodes.append(node_queue[0])
        node_queue.pop(0)
    return reached_nodes


def get_cycles(base_node, node_mask):
    path = []
    cycles = []
    get_cycles_recursive(path, base_node, node_mask, cycles)
    cycles = remove_duplicate_cycles(cycles)
    return cycles


def get_cycles_recursive(path, current_node, node_mask, cycles):
    if node_in_list(current_node, path):
        cycle = []
        started_cycle = False
        for node in path:
            if node == current_node:
                started_cycle = True
            if started_cycle:
                cycle.append(node)
        cycles.append(cycle)
        return

    path.append(current_node)

    for node in current_node.get_connected_nodes():
        if len(path) <= 1:
            if node_in_list(node, node_mask):
                get_cycles_recursive(path, node, node_mask, cycles)
        elif node != path[-2] and node_in_list(node, node_mask):
            get_cycles_recursive(path, node, node_mask, cycles)
    path.pop()


def node_in_list(node, node_list):
    for nod in node_list:
        if nod == node:
            return True
    return False


def lists_contain_same_nodes(list1, list2):
    if len(list1) != len(list2):
        return False

    for node in list1:
        if not node_in_list(node, list2):
            return False
    return True


def remove_duplicates_in_list(duplicat_list):
    for i in range(len(duplicat_list)):
        if i < len(duplicat_list):
            for j in range(i + 1, len(duplicat_list)):
                if j < len(duplicat_list) and duplicat_list[i] == duplicat_list[j]:
                    duplicat_list.pop(j)
    return duplicat_list


def remove_duplicate_cycles(cycles):
    i = 0
    for a in range(len(cycles) - 1):
        j = i + 1
        for b in range(i + 1, len(cycles)):
            if lists_contain_same_nodes(cycles[i], cycles[j]):
                cycles.pop(j)
                j -= 1
            j += 1
        i += 1
    return cycles


def get_nodes_of_list_of_cycles(cycles):
    nodes = []
    for cycle in cycles:
        for node in cycle:
            if not node_in_list(node, nodes):
                nodes.append(node)
    return nodes


def remove_nodes_from_list(nodes_to_be_removed, nodes_list):
    new_list = []
    for node in nodes_list:
        if not node_in_list(node, nodes_to_be_removed):
            new_list.append(node)
    return new_list
