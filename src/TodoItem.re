let component = ReasonReact.statelessComponent("TodoItem");

let getTargetValue = event => ReactDOMRe.domElementToObj(ReactEventRe.Form.target(event))##value;

let s = ReasonReact.stringToElement;
let className = (completed, editing) =>
	String.concat(" ", [
		completed ? "completed" : "",
		editing ? "editing" : ""
	]);

let make = (
	~completed=false,
	~description="",
	~editing=false,
	~onToggleCompleted,
	~onToggleEditing,
	~onDescriptionChanged,
	~onRemove,
	_children) => {
	...component,
	render: (_self) =>
		/* List items should get the class `editing` when editing and `completed` when marked as completed */
		<li className=(className(completed, editing))>
			<div className="view">
				<input
					className="toggle"
					_type="checkbox"
					checked=(Js.Boolean.to_js_boolean(completed))
					onChange=((_event) => onToggleCompleted())
				/>
				<label onDoubleClick=((_event) => {
					Js.log("item label onDoubleClick");
					onToggleEditing()
				})>
					(s(description))
				</label>
				<button className="destroy" onClick=(_e => onRemove())></button>
			</div>
			<input
				className="edit"
				value=(description)
				onChange=(
					(event) => onDescriptionChanged(getTargetValue(event))
				)
				onKeyDown=((event) =>
					if (ReactEventRe.Keyboard.key(event) == "Enter") {
						onToggleEditing()
					}
				)
			/>
		</li>
};
