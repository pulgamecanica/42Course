import concurrent.futures


def process_action(action):
    # Simulate processing an action
    return f"Processed {action}"


def main():
    actions = ['action1', 'action2', 'action3']
    try:
        with concurrent.futures.ThreadPoolExecutor() as executor:
            results = list(executor.map(process_action, actions))
        print(results)
    except Exception as e:
        print(f"An error occurred: {e}")


if __name__ == "__main__":
    main()
