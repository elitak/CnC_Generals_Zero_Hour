{
  description = "Nix flake for building CnC Generals Zero Hour";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    gamespy = {
      url = "https://github.com/feliwir/GamespySDK/archive/582c79105aa851c5aa847f638722f61195b79c9b.tar.gz";
      flake = false;
    };
    miles = {
      url = "https://github.com/TheSuperHackers/miles-sdk-stub/archive/0fef646a85c822475d55f19e3ca185263fb4a967.tar.gz";
      flake = false;
    };
    liblzhl = {
      url = "https://github.com/feliwir/liblzhl/archive/fd7c70c4bb96e7a4a682f574e788c499f00a0b8d.tar.gz";
      flake = false;
    };
    gli-src = {
      url = "https://github.com/g-truc/gli/archive/779b99ac6656e4d30c3b24e96e0136a59649a869.tar.gz";
      flake = false;
    };
    dxvk = {
      url = "https://github.com/doitsujin/dxvk/releases/download/v2.6/dxvk-native-2.6-steamrt-sniper.tar.gz";
      flake = false;
    };
  };

  outputs = { self, nixpkgs, gamespy, miles, liblzhl, gli-src, dxvk }:
    let
      systems = [ "x86_64-linux" "aarch64-linux" ];
      forAllSystems = f: builtins.listToAttrs (map (system: {
        name = system;
        value = f system;
      }) systems);
    in
    {
      packages = forAllSystems (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        gli = pkgs.stdenv.mkDerivation {
          pname = "gli";
          version = "1.0.1";
          src = gli-src;
          nativeBuildInputs = [ pkgs.cmake ];
        };

        sage = pkgs.stdenv.mkDerivation {
          pname = "cnc-generals-zero-hour";
          version = "0.1.0";
          src = ./.;

          nativeBuildInputs = with pkgs; [
            cmake
            ninja
            pkg-config
            git
            nasm
            autoconf
            automake
            libtool
            python3Packages.jinja2
          ];

          buildInputs = with pkgs; [
            zlib
            glm
            openal
            ffmpeg
            freetype
            fontconfig
            sdl3
            sdl3-image
            gli
            libx11
            libxext
            libxft
            wayland
            libxkbcommon
            mesa
            ibus
          ];

          cmakeFlags = [
            "-GNinja"
            "-DSAGE_USE_DX8=OFF"
            "-DSAGE_USE_BINK=OFF"
            "-DSAGE_USE_GLM=ON"
            "-DSAGE_USE_SDL3=ON"
            "-DSAGE_USE_FFMPEG=ON"
            "-DSAGE_USE_OPENAL=ON"
            "-DSAGE_USE_FREETYPE=ON"
            "-DSAGE_USE_FONTCONFIG=ON"
            "-DFETCHCONTENT_FULLY_DISCONNECTED=ON"
            "-DFETCHCONTENT_SOURCE_DIR_gamespy=${gamespy}"
            "-DFETCHCONTENT_SOURCE_DIR_miles=${miles}"
            "-DFETCHCONTENT_SOURCE_DIR_liblzhl=${liblzhl}"
            "-DFETCHCONTENT_SOURCE_DIR_dxvk=${dxvk}"
          ];

          postInstall = ''
            mkdir -p $out/bin
            cp GeneralsMD/Code/RTS $out/bin/
          '';
        };
      in
      {
        default = sage;
      });
      devShells = forAllSystems (system:
        let
          pkgs = nixpkgs.legacyPackages.${system};
          package = self.packages.${system}.default;
        in
        {
          default = pkgs.mkShell {
            inputsFrom = [ package ];
          };
        });
    };
}
