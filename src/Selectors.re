let numRemainingTodos = TodoAppReducer.(
	(state: state) =>
		List.length(
			List.filter(
				(item: todoItem) => !item.completed,
				state.items
			)
		)
);

let filteredItems =
	TodoAppReducer.(
		(items: list(todoItem), visibility: visibility) =>
			switch visibility {
			| All => items
			| Active => List.filter(i => !i.completed, items)
			| Completed => List.filter(i => i.completed, items)
			}
	)
