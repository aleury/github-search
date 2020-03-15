open Json.Decode;

type repository = {
  url: string,
  name: string,
  description: string,
};

let decodeResults =
  field(
    "items",
    list(
      optional(json =>
        {
          name: field("name", string, json),
          url: field("html_url", string, json),
          description: field("description", string, json),
        }
      ),
    ),
  );

let search = query =>
  Js.Promise.(
    Fetch.fetch("https://api.github.com/search/repositories?q=" ++ query)
    |> then_(Fetch.Response.json)
    |> then_(json => decodeResults(json) |> resolve)
    |> then_(results => results->Belt.List.keepMap(x => x)->Some->resolve)
    |> catch(_err => Js.Promise.resolve(None))
  );