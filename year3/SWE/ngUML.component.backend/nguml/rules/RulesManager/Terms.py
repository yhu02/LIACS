class Operator:
    def __init__(self, operator):
        self.name = operator

    def __str__(self):
        return self.name


class Value:
    def __init__(self, name):
        self.name = name

    def __str__(self):
        return self.name


class AwaitingDesignation:
    def __init__(self, name):
        self.name = name

    def __str__(self):
        return self.name
