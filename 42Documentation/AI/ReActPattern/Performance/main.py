class ReActAgent:
    def __init__(self):
        self.memo = {}

    def compute(self, input_data):
        if input_data in self.memo:
            return self.memo[input_data]

        # Simulate a computationally expensive operation
        result = self.expensive_operation(input_data)
        self.memo[input_data] = result
        return result

    def expensive_operation(self, data):
        # Placeholder for a complex calculation
        return data * 2
