open Json.Decode;

type repository = {
  url: string,
  name: string,
  description: string,
};

let decodeRepository = json => {
  name: field("name", string, json),
  url: field("html_url", string, json),
  description: field("description", string, json),
};

let decodeResults = field("items", list(optional(decodeRepository)));

let search = query =>
  Js.Promise.(
    Fetch.fetch("https://api.github.com/search/repositories?q=" ++ query)
    |> then_(Fetch.Response.json)
    |> then_(json => decodeResults(json) |> resolve)
    |> then_(results => results->Belt.List.keepMap(x => x)->Some->resolve)
    |> catch(_err => Js.Promise.resolve(None))
  );