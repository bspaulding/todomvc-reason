type todoItem = {
	id: int,
	description: string,
	completed: bool,
	editing: bool
};

type visibility = All | Active | Completed;

type action =
	| AddTodo
	| UpdateNewDescription(string)
	| ToggleCompleted(int)
	| ToggleEditing(int)
	| Remove(int)
	| DescriptionChanged(int, string)
	| ClearCompleted
	| Clear
	| ToggleAllCompleted
	| UpdateVisibility(visibility);

type state = {
	items: list(todoItem),
	newDescription: string,
	nextId: int,
	visibility: visibility
};
let initialState = () => {
	items: [],
	newDescription: "",
	nextId: 2,
	visibility: All
};

let reducer = (action, state) =>
	switch action {
	| AddTodo => ReasonReact.Update({
		...state,
		items: List.append(
			state.items,
			[{ id: state.nextId, description: state.newDescription, completed: false, editing: false }]
		),
		newDescription: "",
		nextId: state.nextId + 1
	})

	| Clear => ReasonReact.Update({...state, items: [] })
	| UpdateNewDescription(newDescription) => ReasonReact.Update({ ...state, newDescription })
	| ToggleCompleted(id) => ReasonReact.Update({
			...state,
			items: List.map((item) =>
				item.id == id
					? { ...item, completed: !item.completed }
					: item,
				state.items
			)
		})
	| ToggleEditing(id) =>
			ReasonReact.Update({
				...state,
				items: List.map(item =>
					item.id == id
						? { ...item, editing: !item.editing }
						: item,
					state.items
				)
			})
	| DescriptionChanged(id, description) =>
			ReasonReact.Update({
				...state,
				items: List.map(item =>
					item.id == id
						? { ...item, description }
						: item,
					state.items
				)
			})
	| ClearCompleted =>
			ReasonReact.Update({
				...state,
				items: List.filter((item) => !item.completed, state.items)
			})
	| ToggleAllCompleted =>
			ReasonReact.Update({
				...state,
				items: List.for_all(i => i.completed, state.items)
					? List.map(i => { ...i, completed: false }, state.items)
					: List.map(i => { ...i, completed: true }, state.items)
			});
	| Remove(id) =>
			ReasonReact.Update({
				...state,
				items: List.filter(i => i.id != id, state.items)
			})
	| UpdateVisibility(visibility) => ReasonReact.Update({ ...state, visibility })
	};
