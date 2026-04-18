{
  description = "Nix flake for building CnC Generals Zero Hour";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
    gamespy = {
      url = "github:feliwir/GamespySDK/582c79105aa851c5aa847f638722f61195b79c9b";
      flake = false;
    };
    miles = {
      url = "github:TheSuperHackers/miles-sdk-stub/0fef646a85c822475d55f19e3ca185263fb4a967";
      flake = false;
    };
    liblzhl = {
      url = "github:feliwir/liblzhl/fd7c70c4bb96e7a4a682f574e788c499f00a0b8d";
      flake = false;
    };
    dxvk = {
      url = "https://github.com/doitsujin/dxvk/releases/download/v2.6/dxvk-native-2.6-steamrt-sniper.tar.gz";
      flake = false;
    };
  };

  outputs = { self, nixpkgs, flake-utils, gamespy, miles, liblzhl, dxvk }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs {
          inherit system;
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
            gli
            openal
            ffmpeg
            freetype
            fontconfig
            SDL3
            SDL3_image
            xorg.libX11
            xorg.libXext
            xorg.libXft
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
        packages.default = sage;

        devShells.default = pkgs.mkShell {
          inputsFrom = [ sage ];
        };
      });
}
