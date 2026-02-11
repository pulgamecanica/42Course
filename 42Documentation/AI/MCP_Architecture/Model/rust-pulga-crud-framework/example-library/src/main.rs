mod book;
mod library;

use std::path::PathBuf;
use colored::*;
use dialoguer::{Input, Select};
use pulga_crud_storage::FileStorage;
use pulga_crud_core::{HasMany, BelongsTo, Storage};
use book::Book;
use library::Library;

fn banner() {
    println!("{}", "====================================".blue());
    println!("{}", "📚   RUSTY LIBRARY MANAGER   📚".bold().yellow());
    println!("{}", "====================================".blue());
}

fn separator() {
    println!("{}", "------------------------------------".bright_black());
}

fn main() {
    banner();

    let library_storage = FileStorage::<Library>::new(PathBuf::from("data"));
    let book_storage = FileStorage::<Book>::new(PathBuf::from("data"));

    loop {
        let options = vec![
            "Create Library",
            "Create Book",
            "List Libraries (with books)",
            "List Books (with library)",
            "Delete Book",
            "Exit",
        ];

        let selection = Select::new()
            .items(&options)
            .default(0)
            .interact()
            .unwrap();

        match selection {
            // CREATE LIBRARY
            0 => {
                let name: String =
                    Input::new().with_prompt("Library name").interact().unwrap();
                let lib = Library::new(name);
                library_storage.create(&lib).unwrap();
                println!("{}", "Library created!".green());
                separator();
            }

            // CREATE BOOK
            1 => {
                let title: String =
                    Input::new().with_prompt("Book title").interact().unwrap();
                let pages: u32 =
                    Input::new().with_prompt("Pages").interact().unwrap();

                // Fetch all libraries
                let libraries = library_storage.list(0, 1000).unwrap();

                if libraries.is_empty() {
                    println!("{}", "No libraries available! Create one first.".red());
                    separator();
                    continue;
                }

                // Show library selection
                let lib_names: Vec<String> = libraries
                    .iter()
                    .map(|l| format!("{} ({})", l.name, l.id))
                    .collect();

                let lib_index = Select::new()
                    .with_prompt("Select a Library")
                    .items(&lib_names)
                    .default(0)
                    .interact()
                    .unwrap();

                let lib_id = &libraries[lib_index].id;

                let book = Book::new(title, pages, lib_id.clone());
                book_storage.create(&book).unwrap();
                println!("{}", "Book created!".green());
                separator();
            }

            // LIST LIBRARIES WITH BOOKS
            2 => {
                let libs = library_storage.list(0, 10_000).unwrap();

                for lib in libs {
                    println!(
                        "{} {}",
                        "🏛".cyan(),
                        lib.name.bold().underline()
                    );

                    match lib.has_many(&book_storage) {
                        Ok(books) if books.is_empty() => {
                            println!("   {}", "No books yet".dimmed());
                        }
                        Ok(books) => {
                            for book in books {
                                let icon = if book.pages % 2 == 0 {
                                    "📘"
                                } else {
                                    "📕"
                                };
                                println!(
                                    "   {} {} ({} pages)",
                                    icon,
                                    book.title.bold(),
                                    book.pages
                                );
                            }
                        }
                        Err(_) => println!("   {}", "Error loading books".red()),
                    }

                    separator();
                }
            }

            // LIST BOOKS WITH THEIR LIBRARY
            3 => {
                let books = book_storage.list(0, 10_000).unwrap();

                for book in books {
                    let icon = if book.pages % 2 == 0 {
                        "📘"
                    } else {
                        "📕"
                    };

                    print!(
                        "{} {} ({} pages)",
                        icon,
                        book.title.bold(),
                        book.pages
                    );

                    match book.belongs_to(&library_storage) {
                        Ok(library) => {
                            println!(
                                "  →  {} {}",
                                "Library:".bright_black(),
                                library.name.cyan()
                            );
                        }
                        Err(_) => {
                            println!("  →  {}", "Unknown Library".red());
                        }
                    }
                }

                separator();
            }

            // DELETE BOOK
            4 => {
                // Fetch all books
                let books = book_storage.list(0, 10_000).unwrap();

                if books.is_empty() {
                    println!("{}", "No books available to delete.".red());
                    separator();
                    continue;
                }

                // Display books for selection
                let book_names: Vec<String> = books
                    .iter()
                    .map(|b| format!("{} ({} pages) [{}]", b.title, b.pages, b.id))
                    .collect();

                let book_index = Select::new()
                    .with_prompt("Select a book to delete")
                    .items(&book_names)
                    .default(0)
                    .interact()
                    .unwrap();

                let book_id = &books[book_index].id;
                book_storage.delete(book_id).unwrap();

                println!("{}", "Book deleted!".red());
                separator();
            }

            // EXIT
            _ => break,
        }
    }
}
