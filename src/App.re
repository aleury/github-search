type state = {
  input: string,
  isLoading: bool,
  results: list(Github.repository),
};

type action =
  | Search
  | UpdateInput(string)
  | UpdateResults(list(Github.repository));

let initialState = {input: "", isLoading: false, results: []};

let reducer = (state, action) => {
  switch (action) {
  | Search => {...state, isLoading: true}
  | UpdateInput(input) => {...state, input}
  | UpdateResults(results) => {...state, isLoading: false, results}
  };
};

let value = ev => ReactEvent.Form.target(ev)##value;
let preventDefault = ev => ReactEvent.Form.preventDefault(ev);

module Styles = {
  open TW;

  let header = make([Padding(P5)]);

  let searchForm = make([Display(Flex), JustifyContent(JustifyCenter)]);

  let searchInput =
    make([
      Padding(Py2),
      Padding(Px4),
      Display(Block),
      AppearanceNone,
      LineHeight(LeadingNormal),
      BorderWidth(Border),
      BorderColor(BorderGray400),
      BackgroundColor(BgWhite),
      BackgroundColor(HoverBgGray100),
    ]);

  let searchBtn =
    make([
      Padding(Py2),
      Padding(Px4),
      Display(Flex),
      TextColor(TextWhite),
      BackgroundColor(BgBlue500),
      BackgroundColor(HoverBgBlue400),
    ]);
};

[@react.component]
let make = () => {
  let (state, dispatch) = React.useReducer(reducer, initialState);

  let handleResults = results =>
    switch (results) {
    | None =>
      dispatch(UpdateResults([]));
      Js.Promise.resolve();
    | Some(results) =>
      dispatch(UpdateResults(results));
      Js.Promise.resolve();
    };

  let onSubmit = ev => {
    preventDefault(ev);
    dispatch(Search);
    let _ = Github.search(state.input) |> Js.Promise.then_(handleResults);
    ();
  };

  <div>
    <div className=Styles.header>
      <form onSubmit className=Styles.searchForm>
        <input
          id="search"
          name="search"
          value={state.input}
          placeholder="Search for a repository"
          className=Styles.searchInput
          onChange={ev => dispatch(UpdateInput(value(ev)))}
        />
        <button type_="submit" className=Styles.searchBtn>
          <i className="material-icons"> {React.string("search")} </i>
        </button>
      </form>
    </div>
    <div>
      {state.isLoading
         ? React.string("Loading...")
         : state.results
           ->Belt.List.map(({name, url, description}) =>
               <Card key=url name href=url description />
             )
           ->Belt.List.toArray
           ->React.array}
    </div>
  </div>;
};